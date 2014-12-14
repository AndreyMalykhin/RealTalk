
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/long_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

string LongDataType::GetName() const {
  return SimpleLexer::kLongKeyword;
}

unique_ptr<DataType> LongDataType::Clone() const {
  return unique_ptr<DataType>(new LongDataType());
}

bool LongDataType::IsEqual(const DataType&) const {
  return true;
}

void LongDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
