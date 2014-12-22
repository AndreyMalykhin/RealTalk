
#include <string>
#include "real_talk/semantic/double_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string DoubleDataType::GetName() const {
  return "double";
}

unique_ptr<DataType> DoubleDataType::Clone() const {
  return unique_ptr<DataType>(new DoubleDataType());
}

const DataType &DoubleDataType::AsPrimitive() const {
  return *this;
}

bool DoubleDataType::IsEqual(const DataType&) const {
  return true;
}

void DoubleDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
