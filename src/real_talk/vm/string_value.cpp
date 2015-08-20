
#include <cassert>
#include <string>
#include "real_talk/vm/string_value.h"

using std::string;
using std::ostream;

namespace real_talk {
namespace vm {

class StringValue::Storage {
 public:
  explicit Storage(const string &data);
  size_t &GetRefsCount() noexcept;
  const string &GetData() const noexcept;

 private:
  std::string data_;
  size_t refs_count_;
};

StringValue::StringValue(const string &str): storage_(new Storage(str)) {}

StringValue::StringValue(const StringValue &rhs) noexcept
    : storage_(rhs.storage_) {
  assert(storage_);
  ++(storage_->GetRefsCount());
}

StringValue::StringValue(StringValue &&rhs) noexcept
    : storage_(rhs.storage_) {
  assert(storage_);
  rhs.storage_ = nullptr;
}

void StringValue::operator=(const StringValue &rhs) noexcept {
  if (this != &rhs) {
    assert(rhs.storage_);
    DecRefsCount();
    storage_ = rhs.storage_;
    ++(storage_->GetRefsCount());
  }
}

void StringValue::operator=(StringValue &&rhs) noexcept {
  if (this != &rhs) {
    assert(rhs.storage_);
    storage_ = rhs.storage_;
    rhs.storage_ = nullptr;
  }
}

StringValue::~StringValue() {DecRefsCount();}

bool operator==(const StringValue &lhs, const StringValue &rhs) noexcept {
  assert(lhs.storage_);
  assert(rhs.storage_);
  return lhs.storage_ == rhs.storage_
      || lhs.storage_->GetData() == rhs.storage_->GetData();
}

ostream &operator<<(ostream &stream, const StringValue &value) {
  assert(value.storage_);
  return stream << "refs_count=" << value.storage_->GetRefsCount()
                << "; data=" << value.storage_->GetData();
}

void StringValue::DecRefsCount() noexcept {
  if (storage_ != nullptr && --(storage_->GetRefsCount()) == 0) {
    delete storage_;
    storage_ = nullptr;
  }
}

StringValue::Storage::Storage(const string &data)
    : data_(data), refs_count_(1) {}

inline size_t &StringValue::Storage::GetRefsCount() noexcept {
  return refs_count_;
}

inline const string &StringValue::Storage::GetData() const noexcept {
  return data_;
}
}
}
