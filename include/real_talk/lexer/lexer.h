
#ifndef _REAL_TALK_LEXER_LEXER_H_
#define _REAL_TALK_LEXER_LEXER_H_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <iostream>

namespace real_talk {
namespace lexer {

class TokenInfo;

class Lexer {
 public:
  class UnexpectedCharError: public std::runtime_error {
   public:
    UnexpectedCharError(char c,
                        std::uint32_t line_number,
                        std::uint32_t column_number,
                        const std::string &msg);
    char GetChar() const;
    std::uint32_t GetLineNumber() const;
    std::uint32_t GetColumnNumber() const;
    friend bool operator==(const Lexer::UnexpectedCharError &lhs,
                           const Lexer::UnexpectedCharError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream, const Lexer::UnexpectedCharError &error);

   private:
    char c_;
    std::uint32_t line_number_;
    std::uint32_t column_number_;
  };

  virtual ~Lexer() {}

  /**
   * @throws real_talk::lexer::Lexer::UnexpectedCharError
   * @throws real_talk::util::IOError
   */
  virtual TokenInfo GetNextToken() = 0;
};
}
}

#endif
