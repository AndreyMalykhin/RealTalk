
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/format.hpp>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include "real_talk/parser/program_node.h"
#include "real_talk/semantic/semantic_analyzer.h"
#include "real_talk/semantic/semantic_error.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/util/file.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/errors.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/file_parser.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler.h"

using std::move;
using std::vector;
using std::unique_ptr;
using std::ostream;
using std::string;
using boost::filesystem::path;
using boost::format;
using testing::InvokeWithoutArgs;
using testing::Test;
using testing::Eq;
using testing::Return;
using testing::ByRef;
using testing::Ref;
using testing::_;
using testing::Throw;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::SemanticError;
using real_talk::semantic::SemanticProblem;
using real_talk::semantic::SemanticProblemVisitor;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::code::Code;
using real_talk::code::CodeGenerator;
using real_talk::util::File;
using real_talk::util::DirCreator;
using real_talk::util::IOError;

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
    vector<string> import_dir_path_strings;

    for (const path &path: import_dir_paths) {
      import_dir_path_strings.push_back(path.string());
    }

    return unique_ptr<Programs>(Parse_(file_path.string(),
                                       src_dir_path.string(),
                                       vendor_dir_path.string(),
                                       import_dir_path_strings));
  }

  MOCK_CONST_METHOD4(Parse_, Programs*(
      const string&, const string&, const string&, const vector<string>&));
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
      PrintSemanticProblems, void(const SemanticAnalysis::ProgramProblems&));
  MOCK_CONST_METHOD1(PrintError, void(const string&));
};

class CodeGeneratorMock: public CodeGenerator {
 public:
  MOCK_METHOD4(Generate, void(
      const ProgramNode&, const SemanticAnalysis&, uint32_t, Code*));
};

class DirCreatorMock: public DirCreator {
 public:
  virtual void Create(const path &dir_path) const override {
    Create_(dir_path.string());
  }

  MOCK_CONST_METHOD1(Create_, void(const string&));
};

class FileMock: public File {
 public:
  virtual void Open(const path &file_path) override {
    Open_(file_path.string());
  }

  MOCK_METHOD1(Open_, void(const string&));
  MOCK_METHOD1(Write, void(const Code&));
};

class SemanticErrorMock: public SemanticError {
 public:
  virtual void Accept(SemanticProblemVisitor*) override {assert(false);}
  virtual void Print(ostream&) const override {}
  virtual bool IsEqual(const SemanticProblem&) const override {return true;}
};

class CompilerTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(CompilerTest, Compile) {
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
  config.SetModuleFileExtension("rtm2");
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
  vector<string> import_dir_path_strings;

  for (const path &path: config.GetImportDirPaths()) {
    import_dir_path_strings.push_back(path.string());
  }

  FileParserMock file_parser;
  EXPECT_CALL(file_parser, Parse_(config.GetInputFilePath().string(),
                                  config.GetSrcDirPath().string(),
                                  config.GetVendorDirPath().string(),
                                  import_dir_path_strings))
      .Times(1)
      .WillOnce(Return(programs.release()));

  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(&(programs_ptr->GetMain()), SemanticAnalysis::Problems()));
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
              PrintSemanticProblems(Ref(semantic_analysis_ptr->GetProblems())))
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
  EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
      .Times(1);

  FileMock file;
  path output_file_path("build2/bin2/app/module/component.rtm2");
  EXPECT_CALL(file, Open_(output_file_path.string()))
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

TEST_F(CompilerTest, CompileWithSemanticErrors) {
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
  config.SetModuleFileExtension("rtm2");
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
  vector<string> import_dir_path_strings;

  for (const path &path: config.GetImportDirPaths()) {
    import_dir_path_strings.push_back(path.string());
  }

  FileParserMock file_parser;
  EXPECT_CALL(file_parser, Parse_(config.GetInputFilePath().string(),
                                  config.GetSrcDirPath().string(),
                                  config.GetVendorDirPath().string(),
                                  import_dir_path_strings))
      .Times(1)
      .WillOnce(Return(programs.release()));

  SemanticAnalysis::ProgramProblems semantic_problems;
  unique_ptr<SemanticProblem> semantic_error(new SemanticErrorMock());
  semantic_problems[&(programs_ptr->GetMain())].push_back(
      move(semantic_error));
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
              PrintSemanticProblems(Ref(semantic_analysis_ptr->GetProblems())))
      .Times(1);

  CodeGeneratorMock code_generator;
  EXPECT_CALL(code_generator, Generate(_, _, _, _))
      .Times(0);

  DirCreatorMock dir_creator;
  EXPECT_CALL(dir_creator, Create_(_))
      .Times(0);

  FileMock file;
  EXPECT_CALL(file, Open_(_))
      .Times(0);
  EXPECT_CALL(file, Write(_))
      .Times(0);

  Code code;
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

TEST_F(CompilerTest, CompileWithIOErrorWhileWritingOutputFile) {
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
  config.SetModuleFileExtension("rtm2");
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
  vector<string> import_dir_path_strings;

  for (const path &path: config.GetImportDirPaths()) {
    import_dir_path_strings.push_back(path.string());
  }

  FileParserMock file_parser;
  EXPECT_CALL(file_parser, Parse_(config.GetInputFilePath().string(),
                                  config.GetSrcDirPath().string(),
                                  config.GetVendorDirPath().string(),
                                  import_dir_path_strings))
      .Times(1)
      .WillOnce(Return(programs.release()));

  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(&(programs_ptr->GetMain()), SemanticAnalysis::Problems()));
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
              PrintSemanticProblems(Ref(semantic_analysis_ptr->GetProblems())))
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
  EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
      .Times(1);

  FileMock file;
  path output_file_path("build2/bin2/app/module/component.rtm2");
  EXPECT_CALL(file, Open_(output_file_path.string()))
      .Times(1);
  IOError io_error("test");
  EXPECT_CALL(file, Write(Ref(code)))
      .Times(1)
      .WillOnce(Throw(io_error));

  string msg =
      "Failed to write to file \"build2/bin2/app/module/component.rtm2\"";
  EXPECT_CALL(msg_printer, PrintError(msg))
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

TEST_F(CompilerTest, CompileWithIOErrorWhileOpeningOutputFile) {
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
  config.SetModuleFileExtension("rtm2");
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
  vector<string> import_dir_path_strings;

  for (const path &path: config.GetImportDirPaths()) {
    import_dir_path_strings.push_back(path.string());
  }

  FileParserMock file_parser;
  EXPECT_CALL(file_parser, Parse_(config.GetInputFilePath().string(),
                                  config.GetSrcDirPath().string(),
                                  config.GetVendorDirPath().string(),
                                  import_dir_path_strings))
      .Times(1)
      .WillOnce(Return(programs.release()));

  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(&(programs_ptr->GetMain()), SemanticAnalysis::Problems()));
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
              PrintSemanticProblems(Ref(semantic_analysis_ptr->GetProblems())))
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
  EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
      .Times(1);

  FileMock file;
  path output_file_path("build2/bin2/app/module/component.rtm2");
  IOError io_error("test");
  EXPECT_CALL(file, Open_(output_file_path.string()))
      .Times(1)
      .WillOnce(Throw(io_error));
  EXPECT_CALL(file, Write(_))
      .Times(0);

  string msg =
      "Failed to open file \"build2/bin2/app/module/component.rtm2\"";
  EXPECT_CALL(msg_printer, PrintError(msg))
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
