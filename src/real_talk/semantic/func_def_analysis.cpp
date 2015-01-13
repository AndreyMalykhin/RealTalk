
#include <cassert>
#include "real_talk/semantic/func_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

FuncDefAnalysis::FuncDefAnalysis(
    unique_ptr<FuncDataType> data_type, bool is_native)
    : data_type_(move(data_type)), is_native_(is_native) {
  assert(data_type_);
}

ValueType FuncDefAnalysis::GetValueType() const {
  return ValueType::kRight;
}

const FuncDataType &FuncDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool FuncDefAnalysis::IsEqual(const DefAnalysis &analysis) const {
  const FuncDefAnalysis &rhs = static_cast<const FuncDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_) && is_native_ == rhs.is_native_;
}

void FuncDefAnalysis::Print(ostream &stream) const {
  stream << "data_type=" << *data_type_ << "; native=" << is_native_;
}
}
}
