
#ifndef _REAL_TALK_VM_MSG_PRINTER_H_
#define _REAL_TALK_VM_MSG_PRINTER_H_

#include <string>

namespace real_talk {
namespace vm {

class MsgPrinter {
 public:
  virtual ~MsgPrinter() {}
  virtual void PrintHelp(const std::string &help) const = 0;
  virtual void PrintError(const std::string &error) const = 0;
};
}
}
#endif
