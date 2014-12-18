
#include "real_talk/semantic/local_data_storage.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool LocalDataStorage::IsEqual(const DataStorage&) const {
  return true;
}

void LocalDataStorage::Print(std::ostream &stream) const {
  stream << "local";
}
}
}
