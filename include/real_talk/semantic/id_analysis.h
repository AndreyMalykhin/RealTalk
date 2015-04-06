
#ifndef _REAL_TALK_SEMANTIC_ID_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_ID_ANALYSIS_H_

#include <memory>
#include "real_talk/semantic/expr_analysis.h"
#include "real_talk/semantic/base_expr_analysis.h"

namespace real_talk {
namespace parser {

class DefNode;
}

namespace semantic {

class IdAnalysis: public ExprAnalysis {
 public:
  IdAnalysis(std::unique_ptr<DataType> data_type,
             ValueType value_type,
             const real_talk::parser::DefNode* def,
             bool is_assignee);
  virtual const DataType &GetDataType() const override;
  virtual ValueType GetValueType() const override;
  const real_talk::parser::DefNode *GetDef() const;
  bool IsAssignee() const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  BaseExprAnalysis base_analysis_;
  const real_talk::parser::DefNode* def_;
  bool is_assignee_;
};
}
}
#endif
