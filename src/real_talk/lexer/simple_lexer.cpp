
#include <boost/format.hpp>
#include <cassert>
#include <cctype>
#include <string>
#include "real_talk/lexer/simple_lexer.h"

using std::istream;
using std::string;
using std::unordered_map;
using std::char_traits;
using boost::format;

namespace real_talk {
namespace lexer {

const char *SimpleLexer::kStringKeyword = "string";
const char *SimpleLexer::kDoubleKeyword = "double";
const char *SimpleLexer::kIntKeyword = "int";
const char *SimpleLexer::kLongKeyword = "long";
const char *SimpleLexer::kCharKeyword = "char";
const char *SimpleLexer::kVoidKeyword = "void";
const char *SimpleLexer::kBoolKeyword = "bool";
const char *SimpleLexer::kTrueKeyword = "yeah";
const char *SimpleLexer::kFalseKeyword = "nah";
const char SimpleLexer::kSubscriptStartChar = '[';
const char SimpleLexer::kSubscriptEndChar = ']';
const char SimpleLexer::kEscapeChar = '\\';

const unordered_map<string, Token> &SimpleLexer::kKeywordTokens =
    *new unordered_map<string, Token>({
        {SimpleLexer::kIntKeyword, Token::kIntType},
        {SimpleLexer::kLongKeyword, Token::kLongType},
        {SimpleLexer::kCharKeyword, Token::kCharType},
        {SimpleLexer::kDoubleKeyword, Token::kDoubleType},
        {SimpleLexer::kStringKeyword, Token::kStringType},
        {SimpleLexer::kVoidKeyword, Token::kVoidType},
        {SimpleLexer::kBoolKeyword, Token::kBoolType},
        {SimpleLexer::kTrueKeyword, Token::kBoolTrueLit},
        {SimpleLexer::kFalseKeyword, Token::kBoolFalseLit},
        {"fresh", Token::kNew},
        {"checkthisout", Token::kImport},
        {"holdon", Token::kBreak},
        {"bringitback", Token::kReturn},
        {"keeprollin", Token::kContinue},
        {"roll", Token::kWhile},
        {"if", Token::kIf},
        {"else", Token::kElse}
      });

SimpleLexer::SimpleLexer(istream &stream)
    : stream_(stream),
      stream_it_(stream),
      stream_end_it_(),
      line_number_(UINT32_C(0)),
      column_number_(UINT32_C(0)) {
  stream_.exceptions(istream::failbit | istream::badbit);
}

TokenInfo SimpleLexer::GetNextToken() {
  while (IsNextCharExists()) {
    switch (GetNextChar()) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        return ParseNumberToken();
      }
      case '+': {
        return ParsePlusOrMinusToken(Token::kSumOp, Token::kPreIncOp, '+');
      }
      case '-': {
        return ParsePlusOrMinusToken(Token::kSubOp, Token::kPreDecOp, '-');
      }
      case '>': {
        return ParseOneOrTwoCharTokenWithSharedPrefix(
            Token::kGreaterOp, Token::kGreaterOrEqualOp, '=');
      }
      case '<': {
        return ParseOneOrTwoCharTokenWithSharedPrefix(
            Token::kLessOp, Token::kLessOrEqualOp, '=');
      }
      case '=': {
        return ParseOneOrTwoCharTokenWithSharedPrefix(
            Token::kAssignOp, Token::kEqualOp, '=');
      }
      case '!': {
        return ParseOneOrTwoCharTokenWithSharedPrefix(
            Token::kNotOp, Token::kNotEqualOp, '=');
      }
      case '*': {
        return ParseOneCharToken(Token::kMulOp);
      }
      case '/': {
        return ParseOneCharToken(Token::kDivOp);
      }
      case '&': {
        return ParseTwoCharToken(Token::kAndOp, '&');
      }
      case '|': {
        return ParseTwoCharToken(Token::kOrOp, '|');
      }
      case kSubscriptStartChar: {
        return ParseOneCharToken(Token::kSubscriptStart);
      }
      case kSubscriptEndChar: {
        return ParseOneCharToken(Token::kSubscriptEnd);
      }
      case '(': {
        return ParseOneCharToken(Token::kGroupStart);
      }
      case ')': {
        return ParseOneCharToken(Token::kGroupEnd);
      }
      case '{': {
        return ParseOneCharToken(Token::kScopeStart);
      }
      case '}': {
        return ParseOneCharToken(Token::kScopeEnd);
      }
      case ',': {
        return ParseOneCharToken(Token::kSeparator);
      }
      case ';': {
        return ParseOneCharToken(Token::kStmtEnd);
      }
      case '\'': {
        return ParseStringToken(Token::kCharLit, GetNextChar());
      }
      case '"': {
        return ParseStringToken(Token::kStringLit, GetNextChar());
      }
      case '\n': {
        ConsumeLineEndChar();
        break;
      }
      case ' ':
      case '\r':
      case '\t': {
        ConsumeNextChar();
        break;
      }
      default: {
        if (!isalpha(GetNextChar())) {
          UnexpectedNextChar();
        }

        return ParseAlphaNumToken();
      }
    }
  }

  return TokenInfo(Token::kFileEnd, "", line_number_, column_number_);
}

void SimpleLexer::UnexpectedNextChar() {
  UnexpectedChar(GetNextChar());
}

void SimpleLexer::UnexpectedEndOfFile() {
  UnexpectedChar(char_traits<char>::eof());
}

void SimpleLexer::UnexpectedChar(char c) {
  const string &msg =
      (format("Unexpected char; char=%1%; line=%2%; column=%3%")
       % c % line_number_ % column_number_).str();
  throw UnexpectedCharError(c, line_number_, column_number_, msg);
}

