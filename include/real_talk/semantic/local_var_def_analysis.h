
#ifndef _REAL_TALK_SEMANTIC_LOCAL_VAR_DEF_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_LOCAL_VAR_DEF_ANALYSIS_H_

#include <vector>
#include <memory>
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/def_analysis.h"

namespace real_talk {
namespace parser {

class VarDefNode;
}

namespace semantic {

class LocalVarDefAnalysis: public DefAnalysis {
 public:
  LocalVarDefAnalysis(
      std::unique_ptr<DataType> data_type,
      const std::vector<const real_talk::parser::VarDefNode*> &flow_local_var_defs);
  virtual const DataType &GetDataType() const override;
  virtual ValueType GetValueType() const override;
  virtual void Accept(DefAnalysisVisitor &visitor) const override;
  const std::vector<const real_talk::parser::VarDefNode*> &GetFlowLocalVarDefs()
      const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<DataType> data_type_;
  std::vector<const real_talk::parser::VarDefNode*> flow_local_var_defs_;
};
}
}
#endif
