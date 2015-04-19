
#ifndef _REAL_TALK_SEMANTIC_RETURN_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_RETURN_ANALYSIS_H_

#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace parser {

class FuncDefNode;
}

namespace semantic {

class ReturnAnalysis: public NodeSemanticAnalysis {
 public:
  explicit ReturnAnalysis(const real_talk::parser::FuncDefNode *func_def);
  const real_talk::parser::FuncDefNode *GetFuncDef() const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  const real_talk::parser::FuncDefNode *func_def_;
};
}
}
#endif
