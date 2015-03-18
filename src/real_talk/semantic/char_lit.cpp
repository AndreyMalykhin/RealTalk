
#include "real_talk/semantic/char_lit.h"

using std::ostream;
using std::unique_ptr;

namespace real_talk {
namespace semantic {

CharLit::CharLit(char value): value_(value) {}

char CharLit::GetValue() const {
  return value_;
}

bool CharLit::IsEqual(const Lit &lit) const {
  const CharLit &rhs = static_cast<const CharLit&>(lit);
  return value_ == rhs.value_;
}

void CharLit::Print(ostream &stream) const {
  stream << value_;
}
}
}
