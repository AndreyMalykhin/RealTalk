
#include <cassert>
#include <string>
#include "real_talk/vm/data_storage.h"

using std::string;
using std::runtime_error;
using std::unique_ptr;
using std::memcpy;
using std::memcmp;
using std::move;
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
static_assert(sizeof(VarAddressValue)
              <= static_cast<size_t>(DataTypeSize::kVarAddress)
              * sizeof(DataStorage::Slot),
              "Unsupported 'var address' size");
static_assert(sizeof(ArrayValue<IntValue>)
              <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'int array' size");
static_assert(sizeof(ArrayValue<LongValue>)
              <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'long array' size");
static_assert(sizeof(ArrayValue<DoubleValue>)
              <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'double array' size");
static_assert(sizeof(ArrayValue<CharValue>)
              <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'char array' size");
static_assert(sizeof(ArrayValue<BoolValue>)
              <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'bool array' size");
static_assert(sizeof(ArrayValue<StringValue>)
              <= static_cast<size_t>(DataTypeSize::kArray)
              * sizeof(DataStorage::Slot),
              "Unsupported 'string array' size");

DataStorage::OverflowError::OverflowError(const string &msg)
    : runtime_error(msg) {}

DataStorage::DataStorage(size_t size, size_t capacity)
    : capacity_(capacity),
      data_(new Slot[capacity]()),
      current_slot_(data_.get() + size) {
  assert(capacity >= size);
}

DataStorage::DataStorage(): DataStorage(0, 1024) {}

template<typename T> void DataStorage::Create(size_t index, T value) {
  new(GetSlot(index)) T(move(value));
}

template void DataStorage::Create<IntValue>(size_t index, IntValue value);
template void DataStorage::Create<LongValue>(size_t index, LongValue value);
template void DataStorage::Create<DoubleValue>(size_t index, DoubleValue value);
template void DataStorage::Create<CharValue>(size_t index, CharValue value);
template void DataStorage::Create<BoolValue>(size_t index, BoolValue value);
template void DataStorage::Create<StringValue>(size_t index, StringValue value);
template void DataStorage::Create< ArrayValue<IntValue> >(
    size_t index, ArrayValue<IntValue> value);
template void DataStorage::Create< ArrayValue<LongValue> >(
    size_t index, ArrayValue<LongValue> value);
template void DataStorage::Create< ArrayValue<DoubleValue> >(
    size_t index, ArrayValue<DoubleValue> value);
template void DataStorage::Create< ArrayValue<CharValue> >(
    size_t index, ArrayValue<CharValue> value);
template void DataStorage::Create< ArrayValue<BoolValue> >(
    size_t index, ArrayValue<BoolValue> value);
template void DataStorage::Create< ArrayValue<StringValue> >(
    size_t index, ArrayValue<StringValue> value);

VarAddressValue DataStorage::GetAddress(size_t index) const noexcept {
  return GetSlot(index);
}

template<typename T> const T &DataStorage::Get(size_t index) const noexcept {
  return *(reinterpret_cast<T*>(GetSlot(index)));
}

template<typename T> T &DataStorage::Get(size_t index) noexcept {
  return *(reinterpret_cast<T*>(GetSlot(index)));
}

template const IntValue &DataStorage::Get<IntValue>(size_t index)
    const noexcept;
template const LongValue &DataStorage::Get<LongValue>(size_t index)
    const noexcept;
template const DoubleValue &DataStorage::Get<DoubleValue>(size_t index)
    const noexcept;
template const CharValue &DataStorage::Get<CharValue>(size_t index)
    const noexcept;
template const BoolValue &DataStorage::Get<BoolValue>(size_t index)
    const noexcept;
template const StringValue &DataStorage::Get<StringValue>(size_t index)
    const noexcept;
template const FuncValue &DataStorage::Get<FuncValue>(size_t index)
    const noexcept;
template const NativeFuncValue &DataStorage::Get<NativeFuncValue>(
    size_t index) const noexcept;
template const VarAddressValue &DataStorage::Get<VarAddressValue>(
    size_t index) const noexcept;

template const ArrayValue<IntValue>
&DataStorage::Get< ArrayValue<IntValue> >(size_t index) const noexcept;
template const ArrayValue<LongValue>
&DataStorage::Get< ArrayValue<LongValue> >(size_t index) const noexcept;
template const ArrayValue<DoubleValue>
&DataStorage::Get< ArrayValue<DoubleValue> >(size_t index) const noexcept;
template const ArrayValue<CharValue>
&DataStorage::Get< ArrayValue<CharValue> >(size_t index) const noexcept;
template const ArrayValue<BoolValue>
&DataStorage::Get< ArrayValue<BoolValue> >(size_t index) const noexcept;
template const ArrayValue<StringValue>
&DataStorage::Get< ArrayValue<StringValue> >(size_t index) const noexcept;

template IntValue &DataStorage::Get<IntValue>(size_t index)
    noexcept;
template LongValue &DataStorage::Get<LongValue>(size_t index)
    noexcept;
template DoubleValue &DataStorage::Get<DoubleValue>(size_t index)
    noexcept;
template CharValue &DataStorage::Get<CharValue>(size_t index)
    noexcept;
template BoolValue &DataStorage::Get<BoolValue>(size_t index)
    noexcept;
template StringValue &DataStorage::Get<StringValue>(size_t index)
    noexcept;
template VarAddressValue &DataStorage::Get<VarAddressValue>(size_t index)
    noexcept;

template ArrayValue<IntValue>
&DataStorage::Get< ArrayValue<IntValue> >(size_t index) noexcept;
template ArrayValue<LongValue>
&DataStorage::Get< ArrayValue<LongValue> >(size_t index) noexcept;
template ArrayValue<DoubleValue>
&DataStorage::Get< ArrayValue<DoubleValue> >(size_t index) noexcept;
template ArrayValue<CharValue>
&DataStorage::Get< ArrayValue<CharValue> >(size_t index) noexcept;
template ArrayValue<BoolValue>
&DataStorage::Get< ArrayValue<BoolValue> >(size_t index) noexcept;
template ArrayValue<StringValue>
&DataStorage::Get< ArrayValue<StringValue> >(size_t index) noexcept;

template<> const IntValue &DataStorage::GetTop<IntValue>() const noexcept {
  return DoGetTop<IntValue, DataTypeSize::kInt>();
}

template<> const LongValue &DataStorage::GetTop<LongValue>() const noexcept {
  return DoGetTop<LongValue, DataTypeSize::kLong>();
}

template<> const DoubleValue &DataStorage::GetTop<DoubleValue>()
    const noexcept {
  return DoGetTop<DoubleValue, DataTypeSize::kDouble>();
}

template<> const CharValue &DataStorage::GetTop<CharValue>() const noexcept {
  return DoGetTop<CharValue, DataTypeSize::kChar>();
}

template<> const BoolValue &DataStorage::GetTop<BoolValue>() const noexcept {
  return DoGetTop<BoolValue, DataTypeSize::kBool>();
}

template<> const StringValue &DataStorage::GetTop<StringValue>()
    const noexcept {
  return DoGetTop<StringValue, DataTypeSize::kString>();
}

template<> const VarAddressValue &DataStorage::GetTop<VarAddressValue>()
    const noexcept {
  return DoGetTop<VarAddressValue, DataTypeSize::kVarAddress>();
}

template<> const FuncValue &DataStorage::GetTop<FuncValue>()
    const noexcept {
  return DoGetTop<FuncValue, DataTypeSize::kFunc>();
}

template<> const NativeFuncValue &DataStorage::GetTop<NativeFuncValue>()
    const noexcept {
  return DoGetTop<NativeFuncValue, DataTypeSize::kNativeFunc>();
}

template<> const ArrayValue<IntValue>
&DataStorage::GetTop<ArrayValue< IntValue> >() const noexcept {
  return DoGetTop<ArrayValue<IntValue>, DataTypeSize::kArray>();
}

template<> const ArrayValue<LongValue>
&DataStorage::GetTop< ArrayValue<LongValue> >() const noexcept {
  return DoGetTop<ArrayValue<LongValue>, DataTypeSize::kArray>();
}

template<> const ArrayValue<DoubleValue>
&DataStorage::GetTop< ArrayValue<DoubleValue> >() const noexcept {
  return DoGetTop<ArrayValue<DoubleValue>, DataTypeSize::kArray>();
}

template<> const ArrayValue<CharValue>
&DataStorage::GetTop< ArrayValue<CharValue> >() const noexcept {
  return DoGetTop<ArrayValue<CharValue>, DataTypeSize::kArray>();
}

template<> const ArrayValue<BoolValue>
&DataStorage::GetTop< ArrayValue<BoolValue> >() const noexcept {
  return DoGetTop<ArrayValue<BoolValue>, DataTypeSize::kArray>();
}

template<> const ArrayValue<StringValue>
&DataStorage::GetTop< ArrayValue<StringValue> >() const noexcept {
  return DoGetTop<ArrayValue<StringValue>, DataTypeSize::kArray>();
}

template<typename TType, DataTypeSize TSize>
const TType &DataStorage::DoGetTop() const noexcept {
  const size_t casted_size = static_cast<size_t>(TSize);
  assert(HasSlots(casted_size));
  return *(reinterpret_cast<TType*>(current_slot_ - casted_size));
}

template<> void DataStorage::Push<IntValue>(IntValue value) {
  DoPush<IntValue, DataTypeSize::kInt>(value);
}

template<> void DataStorage::Push<LongValue>(LongValue value) {
  DoPush<LongValue, DataTypeSize::kLong>(value);
}

template<> void DataStorage::Push<DoubleValue>(DoubleValue value) {
  DoPush<DoubleValue, DataTypeSize::kDouble>(value);
}

template<> void DataStorage::Push<CharValue>(CharValue value) {
  DoPush<CharValue, DataTypeSize::kChar>(value);
}

template<> void DataStorage::Push<BoolValue>(BoolValue value) {
  DoPush<BoolValue, DataTypeSize::kBool>(value);
}

template<> void DataStorage::Push<StringValue>(StringValue value) {
  DoPush<StringValue, DataTypeSize::kString>(move(value));
}

template<> void DataStorage::Push<FuncValue>(FuncValue value) {
  DoPush<FuncValue, DataTypeSize::kFunc>(value);
}

template<> void DataStorage::Push<NativeFuncValue>(NativeFuncValue value) {
  DoPush<NativeFuncValue, DataTypeSize::kNativeFunc>(value);
}

template<> void DataStorage::Push<VarAddressValue>(VarAddressValue value) {
  DoPush<VarAddressValue, DataTypeSize::kVarAddress>(value);
}

template<> void DataStorage::Push< ArrayValue<IntValue> >(
    ArrayValue<IntValue> value) {
  DoPush<ArrayValue<IntValue>, DataTypeSize::kArray>(move(value));
}

template<> void DataStorage::Push< ArrayValue<LongValue> >(
    ArrayValue<LongValue> value) {
  DoPush<ArrayValue<LongValue>, DataTypeSize::kArray>(move(value));
}

template<> void DataStorage::Push< ArrayValue<DoubleValue> >(
    ArrayValue<DoubleValue> value) {
  DoPush<ArrayValue<DoubleValue>, DataTypeSize::kArray>(move(value));
}

template<> void DataStorage::Push< ArrayValue<CharValue> >(
    ArrayValue<CharValue> value) {
  DoPush<ArrayValue<CharValue>, DataTypeSize::kArray>(move(value));
}

template<> void DataStorage::Push< ArrayValue<BoolValue> >(
    ArrayValue<BoolValue> value) {
  DoPush<ArrayValue<BoolValue>, DataTypeSize::kArray>(move(value));
}

template<> void DataStorage::Push< ArrayValue<StringValue> >(
    ArrayValue<StringValue> value) {
  DoPush<ArrayValue<StringValue>, DataTypeSize::kArray>(move(value));
}

template<typename TType, DataTypeSize TSize> void DataStorage::DoPush(
    TType value) {
  const size_t size = static_cast<size_t>(TSize);
  EnsureCapacity(size);
  new(current_slot_) TType(move(value));
  AfterPush(size);
}

template<> IntValue DataStorage::Pop<IntValue>() noexcept {
  return DoPop<IntValue, DataTypeSize::kInt>();
}

template<> LongValue DataStorage::Pop<LongValue>() noexcept {
  return DoPop<LongValue, DataTypeSize::kLong>();
}

template<> DoubleValue DataStorage::Pop<DoubleValue>() noexcept {
  return DoPop<DoubleValue, DataTypeSize::kDouble>();
}

template<> CharValue DataStorage::Pop<CharValue>() noexcept {
  return DoPop<CharValue, DataTypeSize::kChar>();
}

template<> BoolValue DataStorage::Pop<BoolValue>() noexcept {
  return DoPop<BoolValue, DataTypeSize::kBool>();
}

template<> StringValue DataStorage::Pop<StringValue>() noexcept {
  return DoPop<StringValue, DataTypeSize::kString>();
}

template<> FuncValue DataStorage::Pop<FuncValue>() noexcept {
  return DoPop<FuncValue, DataTypeSize::kFunc>();
}

template<> NativeFuncValue DataStorage::Pop<NativeFuncValue>() noexcept {
  return DoPop<NativeFuncValue, DataTypeSize::kNativeFunc>();
}

template<> VarAddressValue DataStorage::Pop<VarAddressValue>() noexcept {
  return DoPop<VarAddressValue, DataTypeSize::kVarAddress>();
}

template<> ArrayValue<IntValue> DataStorage::Pop<ArrayValue< IntValue> >()
    noexcept {
  return DoPop<ArrayValue<IntValue>, DataTypeSize::kArray>();
}

template<> ArrayValue<LongValue> DataStorage::Pop< ArrayValue<LongValue> >()
    noexcept {
  return DoPop<ArrayValue<LongValue>, DataTypeSize::kArray>();
}

template<> ArrayValue<DoubleValue> DataStorage::Pop< ArrayValue<DoubleValue> >()
    noexcept {
  return DoPop<ArrayValue<DoubleValue>, DataTypeSize::kArray>();
}

template<> ArrayValue<CharValue> DataStorage::Pop< ArrayValue<CharValue> >()
    noexcept {
  return DoPop<ArrayValue<CharValue>, DataTypeSize::kArray>();
}

template<> ArrayValue<BoolValue> DataStorage::Pop< ArrayValue<BoolValue> >()
    noexcept {
  return DoPop<ArrayValue<BoolValue>, DataTypeSize::kArray>();
}

template<> ArrayValue<StringValue> DataStorage::Pop< ArrayValue<StringValue> >()
    noexcept {
  return DoPop<ArrayValue<StringValue>, DataTypeSize::kArray>();
}

template<typename TType, DataTypeSize TSize> TType DataStorage::DoPop()
    noexcept {
  const size_t casted_size = static_cast<size_t>(TSize);
  assert(HasSlots(casted_size));
  current_slot_ -= casted_size;
  return move(*(reinterpret_cast<TType*>(current_slot_)));
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

void DataStorage::EnsureCapacity(size_t slots_count) const {
  if (HasEnoughCapacity(slots_count)) {
    return;
  }

  throw OverflowError("Storage size exceeds it's capacity");
}

inline bool DataStorage::HasEnoughCapacity(size_t slots_count) const noexcept {
  return capacity_ - GetSize() >= slots_count;
}

inline void DataStorage::AfterPush(size_t pushed_slots_count) noexcept {
  current_slot_ += pushed_slots_count;
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