bool SimpleLexer::IsNextCharExists() {
  return stream_it_ != stream_end_it_;
}

char SimpleLexer::GetNextChar() {
  return *stream_it_;
}

void SimpleLexer::ConsumeNextChar() {
  ++stream_it_;
  ++column_number_;
}

void SimpleLexer::ConsumeLineEndChar() {
  ++stream_it_;
  ++line_number_;
  column_number_ = UINT32_C(0);
}

void SimpleLexer::AssertNextCharExists() {
  if (!IsNextCharExists()) {
    UnexpectedEndOfFile();
  }
}

void SimpleLexer::AssertNextChar(char c) {
  AssertNextCharExists();

  if (GetNextChar() != c) {
    UnexpectedNextChar();
  }
}

TokenInfo SimpleLexer::ParseOneCharToken(Token token) {
  const uint32_t start_column = column_number_;
  const string token_value(1, GetNextChar());
  ConsumeNextChar();
  return TokenInfo(token, token_value, line_number_, start_column);
}

TokenInfo SimpleLexer::ParseTwoCharToken(Token token, char second_char) {
  const uint32_t start_column = column_number_;
  string token_value(1, GetNextChar());
  ConsumeNextChar();
  AssertNextChar(second_char);
  ConsumeNextChar();
  token_value += second_char;
  return TokenInfo(token, token_value, line_number_, start_column);
}

TokenInfo SimpleLexer::ParseOneOrTwoCharTokenWithSharedPrefix(
    Token one_char_token, Token two_char_token, char second_char) {
  const uint32_t start_column = column_number_;
  string token_value(1, GetNextChar());
  ConsumeNextChar();

  if (IsNextCharExists() && GetNextChar() == second_char) {
    token_value += GetNextChar();
    ConsumeNextChar();
    return TokenInfo(two_char_token, token_value, line_number_, start_column);
  }

  return TokenInfo(one_char_token, token_value, line_number_, start_column);
}

TokenInfo SimpleLexer::ParseStringToken(Token token, char enclosing_char) {
  const uint32_t start_column = column_number_;
  const uint32_t start_line = line_number_;
  string token_value(1, GetNextChar());
  ConsumeNextChar();

  while (IsNextCharExists()) {
    switch (GetNextChar()) {
      case kEscapeChar: {
        token_value += GetNextChar();
        ConsumeNextChar();
        AssertNextCharExists();
        token_value += GetNextChar();
        ConsumeNextChar();
        break;
      }
      case '\n': {
        token_value += GetNextChar();
        ConsumeLineEndChar();
        break;
      }
      default: {
        if (GetNextChar() == enclosing_char) {
          token_value += GetNextChar();
          ConsumeNextChar();
          return TokenInfo(token, token_value, start_line, start_column);
        }

        token_value += GetNextChar();
        ConsumeNextChar();
        break;
      }
    }
  }

  UnexpectedEndOfFile();
  assert(false);
}

TokenInfo SimpleLexer::ParseNumberToken() {
  string token_value(1, GetNextChar());
  const uint32_t start_column = column_number_;
  ConsumeNextChar();

  while (IsNextCharExists()) {
    switch (GetNextChar()) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        token_value += GetNextChar();
        ConsumeNextChar();
        break;
      }
      case 'L': {
        token_value += GetNextChar();
        ConsumeNextChar();
        return TokenInfo(Token::kLongLit, token_value, line_number_, start_column);
      }
      case '.': {
        token_value += GetNextChar();
        ConsumeNextChar();

        while (IsNextCharExists() && isdigit(GetNextChar())) {
          token_value += GetNextChar();
          ConsumeNextChar();
        }

        return TokenInfo(
            Token::kDoubleLit, token_value, line_number_, start_column);
      }
      default: {
        return TokenInfo(
            Token::kIntLit, token_value, line_number_, start_column);
      }
    }
  }

  return TokenInfo(Token::kIntLit, token_value, line_number_, start_column);
}

TokenInfo SimpleLexer::ParsePlusOrMinusToken(Token common_token,
                                             Token change_by_one_token,
                                             char change_by_one_token_char) {
  const uint32_t start_column = column_number_;
  string token_value(1, GetNextChar());
  ConsumeNextChar();

  if (IsNextCharExists()) {
    if (GetNextChar() == change_by_one_token_char) {
      token_value += change_by_one_token_char;
      ConsumeNextChar();
      return TokenInfo(
          change_by_one_token, token_value, line_number_, start_column);
    }
  }

  return TokenInfo(common_token, token_value, line_number_, start_column);
}

TokenInfo SimpleLexer::ParseAlphaNumToken() {
  const uint32_t start_column = column_number_;
  string token_value(1, GetNextChar());
  ConsumeNextChar();

  while (IsNextCharExists()
         && (isalnum(GetNextChar()) || GetNextChar() == '_')) {
    token_value += GetNextChar();
    ConsumeNextChar();
  }

  unordered_map<string, Token>::const_iterator keyword_token_id_it =
      kKeywordTokens.find(token_value);

  if (keyword_token_id_it != kKeywordTokens.end()) {
    return TokenInfo(
        keyword_token_id_it->second, token_value, line_number_, start_column);
  }

  return TokenInfo(Token::kName, token_value, line_number_, start_column);
}

UnexpectedCharError::UnexpectedCharError(
    char c,
    std::uint32_t line_number,
    std::uint32_t column_number,
    const std::string &msg)
    : runtime_error(msg),
      c_(c),
      line_number_(line_number),
      column_number_(column_number) {
}

char UnexpectedCharError::GetChar() const {
  return c_;
}

std::uint32_t UnexpectedCharError::GetLineNumber() const {
  return line_number_;
}

std::uint32_t UnexpectedCharError::GetColumnNumber() const {
  return column_number_;
}
}
}
