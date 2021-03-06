
#include <string>
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

DataTypeId CharDataType::GetId() const {
  return DataTypeId::kChar;
}

string CharDataType::GetName() const {
  return "char";
}

void CharDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitChar(*this);
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
