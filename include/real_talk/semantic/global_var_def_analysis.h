
#ifndef _REAL_TALK_SEMANTIC_GLOBAL_VAR_DEF_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_GLOBAL_VAR_DEF_ANALYSIS_H_

#include <memory>
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/def_analysis.h"

namespace real_talk {
namespace semantic {

class GlobalVarDefAnalysis: public DefAnalysis {
 public:
  explicit GlobalVarDefAnalysis(std::unique_ptr<DataType> data_type);
  virtual const DataType &GetDataType() const override;
  virtual ValueType GetValueType() const override;
  virtual void Accept(DefAnalysisVisitor &visitor) const override;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<DataType> data_type_;
};
}
}
#endif
