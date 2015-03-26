
#ifndef _REAL_TALK_SEMANTIC_SCOPE_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_SCOPE_ANALYSIS_H_

#include <cstdint>
#include <memory>
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace semantic {

class ScopeAnalysis: public NodeSemanticAnalysis {
 public:
  explicit ScopeAnalysis(uint32_t local_vars_count);
  uint32_t GetLocalVarsCount() const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  uint32_t local_vars_count_;
};
}
}
#endif
