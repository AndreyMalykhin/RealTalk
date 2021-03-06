
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/format.hpp>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/lexer_factory.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/parser.h"
#include "real_talk/semantic/semantic_analyzer.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/module_writer.h"
#include "real_talk/util/file.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/errors.h"
#include "real_talk/util/file_searcher.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler_cmd.h"
#include "real_talk/compiler/compiler_app.h"

using std::move;
using std::unordered_map;
using std::istream;
using std::vector;
using std::unique_ptr;
using std::ostream;
using std::string;
using std::stringstream;
using std::equal;
using std::make_pair;
using boost::filesystem::path;
using boost::format;
using testing::InvokeWithoutArgs;
using testing::Test;
using testing::NotNull;
using testing::Invoke;
using testing::Truly;
using testing::SetArgPointee;
using testing::Eq;
using testing::Return;
using testing::IsEmpty;
using testing::ByRef;
using testing::Ref;
using testing::_;
using testing::Throw;
using testing::InSequence;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Lexer;
using real_talk::lexer::LexerFactory;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::parser::ImportNode;
using real_talk::parser::StringNode;
using real_talk::parser::Parser;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::SemanticError;
using real_talk::semantic::SemanticProblem;
using real_talk::semantic::StringWithEmptyHexValueError;
using real_talk::semantic::StringWithOutOfRangeHexValueError;
using real_talk::semantic::SemanticProblemVisitor;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::semantic::LitParser;
using real_talk::code::Code;
using real_talk::code::IdSize;
using real_talk::code::IdAddress;
using real_talk::code::IdAddresses;
using real_talk::code::CodeGenerator;
using real_talk::code::Module;
using real_talk::code::ModuleWriter;
using real_talk::util::File;
using real_talk::util::DirCreator;
using real_talk::util::IOError;
using real_talk::util::FileSearcher;

namespace real_talk {
namespace compiler {
namespace {

class CompilerConfigParserMock: public CompilerConfigParser {
 public:
  MOCK_CONST_METHOD4(
      Parse, void(int, const char*[], CompilerConfig*, CompilerCmd*));
  MOCK_CONST_METHOD0(GetHelp, string());
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
  MOCK_CONST_METHOD2(PrintSemanticProblems, void(
      const SemanticAnalysis::ProgramProblems&, const ProgramFilePaths&));
  MOCK_CONST_METHOD2(
      PrintSemanticProblem, void(const SemanticProblem&, const path&));
  MOCK_CONST_METHOD1(PrintError, void(const string&));
  MOCK_CONST_METHOD3(
      PrintTokenError, void(const TokenInfo&, const path&, const string&));
  MOCK_CONST_METHOD2(
      PrintUnexpectedTokenError, void(const TokenInfo&, const path&));
  MOCK_CONST_METHOD4(
      PrintUnexpectedCharError, void(char, uint32_t, uint32_t, const path&));
  MOCK_CONST_METHOD1(PrintHelp, void(const string&));
};

class CodeGeneratorMock: public CodeGenerator {
 public:
  virtual unique_ptr<Module> Generate(
      const ProgramNode &program,
      const SemanticAnalysis &semantic_analysis,
      uint32_t version) override {
    return unique_ptr<Module>(Generate_(program, semantic_analysis, version));
  }

  MOCK_METHOD3(Generate_, Module*(
      const ProgramNode&, const SemanticAnalysis&, uint32_t));
};

class ModuleWriterMock: public ModuleWriter {
 public:
  MOCK_CONST_METHOD2(Write, void(const Module&, Code*));
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
  virtual unique_ptr<istream> Read(const path &file_path) const override {
    return unique_ptr<istream>(Read_(file_path.string()));
  }

  virtual void Write(const path &file_path, const Code &code) const override {
    Write_(file_path.string(), code);
  }

  MOCK_CONST_METHOD1(Read_, istream*(const string&));
  MOCK_CONST_METHOD2(Write_, void(const string&, const Code&));
};

class LitParserMock: public LitParser {
 public:
  MOCK_CONST_METHOD1(ParseString, string(const string&));

  virtual bool ParseBool(const string&) const override {
    assert(false);
    return false;
  }

