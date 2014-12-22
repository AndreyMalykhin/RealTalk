
#include <string>
#include "real_talk/semantic/bool_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string BoolDataType::GetName() const {
  return "bool";
}

const DataType &BoolDataType::AsPrimitive() const {
  return *this;
}

unique_ptr<DataType> BoolDataType::Clone() const {
  return unique_ptr<DataType>(new BoolDataType());
}

bool BoolDataType::IsEqual(const DataType&) const {
  return true;
}

void BoolDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
