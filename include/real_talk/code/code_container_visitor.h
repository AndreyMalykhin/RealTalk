
#ifndef _REAL_TALK_CODE_CODE_CONTAINER_VISITOR_H_
#define _REAL_TALK_CODE_CODE_CONTAINER_VISITOR_H_

namespace real_talk {
namespace code {

class Module;
class Exe;

class CodeContainerVisitor {
 public:
  virtual ~CodeContainerVisitor() {}
  virtual void VisitModule(const Module &module) const = 0;
  virtual void VisitExe(const Exe &exe) const = 0;
};
}
}
#endif
