
#include <string>
#include "real_talk/semantic/string_lit.h"

using std::ostream;
using std::string;

namespace real_talk {
namespace semantic {

StringLit::StringLit(const string &value): value_(value) {
}

bool StringLit::IsEqual(const Lit &lit) const {
  const StringLit &rhs = static_cast<const StringLit&>(lit);
  return value_ == rhs.value_;
}

void StringLit::Print(ostream &stream) const {
  stream << value_;
}
}
}
