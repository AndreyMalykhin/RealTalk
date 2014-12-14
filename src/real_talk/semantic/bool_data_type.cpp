
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/bool_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

string BoolDataType::GetName() const {
  return SimpleLexer::kBoolKeyword;
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
