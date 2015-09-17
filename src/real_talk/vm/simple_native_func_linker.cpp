
#include <cassert>
#include <vector>
#include "real_talk/vm/simple_native_func_linker.h"

using std::vector;
using real_talk::code::Exe;

namespace real_talk {
namespace vm {

void SimpleNativeFuncLinker::Link(
    const NativeFuncStorage::NativeFuncsMap &available_funcs,
    vector<NativeFuncValue> *output_used_funcs,
    Exe *exe) const {
  assert(output_used_funcs);
  assert(exe);
  (void) available_funcs;
}
}
}
