
#ifndef _REAL_TALK_LINKER_SIMPLE_MSG_PRINTER_H_
#define _REAL_TALK_LINKER_SIMPLE_MSG_PRINTER_H_

#include <string>
#include "real_talk/linker/msg_printer.h"

namespace real_talk {
namespace linker {

class SimpleMsgPrinter: public MsgPrinter {
 public:
  explicit SimpleMsgPrinter(std::ostream *output_stream);
  virtual void PrintError(const std::string &error) const override;
  virtual void PrintHelp(const std::string &help) const override;

 private:
  std::ostream *output_stream_;
};
}
}
#endif
