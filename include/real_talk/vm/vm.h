
#ifndef _REAL_TALK_VM_VM_H_
#define _REAL_TALK_VM_VM_H_

namespace real_talk {
namespace vm {

class VM {
 public:
  virtual ~VM() {}
  virtual void Execute() const = 0;
};
}
}
#endif
