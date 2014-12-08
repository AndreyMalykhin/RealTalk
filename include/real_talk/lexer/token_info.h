#ifndef _REAL_TALK_LEXER_TOKEN_INFO_H_
#define _REAL_TALK_LEXER_TOKEN_INFO_H_

#include <cstdint>
#include <string>
#include <utility>
#include <functional>
#include <iostream>

namespace real_talk {
namespace lexer {

enum class Token: std::uint8_t {
  kIntType = UINT8_C(1),
  kLongType,
  kCharType,
  kDoubleType,
  kBoolType,
  kStringType,
  kVoidType,
  kIntLit,
  kLongLit,
  kDoubleLit,
  kBoolTrueLit,
  kBoolFalseLit,
  kCharLit,
  kStringLit,
  kName,
  kNew,
  kImport,
  kBreak,
  kContinue,
  kWhile,
  kIf,
  kElse,
  kAssignOp,
  kGreaterOrEqualOp,
  kGreaterOp,
  kLessOrEqualOp,
  kLessOp,
  kEqualOp,
  kNotEqualOp,
  kPreIncOp,
  kPreDecOp,
  kNotOp,
  kAndOp,
  kOrOp,
  kSumOp,
  kSubOp,
  kMulOp,
  kDivOp,
  kSubscriptStart,
  kSubscriptEnd,
  kGroupStart,
  kGroupEnd,
  kScopeStart,
  kScopeEnd,
  kSeparator,
  kStmtEnd,
  kFileEnd
};

class TokenInfo {
 public:
  TokenInfo(
      Token id,
      const std::string &value,
      std::uint32_t line,
      std::uint32_t column);
  Token GetId() const;
  const std::string &GetValue() const;
  std::uint32_t GetLine() const;
  std::uint32_t GetColumn() const;
  friend bool operator==(const TokenInfo &lhs, const TokenInfo &rhs);
  friend std::ostream &operator<<(std::ostream &stream, const TokenInfo &token);

 private:
  Token id_;
  std::string value_;
  std::uint32_t line_;
  std::uint32_t column_;
};
}
}

namespace std {

template<> struct hash<real_talk::lexer::Token> {
  std::size_t operator()(const real_talk::lexer::Token &token) const {
    return std::hash<std::uint8_t>()(static_cast<std::uint8_t>(token));
  }
};
}

#endif
