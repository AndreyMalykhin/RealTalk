
#include <cassert>
#include "real_talk/semantic/func_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

FuncDefAnalysis::FuncDefAnalysis(
    unique_ptr<FuncDataType> data_type,
    bool is_native,
    bool has_return)
    : data_type_(move(data_type)),
      is_native_(is_native),
      has_return_(has_return) {
  assert(data_type_);
}

ValueType FuncDefAnalysis::GetValueType() const {
  return ValueType::kRight;
}

const FuncDataType &FuncDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool FuncDefAnalysis::IsNative() const {
  return is_native_;
}

bool FuncDefAnalysis::HasReturn() const {
  return has_return_;
}

void FuncDefAnalysis::SetHasReturn(bool has_return) {
  has_return_ = has_return;
}

bool FuncDefAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const FuncDefAnalysis &rhs = static_cast<const FuncDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_)
      && is_native_ == rhs.is_native_
      && has_return_ == rhs.has_return_;
}

void FuncDefAnalysis::Print(ostream &stream) const {
  stream << "data_type=" << *data_type_ << "; native=" << is_native_
         << "; has_return=" << has_return_;
}
}
}
