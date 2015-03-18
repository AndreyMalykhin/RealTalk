
#include <cmath>
#include <limits>
#include "real_talk/semantic/double_lit.h"

using std::ostream;
using std::unique_ptr;
using std::fabs;
using std::numeric_limits;

namespace real_talk {
namespace semantic {

DoubleLit::DoubleLit(double value): value_(value) {}

double DoubleLit::GetValue() const {
  return value_;
}

bool DoubleLit::IsEqual(const Lit &lit) const {
  const DoubleLit &rhs = static_cast<const DoubleLit&>(lit);
  return fabs(value_ - rhs.value_) <= numeric_limits<double>::epsilon();
}

void DoubleLit::Print(ostream &stream) const {
  stream << value_;
}
}
}
