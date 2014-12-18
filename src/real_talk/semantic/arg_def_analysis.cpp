
#include <cassert>
#include "real_talk/semantic/arg_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

ArgDefAnalysis::ArgDefAnalysis(unique_ptr<DataType> data_type)
    : data_type_(move(data_type)) {
  assert(data_type_);
}

const DataType &ArgDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool ArgDefAnalysis::IsEqual(const DefAnalysis &analysis) const {
  const ArgDefAnalysis &rhs = static_cast<const ArgDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_);
}

void ArgDefAnalysis::Print(ostream &stream) const {
  stream << *data_type_;
}
}
}
