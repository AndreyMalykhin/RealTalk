
#include <typeinfo>
#include "real_talk/semantic/data_type.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const DataType &lhs, const DataType &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
}

bool operator!=(const DataType &lhs, const DataType &rhs) {
  return !(lhs == rhs);
}

ostream &operator<<(ostream &stream,
                    const DataType &data_type) {
  data_type.Print(stream);
  return stream;
}
}
}
