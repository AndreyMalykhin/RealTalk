
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <vector>
#include "real_talk/semantic/semantic_analyzer.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/util/file.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/file_parser.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler.h"

using std::move;
using std::vector;
using std::unique_ptr;
using boost::filesystem::path;
using testing::InvokeWithoutArgs;
using testing::Test;
using testing::Eq;
using testing::Return;
using testing::ByRef;
using real_talk::parser::ProgramNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::code::Code;
using real_talk::code::CodeGenerator;
using real_talk::util::File;
using real_talk::util::DirCreator;

namespace real_talk {
namespace compiler {

class CompilerConfigParserMock: public CompilerConfigParser {
 public:
  MOCK_CONST_METHOD3(Parse, void(int, const char*[], CompilerConfig*));
};

class FileParserMock: public FileParser {
 public:
  MOCK_CONST_METHOD4(Parse, FileParser::Programs(
      const path&, const path&, const path&, const vector<path>&));
};

class SemanticAnalyzerMock: public SemanticAnalyzer {
 public:
  MOCK_METHOD2(Analyze, SemanticAnalysis(
      const ProgramNode&, const SemanticAnalyzer::ImportPrograms&));
};

class MsgPrinterMock: public MsgPrinter {
 public:
  MOCK_CONST_METHOD1(
      PrintProblems, void(const SemanticAnalysis::ProgramProblems&));
};

class CodeGeneratorMock: public CodeGenerator {
 public:
  MOCK_METHOD4(Generate, void(
      const ProgramNode&, const SemanticAnalysis&, uint32_t, Code*));
};

class DirCreatorMock: public DirCreator {
 public:
  MOCK_CONST_METHOD1(Create, void(const path&));
};

class FileMock: public File {
 public:
  MOCK_METHOD1(Open, void(const path&));
  MOCK_METHOD1(Write, void(const Code&));
};

class CompilerTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(CompilerTest, Compile) {
  int argc = 4;
  char *argv[] = {"realtalkc",
                  "--import=/mylib",
                  "--import=/mylib2",
                  "app/module/component.rts"};
  path input_file_path("app/module/component.rts");
  CompilerConfig compiler_config(input_file_path);
  compiler_config.SetSrcDirPath("src2");
  compiler_config.SetBinDirPath("build2/bin2");
  compiler_config.SetImportDirPaths(
      vector<path>({"/mylib", "/mylib2"}));
  compiler_config.SetVendorDirPath("vendor2");
  CompilerConfigParserMock compiler_config_parser;
  EXPECT_CALL(compiler_config_parser, Parse(argc, argv, &compiler_config))
      .Times(1);

  unique_ptr<ProgramNode> main_program;
  SemanticAnalysis::ImportPrograms import_programs;
  FileParser::Programs programs(move(main_program), move(import_programs));
  FileParserMock file_parser;
  EXPECT_CALL(file_parser, Parse(compiler_config.GetInputFilePath(),
                                 compiler_config.GetSrcDirPath(),
                                 compiler_config.GetVendorDirPath(),
                                 compiler_config.GetImportDirPaths()))
      .Times(1)
      .WillOnce(Return(InvokeWithoutArgs([&programs]() {
              return programs;
            })));

  SemanticAnalysis::ProgramProblems semantic_problems;
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis semantic_analysis(
      move(semantic_problems), move(node_analyzes));
  SemanticAnalyzerMock semantic_analyzer;
  EXPECT_CALL(semantic_analyzer, Analyze(Eq(ByRef(programs.GetMain())),
                                         Eq(ByRef(programs.GetImport()))))
      .Times(1)
      .WillOnce(Return(InvokeWithoutArgs([&semantic_analysis]() {
              return semantic_analysis;
            })));

  MsgPrinterMock msg_printer;
  EXPECT_CALL(msg_printer,
              PrintProblems(Eq(ByRef(semantic_analysis.GetProblems()))))
      .Times(1);

  Code code;
  uint32_t code_version = UINT32_C(1);
  CodeGeneratorMock code_generator;
  EXPECT_CALL(code_generator, Generate(Eq(ByRef(programs.GetMain())),
                                       Eq(ByRef(semantic_analysis)),
                                       code_version,
                                       &code))
      .Times(1);

  DirCreatorMock dir_creator;
  path output_dir_path("build2/bin2/app/module");
  EXPECT_CALL(dir_creator, Create(output_dir_path))
      .Times(1);

  FileMock file;
  path output_file_path("build2/bin2/app/module/component.rtm");
  EXPECT_CALL(file, Open(output_file_path))
      .Times(1);
  EXPECT_CALL(file, Write(Eq(ByRef(code))))
      .Times(1);

  Compiler compiler(compiler_config_parser,
                    file_parser,
                    semantic_analyzer,
                    code_generator,
                    msg_printer,
                    dir_creator,
                    &compiler_config,
                    &file,
                    &code);
  compiler_config.Compile(argc, argv);
}
}
}
