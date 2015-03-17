
#ifndef _REAL_TALK_SEMANTIC_VALUE_TYPE_H_
#define _REAL_TALK_SEMANTIC_VALUE_TYPE_H_

#include <cstdint>
#include <iostream>

namespace real_talk {
namespace semantic {

enum class ValueType: uint8_t {
  kLeft = UINT8_C(1),
  kRight = UINT8_C(2)
};

inline std::ostream &operator<<(std::ostream &stream, ValueType value_type) {
  return stream << static_cast<uint32_t>(value_type);
}
}
}
#endif
