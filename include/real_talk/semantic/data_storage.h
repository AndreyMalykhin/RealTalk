
#ifndef _REAL_TALK_SEMANTIC_DATA_STORAGE_H_
#define _REAL_TALK_SEMANTIC_DATA_STORAGE_H_

#include <cstdint>
#include <iostream>

namespace real_talk {
namespace semantic {

enum class DataStorage: uint8_t {
  kGlobal = UINT8_C(1),
  kLocal
};

std::ostream &operator<<(std::ostream &stream,
                         DataStorage data_storage);
}
}
#endif
