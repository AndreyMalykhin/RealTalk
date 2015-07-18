
#ifndef _REAL_TALK_VM_VALUES_H_
#define _REAL_TALK_VM_VALUES_H_

#include <cstdint>

namespace real_talk {
namespace vm {

typedef int32_t IntValue;

class Array {};

class ArrayValue {
 public:
  ArrayValue();
  ~ArrayValue();
  ArrayValue(const ArrayValue &value);
  ArrayValue &operator=(const ArrayValue &value);

 private:
  Array *array_;
};
}
}
#endif
