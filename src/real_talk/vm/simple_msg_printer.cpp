
#include <cassert>
#include <string>
#include <iostream>
#include "real_talk/vm/simple_msg_printer.h"

using std::ostream;
using std::string;

namespace real_talk {
namespace vm {

SimpleMsgPrinter::SimpleMsgPrinter(ostream *output_stream)
    : output_stream_(output_stream) {
  assert(output_stream);
}

void SimpleMsgPrinter::PrintError(const string &error) const {
  *output_stream_ << "[Error] " << error;
}

void SimpleMsgPrinter::PrintHelp(const string &help) const {
  *output_stream_ << help;
}
}
}
