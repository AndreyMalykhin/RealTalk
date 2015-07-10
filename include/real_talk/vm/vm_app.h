
#ifndef _REAL_TALK_VM_VM_APP_H_
#define _REAL_TALK_VM_VM_APP_H_

namespace real_talk {
namespace code {

class ExeReader;
}

namespace util {

class File;
}

namespace vm {

class VMConfigParser;
class NativeFuncStorage;
class NativeFuncLinker;
class VMFactory;
class VMConfig;

class VMApp {
 public:
  VMApp(const VMConfigParser &config_parser,
        const real_talk::util::File &file,
        const real_talk::code::ExeReader &exe_reader,
        const NativeFuncStorage &native_func_storage,
        const NativeFuncLinker &native_func_linker,
        const VMFactory &vm_factory,
        VMConfig *config);
  void Run(int argc, const char *argv[]);

 private:
  const VMConfigParser &config_parser_;
  const real_talk::util::File &file_;
  const real_talk::code::ExeReader &exe_reader_;
  const NativeFuncStorage &native_func_storage_;
  const NativeFuncLinker &native_func_linker_;
  const VMFactory &vm_factory_;
  VMConfig *config_;
};
}
}
#endif
