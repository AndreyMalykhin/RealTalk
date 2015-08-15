
#include <cassert>
#include <string>
#include "real_talk/code/data_type_size.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/int_value.h"
#include "real_talk/vm/long_value.h"
#include "real_talk/vm/double_value.h"
#include "real_talk/vm/bool_value.h"
#include "real_talk/vm/char_value.h"
#include "real_talk/vm/string_value.h"
#include "real_talk/vm/array_value.h"

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
              "Unsupported 'int' size");
static_assert(sizeof(LongValue) <= static_cast<size_t>(DataTypeSize::kLong)
              * sizeof(DataStorage::Slot),
              "Unsupported 'long' size");
static_assert(sizeof(DoubleValue) <= static_cast<size_t>(DataTypeSize::kDouble)
              * sizeof(DataStorage::Slot),
              "Unsupported 'double' size");
static_assert(sizeof(BoolValue) <= static_cast<size_t>(DataTypeSize::kBool)
              * sizeof(DataStorage::Slot),
              "Unsupported 'bool' size");
static_assert(sizeof(BoolValue) <= static_cast<size_t>(DataTypeSize::kBool)
              * sizeof(DataStorage::Slot),
              "Unsupported 'char' size");
static_assert(sizeof(StringValue) <= static_cast<size_t>(DataTypeSize::kString)
              * sizeof(DataStorage::Slot),
              "Unsupported 'string' size");
static_assert(sizeof(ArrayValue) <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'array' size");

DataStorage::MemorySizeOverflowError::MemorySizeOverflowError(const string &msg)
    : runtime_error(msg) {}

DataStorage::DataStorage(size_t size): capacity_(size * 2),
                                       data_(new Slot[capacity_]()),
                                       current_slot_(data_.get() + size) {}

DataStorage::DataStorage(): DataStorage(0) {}

void DataStorage::CreateInt(size_t) noexcept {
  // slot already filled with zeros
}

void DataStorage::CreateLong(size_t) noexcept {
  // slot already filled with zeros
}

void DataStorage::CreateDouble(size_t) noexcept {
  // slot already filled with zeros
}

void DataStorage::CreateBool(size_t) noexcept {
  // slot already filled with zeros
}

void DataStorage::CreateChar(size_t) noexcept {
  // slot already filled with zeros
}

void DataStorage::CreateString(size_t index) {
  new(GetSlot(index)) StringValue();
}

void DataStorage::CreateArray(size_t index) {
  new(GetSlot(index)) ArrayValue();
}

// template void DataStorage::CreateArray<IntValue>(size_t index);
// template void DataStorage::CreateArray<LongValue>(size_t index);
// template void DataStorage::CreateArray<DoubleValue>(size_t index);
// template void DataStorage::CreateArray<CharValue>(size_t index);
// template void DataStorage::CreateArray<BoolValue>(size_t index);
// template void DataStorage::CreateArray<StringValue>(size_t index);

IntValue DataStorage::GetInt(size_t index) const noexcept {
  return *Get<IntValue>(index);
}

LongValue DataStorage::GetLong(size_t index) const noexcept {
  return *Get<LongValue>(index);
}

DoubleValue DataStorage::GetDouble(size_t index) const noexcept {
  return *Get<DoubleValue>(index);
}

BoolValue DataStorage::GetBool(size_t index) const noexcept {
  return *Get<BoolValue>(index);
}

CharValue DataStorage::GetChar(size_t index) const noexcept {
  return *Get<CharValue>(index);
}

const StringValue &DataStorage::GetString(size_t index) const noexcept {
  return *Get<StringValue>(index);
}

const ArrayValue &DataStorage::GetArray(size_t index) const noexcept {
  return *Get<ArrayValue>(index);
}

void DataStorage::PushInt(IntValue value) noexcept {
  const size_t size = static_cast<size_t>(DataTypeSize::kInt);
  EnsureCapacity(size);
  *(reinterpret_cast<IntValue*>(current_slot_)) = value;
  AfterPush(size);
}

void DataStorage::PushString(StringValue value) {
  const size_t size = static_cast<size_t>(DataTypeSize::kString);
  EnsureCapacity(size);
  new(current_slot_) StringValue(value);
  AfterPush(size);
}

size_t DataStorage::GetSize() const noexcept {
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

template<typename T> const T *DataStorage::Get(size_t index) const noexcept {
  return reinterpret_cast<T*>(GetSlot(index));
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
}
}
