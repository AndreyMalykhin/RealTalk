
#ifndef _REAL_TALK_SEMANTIC_BOOL_LIT_H_
#define _REAL_TALK_SEMANTIC_BOOL_LIT_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class BoolLit: public Lit {
 public:
  explicit BoolLit(bool value);
  bool GetValue() const;

 private:
  virtual bool IsEqual(const Lit &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  bool value_;
};
}
}
#endif
