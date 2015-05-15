
#ifndef _REAL_TALK_COMPILER_MSG_PRINTER_H_
#define _REAL_TALK_COMPILER_MSG_PRINTER_H_

#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace compiler {

class MsgPrinter {
 public:
  virtual ~MsgPrinter() {}
  virtual void PrintProblems(const real_talk::semantic::SemanticAnalysis::ProgramProblems &problems) const = 0;
};
}
}
#endif
