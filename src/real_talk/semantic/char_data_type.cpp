
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/char_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

string CharDataType::GetName() const {
  return SimpleLexer::kCharKeyword;
}

unique_ptr<DataType> CharDataType::Clone() const {
  return unique_ptr<DataType>(new CharDataType());
}

bool CharDataType::IsEqual(const DataType&) const {
  return true;
}

void CharDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
