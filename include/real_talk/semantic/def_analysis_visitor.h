
#ifndef _REAL_TALK_SEMANTIC_DEF_ANALYSIS_VISITOR_H_
#define _REAL_TALK_SEMANTIC_DEF_ANALYSIS_VISITOR_H_

namespace real_talk {
namespace semantic {

class ArgDefAnalysis;
class LocalVarDefAnalysis;
class GlobalVarDefAnalysis;
class FuncDefAnalysis;

class DefAnalysisVisitor {
 public:
  virtual ~DefAnalysisVisitor() {}
  virtual void VisitArgDef(const ArgDefAnalysis &analysis) = 0;
  virtual void VisitLocalVarDef(const LocalVarDefAnalysis &analysis) = 0;
  virtual void VisitGlobalVarDef(const GlobalVarDefAnalysis &analysis) = 0;
  virtual void VisitFuncDef(const FuncDefAnalysis &analysis) = 0;
};
}
}
#endif
