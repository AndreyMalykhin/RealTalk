
#include <typeinfo>
#include "real_talk/semantic/lit.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const Lit &lhs, const Lit &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
}

bool operator!=(const Lit &lhs, const Lit &rhs) {
  return !(lhs == rhs);
}

ostream &operator<<(ostream &stream, const Lit &lit) {
  lit.Print(stream);
  return stream;
}
}
}

