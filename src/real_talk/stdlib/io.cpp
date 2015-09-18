
#include "real_talk/vm/vm.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/stdlib/io.h"

using std::cout;
using std::to_string;
using real_talk::vm::VM;
using real_talk::vm::StringValue;
using real_talk::vm::DoubleValue;
using real_talk::vm::DataStorage;

namespace real_talk {
namespace stdlib {

void Print(VM *vm) {
  cout << vm->GetOperands().Pop<StringValue>().GetData();
}

void DoubleToString(real_talk::vm::VM *vm) {
  DataStorage &operands = vm->GetOperands();
  operands.Push(StringValue(to_string(operands.Pop<DoubleValue>())));
}
}
}
