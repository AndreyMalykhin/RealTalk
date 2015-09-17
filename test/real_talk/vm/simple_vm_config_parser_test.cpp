
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/vm/vm_config.h"
#include "real_talk/vm/vm_cmd.h"
#include "real_talk/vm/simple_vm_config_parser.h"

using std::vector;
using std::string;
using testing::Test;
using boost::filesystem::path;

namespace real_talk {
namespace vm {

class SimpleVMConfigParserTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestParse(int argc,
                 const char *argv[],
                 const VMConfig &expected_config,
                 VMCmd expected_cmd) {
    VMConfig actual_config;
    VMCmd actual_cmd;
    SimpleVMConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    ASSERT_EQ(expected_config, actual_config);
    ASSERT_EQ(expected_cmd, actual_cmd);
  }
};

TEST_F(SimpleVMConfigParserTest, ParseWithAllOptions) {
  int argc = 4;
  const char *argv[] = {"realtalkvm", "--debug", "--help", "/myapp/main.rte"};
  VMConfig expected_config;
  expected_config.SetDebug(true);
  expected_config.SetInputFilePath("/myapp/main.rte");
  VMCmd expected_cmd = VMCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleVMConfigParserTest, ParseWithRequiredOptions) {
  int argc = 2;
  const char *argv[] = {"realtalkvm", "/myapp/main.rte"};
  VMConfig expected_config;
  expected_config.SetInputFilePath("/myapp/main.rte");
  VMCmd expected_cmd = VMCmd::kExecute;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleVMConfigParserTest, ParseWithHelpOption) {
  int argc = 2;
  const char *argv[] = {"realtalkvm", "--help"};
  VMConfig expected_config;
  VMCmd expected_cmd = VMCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleVMConfigParserTest, ParseWithoutOptions) {
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig expected_config;
  VMCmd expected_cmd = VMCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleVMConfigParserTest, ParseWithoutInputOption) {
  int argc = 2;
  const char *argv[] = {"realtalkl", "--debug"};
  VMConfig actual_config;
  VMCmd actual_cmd;

  try {
    SimpleVMConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    FAIL();
  } catch (const VMConfigParser::BadArgsError&) {}
}

TEST_F(SimpleVMConfigParserTest, GetHelp) {
  string expected_help = "Options:\n"
                         "  -h [ --help ] \n"
                         "  --input arg           Input file\n"
                         "  -d [ --debug ]        Display debug info\n";
  string actual_help = SimpleVMConfigParser().GetHelp();
  ASSERT_EQ(expected_help, actual_help);
}
}
}
