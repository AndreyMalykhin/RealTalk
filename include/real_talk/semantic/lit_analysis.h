
#ifndef _REAL_TALK_SEMANTIC_LIT_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_LIT_ANALYSIS_H_

#include <memory>
#include <iostream>
#include "real_talk/semantic/lit.h"

namespace real_talk {
namespace semantic {

class LitAnalysis {
 public:
  explicit LitAnalysis(std::unique_ptr<Lit> lit);
  const Lit &GetLit() const;
  friend bool operator==(const LitAnalysis &lhs,
                         const LitAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const LitAnalysis &analysis);

 private:
  std::unique_ptr<Lit> lit_;
};
}
}
#endif
