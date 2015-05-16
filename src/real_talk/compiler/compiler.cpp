
#include <boost/filesystem.hpp>
#include <memory>
#include "real_talk/parser/program_node.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/node_semantic_analysis.h"
#include "real_talk/semantic/semantic_analyzer.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/file.h"
#include "real_talk/compiler/msg_printer.h"
#include "real_talk/compiler/compiler_config_parser.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/file_parser.h"
#include "real_talk/compiler/compiler.h"

using std::unique_ptr;
using boost::filesystem::path;
using real_talk::semantic::SemanticAnalyzer;
using real_talk::semantic::SemanticAnalysis;
using real_talk::code::CodeGenerator;
using real_talk::code::Code;
using real_talk::util::DirCreator;
using real_talk::util::File;

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
  msg_printer_.PrintProblems(semantic_analysis->GetProblems());
  const uint32_t code_version = UINT32_C(1);
  code_generator_->Generate(
      programs->GetMain(), *semantic_analysis, code_version, code_);
  const path output_dir_path("build2/bin2/app/module");
  dir_creator_.Create(output_dir_path);
  const path output_file_path(
      config_->GetBinDirPath() / config_->GetInputFilePath());
  file_->Open(output_file_path);
  file_->Write(*code_);
}
}
}
