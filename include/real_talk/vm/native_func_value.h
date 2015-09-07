
#ifndef _REAL_TALK_VM_NATIVE_FUNC_VALUE_H_
#define _REAL_TALK_VM_NATIVE_FUNC_VALUE_H_

namespace real_talk {
namespace vm {

class VM;

typedef void (*NativeFuncValue)(VM *vm);
}
}
#endif
