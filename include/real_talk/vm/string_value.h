
#ifndef _REAL_TALK_VM_STRING_VALUE_H_
#define _REAL_TALK_VM_STRING_VALUE_H_

#include <string>
#include <iostream>

namespace real_talk {
namespace vm {

class StringValue {
 public:
  explicit StringValue(const std::string& str = "");
  StringValue(const StringValue &value);
  StringValue &operator=(const StringValue&) = delete;
  ~StringValue();
  friend bool operator==(const StringValue &lhs, const StringValue &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const StringValue &value);

 private:
  class Storage;
  void DecRefsCount() noexcept;

  Storage *storage_;
};
}
}
#endif
