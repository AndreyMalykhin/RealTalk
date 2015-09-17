
#ifndef _REAL_TALK_VM_SIMPLE_MSG_PRINTER_H_
#define _REAL_TALK_VM_SIMPLE_MSG_PRINTER_H_

#include <string>
#include "real_talk/vm/msg_printer.h"

namespace real_talk {
namespace vm {

class SimpleMsgPrinter: public MsgPrinter {
 public:
  explicit SimpleMsgPrinter(std::ostream *output_stream);
  virtual void PrintHelp(const std::string &help) const override;
  virtual void PrintError(const std::string &error) const override;

 private:
  std::ostream *output_stream_;
};
}
}
#endif
