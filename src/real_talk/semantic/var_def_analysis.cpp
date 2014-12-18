
#include <cassert>
#include "real_talk/semantic/var_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

VarDefAnalysis::VarDefAnalysis(
    unique_ptr<DataType> data_type,
    unique_ptr<DataStorage> storage)
    : data_type_(move(data_type)),
      storage_(move(storage)) {
  assert(data_type_);
  assert(storage_);
}

const DataType &VarDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool VarDefAnalysis::IsEqual(const DefAnalysis &analysis) const {
  const VarDefAnalysis &rhs = static_cast<const VarDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_) && *storage_ == *(rhs.storage_);
}

void VarDefAnalysis::Print(ostream &stream) const {
  stream << *storage_ << ' ' << *data_type_;
}
}
}
