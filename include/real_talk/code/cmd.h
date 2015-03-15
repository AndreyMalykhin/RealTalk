
#ifndef _REAL_TALK_CODE_CMD_H_
#define _REAL_TALK_CODE_CMD_H_

#include <cstdint>
#include <typeinfo>
#include <iostream>

namespace real_talk {
namespace code {

enum class CmdId: uint8_t {
  kEndMain = UINT8_C(1),
  kEndFuncs = UINT8_C(2),
  kCreateGlobalIntVar = UINT8_C(3),
  kCreateGlobalArrayVar = UINT8_C(4),
  kCreateGlobalLongVar = UINT8_C(5),
  kCreateGlobalDoubleVar = UINT8_C(6),
  kCreateGlobalCharVar = UINT8_C(7),
  kCreateGlobalStringVar = UINT8_C(8),
  kCreateGlobalBoolVar = UINT8_C(9)
};

class Cmd {
 public:
  virtual ~Cmd() {}

  friend std::ostream &operator<<(std::ostream &stream, const Cmd &cmd) {
    stream << typeid(cmd).name() << "; ";
    cmd.Print(stream);
    return stream;
  }

  friend bool operator==(const Cmd &lhs, const Cmd &rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
  }

 private:
  virtual void Print(std::ostream &stream) const = 0;
  virtual bool IsEqual(const Cmd &cmd) const = 0;
};
}
}
#endif
