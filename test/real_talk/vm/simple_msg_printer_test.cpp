
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "real_talk/vm/simple_msg_printer.h"

using std::string;
using std::stringstream;
using testing::Test;

namespace real_talk {
namespace vm {

class VMSimpleMsgPrinterTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(VMSimpleMsgPrinterTest, PrintError) {
  const string error = "test";
  const string expected_output = "[Error] test";
  stringstream output_stream;
  SimpleMsgPrinter printer(&output_stream);
  printer.PrintError(error);
  const string &actual_output = output_stream.str();
  ASSERT_EQ(expected_output, actual_output);
}

TEST_F(VMSimpleMsgPrinterTest, PrintHelp) {
  const string help = "test";
  const string expected_output = "test";
  stringstream output_stream;
  SimpleMsgPrinter printer(&output_stream);
  printer.PrintHelp(help);
  const string &actual_output = output_stream.str();
  ASSERT_EQ(expected_output, actual_output);
}
}
}
