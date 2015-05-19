
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <memory>
#include <string>
#include <vector>
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/parser.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/lexer_factory.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/file.h"
#include "real_talk/util/errors.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/import_file_searcher.h"
#include "real_talk/compiler/compiler.h"

using std::unique_ptr;
using std::string;
using std::vector;
using std::istream;
using boost::filesystem::path;
using boost::format;
using real_talk::lexer::LexerFactory;
using real_talk::lexer::Lexer;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::parser::Parser;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::LitParser;
using real_talk::code::CodeGenerator;
using real_talk::code::Code;
using real_talk::util::DirCreator;
using real_talk::util::File;
using real_talk::util::IOError;

namespace real_talk {
namespace compiler {

Compiler::Compiler(
    const ImportFileSearcher &file_searcher,
    const LexerFactory &lexer_factory,
    Parser *src_parser,
    const LitParser &lit_parser,
    const CompilerConfigParser &config_parser,
    SemanticAnalyzer *semantic_analyzer,
    CodeGenerator *code_generator,
    const MsgPrinter &msg_printer,
    const DirCreator &dir_creator,
    CompilerConfig *config,
    File *file,
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
      config_(config),
      file_(file),
      code_(code) {
  assert(src_parser_);
  assert(semantic_analyzer_);
  assert(code_generator_);
  assert(config_);
  assert(file_);
  assert(code_);
}

void Compiler::Compile(int argc, const char *argv[]) const {
  config_parser_.Parse(argc, argv, config_);
  unique_ptr<ProgramNode> main_program;
  SemanticAnalyzer::ImportPrograms import_programs;
  ParseFiles(&main_program, &import_programs);
  const unique_ptr<SemanticAnalysis> semantic_analysis =
      semantic_analyzer_->Analyze(*main_program, import_programs);
  msg_printer_.PrintSemanticProblems(semantic_analysis->GetProblems());

  if (HasSemanticErrors(*semantic_analysis)) {
    return;
  }

  const uint32_t code_version = UINT32_C(1);

  try {
    code_generator_->Generate(
        *main_program, *semantic_analysis, code_version, code_);
  } catch (const Code::CodeSizeOverflowError&) {
    msg_printer_.PrintError("Code size exceeds 32 bits");
    return;
  }

  path output_file_path(config_->GetBinDirPath() / config_->GetInputFilePath());
  output_file_path.replace_extension(config_->GetModuleFileExtension());

  try {
    dir_creator_.Create(output_file_path.parent_path());
  } catch (const IOError&) {
    const string msg = (format("Failed to create folder \"%1%\"")
                        % output_file_path.parent_path().string()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  try {
    file_->Open(output_file_path);
  } catch (const IOError&) {
    const string msg = (format("Failed to open file \"%1%\"")
                        % output_file_path.string()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  try {
    file_->Write(*code_);
  } catch (const IOError&) {
    const string msg = (format("Failed to write to file \"%1%\"")
                        % output_file_path.string()).str();
    msg_printer_.PrintError(msg);
    return;
  }
}

void Compiler::ParseFiles(
    unique_ptr<ProgramNode> *main_program,
    SemanticAnalyzer::ImportPrograms *import_programs) const {
  {
    path input_file_path("src2/app/module/component.rts");
    file_->Open(input_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    *main_program = src_parser_->Parse(lexer.get());
  }

  {
    string import_file_path_token = "\"app/module/import1.rts\"";
    string search_import_file_path =
        lit_parser_.ParseString(import_file_path_token);
    path found_import_file_path = file_searcher_.Search(
        search_import_file_path,
        config_->GetSrcDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    file_->Open(found_import_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    unique_ptr<ProgramNode> import_program = src_parser_->Parse(lexer.get());
    import_programs->push_back(move(import_program));
  }

  {
    string import_file_path_token = "\"app/module/import2.rts\"";
    string search_import_file_path =
        lit_parser_.ParseString(import_file_path_token);
    path found_import_file_path = file_searcher_.Search(
        search_import_file_path,
        config_->GetSrcDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    file_->Open(found_import_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    unique_ptr<ProgramNode> import_program = src_parser_->Parse(lexer.get());
    import_programs->push_back(move(import_program));
  }

  {
    string import_file_path_token = "\"app/module/import3.rts\"";
    string search_import_file_path =
        lit_parser_.ParseString(import_file_path_token);
    path found_import_file_path = file_searcher_.Search(
        search_import_file_path,
        config_->GetSrcDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    file_->Open(found_import_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    unique_ptr<ProgramNode> import_program = src_parser_->Parse(lexer.get());
    import_programs->push_back(move(import_program));
  }

  {
    string import_file_path_token = "\"app/module/import4.rts\"";
    string search_import_file_path =
        lit_parser_.ParseString(import_file_path_token);
    path found_import_file_path = file_searcher_.Search(
        search_import_file_path,
        config_->GetSrcDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    file_->Open(found_import_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    unique_ptr<ProgramNode> import_program = src_parser_->Parse(lexer.get());
    import_programs->push_back(move(import_program));
  }

  {
    string import_file_path_token = "\"app/module/import5.rts\"";
    string search_import_file_path =
        lit_parser_.ParseString(import_file_path_token);
    path found_import_file_path = file_searcher_.Search(
        search_import_file_path,
        config_->GetSrcDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    file_->Open(found_import_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    unique_ptr<ProgramNode> import_program = src_parser_->Parse(lexer.get());
    import_programs->push_back(move(import_program));
  }

  {
    string import_file_path_token = "\"app/module/import6.rts\"";
    string search_import_file_path =
        lit_parser_.ParseString(import_file_path_token);
    path found_import_file_path = file_searcher_.Search(
        search_import_file_path,
        config_->GetSrcDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    file_->Open(found_import_file_path);
    unique_ptr<istream> file_stream = file_->Read();
    file_->Close();
    unique_ptr<Lexer> lexer = lexer_factory_.Create(*file_stream);
    unique_ptr<ProgramNode> import_program = src_parser_->Parse(lexer.get());
    import_programs->push_back(move(import_program));
  }
}

bool Compiler::HasSemanticErrors(
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
}
}
