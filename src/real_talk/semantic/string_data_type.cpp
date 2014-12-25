
#include <string>
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string StringDataType::GetName() const {
  return "string";
}

void StringDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitString(*this);
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
