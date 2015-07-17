
#include <cassert>
#include "real_talk/vm/data_storage.h"

using std::ostream;
using std::hex;

namespace real_talk {
namespace vm {

static_assert(sizeof(IntValue) <= sizeof(DataStorage::Slot), "Int is too big");

DataStorage::DataStorage(size_t size): capacity_(size * 2),
                                       data_(new Slot[capacity_]()),
                                       current_slot_(data_.get() + size) {}

DataStorage::DataStorage(): DataStorage(0) {}

void DataStorage::PushInt(IntValue value) {
  const size_t size = DataTypeSize::kInt;
  EnsureCapacity(size);
  *(reinterpret_cast<IntValue*>(current_slot_)) = value;
  AfterPush(size);
}

void DataStorage::PopInt() {
  const size_t size = DataTypeSize::kInt;
  assert(HasSlots(size));
  current_slot_ -= size;
  AfterPop(size);
}

IntValue &DataStorage::GetInt(size_t index) {
  return *(reinterpret_cast<IntValue*>(GetSlot(index)));
}

void DataStorage::EnsureCapacity(size_t capacity) {
  assert(false);
}

void DataStorage::AfterPush(size_t pushed_slots_count) {
  current_slot_ += pushed_slots_count;
}

void DataStorage::AfterPop(size_t popped_slots_count) {
  current_slot_ -= popped_slots_count;
}

Slot *DataStorage::GetSlot(size_t index) const {
  assert(index < GetSize());
  return data_.get() + index;
}

bool DataStorage::HasSlots(size_t count) const {
  return GetSize() >= count;
}

size_t DataStorage::GetSize() const {
  assert(current_slot_ >= data_.get());
  return current_slot_ - data_.get();
}

bool operator==(const DataStorage &lhs, const DataStorage &rhs) {
  return lhs.capacity_ == rhs.capacity_
      && lhs.GetSize() == rhs.GetSize()
      && memcmp(lhs.data_.get(), rhs.data_.get(), lhs.GetSize());
}

ostream &operator<<(
    ostream &stream, const DataStorage &storage) {
  stream << "size=" << storage.GetSize() << "; capacity=" << storage.capacity_;

  for (const Slot *data_it = storage.data_.get();
       data_it != storage.data_.get() + storage.GetSize();
       ++data_it) {
    stream << hex << *data_it << ' ';
  }

  return stream;
}
}
}
