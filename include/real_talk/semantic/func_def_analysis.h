
#ifndef _REAL_TALK_SEMANTIC_FUNC_DEF_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_FUNC_DEF_ANALYSIS_H_

#include <memory>
#include "real_talk/semantic/func_data_type.h"
#include "real_talk/semantic/def_analysis.h"

namespace real_talk {
namespace semantic {

class FuncDefAnalysis: public DefAnalysis {
 public:
  FuncDefAnalysis(std::unique_ptr<FuncDataType> data_type,
                  bool is_native,
                  bool has_return);
  virtual const FuncDataType &GetDataType() const override;
  virtual ValueType GetValueType() const override;
  bool IsNative() const;
  bool HasReturn() const;
  void SetHasReturn(bool has_return);

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<FuncDataType> data_type_;
  bool is_native_;
  bool has_return_;
};
}
}
#endif
