
#include "real_talk/semantic/global_data_storage.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool GlobalDataStorage::IsEqual(const DataStorage&) const {
  return true;
}

void GlobalDataStorage::Print(std::ostream &stream) const {
  stream << "global";
}
}
}
