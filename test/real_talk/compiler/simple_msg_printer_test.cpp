
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <memory>
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/not_node.h"
#include "real_talk/parser/sum_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
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
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/compiler/simple_msg_printer.h"

using std::unique_ptr;
using std::vector;
using std::move;
using std::stringstream;
using std::string;
using boost::format;
using boost::filesystem::path;
using testing::Test;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Token;
using real_talk::parser::ProgramNode;
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::FuncDefWithoutBodyNode;
using real_talk::parser::NotNode;
using real_talk::parser::SumNode;
using real_talk::parser::CallNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::ImportNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::StmtNode;
using real_talk::parser::IfNode;
using real_talk::parser::IfElseIfNode;
using real_talk::parser::ElseIfNode;
using real_talk::parser::BreakNode;
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
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::ExprNode;
using real_talk::parser::AssignNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::IdNode;
using real_talk::parser::BoolNode;
using real_talk::parser::SubscriptNode;
using real_talk::semantic::SemanticAnalysis;
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
using real_talk::semantic::ArrayAllocWithUnsupportedSizeTypeError;
using real_talk::semantic::IfWithIncompatibleTypeError;
using real_talk::semantic::BreakNotWithinLoopError;
using real_talk::semantic::ContinueNotWithinLoopError;
using real_talk::semantic::PreTestLoopWithIncompatibleTypeError;
using real_talk::semantic::ImportIsNotFirstStmtError;
using real_talk::semantic::VarDefWithIncompatibleValueTypeError;
using real_talk::semantic::ReturnWithoutValueError;
using real_talk::semantic::ReturnNotWithinFuncError;
using real_talk::semantic::ReturnWithIncompatibleTypeError;
using real_talk::semantic::CallWithIncompatibleArgTypeError;
using real_talk::semantic::BinaryExprWithUnsupportedTypesError;
using real_talk::semantic::UnaryExprWithUnsupportedTypeError;
using real_talk::semantic::DefWithUnsupportedTypeError;
using real_talk::semantic::DuplicateDefError;
using real_talk::semantic::FuncDefWithoutBodyNotNativeError;
using real_talk::semantic::FuncDefWithBodyIsNativeError;
using real_talk::semantic::FuncDefWithinNonGlobalScopeError;
using real_talk::semantic::FuncDefWithoutReturnValueError;
using real_talk::semantic::CallWithUnsupportedTypeError;
using real_talk::semantic::CallWithInvalidArgsCountError;

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
      "[Error] %1%:2:3: \"int\" data type is not supported as element of array\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithArrayAllocWithUnsupportedSizeTypeError) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(2), UINT32_C(3))));
  vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
  unique_ptr<ExprNode> size_value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(6), UINT32_C(7))));
  auto *size_node_ptr = new BoundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(5)),
      move(size_value_node),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(8), UINT32_C(9)));
  unique_ptr<BoundedArraySizeNode> size_node(size_node_ptr);
  size_nodes.push_back(move(size_node));
  ArrayAllocWithoutInitNode array_alloc_node(
      TokenInfo(Token::kNew, "new", UINT32_C(0), UINT32_C(1)),
      move(data_type_node),
      move(size_nodes));
  ArrayAllocWithUnsupportedSizeTypeError problem(
      array_alloc_node,
      *size_node_ptr,
      unique_ptr<DataType>(new IntDataType()));
  string expected_msg =
      "[Error] %1%:6:7: \"int\" data type is not supported as size of array\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithIfWithIncompatibleTypeError) {
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(11))));
  unique_ptr<ExprNode> cond_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(4), UINT32_C(5))));
  auto *if_node_ptr = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(3)),
      move(cond_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)),
      move(body_node));
  unique_ptr<IfNode> if_node(if_node_ptr);
  IfElseIfNode if_else_if_node(
      move(if_node), vector< unique_ptr<ElseIfNode> >());
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new LongDataType());
  IfWithIncompatibleTypeError problem(
      if_else_if_node,
      *if_node_ptr,
      move(dest_data_type),
      move(src_data_type));
  string expected_msg =
      "[Error] %1%:4:5: expected \"int\" data type, but got \"long\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithBreakNotWithinLoopError) {
  BreakNode break_node(
      TokenInfo(Token::kBreak, "break", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(3)));
  BreakNotWithinLoopError problem(break_node);
  string expected_msg = "[Error] %1%:0:1: \"break\" is not within loop\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithContinueNotWithinLoopError) {
  ContinueNode continue_node(
      TokenInfo(Token::kContinue, "continue", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(3)));
  ContinueNotWithinLoopError problem(continue_node);
  string expected_msg = "[Error] %1%:0:1: \"continue\" is not within loop\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithPreTestLoopWithIncompatibleTypeError) {
  unique_ptr<ExprNode> cond_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(4), UINT32_C(5))));
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(11))));
  PreTestLoopNode loop_node(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(3)),
      move(cond_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)),
      move(body_node));
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new LongDataType());
  PreTestLoopWithIncompatibleTypeError problem(
      loop_node, move(dest_data_type), move(src_data_type));
  string expected_msg =
      "[Error] %1%:4:5: expected \"int\" data type, but got \"long\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithImportIsNotFirstStmtError) {
  unique_ptr<StringNode> file_path_node(new StringNode(
      TokenInfo(Token::kStringLit, "\"abc\"", UINT32_C(2), UINT32_C(3))));
  ImportNode import_node(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(1)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(5)));
  ImportIsNotFirstStmtError problem(import_node);
  string expected_msg =
      "[Error] %1%:0:1: \"import\" statements must be before any other\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithVarDefWithIncompatibleValueTypeError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  unique_ptr<ExprNode> value_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(6), UINT32_C(7))));
  VarDefWithInitNode var_def_node(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(4), UINT32_C(5)),
      move(value_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(9)));
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new LongDataType());
  VarDefWithIncompatibleValueTypeError problem(
      var_def_node, move(dest_data_type), move(src_data_type));
  string expected_msg =
      "[Error] %1%:6:7: expected \"int\" data type, but got \"long\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithReturnWithoutValueError) {
  ReturnWithoutValueNode return_node(
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(3)));
  ReturnWithoutValueError problem(return_node);
  string expected_msg = "[Error] %1%:0:1: \"return\" is without value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithReturnNotWithinFuncError) {
  ReturnWithoutValueNode return_node(
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(3)));
  ReturnNotWithinFuncError problem(return_node);
  string expected_msg = "[Error] %1%:0:1: \"return\" is not within function\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithReturnWithIncompatibleTypeError) {
  unique_ptr<ExprNode> value_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(2), UINT32_C(3))));
  ReturnValueNode return_node(
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(1)),
      move(value_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(5)));
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new LongDataType());
  ReturnWithIncompatibleTypeError problem(
      return_node, move(dest_data_type), move(src_data_type));
  string expected_msg =
      "[Error] %1%:2:3: expected \"int\" data type, but got \"long\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithCallWithIncompatibleArgTypeError) {
  unique_ptr<ExprNode> operand_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(1))));
  vector< unique_ptr<ExprNode> > arg_nodes;
  unique_ptr<ExprNode> arg_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(4), UINT32_C(5))));
  arg_nodes.push_back(move(arg_node));
  vector<TokenInfo> arg_separator_tokens;
  CallNode call_node(
      move(operand_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(3)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)));
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new LongDataType());
  size_t arg_index = 0;
  CallWithIncompatibleArgTypeError problem(
      call_node, arg_index, move(dest_data_type), move(src_data_type));
  string expected_msg =
      "[Error] %1%:4:5: expected \"int\" data type, but got \"long\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithBinaryExprWithUnsupportedTypesError) {
  unique_ptr<ExprNode> left_operand_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(1))));
  unique_ptr<ExprNode> right_operand_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(4), UINT32_C(5))));
  SumNode sum_node(
      TokenInfo(Token::kSumOp, "+", UINT32_C(2), UINT32_C(3)),
      move(left_operand_node),
      move(right_operand_node));
  unique_ptr<DataType> left_data_type(new IntDataType());
  unique_ptr<DataType> right_data_type(new LongDataType());
  BinaryExprWithUnsupportedTypesError problem(
      sum_node, move(left_data_type), move(right_data_type));
  string expected_msg =
      "[Error] %1%:2:3: \"int\" and \"long\" data types are not supported by operator \"+\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithUnaryExprWithUnsupportedTypeError) {
  unique_ptr<ExprNode> operand_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(2), UINT32_C(3))));
  NotNode not_node(
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(1)),
      move(operand_node));
  UnaryExprWithUnsupportedTypeError problem(
      not_node, unique_ptr<DataType>(new IntDataType()));
  string expected_msg =
      "[Error] %1%:2:3: \"int\" data type is not supported by operator \"!\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithDefWithUnsupportedTypeError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  VarDefWithoutInitNode var_def_node(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(5)));
  DefWithUnsupportedTypeError problem(
      var_def_node, unique_ptr<DataType>(new IntDataType()));
  string expected_msg =
      "[Error] %1%:0:1: definition doesn't support \"int\" data type\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithDuplicateDefError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  VarDefWithoutInitNode var_def_node(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(5)));
  DuplicateDefError problem(var_def_node);
  string expected_msg = "[Error] %1%:2:3: duplicate definition\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithFuncDefWithoutBodyNotNativeError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  vector<TokenInfo> modifier_tokens;
  vector< unique_ptr<ArgDefNode> > arg_nodes;
  vector<TokenInfo> arg_separator_tokens;
  FuncDefWithoutBodyNode func_def_node(
      modifier_tokens,
      move(data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(5)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(9)));
  FuncDefWithoutBodyNotNativeError problem(func_def_node);
  string expected_msg =
      "[Error] %1%:2:3: non-native function definition without body\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithFuncDefWithBodyIsNativeError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  vector<TokenInfo> modifier_tokens;
  vector< unique_ptr<ArgDefNode> > arg_nodes;
  vector<TokenInfo> arg_separator_tokens;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(11))));
  FuncDefWithBodyNode func_def_node(
      modifier_tokens,
      move(data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(5)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)),
      move(body_node));
  FuncDefWithBodyIsNativeError problem(func_def_node);
  string expected_msg =
      "[Error] %1%:2:3: native function definition with body\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithFuncDefWithinNonGlobalScopeError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  vector<TokenInfo> modifier_tokens;
  vector< unique_ptr<ArgDefNode> > arg_nodes;
  vector<TokenInfo> arg_separator_tokens;
  FuncDefWithoutBodyNode func_def_node(
      modifier_tokens,
      move(data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(5)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(9)));
  FuncDefWithinNonGlobalScopeError problem(func_def_node);
  string expected_msg =
      "[Error] %1%:2:3: function definition within non-global scope\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithFuncDefWithoutReturnValueError) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(1))));
  vector<TokenInfo> modifier_tokens;
  vector< unique_ptr<ArgDefNode> > arg_nodes;
  vector<TokenInfo> arg_separator_tokens;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(9)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(11))));
  FuncDefWithBodyNode func_def_node(
      modifier_tokens,
      move(data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(5)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)),
      move(body_node));
  FuncDefWithoutReturnValueError problem(func_def_node);
  string expected_msg =
      "[Error] %1%:2:3: function doesn't always returns value\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithCallWithUnsupportedTypeError) {
  unique_ptr<ExprNode> operand_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(1))));
  vector< unique_ptr<ExprNode> > arg_nodes;
  unique_ptr<ExprNode> arg_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(4), UINT32_C(5))));
  arg_nodes.push_back(move(arg_node));
  vector<TokenInfo> arg_separator_tokens;
  CallNode call_node(
      move(operand_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(3)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)));
  CallWithUnsupportedTypeError problem(
      call_node, unique_ptr<DataType>(new BoolDataType()));
  string expected_msg =
      "[Error] %1%:0:1: \"bool\" data type is not supported by operator \"()\"\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest,
       PrintSemanticProblemWithCallWithInvalidArgsCountError) {
  unique_ptr<ExprNode> operand_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(1))));
  vector< unique_ptr<ExprNode> > arg_nodes;
  unique_ptr<ExprNode> arg_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(4), UINT32_C(5))));
  arg_nodes.push_back(move(arg_node));
  vector<TokenInfo> arg_separator_tokens;
  CallNode call_node(
      move(operand_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(3)),
      move(arg_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(7)));
  size_t expected_args_count = 0;
  size_t actual_args_count = 1;
  CallWithInvalidArgsCountError problem(
      call_node, expected_args_count, actual_args_count);
  string expected_msg = "[Error] %1%:2:3: expected 0 argument(s), but got 1\n";
  TestPrintSemanticProblem(problem, expected_msg);
}

TEST_F(SimpleMsgPrinterTest, PrintSemanticProblems) {
  SemanticAnalysis::ProgramProblems problems;
  MsgPrinter::ProgramFilePaths file_paths;
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ProgramNode program_node(move(stmt_nodes));
  IntNode int_node(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(1)));
  unique_ptr<SemanticProblem> problem(
      new IntWithOutOfRangeValueError(int_node));
  problems[&program_node].push_back(move(problem));
  path file_path("src/module.rts");
  file_paths[&program_node] = file_path;

  vector< unique_ptr<StmtNode> > stmt_nodes2;
  ProgramNode program_node2(move(stmt_nodes2));
  IdNode id_node(
      TokenInfo(Token::kName, "id", UINT32_C(2), UINT32_C(3)));
  unique_ptr<SemanticProblem> problem2(new IdWithoutDefError(id_node));
  problems[&program_node2].push_back(move(problem2));
  path file_path2("src/module2.rts");
  file_paths[&program_node2] = file_path2;
  string expected_output = (format("[Error] %2%:2:3: undefined identifier\n"
                                   "[Error] %1%:0:1: out of range value\n")
                            % file_path.string() % file_path2.string()).str();
  stringstream stream;
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintSemanticProblems(problems, file_paths);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}
}
}
