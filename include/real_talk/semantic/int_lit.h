
#ifndef _REAL_TALK_SEMANTIC_INT_LIT_H_
#define _REAL_TALK_SEMANTIC_INT_LIT_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class IntLit: public Lit {
 public:
  explicit IntLit(std::int32_t value);
  std::unique_ptr<IntLit> Clone() const;

 private:
  virtual bool IsEqual(const Lit &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::int32_t value_;
};
}
}
#endif
