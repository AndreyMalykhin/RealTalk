
#include <string>
#include "real_talk/parser/parser.h"

using std::string;
using std::runtime_error;
using real_talk::lexer::TokenInfo;

namespace real_talk {
namespace parser {

UnexpectedTokenError::UnexpectedTokenError(
    const TokenInfo &token,
    const string &msg)
    : runtime_error(msg),
      token_(token) {
}

const TokenInfo &UnexpectedTokenError::GetToken() const {
  return token_;
}
}
}
