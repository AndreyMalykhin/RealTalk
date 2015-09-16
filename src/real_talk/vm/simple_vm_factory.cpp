
#include <cassert>
#include <vector>
#include "real_talk/vm/simple_vm.h"
#include "real_talk/vm/simple_vm_factory.h"

using std::vector;
using std::unique_ptr;
using real_talk::code::Exe;

namespace real_talk {
namespace vm {

unique_ptr<VM> SimpleVMFactory::Create(
    Exe *exe, const vector<NativeFuncValue> &native_funcs) const {
  assert(exe);
  return unique_ptr<VM>(new SimpleVM(exe, native_funcs));
}
}
}
