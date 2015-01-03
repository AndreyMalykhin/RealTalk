
#include <string>
#include "real_talk/lexer/lexer.h"

using std::string;
using std::runtime_error;

namespace real_talk {
namespace lexer {

UnexpectedCharError::UnexpectedCharError(
    char c,
    uint32_t line_number,
    uint32_t column_number,
    const string &msg)
    : runtime_error(msg),
      c_(c),
      line_number_(line_number),
      column_number_(column_number) {
}

char UnexpectedCharError::GetChar() const {
  return c_;
}

uint32_t UnexpectedCharError::GetLineNumber() const {
  return line_number_;
}

uint32_t UnexpectedCharError::GetColumnNumber() const {
  return column_number_;
}
}
}
