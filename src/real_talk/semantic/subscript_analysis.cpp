
#include <cassert>
#include "real_talk/semantic/subscript_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

SubscriptAnalysis::SubscriptAnalysis(
    unique_ptr<DataType> data_type,
    unique_ptr<DataType> casted_data_type,
    ValueType value_type,
    bool is_assignee)
    : base_analysis_(move(data_type), move(casted_data_type), value_type),
      is_assignee_(is_assignee) {
}

bool SubscriptAnalysis::IsAssignee() const {
  return is_assignee_;
}

const DataType *SubscriptAnalysis::GetCastedDataType() const {
  return base_analysis_.GetCastedDataType();
}

const DataType &SubscriptAnalysis::GetDataType() const {
  return base_analysis_.GetDataType();
}

ValueType SubscriptAnalysis::GetValueType() const {
  return base_analysis_.GetValueType();
}

bool SubscriptAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const SubscriptAnalysis &rhs =
      static_cast<const SubscriptAnalysis&>(analysis);
  return base_analysis_ == rhs.base_analysis_
      && is_assignee_ == rhs.is_assignee_;
}

void SubscriptAnalysis::Print(ostream &stream) const {
  stream << base_analysis_ << "; is_assignee=" << is_assignee_;
}
}
}
