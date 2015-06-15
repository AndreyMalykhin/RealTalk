
#include <cassert>
#include "real_talk/linker/linker_app.h"

using real_talk::util::FileSearcher;
using real_talk::util::DirCreator;
using real_talk::util::File;
using real_talk::code::ModuleReader;
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

}
}
}
