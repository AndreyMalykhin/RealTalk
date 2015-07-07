
#ifndef _REAL_TALK_CODE_DATA_TYPE_SIZE_H_
#define _REAL_TALK_CODE_DATA_TYPE_SIZE_H_

#include <cstdint>

namespace real_talk {
namespace code {

enum class DataTypeSize: uint8_t {
  kBool = UINT8_C(1),
  kChar = UINT8_C(1),
  kInt = UINT8_C(1),
  kFunc = UINT8_C(1),
  kNativeFunc = UINT8_C(2),
  kLong = UINT8_C(2),
  kDouble = UINT8_C(2),
  kVarAddress = UINT8_C(2),
  kString = UINT8_C(4),
  kArray = UINT8_C(4)
};
}
}
#endif
