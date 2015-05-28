
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <memory>
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/semantic/semantic_problems.h"
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
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::ExprNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::semantic::ArrayAllocWithTooManyDimensionsError;

namespace real_talk {
namespace compiler {

class SimpleMsgPrinterTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
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
  stringstream stream;
  path file_path("src/app.rts");
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
  string expected_output =
      (format("[Error] %1%:2:3: array can't have more than 7 dimensions\n")
       % file_path.string()).str();
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintSemanticProblem(problem, file_path);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}
}
}
