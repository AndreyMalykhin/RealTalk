
#include <string>
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

DataTypeId VoidDataType::GetId() const {
  return DataTypeId::kVoid;
}

string VoidDataType::GetName() const {
  return "void";
}

void VoidDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitVoid(*this);
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
