
#include "real_talk/stdlib/io.h"
#include "real_talk/vm/simple_native_func_storage.h"

namespace real_talk {
namespace vm {
namespace {

const auto &funcs = *new NativeFuncStorage::NativeFuncsMap({
    {"print", &real_talk::stdlib::Print}
  });
}

const NativeFuncStorage::NativeFuncsMap &SimpleNativeFuncStorage::GetAll()
    const {
  return funcs;
}
}
}
