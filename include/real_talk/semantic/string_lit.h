
#ifndef _REAL_TALK_SEMANTIC_STRING_LIT_H_
#define _REAL_TALK_SEMANTIC_STRING_LIT_H_

#include <iostream>
#include <string>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class StringLit: public Lit {
 public:
  explicit StringLit(const std::string &value);

 private:
  virtual bool IsEqual(const Lit &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::string value_;
};
}
}
#endif
