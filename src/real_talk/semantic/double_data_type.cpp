
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/double_data_type.h"

using std::unique_ptr;
using std::string;
using std::ostream;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

string DoubleDataType::GetName() const {
  return SimpleLexer::kDoubleKeyword;
}

unique_ptr<DataType> DoubleDataType::Clone() const {
  return unique_ptr<DataType>(new DoubleDataType());
}

bool DoubleDataType::IsEqual(const DataType&) const {
  return true;
}

void DoubleDataType::Print(ostream &stream) const {
  stream << GetName();
}
}
}
