
#include "real_talk/semantic/common_expr_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

CommonExprAnalysis::CommonExprAnalysis(
    unique_ptr<DataType> data_type,
    unique_ptr<DataType> casted_data_type,
    ValueType value_type)
    : base_analysis_(move(data_type), move(casted_data_type), value_type) {
}

const DataType *CommonExprAnalysis::GetCastedDataType() const {
  return base_analysis_.GetCastedDataType();
}

void CommonExprAnalysis::SetCastedDataType(unique_ptr<DataType> data_type) {
  base_analysis_.SetCastedDataType(move(data_type));
}

const DataType &CommonExprAnalysis::GetDataType() const {
  return base_analysis_.GetDataType();
}

ValueType CommonExprAnalysis::GetValueType() const {
  return base_analysis_.GetValueType();
}

bool CommonExprAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const CommonExprAnalysis &rhs =
      static_cast<const CommonExprAnalysis&>(analysis);
  return base_analysis_ == rhs.base_analysis_;
}

void CommonExprAnalysis::Print(ostream &stream) const {
  stream << base_analysis_;
}
}
}
