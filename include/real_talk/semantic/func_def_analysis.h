
#ifndef _REAL_TALK_SEMANTIC_FUNC_DEF_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_FUNC_DEF_ANALYSIS_H_

#include <memory>
#include "real_talk/semantic/func_data_type.h"
#include "real_talk/semantic/def_analysis.h"

namespace real_talk {
namespace semantic {

class FuncDefAnalysis: public DefAnalysis {
 public:
  explicit FuncDefAnalysis(std::unique_ptr<FuncDataType> data_type);
  const FuncDataType &GetDataType() const override;

 private:
  virtual bool IsEqual(const DefAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<FuncDataType> data_type_;
};
}
}
#endif
