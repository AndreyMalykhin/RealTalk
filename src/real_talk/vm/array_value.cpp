
#include <cassert>
#include <memory>
#include <algorithm>
#include <vector>
#include "real_talk/vm/int_value.h"
#include "real_talk/vm/long_value.h"
#include "real_talk/vm/double_value.h"
#include "real_talk/vm/bool_value.h"
#include "real_talk/vm/char_value.h"
#include "real_talk/vm/string_value.h"
#include "real_talk/vm/array_value.h"
#include "real_talk/vm/data_storage.h"

using std::equal;
using std::reverse;
using std::unique_ptr;
using std::ostream;
using std::vector;
using std::move;

namespace real_talk {
namespace vm {

template<typename T> class ArrayValue<T>::Storage {
 public:
  Storage(unique_ptr<unsigned char> items, size_t size) noexcept;
  size_t &GetRefsCount() noexcept;
  const T *GetItems() const noexcept;
  T *GetItems() noexcept;
  const ArrayValue<T> *GetItemsArray() const noexcept;
  ArrayValue<T> *GetItemsArray() noexcept;
  size_t GetSize() const noexcept;

 private:
  size_t refs_count_;
  size_t size_;
  unique_ptr<unsigned char> items_;
};

template<typename T> ArrayValue<T> ArrayValue<T>::Unidimensional(size_t size) {
  ArrayValue<T> array(CreateStorage(size, sizeof(T)));
  const T *items_end_it = array.storage_->GetItems() + size;

  for (T *items_it = array.storage_->GetItems();
       items_it != items_end_it;
       ++items_it) {
    new(items_it) T();
  }

  return array;
}

template<typename T> ArrayValue<T> ArrayValue<T>::Unidimensional(
    size_t values_count, DataStorage *values) {
  ArrayValue<T> array(CreateStorage(values_count, sizeof(T)));
  const T *items_end_it = array.storage_->GetItems() + values_count;

  for (T *items_it = array.storage_->GetItems();
       items_it != items_end_it;
       ++items_it) {
    new(items_it) T(values->Pop<T>());
  }

  return array;
}

template<typename T> ArrayValue<T> ArrayValue<T>::Unidimensional(
    const vector<T> &values) {
  ArrayValue<T> array(CreateStorage(values.size(), sizeof(T)));
  const T *items_end_it = array.storage_->GetItems() + values.size();
  auto values_it = values.cbegin();

  for (T *items_it = array.storage_->GetItems();
       items_it != items_end_it;
       ++items_it, ++values_it) {
    new(items_it) T(*values_it);
  }

  return array;
}

template<typename T> ArrayValue<T> ArrayValue<T>::Multidimensional(
    vector<size_t>::iterator dimensions_start,
    vector<size_t>::iterator dimensions_end) {
  const auto dimensions_count = dimensions_end - dimensions_start;
  assert(dimensions_count > 0);
  const size_t size = *dimensions_start;

  if (dimensions_count == 1) {
    return Unidimensional(size);
  }

  ++dimensions_start;
  ArrayValue<T> array(CreateStorage(size, sizeof(ArrayValue<T>)));
  const ArrayValue<T> *items_end_it = array.storage_->GetItemsArray() + size;

  for (ArrayValue<T> *items_it = array.storage_->GetItemsArray();
       items_it != items_end_it;
       ++items_it) {
    new(items_it) ArrayValue<T>(
        Multidimensional(dimensions_start, dimensions_end));
  }

  return array;
}

template<typename T> ArrayValue<T> ArrayValue<T>::Multidimensional(
    uint8_t dimensions_count, size_t values_count, DataStorage *values) {
  assert(dimensions_count);
  assert(values);

  if (dimensions_count == 1) {
    return Unidimensional(values_count, values);
  }

  ArrayValue<T> array(CreateStorage(values_count, sizeof(ArrayValue<T>)));
  const ArrayValue<T> *items_end_it =
      array.storage_->GetItemsArray() + values_count;

  for (ArrayValue<T> *items_it = array.storage_->GetItemsArray();
       items_it != items_end_it;
       ++items_it) {
    new(items_it) ArrayValue<T>(values->Pop< ArrayValue<T> >());
  }

  return array;
}

template<typename T> ArrayValue<T> ArrayValue<T>::Multidimensional(
    vector< ArrayValue<T> > &&values) {
  ArrayValue<T> array(CreateStorage(values.size(), sizeof(ArrayValue<T>)));
  const ArrayValue<T> *items_end_it =
      array.storage_->GetItemsArray() + values.size();
  auto values_it = values.begin();

  for (ArrayValue<T> *items_it = array.storage_->GetItemsArray();
       items_it != items_end_it;
       ++items_it, ++values_it) {
    new(items_it) ArrayValue<T>(move(*values_it));
  }

  return array;
}

template<typename T> ArrayValue<T>::ArrayValue()
    : storage_(CreateStorage(0, sizeof(T))) {}

template<typename T> ArrayValue<T>::ArrayValue(Storage *storage) noexcept
    : storage_(storage) {
  assert(storage_);
}

template<typename T> ArrayValue<T>::ArrayValue(ArrayValue<T> &&value)
    noexcept: storage_(value.storage_) {
  assert(storage_);
  value.storage_ = nullptr;
}

template<typename T> void ArrayValue<T>::Destroy(uint8_t dimensions_count)
    noexcept {
  DecRefsCount(dimensions_count);
}

template<typename T> void ArrayValue<T>::DecRefsCount(uint8_t dimensions_count)
    noexcept {
  assert(storage_);

  if (--(storage_->GetRefsCount()) == 0) {
    if (dimensions_count == 1) {
      const T *items_end_it = storage_->GetItems() + storage_->GetSize();

      for (const T *items_it = storage_->GetItems();
           items_it != items_end_it;
           ++items_it) {
        items_it->~T();
      }
    } else {
      const ArrayValue<T> *items_end_it =
          storage_->GetItemsArray() + storage_->GetSize();

      for (ArrayValue<T> *items_it = storage_->GetItemsArray();
           items_it != items_end_it;
           ++items_it) {
        items_it->DecRefsCount(dimensions_count - 1);
      }
    }

    delete storage_;
    storage_ = nullptr;
  }
}

template<typename T> ArrayValue<T> ArrayValue<T>::Clone() const noexcept {
  ++(storage_->GetRefsCount());
  return ArrayValue<T>(storage_);
}

template<typename T> T &ArrayValue<T>::GetItem(size_t index) noexcept {
  return const_cast<T&>(static_cast<const ArrayValue*>(this)->GetItem(index));
}

template<typename T> const T &ArrayValue<T>::GetItem(size_t index)
    const noexcept {
  assert(storage_);
  assert(index < storage_->GetSize());
  return storage_->GetItems()[index];
}

template<typename T> ArrayValue<T> &ArrayValue<T>::GetItemArray(size_t index)
    noexcept {
  return const_cast<ArrayValue<T>&>(
      static_cast<const ArrayValue*>(this)->GetItemArray(index));
}

template<typename T> const ArrayValue<T> &ArrayValue<T>::GetItemArray(
    size_t index) const noexcept {
  assert(storage_);
  assert(index < storage_->GetSize());
  return storage_->GetItemsArray()[index];
}

template<typename T> void ArrayValue<T>::Set(
    ArrayValue<T> &&value, uint8_t dimensions_count) noexcept {
  if (this != &value) {
    assert(value.storage_);
    DecRefsCount(dimensions_count);
    storage_ = value.storage_;
    value.storage_ = nullptr;
  }
}

template<typename T> bool ArrayValue<T>::IsDeeplyEqual(
    const ArrayValue<T> &rhs, uint8_t dimensions_count) const noexcept {
  assert(storage_);
  assert(rhs.storage_);

  if (storage_->GetSize() != rhs.storage_->GetSize()) {
    return false;
  }

  if (dimensions_count == 1) {
    return equal(storage_->GetItems(),
                 storage_->GetItems() + storage_->GetSize(),
                 rhs.storage_->GetItems());
  }

  const ArrayValue<T> *items_end_it =
      storage_->GetItemsArray() + storage_->GetSize();
  const ArrayValue<T> *rhs_items_it = rhs.storage_->GetItemsArray();

  for (const ArrayValue<T> *items_it = storage_->GetItemsArray();
       items_it != items_end_it;
       ++items_it, ++rhs_items_it) {
    if (!items_it->IsDeeplyEqual(*rhs_items_it, dimensions_count - 1)) {
      return false;
    }
  }

  return true;
}

template<typename T> bool operator==(
    const ArrayValue<T> &lhs, const ArrayValue<T> &rhs) noexcept {
  assert(lhs.storage_);
  assert(rhs.storage_);
  return lhs.storage_ == rhs.storage_;
}

template<typename T> ostream &ArrayValue<T>::Print(
    ostream &stream, uint8_t dimensions_count) const {
  assert(storage_);
  stream << "refs_count=" << storage_->GetRefsCount()
         << "; size=" << storage_->GetSize() << "; items=[";

  if (dimensions_count == 1) {
    const T *items_end_it = storage_->GetItems() + storage_->GetSize();

    for (const T *items_it = storage_->GetItems();
         items_it != items_end_it;
         ++items_it) {
      stream << *items_it << ", ";
    }
  } else {
    const ArrayValue<T> *items_end_it =
        storage_->GetItemsArray() + storage_->GetSize();

    for (const ArrayValue<T> *items_it = storage_->GetItemsArray();
         items_it != items_end_it;
         ++items_it) {
      stream << '[';
      items_it->Print(stream, dimensions_count - 1) << "], ";
    }
  }

  return stream << ']';
}

template<typename T>
typename ArrayValue<T>::Storage *ArrayValue<T>::CreateStorage(
    size_t size, size_t item_size) {
  unique_ptr<unsigned char> items(static_cast<unsigned char*>(
      ::operator new(size * item_size)));
  return new Storage(move(items), size);
}

template<typename T> ArrayValue<T>::Storage::Storage(
    unique_ptr<unsigned char> items, size_t size) noexcept
    : refs_count_(1), size_(size), items_(move(items)) {
  assert(items_);
}

template<typename T> size_t &ArrayValue<T>::Storage::GetRefsCount() noexcept {
  return refs_count_;
}

template<typename T> const T *ArrayValue<T>::Storage::GetItems()
    const noexcept {
  return reinterpret_cast<const T*>(items_.get());
}

template<typename T> T *ArrayValue<T>::Storage::GetItems() noexcept {
  return reinterpret_cast<T*>(items_.get());
}

template<typename T>
const ArrayValue<T> *ArrayValue<T>::Storage::GetItemsArray() const noexcept {
  return reinterpret_cast<const ArrayValue<T>*>(items_.get());
}

template<typename T>
ArrayValue<T> *ArrayValue<T>::Storage::GetItemsArray() noexcept {
  return reinterpret_cast<ArrayValue<T>*>(items_.get());
}

template<typename T> size_t ArrayValue<T>::Storage::GetSize() const noexcept {
  return size_;
}

template class ArrayValue<IntValue>;
template class ArrayValue<LongValue>;
template class ArrayValue<DoubleValue>;
template class ArrayValue<CharValue>;
template class ArrayValue<BoolValue>;
template class ArrayValue<StringValue>;
}
}
