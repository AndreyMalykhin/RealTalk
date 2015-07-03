
#ifndef _REAL_TALK_SEMANTIC_RETURN_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_RETURN_ANALYSIS_H_

#include <vector>
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace parser {

class FuncDefNode;
class VarDefNode;
}

namespace semantic {

class ReturnAnalysis: public NodeSemanticAnalysis {
 public:
  ReturnAnalysis(
      const real_talk::parser::FuncDefNode *func_def,
      const std::vector<const real_talk::parser::VarDefNode*> &flow_local_var_defs);
  const real_talk::parser::FuncDefNode *GetFuncDef() const;
  const std::vector<const real_talk::parser::VarDefNode*>
  &GetFlowLocalVarDefs() const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  const real_talk::parser::FuncDefNode *func_def_;
  std::vector<const real_talk::parser::VarDefNode*> flow_local_var_defs_;
};
}
}
#endif
