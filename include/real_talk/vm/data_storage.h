
#ifndef _REAL_TALK_VM_DATA_STORAGE_H_
#define _REAL_TALK_VM_DATA_STORAGE_H_

#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>
#include "real_talk/vm/int_value.h"
#include "real_talk/vm/long_value.h"
#include "real_talk/vm/string_value.h"

namespace real_talk {
namespace vm {

class DataStorage {
 public:
  class MemorySizeOverflowError: public std::runtime_error {
   public:
    explicit MemorySizeOverflowError(const std::string &msg);
  };

  typedef void* Slot;

  DataStorage();
  explicit DataStorage(size_t size);
  size_t GetSize() const noexcept;
  void CreateInt(size_t index) noexcept;
  void CreateLong(size_t index) noexcept;
  void CreateDouble(size_t index) noexcept;
  void CreateBool(size_t index) noexcept;
  void CreateChar(size_t index) noexcept;
  void CreateString(size_t index);
  void CreateArray(size_t index);
  IntValue GetInt(size_t index) const noexcept;
  LongValue GetLong(size_t index) const noexcept;
  const StringValue &GetString(size_t index) const noexcept;
  void PushInt(IntValue value) noexcept;
  void PushString(StringValue value);
  friend bool operator==(const DataStorage &lhs, const DataStorage &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const DataStorage &storage);

 private:
  void EnsureCapacity(size_t slots_count);
  bool HasEnoughCapacity(size_t slots_count) const noexcept;
  void AfterPush(size_t pushed_slots_count) noexcept;
  void AfterPop(size_t popped_slots_count) noexcept;
  Slot *GetSlot(size_t index) const noexcept;
  bool HasSlots(size_t count) const noexcept;

  size_t capacity_;
  std::unique_ptr<Slot[]> data_;
  Slot *current_slot_;
};
}
}
#endif
