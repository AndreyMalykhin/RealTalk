
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/string_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

string StringDataType::GetName() const {
  return SimpleLexer::kStringKeyword;
}

unique_ptr<DataType> StringDataType::Clone() const {
  return unique_ptr<DataType>(new StringDataType());
}

bool StringDataType::IsEqual(const DataType&) const {
  return true;
}

void StringDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
