
#include <string>
#include "real_talk/lexer/token_info.h"

using std::string;
using std::ostream;

namespace real_talk {
namespace lexer {

TokenInfo::TokenInfo(
    Token id,
    const string &value,
    uint32_t line,
    uint32_t column)
    : id_(id), value_(value), line_(line), column_(column) {
}

Token TokenInfo::GetId() const {
  return id_;
}

const string &TokenInfo::GetValue() const {
  return value_;
}

uint32_t TokenInfo::GetLine() const {
  return line_;
}

uint32_t TokenInfo::GetColumn() const {
  return column_;
}

bool operator==(const TokenInfo &lhs, const TokenInfo &rhs) {
  return lhs.id_ == rhs.id_
      && lhs.line_ == rhs.line_
      && lhs.column_ == rhs.column_
      && lhs.value_ == rhs.value_;
}

ostream &operator<<(ostream &stream, const TokenInfo &token) {
  return stream << "id=" << static_cast<uint32_t>(token.id_)
                << "; value=" << token.value_
                << "; line=" << token.line_
                << "; column=" << token.column_;
}
}
}
