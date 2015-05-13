
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

using std::vector;
using testing::Test;

namespace real_talk {
namespace compiler {

class CompilerTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(CompilerTest, Compile) {
  path input_file_path("./app/module/component.rts");
  CompilerConfig compiler_config(input_file_path);
  compiler_config.SetSrcDirPath("./src");
  compiler_config.SetBinDirPath("./build/bin");
  EXPECT_CALL(file_parser, Parse(compiler_config.GetInputFilePath(),
                                 compiler_config.GetSrcDirPath(),
                                 compiler_config.GetImportDirPaths()))
      .Times(1)
      .WillOnce(Return(InvokeWithoutArgs([&programs]() {
              return programs;
            })));

  EXPECT_CALL(semantic_analyzer, Analyze(Eq(ByRef(programs.GetMain())),
                                         Eq(ByRef(programs.GetImport()))))
      .Times(1)
      .WillOnce(Return(InvokeWithoutArgs([&semantic_analysis]() {
              return semantic_analysis;
            })));

  EXPECT_CALL(msg_printer,
              PrintProblems(Eq(ByRef(semantic_analysis.GetProblems()))))
      .Times(1);

  Code code;
  uint32_t code_version = UINT32_C(1);
  EXPECT_CALL(code_generator, Generate(Eq(ByRef(programs.GetMain())),
                                       Eq(ByRef(semantic_analysis)),
                                       code_version,
                                       &code))
      .Times(1);

  path output_dir_path("./build/bin/app/module");
  EXPECT_CALL(dir_creator, Create(output_dir_path))
      .Times(1);

  path output_file_path("./build/bin/app/module/component.rtm");
  EXPECT_CALL(file, Open(output_file_path))
      .Times(1);
  EXPECT_CALL(file, Write(Eq(ByRef(code))))
      .Times(1);

  compiler_config.Compile(argc, argv, compiler_config, code);
}
}
}
