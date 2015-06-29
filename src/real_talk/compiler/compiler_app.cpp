
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <unordered_set>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/parser.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/lexer_factory.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/module_writer.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/file.h"
#include "real_talk/util/errors.h"
#include "real_talk/util/file_searcher.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/compiler_cmd.h"
#include "real_talk/compiler/compiler_app.h"

using std::unique_ptr;
using std::unordered_set;
using std::string;
using std::vector;
using std::istream;
using std::ostream;
using std::cout;
using std::make_pair;
using boost::filesystem::path;
using boost::hash;
using boost::format;
using boost::adaptors::reverse;
using real_talk::lexer::LexerFactory;
using real_talk::lexer::Lexer;
using real_talk::lexer::TokenInfo;
using real_talk::parser::ImportNode;
using real_talk::parser::BreakNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::ExprStmtNode;
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::FuncDefWithoutBodyNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::parser::IfElseIfNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::AndNode;
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::AssignNode;
using real_talk::parser::BoolNode;
using real_talk::parser::CallNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::CharNode;
using real_talk::parser::DivNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::EqualNode;
using real_talk::parser::GreaterNode;
using real_talk::parser::GreaterOrEqualNode;
using real_talk::parser::IntNode;
using real_talk::parser::LessNode;
using real_talk::parser::LessOrEqualNode;
using real_talk::parser::LongNode;
using real_talk::parser::MulNode;
using real_talk::parser::NegativeNode;
using real_talk::parser::NotEqualNode;
using real_talk::parser::NotNode;
using real_talk::parser::OrNode;
using real_talk::parser::PreDecNode;
using real_talk::parser::PreIncNode;
using real_talk::parser::StmtNode;
using real_talk::parser::ProgramNode;
using real_talk::parser::StringNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::SubNode;
using real_talk::parser::SumNode;
using real_talk::parser::IdNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::Parser;
using real_talk::parser::NodeVisitor;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::LitParser;
using real_talk::semantic::StringWithEmptyHexValueError;
using real_talk::semantic::StringWithOutOfRangeHexValueError;
using real_talk::code::CodeGenerator;
using real_talk::code::Code;
using real_talk::code::ModuleWriter;
using real_talk::code::Module;
using real_talk::util::DirCreator;
using real_talk::util::File;
using real_talk::util::IOError;
using real_talk::util::FileSearcher;

namespace real_talk {
namespace compiler {
namespace {

class ImportsExtractor: private NodeVisitor {
 public:
  vector<const ImportNode*> Extract(const ProgramNode &program) {
    program.Accept(*this);
    vector<const ImportNode*> result = move(import_stmts_);
    import_stmts_.clear();
    return move(result);
  }

 private:
  virtual void VisitProgram(const ProgramNode &program) override {
    for (const unique_ptr<StmtNode> &stmt: program.GetStmts()) {
      stmt->Accept(*this);
    }
  }

  virtual void VisitImport(const ImportNode &node) override {
    import_stmts_.push_back(&node);
  }

  virtual void VisitBreak(const BreakNode&) override {}
  virtual void VisitContinue(const ContinueNode&) override {}
  virtual void VisitExprStmt(const ExprStmtNode&) override {}
  virtual void VisitFuncDefWithBody(const FuncDefWithBodyNode&) override {}
  virtual void VisitFuncDefWithoutBody(const FuncDefWithoutBodyNode&)
      override {}
  virtual void VisitIfElseIfElse(const IfElseIfElseNode&) override {}
  virtual void VisitIfElseIf(const IfElseIfNode&) override {}
  virtual void VisitPreTestLoop(const PreTestLoopNode&) override {}
  virtual void VisitVarDefWithoutInit(const VarDefWithoutInitNode&) override {}
  virtual void VisitVarDefWithInit(const VarDefWithInitNode&) override {}
  virtual void VisitReturnValue(const ReturnValueNode&) override {}
  virtual void VisitReturnWithoutValue(const ReturnWithoutValueNode&)
      override {}

