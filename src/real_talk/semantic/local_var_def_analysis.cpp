
#include <cassert>
#include <vector>
#include "real_talk/parser/var_def_node.h"
#include "real_talk/semantic/def_analysis_visitor.h"
#include "real_talk/semantic/local_var_def_analysis.h"

using std::unique_ptr;
using std::ostream;
using std::vector;
using real_talk::parser::VarDefNode;

namespace real_talk {
namespace semantic {

LocalVarDefAnalysis::LocalVarDefAnalysis(
    unique_ptr<DataType> data_type,
    const vector<const VarDefNode*> &flow_local_var_defs)
    : data_type_(move(data_type)),
      flow_local_var_defs_(flow_local_var_defs) {
  assert(data_type_);
}

void LocalVarDefAnalysis::Accept(DefAnalysisVisitor &visitor) const {
  visitor.VisitLocalVarDef(*this);
}

ValueType LocalVarDefAnalysis::GetValueType() const {
  return ValueType::kLeft;
}

const DataType &LocalVarDefAnalysis::GetDataType() const {
  return *data_type_;
}

const vector<const VarDefNode*> &LocalVarDefAnalysis::GetFlowLocalVarDefs()
    const {
  return flow_local_var_defs_;
}

bool LocalVarDefAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const LocalVarDefAnalysis &rhs =
      static_cast<const LocalVarDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_)
      && flow_local_var_defs_ == rhs.flow_local_var_defs_;
}

void LocalVarDefAnalysis::Print(ostream &stream) const {
  stream << "data_type=" << *data_type_ << "; flow_local_var_defs=\n";

  for (const VarDefNode *var_def: flow_local_var_defs_) {
    assert(var_def);
    stream << *var_def << '\n';
  }
}
}
}
