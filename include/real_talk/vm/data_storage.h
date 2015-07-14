
#ifndef _REAL_TALK_VM_DATA_STORAGE_H_
#define _REAL_TALK_VM_DATA_STORAGE_H_

#include <iostream>
#include <vector>
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
  std::vector<Slot> data_;
};
}
}
#endif
