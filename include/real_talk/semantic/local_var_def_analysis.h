
#ifndef _REAL_TALK_SEMANTIC_LOCAL_VAR_DEF_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_LOCAL_VAR_DEF_ANALYSIS_H_

#include <cstdint>
#include <memory>
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/data_storage.h"
#include "real_talk/semantic/def_analysis.h"

namespace real_talk {
namespace semantic {

class LocalVarDefAnalysis: public DefAnalysis {
 public:
  LocalVarDefAnalysis(std::unique_ptr<DataType> data_type,
                      uint32_t index_within_func);
  virtual const DataType &GetDataType() const override;
  virtual ValueType GetValueType() const override;
  virtual void Accept(DefAnalysisVisitor &visitor) const override;
  uint32_t GetIndexWithinFunc() const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<DataType> data_type_;
  uint32_t index_within_func_;
};
}
}
#endif
