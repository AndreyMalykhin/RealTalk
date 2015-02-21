
#include <string>
#include "real_talk/lexer/lexer.h"

using std::ostream;
using std::string;
using std::runtime_error;

namespace real_talk {
namespace lexer {

Lexer::UnexpectedCharError::UnexpectedCharError(
    char c,
    uint32_t line_number,
    uint32_t column_number,
    const string &msg)
    : runtime_error(msg),
      c_(c),
      line_number_(line_number),
      column_number_(column_number) {
}

char Lexer::UnexpectedCharError::GetChar() const {
  return c_;
}

uint32_t Lexer::UnexpectedCharError::GetLineNumber() const {
  return line_number_;
}

uint32_t Lexer::UnexpectedCharError::GetColumnNumber() const {
  return column_number_;
}

bool operator==(const Lexer::UnexpectedCharError &lhs,
                const Lexer::UnexpectedCharError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.c_ == rhs.c_
      && lhs.line_number_ == rhs.line_number_
      && lhs.column_number_ == rhs.column_number_;
}

ostream &operator<<(ostream &stream, const Lexer::UnexpectedCharError &error) {
  return stream << "char=" << error.c_ << "; line=" << error.line_number_
                << "; column=" << error.column_number_ << "; msg="
                << error.what();
}
}
}
