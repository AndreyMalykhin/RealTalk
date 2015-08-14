
#ifndef _REAL_TALK_VM_ARRAY_VALUE_H_
#define _REAL_TALK_VM_ARRAY_VALUE_H_

#include <cstddef>
#include <iostream>

namespace real_talk {
namespace vm {

template<typename T> class ArrayValue;
template<typename T> bool operator==(
    const ArrayValue<T> &lhs, const ArrayValue<T> &rhs) noexcept;
template<typename T> std::ostream &operator<<(
    std::ostream &stream, const ArrayValue<T> &value);

template<typename T> class ArrayValue {
 public:
  explicit ArrayValue(size_t size = 0);
  ArrayValue(const ArrayValue<T> &value);
  ~ArrayValue();
  ArrayValue<T> &operator=(const ArrayValue<T>&) = delete;
  friend bool operator== <>(const ArrayValue<T> &lhs, const ArrayValue<T> &rhs)
      noexcept;
  friend std::ostream &operator<< <>(
      std::ostream &stream, const ArrayValue<T> &value);

 private:
  class Storage;
  void DecRefsCount() noexcept;

  Storage *storage_;
};
}
}
#endif
