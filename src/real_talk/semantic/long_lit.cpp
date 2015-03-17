
#include "real_talk/semantic/long_lit.h"

using std::ostream;
using std::unique_ptr;

namespace real_talk {
namespace semantic {

LongLit::LongLit(int64_t value): value_(value) {}

int64_t LongLit::GetValue() const {
  return value_;
}

bool LongLit::IsEqual(const Lit &lit) const {
  const LongLit &rhs = static_cast<const LongLit&>(lit);
  return value_ == rhs.value_;
}

void LongLit::Print(ostream &stream) const {
  stream << value_;
}
}
}
