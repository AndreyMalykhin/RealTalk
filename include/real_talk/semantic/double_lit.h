
#ifndef _REAL_TALK_SEMANTIC_DOUBLE_LIT_H_
#define _REAL_TALK_SEMANTIC_DOUBLE_LIT_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class DoubleLit: public Lit {
 public:
  explicit DoubleLit(double value);

 private:
  virtual bool IsEqual(const Lit &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  double value_;
};
}
}
#endif
