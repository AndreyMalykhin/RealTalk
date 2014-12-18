
#include <cassert>
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/expr_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

ExprAnalysis::ExprAnalysis(unique_ptr<DataType> data_type)
    : data_type_(move(data_type)) {
  assert(data_type_);
}

const DataType &ExprAnalysis::GetDataType() const {
  return *data_type_;
}

bool operator==(const ExprAnalysis &lhs, const ExprAnalysis &rhs) {
  return *(lhs.data_type_) == *(rhs.data_type_);
}

ostream &operator<<(ostream &stream, const ExprAnalysis &expr_analysis) {
  return stream << *(expr_analysis.data_type_);
}
}
}
