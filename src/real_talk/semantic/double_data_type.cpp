
#include <string>
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

DataTypeId DoubleDataType::GetId() const {
  return DataTypeId::kDouble;
}

string DoubleDataType::GetName() const {
  return "double";
}

unique_ptr<DataType> DoubleDataType::Clone() const {
  return unique_ptr<DataType>(new DoubleDataType());
}

void DoubleDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitDouble(*this);
}

bool DoubleDataType::IsEqual(const DataType&) const {
  return true;
}

void DoubleDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
