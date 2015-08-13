
#include "real_talk/vm/array_value.h"

namespace real_talk {
namespace vm {

ArrayValue::ArrayValue(): storage_(nullptr) {}

bool operator==(const ArrayValue &lhs, const ArrayValue &rhs) {
  return lhs.storage_ == rhs.storage_;
}
}
}
