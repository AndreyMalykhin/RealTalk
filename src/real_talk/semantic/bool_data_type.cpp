
#include <string>
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::unique_ptr;
using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

string BoolDataType::GetName() const {
  return "bool";
}

void BoolDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitBool(*this);
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
