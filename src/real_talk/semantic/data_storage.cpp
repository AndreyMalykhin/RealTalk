
#include <typeinfo>
#include "real_talk/semantic/data_storage.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const DataStorage &lhs, const DataStorage &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
}

ostream &operator<<(ostream &stream, const DataStorage &storage) {
  storage.Print(stream);
  return stream;
}
}
}
