
#include <string>
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

DataTypeId LongDataType::GetId() const {
  return DataTypeId::kLong;
}

string LongDataType::GetName() const {
  return "long";
}

void LongDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitLong(*this);
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
