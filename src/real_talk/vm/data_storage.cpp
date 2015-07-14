
#include <cassert>
#include "real_talk/vm/data_storage.h"

using std::ostream;
using std::hex;

namespace real_talk {
namespace vm {

static_assert(sizeof(IntValue) <= sizeof(DataStorage::Slot), "Int is too big");

DataStorage::DataStorage() {}

DataStorage::DataStorage(size_t size): data_(size) {}

void DataStorage::PushInt(IntValue value) {
  assert(data_.size() + 1 > data_.size());
  data_.push_back(Slot(0));
  *(reinterpret_cast<IntValue*>(&(data_.back()))) = value;
}

void DataStorage::PopInt() {
  assert(!data_.empty());
  data_.pop_back();
}

IntValue &DataStorage::GetInt(size_t index) {
  assert(index < data_.size());
  return *(reinterpret_cast<IntValue*>(&(data_[index])));
}

bool operator==(const DataStorage &lhs, const DataStorage &rhs) {
  return lhs.data_ == rhs.data_;
}

ostream &operator<<(
    ostream &stream, const DataStorage &storage) {
  for (const DataStorage::Slot slot: storage.data_) {
    stream << hex << slot << ' ';
  }

  return stream;
}
}
}
