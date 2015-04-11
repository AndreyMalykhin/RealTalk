
#include <boost/iterator/indirect_iterator.hpp>
#include <cassert>
#include <vector>
#include "real_talk/semantic/func_data_type.h"
#include "real_talk/semantic/data_type_visitor.h"

using std::vector;
using std::unique_ptr;
using std::string;
using std::equal;
using std::ostream;
using boost::make_indirect_iterator;

namespace real_talk {
namespace semantic {

FuncDataType::FuncDataType(
    unique_ptr<DataType> return_data_type,
    vector< unique_ptr<DataType> > arg_data_types)
    : return_data_type_(move(return_data_type)),
      arg_data_types_(move(arg_data_types)) {
  assert(return_data_type_);
}

DataTypeId FuncDataType::GetId() const {
  return DataTypeId::kFunc;
}

string FuncDataType::GetName() const {
  string name = return_data_type_->GetName();
  name += '(';

  if (!arg_data_types_.empty()) {
    auto last_arg_data_type_it = arg_data_types_.end() - 1;

    for (auto arg_data_type_it = arg_data_types_.begin();
         arg_data_type_it != last_arg_data_type_it;
         ++arg_data_type_it) {
      name += (*arg_data_type_it)->GetName();
      name += ", ";
    }

    name += (*last_arg_data_type_it)->GetName();
  }

  name += ')';
  return name;
}

unique_ptr<DataType> FuncDataType::Clone() const {
  vector< unique_ptr<DataType> > cloned_arg_data_types;

  for (const unique_ptr<DataType> &arg_data_type: arg_data_types_) {
    cloned_arg_data_types.push_back(arg_data_type->Clone());
  }

  return unique_ptr<DataType>(new FuncDataType(
      return_data_type_->Clone(), move(cloned_arg_data_types)));
}

void FuncDataType::Accept(DataTypeVisitor &visitor) const {
  visitor.VisitFunc(*this);
}

const DataType &FuncDataType::GetReturnDataType() const {
  return *return_data_type_;
}

const vector< unique_ptr<DataType> > &FuncDataType::GetArgDataTypes() const {
  return arg_data_types_;
}

bool FuncDataType::IsEqual(const DataType &data_type) const {
  const FuncDataType &rhs = static_cast<const FuncDataType&>(data_type);
  return *return_data_type_ == *(rhs.return_data_type_)
      && arg_data_types_.size() == rhs.arg_data_types_.size()
      && equal(make_indirect_iterator(arg_data_types_.begin()),
               make_indirect_iterator(arg_data_types_.end()),
               make_indirect_iterator(rhs.arg_data_types_.begin()));
}

void FuncDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
