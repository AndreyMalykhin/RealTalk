
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <cassert>
#include <string>
#include "real_talk/util/errors.h"
#include "real_talk/util/file.h"
#include "real_talk/util/file_searcher.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/code/module.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/code_container_writer.h"
#include "real_talk/linker/msg_printer.h"
#include "real_talk/linker/linker_factory.h"
#include "real_talk/linker/linker_cmd.h"
#include "real_talk/linker/linker_config.h"
#include "real_talk/linker/linker_config_parser.h"
#include "real_talk/linker/linker.h"
#include "real_talk/linker/linker_app.h"

using std::unique_ptr;
using std::istream;
using std::cout;
using std::ostream;
using std::move;
using std::string;
using boost::filesystem::path;
using boost::format;
using real_talk::util::IOError;
using real_talk::util::FileSearcher;
using real_talk::util::DirCreator;
using real_talk::util::File;
using real_talk::code::Module;
using real_talk::code::ModuleReader;
using real_talk::code::CodeContainer;
using real_talk::code::CodeContainerWriter;
using real_talk::code::Code;

namespace real_talk {
namespace linker {

LinkerApp::LinkerApp(
    const LinkerConfigParser &config_parser,
    const MsgPrinter &msg_printer,
    const FileSearcher &file_searcher,
    const ModuleReader &module_reader,
    const LinkerFactory &linker_factory,
    const CodeContainerWriter &code_container_writer,
    const DirCreator &dir_creator,
    const File &file,
    LinkerConfig *config,
    Code *output_code)
    : config_parser_(config_parser),
      msg_printer_(msg_printer),
      file_searcher_(file_searcher),
      module_reader_(module_reader),
      linker_factory_(linker_factory),
      code_container_writer_(code_container_writer),
      dir_creator_(dir_creator),
      file_(file),
      config_(config),
      output_code_(output_code) {
  assert(config);
  assert(output_code);
}

void LinkerApp::Run(int argc, const char *argv[]) const {
  LinkerCmd cmd;

  try {
    config_parser_.Parse(argc, argv, config_, &cmd);
  } catch (const LinkerConfigParser::BadArgsError&) {
    msg_printer_.PrintError("Invalid arguments");
    return;
  }

  if (cmd == LinkerCmd::kHelp) {
    msg_printer_.PrintHelp(config_parser_.GetHelp());
    return;
  }

  assert(!config_->GetInputFilePaths().empty());
  Linker::Modules modules;

  for (const path &search_input_file_path: config_->GetInputFilePaths()) {
    path found_input_file_path;

    try {
      found_input_file_path = file_searcher_.Search(
        search_input_file_path,
        config_->GetBinDirPath(),
        config_->GetVendorDirPath(),
        config_->GetImportDirPaths());
    } catch (const IOError&) {
      const string msg = (format("IO error while searching file %1%")
                          % search_input_file_path.string()).str();
      msg_printer_.PrintError(msg);
      return;
    }

    if (found_input_file_path.empty()) {
      const string msg = (format("File not found %1%")
                          % search_input_file_path.string()).str();
      msg_printer_.PrintError(msg);
      return;
    }

    try {
      unique_ptr<istream> input_file_stream = file_.Read(found_input_file_path);
      unique_ptr<Module> module =
          module_reader_.ReadFromStream(input_file_stream.get());
      modules.push_back(move(module));
    } catch (const Code::CodeSizeOverflowError&) {
      msg_printer_.PrintError("Code size exceeds 32 bits");
      return;
    } catch (const IOError&) {
      const string msg = (format("Failed to read input file %1%")
                          % found_input_file_path.string()).str();
      msg_printer_.PrintError(msg);
      return;
    }
  }

  const unique_ptr<Linker> linker =
      linker_factory_.Create(config_->GetOutputFileType());
  const uint32_t output_code_version = UINT32_C(1);

  try {
    const unique_ptr<CodeContainer> output_code_container =
        linker->Link(modules, output_code_version);
    Log([&output_code_container](ostream *stream) {
        output_code_container->GetCmdsCode().SetPosition(0);
        *stream << "[output]\n\n" << *output_code_container;
      });
    code_container_writer_.Write(*output_code_container, output_code_);
  } catch (const Code::CodeSizeOverflowError&) {
    msg_printer_.PrintError("Code size exceeds 32 bits");
    return;
  } catch (const Linker::DuplicateDefError &error) {
    const string msg =
        (format("Duplicate definition of id \"%1%\"") % error.GetId()).str();
    msg_printer_.PrintError(msg);
    return;
  } catch (const Linker::MissingDefError &error) {
    const string msg =
        (format("Missing definition of id \"%1%\"") % error.GetId()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  path output_file_path(
      config_->GetBinDirPath() / config_->GetOutputFilePath());

  try {
    dir_creator_.Create(output_file_path.parent_path());
  } catch (const IOError&) {
    const string msg = (format("Failed to create output folder \"%1%\"")
                        % output_file_path.parent_path().string()).str();
    msg_printer_.PrintError(msg);
    return;
  }

  try {
    file_.Write(output_file_path, *output_code_);
  } catch (const IOError&) {
    const string msg = (format("Failed to write output file \"%1%\"")
                        % output_file_path.string()).str();
    msg_printer_.PrintError(msg);
  }
}

void LinkerApp::Log(LogDataWriter data_writer) const {
  if (config_->IsDebug()) {
    data_writer(&cout);
  }
}
}
}
