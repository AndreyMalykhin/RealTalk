
#ifndef _REAL_TALK_SEMANTIC_EXPR_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_EXPR_ANALYSIS_H_

#include "real_talk/semantic/value_type.h"
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace semantic {

class DataType;

class ExprAnalysis: public NodeSemanticAnalysis {
 public:
  virtual const DataType &GetDataType() const = 0;
  virtual ValueType GetValueType() const = 0;
};
}
}
#endif
