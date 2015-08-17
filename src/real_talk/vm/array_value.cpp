
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

template<typename T> class ArrayValue<T>::Storage {
 public:
  Storage(unique_ptr<unsigned char> data, size_t size) noexcept;
  size_t &GetRefsCount() noexcept;
  template<typename V> const V *GetData() const noexcept;
  template<typename V> V *GetData() noexcept;
  size_t GetSize() const noexcept;

 private:
  size_t refs_count_;
  size_t size_;
  unique_ptr<unsigned char> data_;
};

template<typename T> ArrayValue<T>::ArrayValue(size_t size)
    : storage_(CreateStorage(size)) {}

template<typename T> ArrayValue<T>::ArrayValue(const ArrayValue<T> &value)
    noexcept: storage_(value.storage_) {
  assert(storage_);
  ++(storage_->GetRefsCount());
}

template<typename T> void ArrayValue<T>::Set(
    const ArrayValue<T> &value, uint8_t dimensions_count) noexcept {
  if (this != &value) {
    assert(value.storage_);
    DecRefsCount(dimensions_count);
    storage_ = value.storage_;
    ++(storage_->GetRefsCount());
  }
}

template<typename T> void ArrayValue<T>::Destroy(uint8_t dimensions_count)
    noexcept {
  DecRefsCount(dimensions_count);
}

template<typename T> bool ArrayValue<T>::IsDeeplyEqual(
    const ArrayValue<T> &rhs, uint8_t dimensions_count) const noexcept {
  assert(storage_);
  assert(rhs.storage_);

  if (storage_->GetSize() != rhs.storage_->GetSize()) {
    return false;
  }

  if (dimensions_count == 1) {
    return equal(storage_->template GetData<T>(),
                 storage_->template GetData<T>() + storage_->GetSize(),
                 rhs.storage_->template GetData<T>());
  }

  const ArrayValue<T> *items_end_it =
      storage_->template GetData< ArrayValue<T> >() + storage_->GetSize();
  const ArrayValue<T> *rhs_items_it =
      rhs.storage_->template GetData< ArrayValue<T> >();

  for (const ArrayValue<T> *items_it =
           storage_->template GetData< ArrayValue<T> >();
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

template<typename T> void ArrayValue<T>::Print(
    ostream &stream, uint8_t dimensions_count) const {
  assert(storage_);
  stream << "refs_count=" << storage_->GetRefsCount()
         << "; size=" << storage_->GetSize() << "; data=[";

  if (dimensions_count == 1) {
    const T *items_end_it =
        storage_->template GetData<T>() + storage_->GetSize();

    for (const T *items_it = storage_->template GetData<T>();
         items_it != items_end_it;
         ++items_it) {
      stream << *items_it << ", ";
    }
  } else {
    const ArrayValue<T> *items_end_it =
        storage_->template GetData< ArrayValue<T> >() + storage_->GetSize();

    for (const ArrayValue<T> *items_it =
             storage_->template GetData< ArrayValue<T> >();
         items_it != items_end_it;
         ++items_it) {
      items_it->Print(stream, dimensions_count - 1);
    }
  }

  stream << ']';
}

template<typename T>
typename ArrayValue<T>::Storage *ArrayValue<T>::CreateStorage(size_t size) {
  unique_ptr<unsigned char> data(
      static_cast<unsigned char*>(::operator new(size * sizeof(T))));
  return new Storage(move(data), size);
}

template<typename T> void ArrayValue<T>::DecRefsCount(uint8_t dimensions_count)
    noexcept {
  assert(storage_);

  if (--(storage_->GetRefsCount()) == 0) {
    if (dimensions_count == 1) {
      const T *items_end_it =
          storage_->template GetData<T>() + storage_->GetSize();

      for (const T *items_it = storage_->template GetData<T>();
           items_it != items_end_it;
           ++items_it) {
        items_it->~T();
      }
    } else {
      const ArrayValue<T> *items_end_it =
          storage_->template GetData< ArrayValue<T> >() + storage_->GetSize();

      for (ArrayValue<T> *items_it =
               storage_->template GetData< ArrayValue<T> >();
           items_it != items_end_it;
           ++items_it) {
        items_it->DecRefsCount(dimensions_count - 1);
      }
    }

    delete storage_;
    storage_ = nullptr;
  }
}

template<typename T> ArrayValue<T>::Storage::Storage(
    unique_ptr<unsigned char> data, size_t size) noexcept
    : refs_count_(1), size_(size), data_(move(data)) {
  assert(data_);
}

template<typename T> size_t &ArrayValue<T>::Storage::GetRefsCount() noexcept {
  return refs_count_;
}

template<typename T> template<typename V>
const V *ArrayValue<T>::Storage::GetData() const noexcept {
  return reinterpret_cast<const V*>(data_.get());
}

template<typename T> template<typename V>
V *ArrayValue<T>::Storage::GetData() noexcept {
  return reinterpret_cast<V*>(data_.get());
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
