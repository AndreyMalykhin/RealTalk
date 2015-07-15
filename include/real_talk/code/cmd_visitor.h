
#ifndef _REAL_TALK_CODE_CMD_VISITOR_H_
#define _REAL_TALK_CODE_CMD_VISITOR_H_

namespace real_talk {
namespace code {

class AndCmd;

class CmdVisitor {
 public:
  virtual ~CmdVisitor() {}
  virtual void VisitAnd(const AndCmd &cmd) = 0;
};
}
}
#endif
