
#include "real_talk/semantic/int_lit.h"

using std::ostream;
using std::unique_ptr;

namespace real_talk {
namespace semantic {

IntLit::IntLit(int32_t value): value_(value) {
}

unique_ptr<IntLit> IntLit::Clone() const {
  return unique_ptr<IntLit>(new IntLit(value_));
}

bool IntLit::IsEqual(const Lit &lit) const {
  const IntLit &rhs = static_cast<const IntLit&>(lit);
  return value_ == rhs.value_;
}

void IntLit::Print(ostream &stream) const {
  stream << value_;
}
}
}
