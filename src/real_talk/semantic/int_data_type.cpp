
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/int_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

string IntDataType::GetName() const {
  return SimpleLexer::kIntKeyword;
}

unique_ptr<DataType> IntDataType::Clone() const {
  return unique_ptr<DataType>(new IntDataType());
}

bool IntDataType::IsEqual(const DataType&) const {
  return true;
}

void IntDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
