
#include <cassert>
#include <string>
#include "real_talk/vm/string_value.h"

using std::string;

namespace real_talk {
namespace vm {

class StringValue::Storage {
 public:
  explicit Storage(const string &data);
  size_t &GetRefsCount() noexcept;

 private:
  std::string data_;
  size_t refs_count_;
};

StringValue::StringValue(const string &str): storage_(new Storage(str)) {}

StringValue::StringValue(const StringValue &value) {
  storage_ = value.storage_;
  assert(storage_);
  ++(storage_->GetRefsCount());
}

StringValue::~StringValue() {
  DecRefsCount();
}

void StringValue::DecRefsCount() noexcept {
  assert(storage_);

  if (--(storage_->GetRefsCount()) == 0) {
    delete storage_;
    storage_ = nullptr;
  }
}

StringValue::Storage::Storage(const string &data)
    : data_(data), refs_count_(1) {}

inline size_t &StringValue::Storage::GetRefsCount() noexcept {
  return refs_count_;
}
}
}
