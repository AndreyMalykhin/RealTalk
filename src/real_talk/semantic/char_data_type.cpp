
#include <string>
#include "real_talk/semantic/char_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string CharDataType::GetName() const {
  return "char";
}

const DataType &CharDataType::AsPrimitive() const {
  return *this;
}

unique_ptr<DataType> CharDataType::Clone() const {
  return unique_ptr<DataType>(new CharDataType());
}

bool CharDataType::IsEqual(const DataType&) const {
  return true;
}

void CharDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
