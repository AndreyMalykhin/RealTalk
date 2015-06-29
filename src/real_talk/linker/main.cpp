
#include "real_talk/util/simple_file_searcher.h"
#include "real_talk/util/simple_dir_creator.h"
#include "real_talk/util/simple_file.h"
#include "real_talk/code/code.h"
#include "real_talk/code/simple_module_reader.h"
#include "real_talk/code/simple_code_container_writer.h"
#include "real_talk/linker/linker_config.h"
#include "real_talk/linker/simple_linker_factory.h"
#include "real_talk/linker/simple_linker_config_parser.h"
#include "real_talk/linker/simple_msg_printer.h"
#include "real_talk/linker/linker_app.h"

using std::cout;
using real_talk::util::SimpleFileSearcher;
using real_talk::util::SimpleDirCreator;
using real_talk::util::SimpleFile;
using real_talk::code::Code;
using real_talk::code::SimpleModuleReader;
using real_talk::code::SimpleCodeContainerWriter;
using real_talk::linker::LinkerConfig;
using real_talk::linker::SimpleLinkerFactory;
using real_talk::linker::SimpleLinkerConfigParser;
using real_talk::linker::SimpleMsgPrinter;
using real_talk::linker::LinkerApp;

int main(int argc, const char *argv[]) {
  LinkerConfig config;
  Code output_code;
  LinkerApp app(SimpleLinkerConfigParser(),
                SimpleMsgPrinter(&cout),
                SimpleFileSearcher(),
                SimpleModuleReader(),
                SimpleLinkerFactory(),
                SimpleCodeContainerWriter(),
                SimpleDirCreator(),
                SimpleFile(),
                &config,
                &output_code);
  app.Run(argc, argv);
  return EXIT_SUCCESS;
}
