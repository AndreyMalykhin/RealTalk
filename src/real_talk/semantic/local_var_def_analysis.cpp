
#include <cassert>
#include "real_talk/semantic/def_analysis_visitor.h"
#include "real_talk/semantic/local_var_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

LocalVarDefAnalysis::LocalVarDefAnalysis(
    unique_ptr<DataType> data_type,
    uint32_t index_within_func)
    : data_type_(move(data_type)),
      index_within_func_(index_within_func) {
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

uint32_t LocalVarDefAnalysis::GetIndexWithinFunc() const {
  return index_within_func_;
}

bool LocalVarDefAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const LocalVarDefAnalysis &rhs =
      static_cast<const LocalVarDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_)
      && index_within_func_ == rhs.index_within_func_;
}

void LocalVarDefAnalysis::Print(ostream &stream) const {
  stream << "index_within_func=" << index_within_func_ << "; data_type="
         << *data_type_;
}
}
}
