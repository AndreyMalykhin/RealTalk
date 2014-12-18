
#include <cassert>
#include "real_talk/semantic/func_def_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

FuncDefAnalysis::FuncDefAnalysis(unique_ptr<DataType> data_type)
    : data_type_(move(data_type)) {
  assert(data_type_);
}

const DataType &FuncDefAnalysis::GetDataType() const {
  return *data_type_;
}

bool FuncDefAnalysis::IsEqual(const DefAnalysis &analysis) const {
  const FuncDefAnalysis &rhs = static_cast<const FuncDefAnalysis&>(analysis);
  return *data_type_ == *(rhs.data_type_);
}

void FuncDefAnalysis::Print(ostream &stream) const {
  stream << *data_type_;
}
}
}
