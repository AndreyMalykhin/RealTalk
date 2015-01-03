
#ifndef _REAL_TALK_LEXER_SIMPLE_LEXER_H_
#define _REAL_TALK_LEXER_SIMPLE_LEXER_H_

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/lexer/lexer.h"

namespace real_talk {
namespace lexer {

class SimpleLexer: public Lexer {
 public:
  static const char *kStringKeyword;
  static const char *kDoubleKeyword;
  static const char *kIntKeyword;
  static const char *kLongKeyword;
  static const char *kCharKeyword;
  static const char *kVoidKeyword;
  static const char *kBoolKeyword;
  static const char *kTrueKeyword;
  static const char *kFalseKeyword;
  static const char kSubscriptStartChar;
  static const char kSubscriptEndChar;
  static const char kEscapeChar;

  explicit SimpleLexer(std::istream &stream);
  virtual TokenInfo GetNextToken() override;

 private:
  bool IsNextCharExists();
  char GetNextChar();
  void ConsumeNextChar();
  void ConsumeLineEndChar();
  void AssertNextChar(char c);
  void AssertNextCharExists();
  void UnexpectedEndOfFile();
  void UnexpectedNextChar();
  void UnexpectedChar(char c);
  TokenInfo ParseAlphaNumToken();
  TokenInfo ParseNumberToken();
  TokenInfo ParseOneOrTwoCharTokenWithSharedPrefix(
    Token one_char_token, Token two_char_token, char second_char);
  TokenInfo ParseStringToken(Token token, char enclosing_char);
  TokenInfo ParsePlusOrMinusToken(Token common_token,
                                  Token change_by_one_token,
                                  char change_by_one_token_char);
  TokenInfo ParseTwoCharToken(Token token, char second_char);
  TokenInfo ParseOneCharToken(Token token);

  static const std::unordered_map<std::string, Token> &kKeywordTokens;
  std::istream &stream_;
  std::istreambuf_iterator<char> stream_it_;
  std::istreambuf_iterator<char> stream_end_it_;
  std::uint32_t line_number_;
  std::uint32_t column_number_;
};
}
}

#endif
