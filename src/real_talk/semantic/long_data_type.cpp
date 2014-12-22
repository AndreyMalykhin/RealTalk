
#include <string>
#include "real_talk/semantic/long_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string LongDataType::GetName() const {
  return "long";
}

const DataType &LongDataType::AsPrimitive() const {
  return *this;
}

unique_ptr<DataType> LongDataType::Clone() const {
  return unique_ptr<DataType>(new LongDataType());
}

bool LongDataType::IsEqual(const DataType&) const {
  return true;
}

void LongDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