  virtual int32_t ParseInt(const string&) const override {
    assert(false);
    return 0;
  }

  virtual int64_t ParseLong(const string&) const override {
    assert(false);
    return 0;
  }

  virtual double ParseDouble(const string&) const override {
    assert(false);
    return 0.0;
  }

  virtual char ParseChar(const string&) const override {
    assert(false);
    return 0;
  }
};

class FileSearcherMock: public FileSearcher {
 public:
  virtual path Search(const path &file_path,
                      const path &src_dir_path,
                      const path &vendor_dir_path,
                      const vector<path> &import_dir_paths) const override {
    vector<string> import_dir_path_strs;

    for (const path &import_dir_path: import_dir_paths) {
      import_dir_path_strs.push_back(import_dir_path.string());
    }

    return path(Search_(file_path.string(),
                        src_dir_path.string(),
                        vendor_dir_path.string(),
                        import_dir_path_strs));
  }

  MOCK_CONST_METHOD4(Search_, string(const string&,
                                     const string&,
                                     const string&,
                                     const vector<string>&));
};

class LexerFactoryMock: public LexerFactory {
 public:
  virtual unique_ptr<Lexer> Create(istream &stream) const override {
    return unique_ptr<Lexer>(Create_(stream));
  }

  MOCK_CONST_METHOD1(Create_, Lexer*(istream&));
};

class SrcParserMock: public Parser {
 public:
  virtual unique_ptr<ProgramNode> Parse(Lexer *lexer) override {
    return unique_ptr<ProgramNode>(Parse_(lexer));
  }

  MOCK_METHOD1(Parse_, ProgramNode*(Lexer*));
};

class LexerMock: public Lexer {
 public:
  virtual TokenInfo GetNextToken() override {
    assert(false);
    return TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(0));
  }
};

class SemanticErrorMock: public SemanticError {
 public:
  virtual void Accept(const SemanticProblemVisitor*) const override {
    assert(false);
  }

  virtual void Print(ostream&) const override {}
  virtual bool IsEqual(const SemanticProblem&) const override {return true;}
};
}

class CompilerAppTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(CompilerAppTest, Compile) {
  struct TestImportFileParse {
    ImportNode *import_stmt;
    istream *import_file_stream;
    path search_import_file_path;
    path found_import_file_path;
    ProgramNode *import_program;
    Lexer *lexer;
  };

  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  path output_dir_path("build2/bin2/app/module");
  path output_file_path("build2/bin2/app/module/component.rtm2");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  config.SetBinDirPath("build2/bin2");
  config.SetVendorDirPath("vendor2");
  config.SetModuleFileExtension("rtm2");
  config.SetImportDirPaths(vector<path>({"/mylib", "/mylib2"}));
  vector<string> import_dir_paths;

  for (const path &path: config.GetImportDirPaths()) {
    import_dir_paths.push_back(path.string());
  }

