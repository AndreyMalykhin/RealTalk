
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <memory>
#include <string>
#include "real_talk/parser/program_node.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/semantic/semantic_analyzer.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/file.h"
#include "real_talk/util/errors.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/file_parser.h"
#include "real_talk/compiler/compiler.h"

using std::unique_ptr;
using std::string;
using boost::filesystem::path;
using boost::format;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::semantic::SemanticAnalysis;
using real_talk::code::CodeGenerator;
using real_talk::code::Code;
using real_talk::util::DirCreator;
using real_talk::util::File;
using real_talk::util::IOError;

namespace real_talk {
namespace compiler {

Compiler::Compiler(
    const CompilerConfigParser &config_parser,
    const FileParser &file_parser,
    SemanticAnalyzer *semantic_analyzer,
    CodeGenerator *code_generator,
    const MsgPrinter &msg_printer,
    const DirCreator &dir_creator,
    CompilerConfig *config,
    File *file,
    Code *code)
    : config_parser_(config_parser),
      file_parser_(file_parser),
      semantic_analyzer_(semantic_analyzer),
      code_generator_(code_generator),
      msg_printer_(msg_printer),
      dir_creator_(dir_creator),
      config_(config),
      file_(file),
      code_(code) {
  assert(semantic_analyzer_);
  assert(code_generator_);
  assert(config_);
  assert(file_);
  assert(code_);
}

void Compiler::Compile(int argc, const char *argv[]) const {
  config_parser_.Parse(argc, argv, config_);
  const unique_ptr<FileParser::Programs> programs =
      file_parser_.Parse(config_->GetInputFilePath(),
                         config_->GetSrcDirPath(),
                         config_->GetVendorDirPath(),
                         config_->GetImportDirPaths());
  const unique_ptr<SemanticAnalysis> semantic_analysis =
      semantic_analyzer_->Analyze(programs->GetMain(), programs->GetImport());
  msg_printer_.PrintSemanticProblems(semantic_analysis->GetProblems());

  if (HasSemanticErrors(*semantic_analysis)) {
    return;
  }

  const uint32_t code_version = UINT32_C(1);
  code_generator_->Generate(
      programs->GetMain(), *semantic_analysis, code_version, code_);
  path output_file_path(config_->GetBinDirPath() / config_->GetInputFilePath());
  output_file_path.replace_extension(config_->GetModuleFileExtension());
  dir_creator_.Create(output_file_path.parent_path());

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
