
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <memory>
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/compiler/simple_msg_printer.h"

using std::unique_ptr;
using std::vector;
using std::stringstream;
using std::string;
using boost::format;
using boost::filesystem::path;
using testing::Test;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Token;
using real_talk::parser::PrimitiveDataTypeNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::LongNode;
using real_talk::parser::IntNode;
using real_talk::parser::CharNode;
using real_talk::parser::StringNode;
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::ExprNode;
using real_talk::parser::AssignNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::IdNode;
using real_talk::parser::BoolNode;
using real_talk::parser::SubscriptNode;
using real_talk::semantic::SemanticProblem;
using real_talk::semantic::DataType;
using real_talk::semantic::IntDataType;
using real_talk::semantic::BoolDataType;
using real_talk::semantic::LongDataType;
using real_talk::semantic::ArrayAllocWithTooManyDimensionsError;
using real_talk::semantic::ArrayTypeWithTooManyDimensionsError;
using real_talk::semantic::DoubleWithOutOfRangeValueError;
using real_talk::semantic::LongWithOutOfRangeValueError;
using real_talk::semantic::IntWithOutOfRangeValueError;
using real_talk::semantic::CharWithMultipleCharsError;
using real_talk::semantic::CharWithEmptyHexValueError;
using real_talk::semantic::CharWithOutOfRangeHexValueError;
using real_talk::semantic::StringWithOutOfRangeHexValueError;
using real_talk::semantic::StringWithEmptyHexValueError;
using real_talk::semantic::AssignWithRightValueAssigneeError;
using real_talk::semantic::IdWithoutDefError;
using real_talk::semantic::SubscriptWithUnsupportedIndexTypeError;
using real_talk::semantic::SubscriptWithUnsupportedOperandTypeError;
using real_talk::semantic::ArrayAllocWithIncompatibleValueTypeError;
using real_talk::semantic::ArrayAllocWithUnsupportedElementTypeError;