  virtual void VisitAnd(const AndNode&) override {assert(false);}
  virtual void VisitArrayAllocWithoutInit(
      const ArrayAllocWithoutInitNode&) override {assert(false);}
  virtual void VisitArrayAllocWithInit(const ArrayAllocWithInitNode&)
      override {assert(false);}
  virtual void VisitAssign(const AssignNode&) override {assert(false);}
  virtual void VisitBool(const BoolNode&) override {assert(false);}
  virtual void VisitCall(const CallNode&) override {assert(false);}
  virtual void VisitChar(const CharNode&) override {assert(false);}
  virtual void VisitDiv(const DivNode&) override {assert(false);}
  virtual void VisitDouble(const DoubleNode&) override {assert(false);}
  virtual void VisitEqual(const EqualNode&) override {assert(false);}
  virtual void VisitGreater(const GreaterNode&) override {assert(false);}
  virtual void VisitGreaterOrEqual(const GreaterOrEqualNode&)
      override {assert(false);}
  virtual void VisitInt(const IntNode&) override {assert(false);}
  virtual void VisitLess(const LessNode&) override {assert(false);}
  virtual void VisitLessOrEqual(const LessOrEqualNode&)
      override {assert(false);}
  virtual void VisitLong(const LongNode&) override {assert(false);}
  virtual void VisitMul(const MulNode&) override {assert(false);}
  virtual void VisitNegative(const NegativeNode&) override {assert(false);}
  virtual void VisitNotEqual(const NotEqualNode&) override {assert(false);}
  virtual void VisitNot(const NotNode&) override {assert(false);}
  virtual void VisitOr(const OrNode&) override {assert(false);}
  virtual void VisitPreDec(const PreDecNode&) override {assert(false);}
  virtual void VisitPreInc(const PreIncNode&) override {assert(false);}
  virtual void VisitString(const StringNode&) override {assert(false);}
  virtual void VisitSubscript(const SubscriptNode&) override {assert(false);}
  virtual void VisitSub(const SubNode&) override {assert(false);}
  virtual void VisitSum(const SumNode&) override {assert(false);}
  virtual void VisitId(const IdNode&) override {assert(false);}
  virtual void VisitIntDataType(const IntDataTypeNode&)
      override {assert(false);}
  virtual void VisitLongDataType(const LongDataTypeNode&)
      override {assert(false);}
  virtual void VisitCharDataType(const CharDataTypeNode&)
      override {assert(false);}
  virtual void VisitStringDataType(const StringDataTypeNode&)
      override {assert(false);}
  virtual void VisitDoubleDataType(const DoubleDataTypeNode&)
      override {assert(false);}
  virtual void VisitBoolDataType(const BoolDataTypeNode&)
      override {assert(false);}
  virtual void VisitVoidDataType(const VoidDataTypeNode&)
      override {assert(false);}
  virtual void VisitArrayDataType(const ArrayDataTypeNode&)
      override {assert(false);}
  virtual void VisitArgDef(const ArgDefNode&) override {assert(false);}
  virtual void VisitScope(const ScopeNode&) override {assert(false);}

