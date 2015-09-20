
#include "real_talk/util/simple_file.h"
#include "real_talk/code/simple_exe_reader.h"
#include "real_talk/vm/simple_vm_config_parser.h"
#include "real_talk/vm/simple_native_func_storage.h"
#include "real_talk/vm/simple_native_func_linker.h"
#include "real_talk/vm/simple_vm_factory.h"
#include "real_talk/vm/simple_msg_printer.h"
#include "real_talk/vm/vm_config.h"
#include "real_talk/vm/vm_app.h"

using std::cout;
using real_talk::util::SimpleFile;
using real_talk::code::SimpleExeReader;
using real_talk::vm::VMConfig;
using real_talk::vm::VMApp;
using real_talk::vm::SimpleVMConfigParser;
using real_talk::vm::SimpleNativeFuncStorage;
using real_talk::vm::SimpleNativeFuncLinker;
using real_talk::vm::SimpleVMFactory;
using real_talk::vm::SimpleMsgPrinter;

int main(int argc, const char* argv[]) {
  VMConfig config;
  VMApp app(
      SimpleVMConfigParser(),
      SimpleFile(),
      SimpleExeReader(),
      SimpleNativeFuncStorage(),
      SimpleNativeFuncLinker(),
      SimpleVMFactory(),
      SimpleMsgPrinter(&cout),
      &config);
  app.Run(argc, argv);
  return EXIT_SUCCESS;
}
