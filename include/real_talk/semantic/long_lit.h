
#ifndef _REAL_TALK_SEMANTIC_LONG_LIT_H_
#define _REAL_TALK_SEMANTIC_LONG_LIT_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class LongLit: public Lit {
 public:
  explicit LongLit(int64_t value);
  int64_t GetValue() const;

 private:
  virtual bool IsEqual(const Lit &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  int64_t value_;
};
}
}
#endif
