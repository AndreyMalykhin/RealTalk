
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include "real_talk/parser/program_node.h"
#include "real_talk/semantic/semantic_analyzer.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/util/file.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/file_parser.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler.h"
#include "real_talk/test/path_printer.h"

using std::move;
using std::vector;
using std::unique_ptr;
using std::ostream;
using boost::filesystem::path;
using testing::InvokeWithoutArgs;
using testing::Test;
using testing::Eq;
using testing::Return;
using testing::ByRef;
using testing::Ref;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
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
  virtual unique_ptr<Programs> Parse(
      const path &file_path,
      const path &src_dir_path,
      const path &vendor_dir_path,
      const vector<path> &import_dir_paths) const override {
    return unique_ptr<Programs>(
        Parse_(file_path, src_dir_path, vendor_dir_path, import_dir_paths));
  }

  MOCK_CONST_METHOD4(Parse_, Programs*(
      const path&, const path&, const path&, const vector<path>&));
};

class SemanticAnalyzerMock: public SemanticAnalyzer {
 public:
  virtual unique_ptr<SemanticAnalysis> Analyze(
      const ProgramNode &main_program,
      const ImportPrograms &import_programs) override {
    return unique_ptr<SemanticAnalysis>(
        Analyze_(main_program, import_programs));
  }

  MOCK_METHOD2(Analyze_,
               SemanticAnalysis*(const ProgramNode&, const ImportPrograms&));
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
  ASSERT_EQ(path("test"), path("test2"));

  int argc = 4;
  const char *argv[] = {"realtalkc",
                        "--import=/mylib",
                        "--import=/mylib2",
                        "app/module/component.rts"};
  path input_file_path("app/module/component.rts");
  CompilerConfig config(input_file_path);
  config.SetSrcDirPath("src2");
  config.SetBinDirPath("build2/bin2");
  config.SetVendorDirPath("vendor2");
  config.SetImportDirPaths(vector<path>({"/mylib", "/mylib2"}));
  CompilerConfigParserMock config_parser;
  EXPECT_CALL(config_parser, Parse(argc, argv, &config))
      .Times(1);

  unique_ptr<ProgramNode> main_program(
      new ProgramNode(vector< unique_ptr<StmtNode> >()));
  FileParser::ImportPrograms import_programs;
  auto *programs_ptr = new FileParser::Programs(
      move(main_program), move(import_programs));
  unique_ptr<FileParser::Programs> programs(programs_ptr);
  FileParserMock file_parser;
  EXPECT_CALL(file_parser, Parse_(config.GetInputFilePath(),
                                  config.GetSrcDirPath(),
                                  config.GetVendorDirPath(),
                                  config.GetImportDirPaths()))
      .Times(1)
      .WillOnce(Return(programs.release()));

  SemanticAnalysis::ProgramProblems semantic_problems;
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis_ptr = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  unique_ptr<SemanticAnalysis> semantic_analysis(semantic_analysis_ptr);
  SemanticAnalyzerMock semantic_analyzer;
  EXPECT_CALL(semantic_analyzer, Analyze_(Ref(programs_ptr->GetMain()),
                                          Ref(programs_ptr->GetImport())))
      .Times(1)
      .WillOnce(Return(semantic_analysis.release()));

  MsgPrinterMock msg_printer;
  EXPECT_CALL(msg_printer,
              PrintProblems(Ref(semantic_analysis_ptr->GetProblems())))
      .Times(1);

  Code code;
  uint32_t code_version = UINT32_C(1);
  CodeGeneratorMock code_generator;
  EXPECT_CALL(code_generator, Generate(Ref(programs_ptr->GetMain()),
                                       Ref(*semantic_analysis_ptr),
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
  EXPECT_CALL(file, Write(Ref(code)))
      .Times(1);

  Compiler compiler(config_parser,
                    file_parser,
                    &semantic_analyzer,
                    &code_generator,
                    msg_printer,
                    dir_creator,
                    &config,
                    &file,
                    &code);
  compiler.Compile(argc, argv);
}
}
}
