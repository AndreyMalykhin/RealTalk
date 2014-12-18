
#include <string>
#include "real_talk/semantic/void_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string VoidDataType::GetName() const {
  return "void";
}

unique_ptr<DataType> VoidDataType::Clone() const {
  return unique_ptr<DataType>(new VoidDataType());
}

bool VoidDataType::IsEqual(const DataType&) const {
  return true;
}

void VoidDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
