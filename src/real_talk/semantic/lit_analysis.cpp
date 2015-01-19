
#include <cassert>
#include "real_talk/semantic/lit_analysis.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

LitAnalysis::LitAnalysis(
    unique_ptr<DataType> data_type, ValueType value_type, unique_ptr<Lit> lit)
    : base_analysis_(move(data_type), value_type), lit_(move(lit)) {
  assert(lit_);
}

const DataType &LitAnalysis::GetDataType() const {
  return base_analysis_.GetDataType();
}

ValueType LitAnalysis::GetValueType() const {
  return base_analysis_.GetValueType();
}

const Lit &LitAnalysis::GetLit() const {
  return *lit_;
}

bool LitAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const LitAnalysis &rhs =
      static_cast<const LitAnalysis&>(analysis);
  return base_analysis_ == rhs.base_analysis_ && *lit_ == *(rhs.lit_);
}

void LitAnalysis::Print(ostream &stream) const {
  stream << base_analysis_ << "; lit=" << *lit_;
}
}
}
