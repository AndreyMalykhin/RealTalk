
#include <string>
#include "real_talk/parser/parser.h"

using std::ostream;
using std::string;
using std::runtime_error;
using real_talk::lexer::TokenInfo;

namespace real_talk {
namespace parser {

Parser::UnexpectedTokenError::UnexpectedTokenError(
    const TokenInfo &token,
    const string &msg)
    : runtime_error(msg),
      token_(token) {
}

const TokenInfo &Parser::UnexpectedTokenError::GetToken() const {
  return token_;
}

bool operator==(const Parser::UnexpectedTokenError &lhs,
                const Parser::UnexpectedTokenError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0 && lhs.token_ == rhs.token_;
}

ostream &operator<<(
    ostream &stream, const Parser::UnexpectedTokenError &error) {
  return stream << "token=" << error.token_ << "; msg=" << error.what();
}
}
}
