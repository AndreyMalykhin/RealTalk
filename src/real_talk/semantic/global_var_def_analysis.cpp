
#include <cassert>
#include "real_talk/semantic/def_analysis_visitor.h"
#include "real_talk/semantic/global_var_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

GlobalVarDefAnalysis::GlobalVarDefAnalysis(unique_ptr<DataType> data_type)
    : data_type_(move(data_type)) {
  assert(data_type_);
}

void GlobalVarDefAnalysis::Accept(DefAnalysisVisitor &visitor) const {
  visitor.VisitGlobalVarDef(*this);
}

ValueType GlobalVarDefAnalysis::GetValueType() const {
  return ValueType::kLeft;
}

const DataType &GlobalVarDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool GlobalVarDefAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const GlobalVarDefAnalysis &rhs =
      static_cast<const GlobalVarDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_);
}

void GlobalVarDefAnalysis::Print(ostream &stream) const {
  stream << "data_type=" << *data_type_;
}
}
}
