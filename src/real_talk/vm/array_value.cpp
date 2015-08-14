
#include <cassert>
#include <memory>
#include "real_talk/vm/int_value.h"
#include "real_talk/vm/long_value.h"
#include "real_talk/vm/double_value.h"
#include "real_talk/vm/bool_value.h"
#include "real_talk/vm/char_value.h"
#include "real_talk/vm/string_value.h"
#include "real_talk/vm/array_value.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace vm {

template<typename T> class ArrayValue<T>::Storage {
 public:
  explicit Storage(size_t size);
  size_t &GetRefsCount() noexcept;
  const T *GetData() const noexcept;
  size_t GetSize() const noexcept;

 private:
  size_t refs_count_;
  size_t size_;
  unique_ptr<T[]> data_;
};

template<typename T> ArrayValue<T>::ArrayValue(size_t size)
    : storage_(new Storage(size)) {}

template<typename T> ArrayValue<T>::ArrayValue(const ArrayValue<T> &value) {
  storage_ = value.storage_;
  assert(storage_);
  ++(storage_->GetRefsCount());
}

template<typename T> ArrayValue<T>::~ArrayValue() {DecRefsCount();}

template<typename T> bool operator==(
    const ArrayValue<T> &lhs, const ArrayValue<T> &rhs) noexcept {
  return lhs.storage_ == rhs.storage_;
}

template bool operator==(
    const ArrayValue<IntValue> &lhs, const ArrayValue<IntValue> &rhs);
template bool operator==(
    const ArrayValue<LongValue> &lhs, const ArrayValue<LongValue> &rhs);
template bool operator==(
    const ArrayValue<DoubleValue> &lhs, const ArrayValue<DoubleValue> &rhs);
template bool operator==(
    const ArrayValue<CharValue> &lhs, const ArrayValue<CharValue> &rhs);
template bool operator==(
    const ArrayValue<BoolValue> &lhs, const ArrayValue<BoolValue> &rhs);
template bool operator==(
    const ArrayValue<StringValue> &lhs, const ArrayValue<StringValue> &rhs);

template<typename T> ostream &operator<<(
    ostream &stream, const ArrayValue<T> &value) {
  assert(value.storage_);
  return stream << "refs_count=" << value.storage_->GetRefsCount()
                << "; data=" << value.storage_->GetData()
                << "; size=" << value.storage_->GetSize();
}

template ostream &operator<<(
    ostream &stream, const ArrayValue<IntValue> &rhs);
template ostream &operator<<(
    ostream &stream, const ArrayValue<LongValue> &rhs);
template ostream &operator<<(
    ostream &stream, const ArrayValue<DoubleValue> &rhs);
template ostream &operator<<(
    ostream &stream, const ArrayValue<CharValue> &rhs);
template ostream &operator<<(
    ostream &stream, const ArrayValue<BoolValue> &rhs);
template ostream &operator<<(
    ostream &stream, const ArrayValue<StringValue> &rhs);

template<typename T> void ArrayValue<T>::DecRefsCount() noexcept {
  assert(storage_);

  if (--(storage_->GetRefsCount()) == 0) {
    delete storage_;
    storage_ = nullptr;
  }
}

template<typename T> ArrayValue<T>::Storage::Storage(size_t size)
    : refs_count_(1), size_(size), data_(new T[size]) {}

template<typename T>
size_t &ArrayValue<T>::Storage::GetRefsCount() noexcept {return refs_count_;}

template<typename T>
const T *ArrayValue<T>::Storage::GetData() const noexcept {return data_.get();}

template<typename T>
size_t ArrayValue<T>::Storage::GetSize() const noexcept {return size_;}

template class ArrayValue<IntValue>;
template class ArrayValue<LongValue>;
template class ArrayValue<DoubleValue>;
template class ArrayValue<CharValue>;
template class ArrayValue<BoolValue>;
template class ArrayValue<StringValue>;
}
}
