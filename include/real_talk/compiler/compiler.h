
#ifndef _REAL_TALK_COMPILER_COMPILER_H_
#define _REAL_TALK_COMPILER_COMPILER_H_

#include <vector>
#include "real_talk/semantic/semantic_analyzer.h"

namespace real_talk {
namespace lexer {

class LexerFactory;
}

namespace parser {

class Parser;
}

namespace code {

class Code;
class CodeGenerator;
}

namespace semantic {

class SemanticAnalyzer;
class SemanticAnalysis;
class LitParser;
}

namespace util {

class DirCreator;
class File;
}

namespace compiler {

class FileParser;
class MsgPrinter;
class CompilerConfig;
class CompilerConfigParser;
class ImportFileSearcher;

class Compiler {
 public:
  Compiler(const ImportFileSearcher &file_searcher,
           const real_talk::lexer::LexerFactory &lexer_factory,
           real_talk::parser::Parser *src_parser,
           const real_talk::semantic::LitParser &lit_parser,
           const CompilerConfigParser &config_parser,
           real_talk::semantic::SemanticAnalyzer *semantic_analyzer,
           real_talk::code::CodeGenerator *code_generator,
           const MsgPrinter &msg_printer,
           const real_talk::util::DirCreator &dir_creator,
           CompilerConfig *config,
           real_talk::util::File *file,
           real_talk::code::Code *code);
  void Compile(int argc, const char *argv[]) const;

 private:
  struct ProgramImportStmt {
    const real_talk::parser::ProgramNode *program;
    const real_talk::parser::ImportNode *stmt;
  };

  bool HasSemanticErrors(
      const real_talk::semantic::SemanticAnalysis &semantic_analysis) const;
  void ParseFiles(
      std::unique_ptr<real_talk::parser::ProgramNode> *main_program,
      real_talk::semantic::SemanticAnalyzer::ImportPrograms *import_programs,
      MsgPrinter::ProgramFilePaths *program_file_paths,
      bool *is_success)
      const;
  void ParseFile(
      const boost::filesystem::path &file_path,
      std::unique_ptr<real_talk::parser::ProgramNode> *program,
      std::vector<ProgramImportStmt> *program_import_stmts,
      bool *is_success) const;

  const ImportFileSearcher &file_searcher_;
  const real_talk::lexer::LexerFactory &lexer_factory_;
  real_talk::parser::Parser *src_parser_;
  const real_talk::semantic::LitParser &lit_parser_;
  const CompilerConfigParser &config_parser_;
  real_talk::semantic::SemanticAnalyzer *semantic_analyzer_;
  real_talk::code::CodeGenerator *code_generator_;
  const MsgPrinter &msg_printer_;
  const real_talk::util::DirCreator &dir_creator_;
  CompilerConfig *config_;
  real_talk::util::File *file_;
  real_talk::code::Code *code_;
};
}
}
#endif
