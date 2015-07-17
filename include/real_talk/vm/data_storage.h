
#ifndef _REAL_TALK_VM_DATA_STORAGE_H_
#define _REAL_TALK_VM_DATA_STORAGE_H_

#include <iostream>
#include <memory>
#include "real_talk/vm/values.h"

namespace real_talk {
namespace vm {

class DataStorage {
 public:
  typedef void* Slot;

  DataStorage();
  explicit DataStorage(size_t size);
  void PushInt(IntValue value);
  void PopInt();
  IntValue &GetInt(size_t index);
  friend bool operator==(const DataStorage &lhs, const DataStorage &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const DataStorage &storage);

 private:
  size_t capacity_;
  std::unique_ptr<Slot[]> data_;
  Slot *current_slot_;
};
}
}
#endif
