
#ifndef _REAL_TALK_VM_DATA_STORAGE_H_
#define _REAL_TALK_VM_DATA_STORAGE_H_

#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>
#include "real_talk/code/data_type_size.h"
#include "real_talk/vm/int_value.h"
#include "real_talk/vm/long_value.h"
#include "real_talk/vm/double_value.h"
#include "real_talk/vm/bool_value.h"
#include "real_talk/vm/char_value.h"
#include "real_talk/vm/string_value.h"
#include "real_talk/vm/array_value.h"
#include "real_talk/vm/func_value.h"
#include "real_talk/vm/native_func_value.h"
#include "real_talk/vm/var_address_value.h"

namespace real_talk {
namespace vm {

class DataStorage {
 public:
  class OverflowError: public std::runtime_error {
   public:
    explicit OverflowError(const std::string &msg);
  };

  typedef void* Slot;

  DataStorage();
  explicit DataStorage(size_t size, size_t capacity);
  size_t GetSize() const noexcept;
  template<typename T> void Create(size_t index, T value = T());
  template<typename T> T &Get(size_t index) noexcept;
  template<typename T> const T &Get(size_t index) const noexcept;
  template<typename T> const T &GetTop() const noexcept;
  VarAddressValue GetAddress(size_t index) const noexcept;

  /**
   * @throws real_talk::vm::DataStorage::OverflowError
   */
  template<typename T> void Push(T value);

  template<typename T> T Pop() noexcept;
  friend bool operator==(const DataStorage &lhs, const DataStorage &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const DataStorage &storage);

 private:
  void EnsureCapacity(size_t slots_count) const;
  bool HasEnoughCapacity(size_t slots_count) const noexcept;
  void AfterPush(size_t pushed_slots_count) noexcept;
  Slot *GetSlot(size_t index) const noexcept;
  bool HasSlots(size_t count) const noexcept;
  template<typename TType, real_talk::code::DataTypeSize TSize> void DoPush(
      TType value);
  template<typename TType, real_talk::code::DataTypeSize TSize> TType DoPop()
      noexcept;
  template<typename TType, real_talk::code::DataTypeSize TSize>
  const TType &DoGetTop() const noexcept;

  size_t capacity_;
  std::unique_ptr<Slot[]> data_;
  Slot *current_slot_;
};
}
}
#endif