  Code module_code;
  uint32_t code_version = UINT32_C(1);
  auto *module = new Module(
      code_version, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  vector< unique_ptr<StmtNode> > main_program_stmts;
  unique_ptr<StringNode> file_path_expr1(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import1.rts\"",
      UINT32_C(1),
      UINT32_C(1))));
  auto *import_stmt_ptr1 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr1),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt1(import_stmt_ptr1);
  main_program_stmts.push_back(move(import_stmt1));
  unique_ptr<StringNode> file_path_expr4(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import4.rts\"",
      UINT32_C(4),
      UINT32_C(4))));
  auto *import_stmt_ptr4 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_expr4),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_stmt4(import_stmt_ptr4);
  main_program_stmts.push_back(move(import_stmt4));
  unique_ptr<StringNode> file_path_expr7(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"./app/module/import4.rts\"",
      UINT32_C(7),
      UINT32_C(7))));
  auto *import_stmt_ptr7 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(6), UINT32_C(6)),
      move(file_path_expr7),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)));
  unique_ptr<StmtNode> import_stmt7(import_stmt_ptr7);
  main_program_stmts.push_back(move(import_stmt7));
  auto *main_program = new ProgramNode(move(main_program_stmts));

  vector<ProgramNode*> import_programs;
  vector< unique_ptr<StmtNode> > import_program_stmts1;
  unique_ptr<StringNode> file_path_expr2(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import2.rts\"",
      UINT32_C(1),
      UINT32_C(1))));
  auto *import_stmt_ptr2 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt2(import_stmt_ptr2);
  import_program_stmts1.push_back(move(import_stmt2));
  unique_ptr<StringNode> file_path_expr3(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import3.rts\"",
      UINT32_C(4),
      UINT32_C(4))));
  auto *import_stmt_ptr3 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_expr3),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_stmt3(import_stmt_ptr3);
  import_program_stmts1.push_back(move(import_stmt3));
  auto *import_program1 =
      new ProgramNode(move(import_program_stmts1));
  import_programs.push_back(import_program1);

  auto *import_program2 =
      new ProgramNode(vector< unique_ptr<StmtNode> >());
  import_programs.push_back(import_program2);
  auto *import_program3 =
      new ProgramNode(vector< unique_ptr<StmtNode> >());
  import_programs.push_back(import_program3);

  vector< unique_ptr<StmtNode> > import_program_stmts4;
  unique_ptr<StringNode> file_path_expr5(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import5.rts\"",
      UINT32_C(1),
      UINT32_C(1))));
  auto *import_stmt_ptr5 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr5),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt5(import_stmt_ptr5);
  import_program_stmts4.push_back(move(import_stmt5));
  unique_ptr<StringNode> file_path_expr6(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import6.rts\"",
      UINT32_C(4),
      UINT32_C(4))));
  auto *import_stmt_ptr6 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_expr6),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_stmt6(import_stmt_ptr6);
  import_program_stmts4.push_back(move(import_stmt6));
  auto *import_program4 =
      new ProgramNode(move(import_program_stmts4));
  import_programs.push_back(import_program4);

  auto *import_program5 =
      new ProgramNode(vector< unique_ptr<StmtNode> >());
  import_programs.push_back(import_program5);
  auto *import_program6 =
      new ProgramNode(vector< unique_ptr<StmtNode> >());
  import_programs.push_back(import_program6);

  MsgPrinter::ProgramFilePaths program_file_paths =
      {{main_program, final_input_file_path}};

  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(main_program, SemanticAnalysis::Problems()));

  for (const ProgramNode *import_program: import_programs) {
    semantic_problems.insert(
        make_pair(import_program, SemanticAnalysis::Problems()));
  }

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  vector<TestImportFileParse> test_import_file_parses;

  {
    auto *import_file_stream = new stringstream();
    path search_import_file_path("app/module/import1.rts");
    path found_import_file_path("src2/app/module/import1.rts");
    TestImportFileParse test_import_file_parse = {import_stmt_ptr1,
                                                  import_file_stream,
                                                  search_import_file_path,
                                                  found_import_file_path,
                                                  import_programs.at(0),
                                                  new LexerMock()};
    test_import_file_parses.push_back(move(test_import_file_parse));
    program_file_paths.insert(
        make_pair(import_programs.at(0), found_import_file_path));
  }

  {
    auto *import_file_stream = new stringstream();
    path search_import_file_path("app/module/import2.rts");
    path found_import_file_path("src2/app/module/import2.rts");
    TestImportFileParse test_import_file_parse = {import_stmt_ptr2,
                                                  import_file_stream,
                                                  search_import_file_path,
                                                  found_import_file_path,
                                                  import_programs.at(1),
                                                  new LexerMock()};
    test_import_file_parses.push_back(move(test_import_file_parse));
    program_file_paths.insert(
        make_pair(import_programs.at(1), found_import_file_path));
  }

  {
    auto *import_file_stream = new stringstream();
    path search_import_file_path("app/module/import3.rts");
    path found_import_file_path("src2/app/module/import3.rts");
    TestImportFileParse test_import_file_parse = {import_stmt_ptr3,
                                                  import_file_stream,
                                                  search_import_file_path,
                                                  found_import_file_path,
                                                  import_programs.at(2),
                                                  new LexerMock()};
    test_import_file_parses.push_back(move(test_import_file_parse));
    program_file_paths.insert(
        make_pair(import_programs.at(2), found_import_file_path));
  }

  {
    auto *import_file_stream = new stringstream();
    path search_import_file_path("app/module/import4.rts");
    path found_import_file_path("src2/app/module/import4.rts");
    TestImportFileParse test_import_file_parse = {import_stmt_ptr4,
                                                  import_file_stream,
                                                  search_import_file_path,
                                                  found_import_file_path,
                                                  import_programs.at(3),
                                                  new LexerMock()};
    test_import_file_parses.push_back(move(test_import_file_parse));
    program_file_paths.insert(
        make_pair(import_programs.at(3), found_import_file_path));
  }

  {
    auto *import_file_stream = new stringstream();
    path search_import_file_path("app/module/import5.rts");
    path found_import_file_path("src2/app/module/import5.rts");
    TestImportFileParse test_import_file_parse = {import_stmt_ptr5,
                                                  import_file_stream,
                                                  search_import_file_path,
                                                  found_import_file_path,
                                                  import_programs.at(4),
                                                  new LexerMock()};
    test_import_file_parses.push_back(move(test_import_file_parse));
    program_file_paths.insert(
        make_pair(import_programs.at(4), found_import_file_path));
  }

  {
    auto *import_file_stream = new stringstream();
    path search_import_file_path("app/module/import6.rts");
    path found_import_file_path("src2/app/module/import6.rts");
    TestImportFileParse test_import_file_parse = {import_stmt_ptr6,
                                                  import_file_stream,
                                                  search_import_file_path,
                                                  found_import_file_path,
                                                  import_programs.at(5),
                                                  new LexerMock()};
    test_import_file_parses.push_back(move(test_import_file_parse));
    program_file_paths.insert(
        make_pair(import_programs.at(5), found_import_file_path));
  }

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream))
        .RetiresOnSaturation();
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer))
        .RetiresOnSaturation();
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program))
        .RetiresOnSaturation();

    for (const TestImportFileParse &test_parse: test_import_file_parses) {
      string import_file_path_token =
          test_parse.import_stmt->GetFilePath()->GetStartToken().GetValue();
      EXPECT_CALL(lit_parser, ParseString(import_file_path_token))
          .Times(1)
          .WillOnce(Return(test_parse.search_import_file_path.string()))
          .RetiresOnSaturation();
      EXPECT_CALL(file_searcher, Search_(
          test_parse.search_import_file_path.string(),
          config.GetSrcDirPath().string(),
          config.GetVendorDirPath().string(),
          import_dir_paths))
          .Times(1)
          .WillOnce(Return(test_parse.found_import_file_path.string()))
          .RetiresOnSaturation();
      EXPECT_CALL(file, Read_(test_parse.found_import_file_path.string()))
          .Times(1)
          .WillOnce(Return(test_parse.import_file_stream))
          .RetiresOnSaturation();
      EXPECT_CALL(lexer_factory, Create_(Ref(*(test_parse.import_file_stream))))
          .Times(1)
          .WillOnce(Return(test_parse.lexer))
          .RetiresOnSaturation();
      EXPECT_CALL(src_parser, Parse_(test_parse.lexer))
          .Times(1)
          .WillOnce(Return(test_parse.import_program))
          .RetiresOnSaturation();
    }

    path search_import_file_path("src2/./app/module/import4.rts");
    path found_import_file_path("src2/app/module/import4.rts");
    EXPECT_CALL(lit_parser, ParseString(
        import_stmt_ptr7->GetFilePath()->GetStartToken().GetValue()))
        .Times(1)
        .WillOnce(Return(search_import_file_path.string()))
        .RetiresOnSaturation();
    EXPECT_CALL(file_searcher, Search_(
        search_import_file_path.string(),
        config.GetSrcDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_import_file_path.string()))
        .RetiresOnSaturation();

    auto import_programs_matcher = [&import_programs](
        const SemanticAnalyzer::ImportPrograms &arg) {
      return import_programs.size() == arg.size()
      && equal(import_programs.cbegin(),
               import_programs.cend(),
               arg.cbegin(),
               [](const ProgramNode *lhs, const unique_ptr<ProgramNode> &rhs) {
                 return lhs == rhs.get();
               });
    };
    EXPECT_CALL(semantic_analyzer, Analyze_(Ref(*main_program),
                                            Truly(import_programs_matcher)))
        .Times(1)
        .WillOnce(Return(semantic_analysis));
    EXPECT_CALL(msg_printer, PrintSemanticProblems(
        Ref(semantic_analysis->GetProblems()),
        program_file_paths))
        .Times(1);
    EXPECT_CALL(code_generator, Generate_(Ref(*main_program),
                                          Ref(*semantic_analysis),
                                          code_version))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(module_writer, Write(Ref(*module), &module_code))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
        .Times(1);
    EXPECT_CALL(file, Write_(output_file_path.string(), Ref(module_code)))
        .Times(1);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &module_code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, Help) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path;
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  string help = "test";

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kHelp));
    EXPECT_CALL(config_parser, GetHelp())
        .Times(1)
        .WillOnce(Return(help));
    EXPECT_CALL(msg_printer, PrintHelp(help))
        .Times(1);
    EXPECT_CALL(file, Read_(_))
        .Times(0);
    EXPECT_CALL(lexer_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(src_parser, Parse_(_))
        .Times(0);
    EXPECT_CALL(lit_parser, ParseString(_))
        .Times(0);
    EXPECT_CALL(file_searcher, Search_(_, _, _, _))
        .Times(0);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, ThereAreSemanticErrors) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  auto *main_program = new ProgramNode(vector< unique_ptr<StmtNode> >());
  vector<ProgramNode*> import_programs;
  SemanticAnalysis::ProgramProblems semantic_problems;
  unique_ptr<SemanticProblem> error(new SemanticErrorMock());
  semantic_problems[main_program].push_back(move(error));
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  MsgPrinter::ProgramFilePaths program_file_paths =
      {{main_program, final_input_file_path}};

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(semantic_analyzer, Analyze_(Ref(*main_program),
                                            IsEmpty()))
        .Times(1)
        .WillOnce(Return(semantic_analysis));
    EXPECT_CALL(msg_printer, PrintSemanticProblems(
        Ref(semantic_analysis->GetProblems()),
        program_file_paths))
        .Times(1);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, IOErrorWhileWritingFile) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  path output_dir_path("build2/bin2/app/module");
  path output_file_path("build2/bin2/app/module/component.rtm2");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  config.SetBinDirPath("build2/bin2");
  config.SetModuleFileExtension("rtm2");
  Code module_code;
  unique_ptr<Code> cmds_code(new Code());
  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t main_cmds_code_size = UINT32_C(0);
  uint32_t code_version = UINT32_C(1);
  auto *module = new Module(code_version,
                            move(cmds_code),
                            main_cmds_code_size,
                            func_defs,
                            global_var_defs,
                            native_func_defs,
                            func_refs,
                            native_func_refs,
                            global_var_refs);
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  auto *main_program = new ProgramNode(vector< unique_ptr<StmtNode> >());
  vector<ProgramNode*> import_programs;
  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(main_program, SemanticAnalysis::Problems()));
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  MsgPrinter::ProgramFilePaths program_file_paths =
      {{main_program, final_input_file_path}};

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(semantic_analyzer, Analyze_(Ref(*main_program),
                                            IsEmpty()))
        .Times(1)
        .WillOnce(Return(semantic_analysis));
    EXPECT_CALL(msg_printer, PrintSemanticProblems(
        Ref(semantic_analysis->GetProblems()),
        program_file_paths))
        .Times(1);
    EXPECT_CALL(code_generator, Generate_(Ref(*main_program),
                                          Ref(*semantic_analysis),
                                          code_version))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(module_writer, Write(Ref(*module), &module_code))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
        .Times(1);
    EXPECT_CALL(file, Write_(output_file_path.string(), Ref(module_code)))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    string msg = (format("Failed to write output file \"%1%\"")
                  % output_file_path.string()).str();
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &module_code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, IOErrorWhileCreatingDir) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  path output_dir_path("build2/bin2/app/module");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  config.SetBinDirPath("build2/bin2");
  Code module_code;
  unique_ptr<Code> cmds_code(new Code());
  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t main_cmds_code_size = UINT32_C(0);
  uint32_t code_version = UINT32_C(1);
  auto *module = new Module(code_version,
                            move(cmds_code),
                            main_cmds_code_size,
                            func_defs,
                            global_var_defs,
                            native_func_defs,
                            func_refs,
                            native_func_refs,
                            global_var_refs);
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  auto *main_program = new ProgramNode(vector< unique_ptr<StmtNode> >());
  vector<ProgramNode*> import_programs;
  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(main_program, SemanticAnalysis::Problems()));
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  MsgPrinter::ProgramFilePaths program_file_paths =
      {{main_program, final_input_file_path}};

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(semantic_analyzer, Analyze_(Ref(*main_program),
                                            IsEmpty()))
        .Times(1)
        .WillOnce(Return(semantic_analysis));
    EXPECT_CALL(msg_printer, PrintSemanticProblems(
        Ref(semantic_analysis->GetProblems()),
        program_file_paths))
        .Times(1);
    EXPECT_CALL(code_generator, Generate_(Ref(*main_program),
                                          Ref(*semantic_analysis),
                                          code_version))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(module_writer, Write(Ref(*module), &module_code))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    string msg = (format("Failed to create output folder \"%1%\"")
                  % output_dir_path.string()).str();
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &module_code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, CodeSizeOverflowErrorWhileGeneratingCode) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  uint32_t code_version = UINT32_C(1);
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  auto *main_program = new ProgramNode(vector< unique_ptr<StmtNode> >());
  vector<ProgramNode*> import_programs;
  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(main_program, SemanticAnalysis::Problems()));
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  MsgPrinter::ProgramFilePaths program_file_paths =
      {{main_program, final_input_file_path}};

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(semantic_analyzer, Analyze_(Ref(*main_program),
                                            IsEmpty()))
        .Times(1)
        .WillOnce(Return(semantic_analysis));
    EXPECT_CALL(msg_printer, PrintSemanticProblems(
        Ref(semantic_analysis->GetProblems()),
        program_file_paths))
        .Times(1);
    EXPECT_CALL(code_generator, Generate_(Ref(*main_program),
                                          Ref(*semantic_analysis),
                                          code_version))
        .Times(1)
        .WillOnce(Throw(Code::CodeSizeOverflowError("test")));
    EXPECT_CALL(msg_printer, PrintError("Code size exceeds 32 bits"))
        .Times(1);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, CodeSizeOverflowErrorWhileWritingModule) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code module_code;
  unique_ptr<Code> cmds_code(new Code());
  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t main_cmds_code_size = UINT32_C(0);
  uint32_t code_version = UINT32_C(1);
  auto *module = new Module(code_version,
                            move(cmds_code),
                            main_cmds_code_size,
                            func_defs,
                            global_var_defs,
                            native_func_defs,
                            func_refs,
                            native_func_refs,
                            global_var_refs);
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  auto *main_program = new ProgramNode(vector< unique_ptr<StmtNode> >());
  vector<ProgramNode*> import_programs;
  SemanticAnalysis::ProgramProblems semantic_problems;
  semantic_problems.insert(
      make_pair(main_program, SemanticAnalysis::Problems()));
  SemanticAnalysis::NodeAnalyzes node_analyzes;
  auto *semantic_analysis = new SemanticAnalysis(
      move(semantic_problems), move(node_analyzes));
  MsgPrinter::ProgramFilePaths program_file_paths =
      {{main_program, final_input_file_path}};

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(semantic_analyzer, Analyze_(Ref(*main_program),
                                            IsEmpty()))
        .Times(1)
        .WillOnce(Return(semantic_analysis));
    EXPECT_CALL(msg_printer, PrintSemanticProblems(
        Ref(semantic_analysis->GetProblems()),
        program_file_paths))
        .Times(1);
    EXPECT_CALL(code_generator, Generate_(Ref(*main_program),
                                          Ref(*semantic_analysis),
                                          code_version))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(module_writer, Write(Ref(*module), &module_code))
        .Times(1)
        .WillOnce(Throw(Code::CodeSizeOverflowError("test")));
    EXPECT_CALL(msg_printer, PrintError("Code size exceeds 32 bits"))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &module_code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, UnexpectedTokenErrorWhileParsingFile) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  Parser::UnexpectedTokenError error(
      TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(2)), "test");

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Throw(error));
    EXPECT_CALL(msg_printer, PrintUnexpectedTokenError(error.GetToken(),
                                                       final_input_file_path))
        .Times(1);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, UnexpectedCharErrorWhileParsingFile) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;
  Lexer::UnexpectedCharError error('a', UINT32_C(1), UINT32_C(2), "test");

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Throw(error));
    EXPECT_CALL(msg_printer, PrintUnexpectedCharError(error.GetChar(),
                                                      error.GetLineNumber(),
                                                      error.GetColumnNumber(),
                                                      final_input_file_path))
        .Times(1);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, IOErrorWhileParsingFile) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    string msg = (format("Failed to read file \"%1%\"")
                  % final_input_file_path.string()).str();
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, IOErrorWhileReadingFile) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    string msg = (format("Failed to read file \"%1%\"")
                  % final_input_file_path.string()).str();
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(lexer_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(src_parser, Parse_(_))
        .Times(0);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, IOErrorWhileSearchingFile) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  path search_import_file_path("src2/app/module/import.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  vector< unique_ptr<StmtNode> > main_program_stmts;
  unique_ptr<StringNode> file_path_expr(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import.rts\"",
      UINT32_C(1),
      UINT32_C(1))));
  auto *import_stmt_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt(import_stmt_ptr);
  main_program_stmts.push_back(move(import_stmt));
  auto *main_program = new ProgramNode(move(main_program_stmts));
  vector<ProgramNode*> import_programs;

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(lit_parser, ParseString(
        import_stmt_ptr->GetFilePath()->GetStartToken().GetValue()))
        .Times(1)
        .WillOnce(Return(search_import_file_path.string()));
    EXPECT_CALL(file_searcher, Search_(
        search_import_file_path.string(),
        config.GetSrcDirPath().string(),
        config.GetVendorDirPath().string(),
        IsEmpty()))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    EXPECT_CALL(msg_printer, PrintTokenError(
        import_stmt_ptr->GetFilePath()->GetStartToken(),
        final_input_file_path,
        "IO error while searching file"))
        .Times(1);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, FileNotExists) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  path search_import_file_path("src2/app/module/import.rts");
  path found_import_file_path;
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  vector< unique_ptr<StmtNode> > main_program_stmts;
  unique_ptr<StringNode> file_path_expr(new StringNode(TokenInfo(
      Token::kStringLit,
      "\"app/module/import.rts\"",
      UINT32_C(1),
      UINT32_C(1))));
  auto *import_stmt_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt(import_stmt_ptr);
  main_program_stmts.push_back(move(import_stmt));
  auto *main_program = new ProgramNode(move(main_program_stmts));
  vector<ProgramNode*> import_programs;

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(lit_parser, ParseString(
        import_stmt_ptr->GetFilePath()->GetStartToken().GetValue()))
        .Times(1)
        .WillOnce(Return(search_import_file_path.string()));
    EXPECT_CALL(file_searcher, Search_(
        search_import_file_path.string(),
        config.GetSrcDirPath().string(),
        config.GetVendorDirPath().string(),
        IsEmpty()))
        .Times(1)
        .WillOnce(Return(found_import_file_path.string()));
    EXPECT_CALL(msg_printer, PrintTokenError(
        import_stmt_ptr->GetFilePath()->GetStartToken(),
        final_input_file_path,
        "File not found"))
        .Times(1);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, EmptyHexValueErrorWhileParsingImportFilePath) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  vector< unique_ptr<StmtNode> > main_program_stmts;
  string import_file_path_token = "\"app/module/import.rts\"";
  auto *file_path_expr_ptr = new StringNode(TokenInfo(
      Token::kStringLit,
      import_file_path_token,
      UINT32_C(1),
      UINT32_C(1)));
  unique_ptr<StringNode> file_path_expr(file_path_expr_ptr);
  auto *import_stmt_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt(import_stmt_ptr);
  main_program_stmts.push_back(move(import_stmt));
  auto *main_program = new ProgramNode(move(main_program_stmts));
  vector<ProgramNode*> import_programs;
  StringWithEmptyHexValueError semantic_problem(*file_path_expr_ptr);

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(lit_parser, ParseString(import_file_path_token))
        .Times(1)
        .WillOnce(Throw(LitParser::EmptyHexValueError("test")));
    EXPECT_CALL(msg_printer, PrintSemanticProblem(Eq(ByRef(semantic_problem)),
                                                  final_input_file_path))
        .Times(1);
    EXPECT_CALL(file_searcher, Search_(_, _, _, _))
        .Times(0);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, HexValueOutOfRangeErrorWhileParsingImportFilePath) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path("app/module/component.rts");
  path final_input_file_path("src2/app/module/component.rts");
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  config.SetSrcDirPath("src2");
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  vector< unique_ptr<StmtNode> > main_program_stmts;
  string import_file_path_token = "\"app/module/import.rts\"";
  auto *file_path_expr_ptr = new StringNode(TokenInfo(
      Token::kStringLit,
      import_file_path_token,
      UINT32_C(1),
      UINT32_C(1)));
  unique_ptr<StringNode> file_path_expr(file_path_expr_ptr);
  auto *import_stmt_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_expr),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_stmt(import_stmt_ptr);
  main_program_stmts.push_back(move(import_stmt));
  auto *main_program = new ProgramNode(move(main_program_stmts));
  vector<ProgramNode*> import_programs;
  StringWithOutOfRangeHexValueError semantic_problem(*file_path_expr_ptr);

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(CompilerCmd::kCompile));
    auto *input_file_stream = new stringstream();
    EXPECT_CALL(file, Read_(final_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    auto *input_file_lexer = new LexerMock();
    EXPECT_CALL(lexer_factory, Create_(Ref(*input_file_stream)))
        .Times(1)
        .WillOnce(Return(input_file_lexer));
    EXPECT_CALL(src_parser, Parse_(input_file_lexer))
        .Times(1)
        .WillOnce(Return(main_program));
    EXPECT_CALL(lit_parser, ParseString(import_file_path_token))
        .Times(1)
        .WillOnce(Throw(LitParser::OutOfRangeError("test")));
    EXPECT_CALL(msg_printer, PrintSemanticProblem(Eq(ByRef(semantic_problem)),
                                                  final_input_file_path))
        .Times(1);
    EXPECT_CALL(file_searcher, Search_(_, _, _, _))
        .Times(0);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}

TEST_F(CompilerAppTest, BadArgsErrorWhileParsingConfig) {
  int argc = 1;
  const char *argv[] = {"realtalkc"};
  path input_file_path;
  CompilerConfig config;
  config.SetInputFilePath(input_file_path);
  Code code;
  LitParserMock lit_parser;
  FileSearcherMock file_searcher;
  FileMock file;
  LexerFactoryMock lexer_factory;
  SrcParserMock src_parser;
  SemanticAnalyzerMock semantic_analyzer;
  MsgPrinterMock msg_printer;
  CodeGeneratorMock code_generator;
  CompilerConfigParserMock config_parser;
  DirCreatorMock dir_creator;
  ModuleWriterMock module_writer;

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(Throw(CompilerConfigParser::BadArgsError("test")));
    EXPECT_CALL(msg_printer, PrintError("Invalid arguments"))
        .Times(1);
    EXPECT_CALL(file, Read_(_))
        .Times(0);
    EXPECT_CALL(lexer_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(src_parser, Parse_(_))
        .Times(0);
    EXPECT_CALL(lit_parser, ParseString(_))
        .Times(0);
    EXPECT_CALL(file_searcher, Search_(_, _, _, _))
        .Times(0);
    EXPECT_CALL(semantic_analyzer, Analyze_(_, _))
        .Times(0);
    EXPECT_CALL(msg_printer, PrintSemanticProblems(_, _))
        .Times(0);
    EXPECT_CALL(code_generator, Generate_(_, _, _))
        .Times(0);
    EXPECT_CALL(module_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  CompilerApp compiler(file_searcher,
                       lexer_factory,
                       &src_parser,
                       lit_parser,
                       config_parser,
                       &semantic_analyzer,
                       &code_generator,
                       msg_printer,
                       dir_creator,
                       module_writer,
                       file,
                       &config,
                       &code);
  compiler.Run(argc, argv);
}
}
}
