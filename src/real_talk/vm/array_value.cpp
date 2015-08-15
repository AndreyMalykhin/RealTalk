
#include <cassert>
#include <memory>
#include <algorithm>
#include "real_talk/vm/int_value.h"
#include "real_talk/vm/long_value.h"
#include "real_talk/vm/double_value.h"
#include "real_talk/vm/bool_value.h"
#include "real_talk/vm/char_value.h"
#include "real_talk/vm/string_value.h"
#include "real_talk/vm/array_value.h"

using std::equal;
using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace vm {

class ArrayValue::Storage {
 public:
  Storage(unique_ptr<unsigned char> data, size_t size) noexcept;
  size_t &GetRefsCount() noexcept;
  template<typename T> const T *GetData() const noexcept;
  size_t GetSize() const noexcept;

 private:
  size_t refs_count_;
  size_t size_;
  unique_ptr<unsigned char> data_;
};

template<typename T> ArrayValue ArrayValue::Create(size_t size) {
  unique_ptr<unsigned char> data(
      static_cast<unsigned char*>(::operator new(size * sizeof(T))));
  return ArrayValue(new Storage(move(data), size));
}

template<typename T> void ArrayValue::CreateAt(void *address, size_t size) {
  unique_ptr<unsigned char> data(
      static_cast<unsigned char*>(::operator new(size * sizeof(T))));
  new(address) ArrayValue(new Storage(move(data), size));
}

template void ArrayValue::CreateAt<IntValue>(void *address, size_t size);
template void ArrayValue::CreateAt<LongValue>(void *address, size_t size);
template void ArrayValue::CreateAt<DoubleValue>(void *address, size_t size);
template void ArrayValue::CreateAt<CharValue>(void *address, size_t size);
template void ArrayValue::CreateAt<BoolValue>(void *address, size_t size);
template void ArrayValue::CreateAt<StringValue>(void *address, size_t size);

ArrayValue::ArrayValue(const ArrayValue &value) noexcept {
  storage_ = value.storage_;
  assert(storage_);
  ++(storage_->GetRefsCount());
}

template<typename T> void ArrayValue::Destruct(uint8_t dimensions_count)
    noexcept {
  DecRefsCount<T>(dimensions_count);
}

template<typename T> bool ArrayValue::IsDeeplyEqual(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept {
  assert(storage_);
  assert(rhs.storage_);

  if (storage_->GetSize() != rhs.storage_->GetSize()) {
    return false;
  }

  if (dimensions_count != 1) {
    const ArrayValue *items_end_it =
        storage_->GetData<ArrayValue>() + storage_->GetSize();
    const ArrayValue *rhs_items_it = rhs.storage_->GetData<ArrayValue>();

    for (const ArrayValue *items_it = storage_->GetData<ArrayValue>();
         items_it != items_end_it;
         ++items_it, ++rhs_items_it) {
      if (!items_it->IsDeeplyEqual<T>(*rhs_items_it, dimensions_count - 1)) {
        return false;
      }
    }

    return true;
  }

  return equal(storage_->GetData<T>(),
               storage_->GetData<T>() + storage_->GetSize(),
               rhs.storage_->GetData<T>());
}

template bool ArrayValue::IsDeeplyEqual<IntValue>(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;
template bool ArrayValue::IsDeeplyEqual<LongValue>(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;
template bool ArrayValue::IsDeeplyEqual<DoubleValue>(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;
template bool ArrayValue::IsDeeplyEqual<CharValue>(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;
template bool ArrayValue::IsDeeplyEqual<BoolValue>(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;
template bool ArrayValue::IsDeeplyEqual<StringValue>(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept;

bool operator==(const ArrayValue &lhs, const ArrayValue &rhs) noexcept {
  assert(lhs.storage_);
  assert(rhs.storage_);
  return lhs.storage_ == rhs.storage_;
}

template<typename T> void ArrayValue::Print(
    ostream &stream, uint8_t dimensions_count) const {
  assert(storage_);
  stream << "refs_count=" << storage_->GetRefsCount()
         << "; size=" << storage_->GetSize() << "; data=[";

  if (dimensions_count != 1) {
    const ArrayValue *items_end_it =
        storage_->GetData<ArrayValue>() + storage_->GetSize();

    for (const ArrayValue *items_it = storage_->GetData<ArrayValue>();
         items_it != items_end_it;
         ++items_it) {
      items_it->Print<T>(stream, dimensions_count - 1);
    }
  } else {
    const T *items_end_it = storage_->GetData<T>() + storage_->GetSize();

    for (const T *items_it = storage_->GetData<T>();
         items_it != items_end_it;
         ++items_it) {
      stream << *items_it << ", ";
    }
  }

  stream << ']';
}

template void ArrayValue::Print<IntValue>(
    ostream &stream, uint8_t dimensions_count) const;
template void ArrayValue::Print<LongValue>(
    ostream &stream, uint8_t dimensions_count) const;
template void ArrayValue::Print<DoubleValue>(
    ostream &stream, uint8_t dimensions_count) const;
template void ArrayValue::Print<CharValue>(
    ostream &stream, uint8_t dimensions_count) const;
template void ArrayValue::Print<BoolValue>(
    ostream &stream, uint8_t dimensions_count) const;
template void ArrayValue::Print<StringValue>(
    ostream &stream, uint8_t dimensions_count) const;

inline ArrayValue::ArrayValue(Storage *storage) noexcept: storage_(storage) {
  assert(storage_);
}

template<typename T> void ArrayValue::DecRefsCount(uint8_t dimensions_count)
    noexcept {
  assert(storage_);

  if (--(storage_->GetRefsCount()) != 0) {
    return;
  }

  if (dimensions_count != 1) {
    const ArrayValue *items_end_it =
        storage_->GetData<ArrayValue>() + storage_->GetSize();

    for (const ArrayValue *items_it = storage_->GetData<ArrayValue>();
         items_it != items_end_it;
         ++items_it) {
      items_it->DecRefsCount<T>(dimensions_count - 1);
    }
  } else {
    const T *items_end_it = storage_->GetData<T>() + storage_->GetSize();

    for (const T *items_it = storage_->GetData<T>();
         items_it != items_end_it;
         ++items_it) {
      items_it->~T();
    }
  }

  delete storage_;
  storage_ = nullptr;
}

inline ArrayValue::Storage::Storage(
    unique_ptr<unsigned char> data, size_t size) noexcept
    : refs_count_(1), size_(size), data_(move(data)) {
  assert(data_);
}

inline size_t &ArrayValue::Storage::GetRefsCount() noexcept {
  return refs_count_;
}

template<typename T> const T *ArrayValue::Storage::GetData() const noexcept {
  return reinterpret_cast<const T*>(data_.get());
}

inline size_t ArrayValue::Storage::GetSize() const noexcept {return size_;}
}
}
