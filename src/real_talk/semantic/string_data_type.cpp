
#include <string>
#include "real_talk/semantic/string_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string StringDataType::GetName() const {
  return "string";
}

const DataType &StringDataType::AsPrimitive() const {
  return *this;
}

unique_ptr<DataType> StringDataType::Clone() const {
  return unique_ptr<DataType>(new StringDataType());
}

bool StringDataType::IsEqual(const DataType&) const {
  return true;
}

void StringDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
