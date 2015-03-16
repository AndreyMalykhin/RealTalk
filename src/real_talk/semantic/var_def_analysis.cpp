
#include <cassert>
#include "real_talk/semantic/var_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

VarDefAnalysis::VarDefAnalysis(
    unique_ptr<DataType> data_type,
    DataStorage storage)
    : data_type_(move(data_type)),
      storage_(storage) {
  assert(data_type_);
}

DataStorage VarDefAnalysis::GetStorage() const {
  return storage_;
}

ValueType VarDefAnalysis::GetValueType() const {
  return ValueType::kLeft;
}

const DataType &VarDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool VarDefAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const VarDefAnalysis &rhs = static_cast<const VarDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_) && storage_ == rhs.storage_;
}

void VarDefAnalysis::Print(ostream &stream) const {
  stream << "storage=" << storage_ << "; data_type=" << *data_type_;
}
}
}
