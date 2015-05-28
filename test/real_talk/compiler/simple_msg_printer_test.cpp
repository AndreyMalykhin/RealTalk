
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/compiler/simple_msg_printer.h"

using std::stringstream;
using std::string;
using boost::format;
using boost::filesystem::path;
using testing::Test;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Token;

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
  TokenInfo token(Token::kStringLit, "\"abc\"", UINT32_C(1), UINT32_C(2));
  string expected_output =
      (format("[Error] %1%:1:2: unexpected token \"\"abc\"\"\n")
       % file_path.string()).str();
  SimpleMsgPrinter msg_printer(&stream);
  msg_printer.PrintUnexpectedTokenError(token, file_path);
  string actual_output = stream.str();
  ASSERT_EQ(expected_output, actual_output);
}
}
}
