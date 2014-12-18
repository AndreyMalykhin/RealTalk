
#include <cassert>
#include <string>
#include "real_talk/semantic/array_data_type.h"

using std::string;
using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

ArrayDataType::ArrayDataType(unique_ptr<DataType> element_data_type)
    : element_data_type_(move(element_data_type)) {
  assert(element_data_type_);
}

string ArrayDataType::GetName() const {
  string name = element_data_type_->GetName();
  name += "[]";
  return name;
}

unique_ptr<DataType> ArrayDataType::Clone() const {
  return unique_ptr<DataType>(new ArrayDataType(element_data_type_->Clone()));
}

bool ArrayDataType::IsEqual(const DataType &data_type) const {
  const ArrayDataType &rhs = static_cast<const ArrayDataType&>(data_type);
  return *element_data_type_ == *(rhs.element_data_type_);
}

void ArrayDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
