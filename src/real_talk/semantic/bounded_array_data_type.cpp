
#include <cassert>
#include <string>
#include <sstream>
#include "real_talk/semantic/bounded_array_data_type.h"
#include "real_talk/semantic/int_lit.h"

using std::string;
using std::unique_ptr;
using std::ostream;
using std::ostringstream;

namespace real_talk {
namespace semantic {

BoundedArrayDataType::BoundedArrayDataType(
    unique_ptr<DataType> element_data_type, unique_ptr<IntLit> size)
    : element_data_type_(move(element_data_type)), size_(move(size)) {
  assert(element_data_type_);
  assert(size_);
}

string BoundedArrayDataType::GetName() const {
  ostringstream name;
  Print(name);
  return name.str();
}

unique_ptr<DataType> BoundedArrayDataType::Clone() const {
  return unique_ptr<DataType>(
      new BoundedArrayDataType(element_data_type_->Clone(), size_->Clone()));
}

bool BoundedArrayDataType::IsEqual(const DataType &data_type) const {
  const BoundedArrayDataType &rhs =
      static_cast<const BoundedArrayDataType&>(data_type);
  return *size_ == *(rhs.size_) && *element_data_type_ == *(rhs.element_data_type_);
}

void BoundedArrayDataType::Print(ostream &stream) const {
  stream << element_data_type_->GetName() << "[" << *size_ << "]";
}
}
}