  vector<const ImportNode*> import_stmts_;
};

ImportsExtractor &kImportsExtractor = *new ImportsExtractor();
}

CompilerApp::CompilerApp(
    const FileSearcher &file_searcher,
    const LexerFactory &lexer_factory,
    Parser *src_parser,
    const LitParser &lit_parser,
    const CompilerConfigParser &config_parser,
    SemanticAnalyzer *semantic_analyzer,
    CodeGenerator *code_generator,
    const MsgPrinter &msg_printer,
    const DirCreator &dir_creator,
    const ModuleWriter &module_writer,
    const File &file,
    CompilerConfig *config,
    Code *code)
    : file_searcher_(file_searcher),
      lexer_factory_(lexer_factory),
      src_parser_(src_parser),
      lit_parser_(lit_parser),
      config_parser_(config_parser),
      semantic_analyzer_(semantic_analyzer),
      code_generator_(code_generator),
      msg_printer_(msg_printer),
      dir_creator_(dir_creator),
      module_writer_(module_writer),
      file_(file),
      config_(config),
      code_(code) {
  assert(src_parser_);
  assert(semantic_analyzer_);
  assert(code_generator_);
  assert(config_);
  assert(code_);
}

void CompilerApp::Run(int argc, const char *argv[]) const {
  CompilerCmd cmd;

  try {
    config_parser_.Parse(argc, argv, config_, &cmd);
  } catch (const CompilerConfigParser::BadArgsError&) {
    msg_printer_.PrintError("Invalid arguments");
    return;
  }

  if (cmd == CompilerCmd::kHelp) {
    msg_printer_.PrintHelp(config_parser_.GetHelp());
    return;
  }

  unique_ptr<ProgramNode> main_program;
  SemanticAnalyzer::ImportPrograms import_programs;
  MsgPrinter::ProgramFilePaths program_file_paths;
  bool is_success;
  ParseFiles(&main_program, &import_programs, &program_file_paths, &is_success);

  if (!is_success) {
    return;
  }

  const unique_ptr<SemanticAnalysis> semantic_analysis =
      semantic_analyzer_->Analyze(*main_program, import_programs);
  msg_printer_.PrintSemanticProblems(
      semantic_analysis->GetProblems(), program_file_paths);

  if (HasSemanticErrors(*semantic_analysis)) {
    return;
  }

  const uint32_t code_version = UINT32_C(1);
  unique_ptr<Module> module;

  try {
    module = code_generator_->Generate(
        *main_program, *semantic_analysis, code_version);
    Log([&module](ostream *stream) {
      module->GetCmdsCode().SetPosition(0);
      *stream << "\n[module]\n\n" << *module;
    });
    module_writer_.Write(*module, code_);
  } catch (const Code::CodeSizeOverflowError&) {
    msg_printer_.PrintError("Code size exceeds 32 bits");
    return;
  }

  path output_file_path(config_->GetBinDirPath() / config_->GetInputFilePath());
  output_file_path.replace_extension(config_->GetModuleFileExtension());

  try {
    dir_creator_.Create(output_file_path.parent_path());
  } catch (const IOError&) {
    const string msg = (format("Failed to create output folder \"%1%\"")
                        % output_file_path.parent_path().string()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  try {
    file_.Write(output_file_path, *code_);
  } catch (const IOError&) {
    const string msg = (format("Failed to write output file \"%1%\"")
                        % output_file_path.string()).str();
    msg_printer_.PrintError(msg);
    return;
  }
}

void CompilerApp::ParseFiles(
    unique_ptr<ProgramNode> *main_program,
    SemanticAnalyzer::ImportPrograms *import_programs,
    MsgPrinter::ProgramFilePaths *program_file_paths,
    bool *is_success) const {
  *is_success = false;
  vector<ProgramImportStmt> program_import_stmts;
  const path input_file_path(
      config_->GetSrcDirPath() / config_->GetInputFilePath());
  bool is_success2;
  ParseFile(input_file_path, main_program, &program_import_stmts, &is_success2);

  if (!is_success2) {
    return;
  }

  program_file_paths->insert(make_pair(main_program->get(), input_file_path));
  unordered_set< path, hash<path> > processed_files = {input_file_path};

  while (!program_import_stmts.empty()) {
    const ProgramImportStmt program_import_stmt = program_import_stmts.back();
    program_import_stmts.pop_back();
    MsgPrinter::ProgramFilePaths::const_iterator program_file_paths_it =
        program_file_paths->find(program_import_stmt.program);
    assert(program_file_paths_it != program_file_paths->cend());
    const path &current_file_path = program_file_paths_it->second;
    const TokenInfo &search_import_file_path_token =
        program_import_stmt.stmt->GetFilePath()->GetStartToken();
    string search_import_file_path;

    try {
      search_import_file_path = lit_parser_.ParseString(
          search_import_file_path_token.GetValue());
    } catch (const LitParser::EmptyHexValueError&) {
      StringWithEmptyHexValueError semantic_problem(
          *(program_import_stmt.stmt->GetFilePath()));
      msg_printer_.PrintSemanticProblem(semantic_problem, current_file_path);
      return;
    } catch (const LitParser::OutOfRangeError&) {
      StringWithOutOfRangeHexValueError semantic_problem(
          *(program_import_stmt.stmt->GetFilePath()));
      msg_printer_.PrintSemanticProblem(semantic_problem, current_file_path);
      return;
    }

    path found_import_file_path;

    try {
      found_import_file_path = file_searcher_.Search(
          path(search_import_file_path),
          config_->GetSrcDirPath(),
          config_->GetVendorDirPath(),
          config_->GetImportDirPaths());
    } catch (const IOError&) {
      msg_printer_.PrintTokenError(search_import_file_path_token,
                                   current_file_path,
                                   "IO error while searching file");
      return;
    }

    if (found_import_file_path.empty()) {
      msg_printer_.PrintTokenError(search_import_file_path_token,
                                   current_file_path,
                                   "File not found");
      return;
    }

    if (processed_files.count(found_import_file_path)) {
      continue;
    }

    unique_ptr<ProgramNode> import_program;
    bool is_success3;
    ParseFile(found_import_file_path,
              &import_program,
              &program_import_stmts,
              &is_success3);

    if (!is_success3) {
      return;
    }

    program_file_paths->insert(
        make_pair(import_program.get(), found_import_file_path));
    import_programs->push_back(move(import_program));
    processed_files.insert(found_import_file_path);
  }

  *is_success = true;
}

void CompilerApp::ParseFile(const path &file_path,
                         unique_ptr<ProgramNode> *program,
                         vector<ProgramImportStmt> *program_import_stmts,
                         bool *is_success) const {
  *is_success = false;
  unique_ptr<istream> file_stream;

  try {
    file_stream = file_.Read(file_path);
  } catch (const IOError&) {
    const string msg =
        (format("Failed to read file \"%1%\"") % file_path.string()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);

  try {
    *program = src_parser_->Parse(lexer.get());
  } catch (const Parser::UnexpectedTokenError &error) {
    msg_printer_.PrintUnexpectedTokenError(error.GetToken(), file_path);
    return;
  } catch (const Lexer::UnexpectedCharError &error) {
    msg_printer_.PrintUnexpectedCharError(error.GetChar(),
                                          error.GetLineNumber(),
                                          error.GetColumnNumber(),
                                          file_path);
    return;
  } catch (const IOError&) {
    const string msg =
        (format("Failed to read file \"%1%\"") % file_path.string()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  Log([&program, &file_path](ostream *stream) {
      *stream << "\n[program]\nfile=" << file_path << "\n\n" << **program;
    });

  // must store result in var, coz reverse() doesn't play well with rvalues
  const vector<const ImportNode*> import_stmts =
      kImportsExtractor.Extract(**program);

  for (const ImportNode *import_stmt: reverse(import_stmts)) {
    program_import_stmts->push_back(
        ProgramImportStmt{program->get(), import_stmt});
  }

  *is_success = true;
}

bool CompilerApp::HasSemanticErrors(
    const SemanticAnalysis &semantic_analysis) const {
  for (const SemanticAnalysis::ProgramProblems::value_type &program_problems
           : semantic_analysis.GetProblems()) {
    const SemanticAnalysis::Problems &problems = program_problems.second;

    if (!problems.empty()) {
      return true;
    }
  }

  return false;
}

void CompilerApp::Log(LogDataWriter data_writer) const {
  if (config_->IsDebug()) {
    data_writer(&cout);
  }
}
}
}
