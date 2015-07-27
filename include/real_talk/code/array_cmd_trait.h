
#ifndef _REAL_TALK_CODE_ARRAY_CMD_TRAIT_H_
#define _REAL_TALK_CODE_ARRAY_CMD_TRAIT_H_

#include <cstdint>
#include <cassert>
#include <iostream>

namespace real_talk {
namespace code {

class ArrayCmdTrait {
 public:
  inline explicit ArrayCmdTrait(uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
    assert(dimensions_count_ != UINT8_C(0));
  }

  inline uint8_t GetDimensionsCount() const noexcept {
    return dimensions_count_;
  }

  inline void SetDimensionsCount(uint8_t dimensions_count) noexcept {
    dimensions_count_ = dimensions_count;
  }

 protected:
  void Print(std::ostream &stream) const {
    stream << "dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  bool IsEqual(const ArrayCmdTrait &rhs) const {
    return dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};
}
}
#endif
