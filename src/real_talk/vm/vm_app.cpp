
#include <boost/format.hpp>
#include <unordered_map>
#include <cassert>
#include <string>
#include <vector>
#include "real_talk/util/file.h"
#include "real_talk/util/errors.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/exe_reader.h"
#include "real_talk/vm/native_func_storage.h"
#include "real_talk/vm/native_func_linker.h"
#include "real_talk/vm/vm_config.h"
#include "real_talk/vm/vm_config_parser.h"
#include "real_talk/vm/vm_cmd.h"
#include "real_talk/vm/vm_factory.h"
#include "real_talk/vm/msg_printer.h"
#include "real_talk/vm/vm.h"
#include "real_talk/vm/vm_app.h"

using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::istream;
using std::string;
using boost::format;
using real_talk::util::File;
using real_talk::util::IOError;
using real_talk::code::ExeReader;
using real_talk::code::Exe;
using real_talk::code::Code;

namespace real_talk {
namespace vm {

VMApp::VMApp(const VMConfigParser &config_parser,
             const File &file,
             const ExeReader &exe_reader,
             const NativeFuncStorage &native_func_storage,
             const NativeFuncLinker &native_func_linker,
             const VMFactory &vm_factory,
             const MsgPrinter &msg_printer,
             VMConfig *config)
    : config_parser_(config_parser),
      file_(file),
      exe_reader_(exe_reader),
      native_func_storage_(native_func_storage),
      native_func_linker_(native_func_linker),
      vm_factory_(vm_factory),
      msg_printer_(msg_printer),
      config_(config) {
  assert(config_);
}

void VMApp::Run(int argc, const char *argv[]) {
  VMCmd cmd;

  try {
    config_parser_.Parse(argc, argv, config_, &cmd);
  } catch (const VMConfigParser::BadArgsError&) {
    msg_printer_.PrintError("Invalid arguments");
    return;
  }

  if (cmd == VMCmd::kHelp) {
    msg_printer_.PrintHelp(config_parser_.GetHelp());
    return;
  }

  unique_ptr<Exe> exe;

  try {
    unique_ptr<istream> exe_stream = file_.Read(config_->GetInputFilePath());
    exe = exe_reader_.ReadFromStream(exe_stream.get());
  } catch (const IOError&) {
    const string msg = (format("Failed to read file %1%")
                        % config_->GetInputFilePath().string()).str();
    msg_printer_.PrintError(msg);
    return;
  } catch (const Code::CodeSizeOverflowError&) {
    msg_printer_.PrintError("Code size exceeds 32 bits");
    return;
  }

  const unordered_map<string, NativeFuncValue> &available_native_funcs =
      native_func_storage_.GetAll();
  vector<NativeFuncValue> used_native_funcs;

  try {
    native_func_linker_.Link(
        available_native_funcs, &used_native_funcs, exe.get());
  } catch (const NativeFuncLinker::MissingFuncError &error) {
    msg_printer_.PrintError(
        (format("Function \"%1%\" is not exists") % error.GetFuncId()).str());
    return;
  }

  unique_ptr<VM> vm = vm_factory_.Create(exe.get(), used_native_funcs);
  vm->Execute();
}
}
}
