
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
  Storage(unique_ptr<void> data, size_t size);
  size_t &GetRefsCount() noexcept;
  const void *GetData() const noexcept;
  size_t GetSize() const noexcept;

 private:
  size_t refs_count_;
  size_t size_;
  unique_ptr<void> data_;
};

template<typename T> static ArrayValue ArrayValue::Create(size_t size) {
  unique_ptr<void> data(new(size * sizeof(T)));
  return ArrayValue(new Storage(move(data), size));
}

ArrayValue::ArrayValue(const ArrayValue &value) noexcept {
  storage_ = value.storage_;
  assert(storage_);
  ++(storage_->GetRefsCount());
}

template<typename T> void Destruct(uint8_t dimensions_count) noexcept {
  assert(false);
}

template<typename T> bool ArrayValue::IsDeeplyEqual(
    const ArrayValue &rhs, uint8_t dimensions_count) const noexcept {
  assert(storage_);
  assert(rhs.storage_);

  if (storage_->GetSize() != rhs.storage_->GetSize()) {
    return false;
  }

  if (dimensions_count != 1) {
    const ArrayValue *items_end_it = storage_->GetData() + storage_->GetSize();
    const ArrayValue *rhs_items_it = rhs.storage_->GetData();

    for (const ArrayValue *items_it = storage_->GetData();
         items_it != items_end_it;
         ++items_it, ++rhs_items_it) {
      if (!items_it->IsDeeplyEqual<T>(rhs_items_it, dimensions_count - 1)) {
        return false;
      }
    }
  }

  return equal(
      static_cast<const T*>(storage_->GetData()),
      static_cast<const T*>(storage_->GetData() + storage_->GetSize()),
      static_cast<const T*>(rhs.storage_->GetData()));
}

bool operator==(const ArrayValue &lhs, const ArrayValue &rhs) noexcept {
  assert(lhs.storage_);
  assert(rhs.storage_);
  return lhs.storage_ == rhs.storage_;
}

template<typename T> void Print(ostream &stream, uint8_t dimensions_count) {
  assert(storage_);
  stream << "refs_count=" << storage_->GetRefsCount() << "; data=";
  const T *items_end_it = storage_->GetData() + storage_->GetSize();

  for (const T *items_it = storage_->GetData();
       items_it != items_end_it;
       ++items_it) {
    stream << *items_it << ", ";
  }

  stream << "; size=" << storage_->GetSize();
}

template void Print<IntValue>(ostream &stream, uint8_t dimensions_count);
template void Print<LongValue>(ostream &stream, uint8_t dimensions_count);
template void Print<DoubleValue>(ostream &stream, uint8_t dimensions_count);
template void Print<CharValue>(ostream &stream, uint8_t dimensions_count);
template void Print<BoolValue>(ostream &stream, uint8_t dimensions_count);
template void Print<StringValue>(ostream &stream, uint8_t dimensions_count);

inline ArrayValue::ArrayValue(Storage *storage) noexcept: storage_(storage) {
  assert(storage_);
}

void ArrayValue::DecRefsCount() noexcept {
  assert(storage_);

  if (--(storage_->GetRefsCount()) == 0) {
    delete storage_;
    storage_ = nullptr;
  }
}

inline ArrayValue::Storage::Storage(unique_ptr<void> data, size_t size)
    : refs_count_(1), size_(size), data_(move(data)) noexcept {
  assert(data_);
}

inline size_t &ArrayValue::Storage::GetRefsCount() noexcept {
  return refs_count_;
}

inline const void *ArrayValue::Storage::GetData() const noexcept {
  return data_.get();
}

inline size_t ArrayValue::Storage::GetSize() const noexcept {return size_;}
}
}
