
#ifndef _REAL_TALK_SEMANTIC_SCOPE_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_SCOPE_ANALYSIS_H_

#include <cstdint>
#include <vector>
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace parser {

class VarDefNode;
}

namespace semantic {

class ScopeAnalysis: public NodeSemanticAnalysis {
 public:
  explicit ScopeAnalysis(
      const std::vector<const real_talk::parser::VarDefNode*> &local_var_defs);
  const std::vector<const real_talk::parser::VarDefNode*> &GetLocalVarDefs()
      const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::vector<const real_talk::parser::VarDefNode*> local_var_defs_;
};
}
}
#endif
