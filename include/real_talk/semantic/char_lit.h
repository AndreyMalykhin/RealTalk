
#ifndef _REAL_TALK_SEMANTIC_CHAR_LIT_H_
#define _REAL_TALK_SEMANTIC_CHAR_LIT_H_

#include <iostream>
#include <memory>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class CharLit: public Lit {
 public:
  explicit CharLit(char value);
  char GetValue() const;

 private:
  virtual bool IsEqual(const Lit &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  char value_;
};
}
}
#endif