namespace real_talk {
namespace compiler {

class SimpleMsgPrinterTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestPrintSemanticProblem(
      const SemanticProblem &problem, const string &expected_msg) {
    path file_path("src/app.rts");
    string expected_output = (format(expected_msg) % file_path.string()).str();
    stringstream stream;
    SimpleMsgPrinter msg_printer(&stream);
    msg_printer.PrintSemanticProblem(problem, file_path);
    string actual_output = stream.str();
    ASSERT_EQ(expected_output, actual_output);
  }
};

TEST_F(SimpleMsgPrinterTest, PrintError) {
  stringstream stream;
  string error = "test";
  string expected_output = "[Error] test\n";
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintError(error);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}

TEST_F(SimpleMsgPrinterTest, PrintHelp) {
  stringstream stream;
  string help = "test";
  string expected_output = "test\n";
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintHelp(help);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}

TEST_F(SimpleMsgPrinterTest, PrintUnexpectedCharError) {
  stringstream stream;
  char c = 'a';
  path file_path("src/app.rts");
  uint32_t line = UINT32_C(2);
  uint32_t column = UINT32_C(3);
  string expected_output =
      (format("[Error] %1%:2:3: unexpected character \"a\"\n")
       % file_path.string()).str();
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintUnexpectedCharError(c, line, column, file_path);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}

TEST_F(SimpleMsgPrinterTest, PrintUnexpectedTokenError) {
  stringstream stream;
  path file_path("src/app.rts");
  TokenInfo token(Token::kIntLit, "7", UINT32_C(1), UINT32_C(2));
  string expected_output =
      (format("[Error] %1%:1:2: unexpected token \"7\"\n")
       % file_path.string()).str();
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintUnexpectedTokenError(token, file_path);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}

TEST_F(SimpleMsgPrinterTest, PrintTokenError) {
  stringstream stream;
  path file_path("src/app.rts");
  TokenInfo token(Token::kIntLit, "7", UINT32_C(1), UINT32_C(2));
  string error = "msg";
  string expected_output =
      (format("[Error] %1%:1:2: msg \"7\"\n") % file_path.string()).str();
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintTokenError(token, file_path, error);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithArrayAllocWithTooManyDimensionsError) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(2), UINT32_C(3))));
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(6), UINT32_C(7))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  vector<TokenInfo> value_separator_tokens;
  ArrayAllocWithInitNode array_alloc_node(
      TokenInfo(Token::kNew, "new", UINT32_C(0), UINT32_C(1)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      move(value_nodes),
      value_separator_tokens,
      TokenInfo(Token::kScopeStart, "}", UINT32_C(10), UINT32_C(11)));
  size_t max_dimensions_count = 7;
  ArrayAllocWithTooManyDimensionsError problem(
      array_alloc_node, max_dimensions_count);
  string expected_msg =
      "[Error] %1%:0:1: array can't have more than 7 dimensions\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithArrayTypeWithTooManyDimensionsError) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  ArrayDataTypeNode array_data_type_node(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(5)));
  size_t max_dimensions_count = 7;
  ArrayTypeWithTooManyDimensionsError problem(
      array_data_type_node, max_dimensions_count);
  string expected_msg =
      "[Error] %1%:0:1: array can't have more than 7 dimensions\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithDoubleWithOutOfRangeValueError) {
  DoubleNode double_node(
    TokenInfo(Token::kDoubleLit, "7.7", UINT32_C(0), UINT32_C(1)));
  DoubleWithOutOfRangeValueError problem(double_node);
  string expected_msg = "[Error] %1%:0:1: out of range value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithLongWithOutOfRangeValueError) {
  LongNode long_node(
    TokenInfo(Token::kLongLit, "7L", UINT32_C(0), UINT32_C(1)));
  LongWithOutOfRangeValueError problem(long_node);
  string expected_msg = "[Error] %1%:0:1: out of range value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithIntWithOutOfRangeValueError) {
  IntNode int_node(
    TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(1)));
  IntWithOutOfRangeValueError problem(int_node);
  string expected_msg = "[Error] %1%:0:1: out of range value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithCharWithOutOfRangeHexValueError) {
  CharNode char_node(
    TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(1)));
  CharWithOutOfRangeHexValueError problem(char_node);
  string expected_msg = "[Error] %1%:0:1: out of range value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithCharWithMultipleCharsError) {
  CharNode char_node(
    TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(1)));
  CharWithMultipleCharsError problem(char_node);
  string expected_msg =
      "[Error] %1%:0:1: multiple characters\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithCharWithEmptyHexValueError) {
  CharNode char_node(
    TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(1)));
  CharWithEmptyHexValueError problem(char_node);
  string expected_msg = "[Error] %1%:0:1: empty hex value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithStringWithOutOfRangeHexValueError) {
  StringNode string_node(
    TokenInfo(Token::kStringLit, "\"abc\"", UINT32_C(0), UINT32_C(1)));
  StringWithOutOfRangeHexValueError problem(string_node);
  string expected_msg = "[Error] %1%:0:1: out of range hex value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithStringWithEmptyHexValueError) {
  StringNode string_node(
    TokenInfo(Token::kStringLit, "\"abc\"", UINT32_C(0), UINT32_C(1)));
  StringWithEmptyHexValueError problem(string_node);
  string expected_msg = "[Error] %1%:0:1: empty hex value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithAssignWithRightValueAssigneeError) {
  unique_ptr<ExprNode> left_operand_node(new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(1))));
  unique_ptr<ExprNode> right_operand_node(new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(4), UINT32_C(5))));
  AssignNode assign_node(
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(3)),
      move(left_operand_node),
      move(right_operand_node));
  AssignWithRightValueAssigneeError problem(assign_node);
  string expected_msg = "[Error] %1%:2:3: can't assign to rvalue\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithIdWithoutDefError) {
  IdNode id_node(
      TokenInfo(Token::kName, "id", UINT32_C(0), UINT32_C(1)));
  IdWithoutDefError problem(id_node);
  string expected_msg = "[Error] %1%:0:1: undefined identifier\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithSubscriptWithUnsupportedIndexTypeError) {
  unique_ptr<ExprNode> operand_node(new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(1))));
  unique_ptr<ExprNode> index_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(4), UINT32_C(5))));
  SubscriptNode subscript_node(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(6), UINT32_C(7)),
      move(operand_node),
      move(index_node));
  SubscriptWithUnsupportedIndexTypeError problem(
      subscript_node, unique_ptr<DataType>(new BoolDataType()));
  string expected_msg =
      "[Error] %1%:4:5: \"bool\" data type is not supported as index of operator \"[]\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithSubscriptWithUnsupportedOperandTypeError) {
  unique_ptr<ExprNode> operand_node(new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(1))));
  unique_ptr<ExprNode> index_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(4), UINT32_C(5))));
  SubscriptNode subscript_node(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(6), UINT32_C(7)),
      move(operand_node),
      move(index_node));
  SubscriptWithUnsupportedOperandTypeError problem(
      subscript_node, unique_ptr<DataType>(new IntDataType()));
  string expected_msg =
      "[Error] %1%:0:1: \"int\" data type is not supported as operand of operator \"[]\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithArrayAllocWithIncompatibleValueTypeError) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(2), UINT32_C(3))));
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(6), UINT32_C(7))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  unique_ptr<ExprNode> value_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(10), UINT32_C(11))));
  value_nodes.push_back(move(value_node));
  vector<TokenInfo> value_separator_tokens;
  ArrayAllocWithInitNode array_alloc_node(
      TokenInfo(Token::kNew, "new", UINT32_C(0), UINT32_C(1)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      move(value_nodes),
      value_separator_tokens,
      TokenInfo(Token::kScopeStart, "}", UINT32_C(12), UINT32_C(13)));
  size_t value_index = 0;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new LongDataType());
  ArrayAllocWithIncompatibleValueTypeError problem(
      array_alloc_node, value_index, move(dest_data_type), move(src_data_type));
  string expected_msg =
      "[Error] %1%:10:11: expected \"int\" data type, but got \"long\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithArrayAllocWithUnsupportedElementTypeError) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(2), UINT32_C(3))));
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(6), UINT32_C(7))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  vector<TokenInfo> value_separator_tokens;
  ArrayAllocWithInitNode array_alloc_node(
      TokenInfo(Token::kNew, "new", UINT32_C(0), UINT32_C(1)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      move(value_nodes),
      value_separator_tokens,
      TokenInfo(Token::kScopeStart, "}", UINT32_C(12), UINT32_C(13)));
  ArrayAllocWithUnsupportedElementTypeError problem(
      array_alloc_node, unique_ptr<DataType>(new IntDataType()));
  string expected_msg =
      "[Error] %1%:2:3: \"int\" data type is not supported by array\n";
  TestPrintSemanticProblem(problem, expected_msg);
}
}
}
