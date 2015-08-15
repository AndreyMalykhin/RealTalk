
#ifndef _REAL_TALK_VM_ARRAY_VALUE_H_
#define _REAL_TALK_VM_ARRAY_VALUE_H_

#include <cstddef>
#include <iostream>

namespace real_talk {
namespace vm {

class ArrayValue {
 public:
  template<typename T> static ArrayValue Create(size_t size = 0);
  template<typename T> static void CreateAt(void *address, size_t size = 0);
  ArrayValue(const ArrayValue &value) noexcept;
  ArrayValue &operator=(const ArrayValue&) = delete;
  template<typename T> void Destruct(uint8_t dimensions_count) noexcept;
  template<typename T> bool IsDeeplyEqual(
      const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;
  template<typename T> void Print(
      std::ostream &stream, uint8_t dimensions_count) const;
  friend bool operator==(const ArrayValue &lhs, const ArrayValue &rhs) noexcept;

 private:
  class Storage;

  explicit ArrayValue(Storage *storage) noexcept;
  template<typename T> void DecRefsCount(uint8_t dimensions_count) noexcept;

  Storage *storage_;
};
}
}
#endif
