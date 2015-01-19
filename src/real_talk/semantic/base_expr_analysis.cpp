
#include <cassert>
#include "real_talk/semantic/base_expr_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

BaseExprAnalysis::BaseExprAnalysis(
    unique_ptr<DataType> data_type, ValueType value_type)
    : data_type_(move(data_type)), value_type_(value_type) {
  assert(data_type_);
}

const DataType &BaseExprAnalysis::GetDataType() const {
  return *data_type_;
}

ValueType BaseExprAnalysis::GetValueType() const {
  return value_type_;
}

bool operator==(const BaseExprAnalysis &lhs,
                const BaseExprAnalysis &rhs) {
  return *(lhs.data_type_) == *(rhs.data_type_)
      && lhs.value_type_ == rhs.value_type_;
}

ostream &operator<<(ostream &stream, const BaseExprAnalysis &analysis) {
  return stream << "data_type=" << *(analysis.data_type_) << "; value_type="
                << analysis.value_type_;
}
}
}
