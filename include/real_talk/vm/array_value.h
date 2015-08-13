
#ifndef _REAL_TALK_VM_ARRAY_VALUE_H_
#define _REAL_TALK_VM_ARRAY_VALUE_H_

#include <cstddef>

namespace real_talk {
namespace vm {

class ArrayValue {
 public:
  ArrayValue();
  friend bool operator==(const ArrayValue &lhs, const ArrayValue &rhs);

 private:
  class Storage;
  Storage *storage_;
};
}
}
#endif
