
#include <cassert>
#include <string>
#include "real_talk/code/data_type_size.h"
#include "real_talk/vm/data_storage.h"

using std::string;
using std::runtime_error;
using std::unique_ptr;
using std::memcpy;
using std::memcmp;
using std::ostream;
using std::hex;
using real_talk::code::DataTypeSize;

namespace real_talk {
namespace vm {

static_assert(sizeof(IntValue) <= static_cast<size_t>(DataTypeSize::kInt)
              * sizeof(DataStorage::Slot),
              "Int value doesn't fit into required slots count");
static_assert(sizeof(ArrayValue) <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Array value doesn't fit into required slots count");

DataStorage::DataStorage(size_t size): capacity_(size * 2),
                                       data_(new Slot[capacity_]()),
                                       current_slot_(data_.get() + size) {}

DataStorage::DataStorage(): DataStorage(0) {}

IntValue &DataStorage::GetInt(size_t index) noexcept {
  return *(reinterpret_cast<IntValue*>(GetSlot(index)));
}

ArrayValue &DataStorage::GetArray(size_t index) noexcept {
  return *(reinterpret_cast<ArrayValue*>(GetSlot(index)));
}

void DataStorage::EnsureCapacity(size_t slots_count) {
  if (HasEnoughCapacity(slots_count)) {
    return;
  }

  const size_t size = GetSize();
  const size_t new_capacity = (size + slots_count) * 2;

  if (new_capacity <= capacity_) {
    throw MemorySizeOverflowError("Memory size exceeds max value");
  }

  unique_ptr<Slot[]> new_data_(new Slot[new_capacity]());
  memcpy(new_data_.get(), data_.get(), capacity_);
  capacity_ = new_capacity;
  data_ = move(new_data_);
  current_slot_ = data_.get() + size;
  assert(capacity_ >= size + slots_count);
}

inline bool DataStorage::HasEnoughCapacity(size_t slots_count) const noexcept {
  return capacity_ - GetSize() >= slots_count;
}

inline void DataStorage::AfterPush(size_t pushed_slots_count) noexcept {
  current_slot_ += pushed_slots_count;
}

inline void DataStorage::AfterPop(size_t popped_slots_count) noexcept {
  current_slot_ -= popped_slots_count;
}

inline DataStorage::Slot *DataStorage::GetSlot(size_t index) const noexcept {
  assert(index < GetSize());
  return data_.get() + index;
}

inline bool DataStorage::HasSlots(size_t count) const noexcept {
  return GetSize() >= count;
}

inline size_t DataStorage::GetSize() const noexcept {
  assert(current_slot_ >= data_.get());
  return static_cast<size_t>(current_slot_ - data_.get());
}

bool operator==(const DataStorage &lhs, const DataStorage &rhs) {
  return lhs.capacity_ == rhs.capacity_
      && lhs.GetSize() == rhs.GetSize()
      && memcmp(lhs.data_.get(), rhs.data_.get(), lhs.GetSize()) == 0;
}

ostream &operator<<(
    ostream &stream, const DataStorage &storage) {
  stream << "size=" << storage.GetSize() << "; capacity=" << storage.capacity_
         << "; data=";

  for (const DataStorage::Slot *data_it = storage.data_.get();
       data_it != storage.data_.get() + storage.GetSize();
       ++data_it) {
    stream << hex << *data_it << ' ';
  }

  return stream;
}

DataStorage::MemorySizeOverflowError::MemorySizeOverflowError(const string &msg)
    : runtime_error(msg) {}
}
}
