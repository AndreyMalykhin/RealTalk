
#ifndef _REAL_TALK_CODE_EXE_H_
#define _REAL_TALK_CODE_EXE_H_

#include "real_talk/code/code_container.h"

namespace real_talk {
namespace code {

class Exe: public CodeContainer {
 public:
  virtual void Accept(const CodeContainerVisitor &visitor) const override;

 private:
  virtual bool IsEqual(const CodeContainer &container) const override;
  virtual void Print(std::ostream &stream) override;
};
}
}
#endif
