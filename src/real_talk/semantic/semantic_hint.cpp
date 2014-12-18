
#include <string>
#include "real_talk/semantic/semantic_hint.h"

using std::string;
using std::ostream;

namespace real_talk {
namespace semantic {

SemanticHint::SemanticHint(
    const string &message,
    uint32_t line_number,
    uint32_t column_number,
    const string &file_path)
    : message_(message),
      line_number_(line_number),
      column_number_(column_number),
      file_path_(file_path) {
}

bool operator==(const SemanticHint &lhs, const SemanticHint &rhs) {
  return lhs.message_ == rhs.message_
      && lhs.line_number_ == rhs.line_number_
      && lhs.column_number_ == rhs.column_number_
      && lhs.file_path_ == rhs.file_path_;
}

ostream &operator<<(ostream &stream, const SemanticHint &hint) {
  return stream << "message=" << hint.message_ << "; line=" << hint.line_number_
                << "; column=" << hint.column_number_ << "; file_path="
                << hint.file_path_;
}
}
}
