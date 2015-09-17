
#include "real_talk/vm/vm.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/stdlib/io.h"

using std::cout;
using real_talk::vm::VM;
using real_talk::vm::StringValue;

namespace real_talk {
namespace stdlib {

void Print(VM *vm) {
  cout << vm->GetOperands().Pop<StringValue>().GetData();
}
}
}
