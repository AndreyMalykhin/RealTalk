
#ifndef _REAL_TALK_VM_ARRAY_VALUE_H_
#define _REAL_TALK_VM_ARRAY_VALUE_H_

#include <cstddef>
#include <iostream>

namespace real_talk {
namespace vm {

template<typename T> class ArrayValue;
template<typename T> bool operator==(
    const ArrayValue<T> &lhs, const ArrayValue<T> &rhs) noexcept;

template<typename T> class ArrayValue {
 public:
  explicit ArrayValue(size_t size = 0);
  ArrayValue(const ArrayValue<T> &value) noexcept;
  ArrayValue<T> &operator=(const ArrayValue<T>&) = delete;
  void Set(const ArrayValue<T> &value, uint8_t dimensions_count) noexcept;
  void Destroy(uint8_t dimensions_count) noexcept;
  T &GetItem(size_t index) noexcept;
  const T &GetItem(size_t index) const noexcept;
  ArrayValue<T> &GetArrayItem(size_t index) noexcept;
  const ArrayValue<T> &GetArrayItem(size_t index) const noexcept;
  bool IsDeeplyEqual(
      const ArrayValue<T> &rhs, uint8_t dimensions_count) const noexcept;
  void Print(std::ostream &stream, uint8_t dimensions_count) const;
  friend bool operator== <>(const ArrayValue<T> &lhs, const ArrayValue<T> &rhs)
      noexcept;

 private:
  class Storage;

  static Storage *CreateStorage(size_t size);
  void DecRefsCount(uint8_t dimensions_count) noexcept;

  Storage *storage_;
};
}
}
#endif
