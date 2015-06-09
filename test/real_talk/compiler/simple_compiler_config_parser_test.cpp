
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/compiler_cmd.h"
#include "real_talk/compiler/simple_compiler_config_parser.h"

using std::vector;
using std::string;
using testing::Test;
using boost::filesystem::path;

namespace real_talk {
namespace compiler {

class SimpleCompilerConfigParserTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestParse(int argc,
                 const char *argv[],
                 const CompilerConfig &expected_config,
                 CompilerCmd expected_cmd) {
    CompilerConfig actual_config;
    CompilerCmd actual_cmd;
    SimpleCompilerConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    ASSERT_EQ(expected_config, actual_config);
    ASSERT_EQ(expected_cmd, actual_cmd);
  }
};

TEST_F(SimpleCompilerConfigParserTest, ParseWithAllOptions) {
  int argc = 7;
  const char *argv[] = {"realtalkc",
                        "--debug",
                        "--help",
                        "--import=/mylib",
                        "-i",
                        "/mylib2",
                        "app/module/component.rts"};
  CompilerConfig expected_config;
  expected_config.SetDebug(true);
  expected_config.SetInputFilePath(path("app/module/component.rts"));
  expected_config.SetImportDirPaths(
      vector<path>({{"/mylib"}, {"/mylib2"}}));
  CompilerCmd expected_cmd = CompilerCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleCompilerConfigParserTest, ParseWithInputOption) {
  int argc = 2;
  const char *argv[] = {"realtalkc", "app/module/component.rts"};
  CompilerConfig expected_config;
  expected_config.SetInputFilePath(path("app/module/component.rts"));
  CompilerCmd expected_cmd = CompilerCmd::kCompile;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleCompilerConfigParserTest, ParseWithHelpOption) {
  int argc = 2;
  const char *argv[] = {"realtalkc", "--help"};
  CompilerConfig expected_config;
  CompilerCmd expected_cmd = CompilerCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleCompilerConfigParserTest, ParseWithoutOptions) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  CompilerConfig expected_config;
  CompilerCmd expected_cmd = CompilerCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleCompilerConfigParserTest, ParseWithoutInputOption) {
  int argc = 2;
  const char *argv[] = {"realtalkc", "--import=/mylib"};
  CompilerConfig actual_config;
  CompilerCmd actual_cmd;

  try {
    SimpleCompilerConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    FAIL();
  } catch (const CompilerConfigParser::BadArgsError&) {}
}

TEST_F(SimpleCompilerConfigParserTest, GetHelp) {
  string expected_help =
      "Options:\n"
      "  -h [ --help ] \n"
      "  --input arg           Input file\n"
      "  -i [ --import ] arg   Folder, where import files will be searched\n"
      "  -d [ --debug ]        Display debug info\n";
  string actual_help = SimpleCompilerConfigParser().GetHelp();
  ASSERT_EQ(expected_help, actual_help);
}
}
}
