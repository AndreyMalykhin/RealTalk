
#ifndef _REAL_TALK_SEMANTIC_DEF_ANALYSIS_VISITOR_H_
#define _REAL_TALK_SEMANTIC_DEF_ANALYSIS_VISITOR_H_

namespace real_talk {
namespace semantic {

class ArgDefAnalysis;
class VarDefAnalysis;
class FuncDefAnalysis;

class DefAnalysisVisitor {
 public:
  virtual ~DefAnalysisVisitor() {}
  virtual void VisitArgDef(const ArgDefAnalysis &analysis) = 0;
  virtual void VisitVarDef(const VarDefAnalysis &analysis) = 0;
  virtual void VisitFuncDef(const FuncDefAnalysis &analysis) = 0;
};
}
}
#endif
