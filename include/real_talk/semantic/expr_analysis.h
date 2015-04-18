
#ifndef _REAL_TALK_SEMANTIC_EXPR_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_EXPR_ANALYSIS_H_

#include "real_talk/semantic/value_type.h"
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace semantic {

class DataType;

class ExprAnalysis: public NodeSemanticAnalysis {
 public:
  virtual ValueType GetValueType() const = 0;
  virtual const DataType &GetDataType() const = 0;

  /**
   * @return Null if expr was not casted
   */
  virtual const DataType *GetCastedDataType() const = 0;

  /**
   * @param data_type Null if expr was not casted
   */
  virtual void SetCastedDataType(std::unique_ptr<DataType> data_type) = 0;
};
}
}
#endif
