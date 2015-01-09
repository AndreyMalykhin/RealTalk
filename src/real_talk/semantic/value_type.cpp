
#include "real_talk/semantic/value_type.h"

using std::ostream;

namespace real_talk {
namespace semantic {

ostream &operator<<(ostream &stream,
                    ValueType value_type) {
  return stream << static_cast<uint32_t>(value_type);
}
}
}
