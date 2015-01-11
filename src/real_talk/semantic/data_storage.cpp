
#include "real_talk/semantic/data_storage.h"

using std::ostream;

namespace real_talk {
namespace semantic {

ostream &operator<<(ostream &stream, DataStorage data_storage) {
  return stream << static_cast<uint32_t>(data_storage);
}
}
}
