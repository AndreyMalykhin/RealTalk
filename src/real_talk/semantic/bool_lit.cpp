
#include "real_talk/semantic/bool_lit.h"

using std::ostream;
using std::unique_ptr;

namespace real_talk {
namespace semantic {

BoolLit::BoolLit(bool value): value_(value) {}

bool BoolLit::GetValue() const {
  return value_;
}

bool BoolLit::IsEqual(const Lit &lit) const {
  const BoolLit &rhs = static_cast<const BoolLit&>(lit);
  return value_ == rhs.value_;
}

void BoolLit::Print(ostream &stream) const {
  stream << value_;
}
}
}
