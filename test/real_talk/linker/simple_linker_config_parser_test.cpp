
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/linker/linker_config.h"
#include "real_talk/linker/linker_cmd.h"
#include "real_talk/linker/simple_linker_config_parser.h"

using std::vector;
using std::string;
using testing::Test;
using boost::filesystem::path;

namespace real_talk {
namespace linker {

class SimpleLinkerConfigParserTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestParse(int argc,
                 const char *argv[],
                 const LinkerConfig &expected_config,
                 LinkerCmd expected_cmd) {
    LinkerConfig actual_config;
    LinkerCmd actual_cmd;
    SimpleLinkerConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    ASSERT_EQ(expected_config, actual_config);
    ASSERT_EQ(expected_cmd, actual_cmd);
  }
};

TEST_F(SimpleLinkerConfigParserTest, ParseWithAllOptions) {
  int argc = 10;
  const char *argv[] = {"realtalkl",
                        "--debug",
                        "--help",
                        "--import=/mylib",
                        "-i",
                        "/mylib2",
                        "--output-type=lib",
                        "app.rtl",
                        "app/module/component.rtm",
                        "app/module/component2.rtm"};
  LinkerConfig expected_config;
  expected_config.SetDebug(true);
  expected_config.SetOutputFileType(OutputFileType::kLib);
  expected_config.SetOutputFilePath(path("app.rtl"));
  expected_config.SetInputFilePaths(
      vector<path>({"app/module/component.rtm", "app/module/component2.rtm"}));
  expected_config.SetImportDirPaths(
      vector<path>({"/mylib", "/mylib2"}));
  LinkerCmd expected_cmd = LinkerCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleLinkerConfigParserTest, ParseWithRequiredOptions) {
  int argc = 4;
  const char *argv[] = {"realtalkl",
                        "app.rte",
                        "app/module/component.rtm",
                        "app/module/component2.rtm"};
  LinkerConfig expected_config;
  expected_config.SetOutputFileType(OutputFileType::kExe);
  expected_config.SetOutputFilePath(path("app.rte"));
  expected_config.SetInputFilePaths(
      vector<path>({"app/module/component.rtm", "app/module/component2.rtm"}));
  LinkerCmd expected_cmd = LinkerCmd::kLink;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleLinkerConfigParserTest, ParseWithHelpOption) {
  int argc = 2;
  const char *argv[] = {"realtalkl", "--help"};
  LinkerConfig expected_config;
  LinkerCmd expected_cmd = LinkerCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleLinkerConfigParserTest, ParseWithoutOptions) {
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig expected_config;
  LinkerCmd expected_cmd = LinkerCmd::kHelp;
  TestParse(argc, argv, expected_config, expected_cmd);
}

TEST_F(SimpleLinkerConfigParserTest, ParseWithoutOutputOption) {
  int argc = 2;
  const char *argv[] = {"realtalkl", "--input=app/module/component.rtm"};
  LinkerConfig actual_config;
  LinkerCmd actual_cmd;

  try {
    SimpleLinkerConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    FAIL();
  } catch (const LinkerConfigParser::BadArgsError&) {}
}

TEST_F(SimpleLinkerConfigParserTest, ParseWithoutInputOption) {
  int argc = 2;
  const char *argv[] = {"realtalkl", "--output=app.rte"};
  LinkerConfig actual_config;
  LinkerCmd actual_cmd;

  try {
    SimpleLinkerConfigParser().Parse(argc, argv, &actual_config, &actual_cmd);
    FAIL();
  } catch (const LinkerConfigParser::BadArgsError&) {}
}

TEST_F(SimpleLinkerConfigParserTest, GetHelp) {
  string expected_help =
      "Options:\n"
      "  -h [ --help ] \n"
      "  --input arg              Input file\n"
      "  --output arg             Output file\n"
      "  --output-type arg (=exe) Output file type\n"
      "  -i [ --import ] arg      Path to project, where input files will be searched\n"
      "  -d [ --debug ]           Display debug info\n";
  string actual_help = SimpleLinkerConfigParser().GetHelp();
  ASSERT_EQ(expected_help, actual_help);
}
}
}
