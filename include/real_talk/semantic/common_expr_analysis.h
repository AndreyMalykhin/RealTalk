
#ifndef _REAL_TALK_SEMANTIC_COMMON_EXPR_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_COMMON_EXPR_ANALYSIS_H_

#include <memory>
#include "real_talk/semantic/expr_analysis.h"
#include "real_talk/semantic/base_expr_analysis.h"

namespace real_talk {
namespace semantic {

class CommonExprAnalysis: public ExprAnalysis {
 public:
  /**
   * @param casted_data_type Null if expr was not casted
   */
  CommonExprAnalysis(std::unique_ptr<DataType> data_type,
                     std::unique_ptr<DataType> casted_data_type,
                     ValueType value_type);
  virtual const DataType &GetDataType() const override;
  virtual const DataType *GetCastedDataType() const override;
  virtual void SetCastedDataType(std::unique_ptr<DataType> data_type) override;
  virtual ValueType GetValueType() const override;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  BaseExprAnalysis base_analysis_;
};
}
}
#endif
