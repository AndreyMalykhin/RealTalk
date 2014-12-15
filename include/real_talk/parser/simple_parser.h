
#ifndef _REAL_TALK_PARSER_SIMPLE_PARSER_H_
#define _REAL_TALK_PARSER_SIMPLE_PARSER_H_

#include <boost/functional/hash.hpp>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <string>
#include <stack>
#include <vector>
#include <stdexcept>
#include <utility>
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ProgramNode;
class StmtNode;
class ExprNode;
class ArgDefNode;
class DataTypeNode;
class ScopeNode;
class IfNode;
class BoundedArrayDataTypeNode;

class UnexpectedTokenError: public std::runtime_error {
 public:
  UnexpectedTokenError(
      const real_talk::lexer::TokenInfo &token,
      const std::string &msg);
  const real_talk::lexer::TokenInfo &GetToken() const;

 private:
  real_talk::lexer::TokenInfo token_;
};

class SimpleParser {
 public:
  explicit SimpleParser(std::unique_ptr<real_talk::lexer::Lexer> lexer);
  std::unique_ptr<ProgramNode> Parse();

 private:
  struct Op;
  struct ExprState;
  enum class OpAssoc: std::uint8_t;

  typedef ExprState (SimpleParser::*ExprStateHandler)(
      std::stack<Op> &operators,
      std::stack< std::unique_ptr<ExprNode> > &operands);
  typedef std::pair<real_talk::lexer::Token, OpAssoc> OpPriosKey;
  typedef std::unordered_map<OpPriosKey,
                             std::uint8_t,
                             boost::hash<OpPriosKey> > OpPrios;

  enum class OpAssoc: std::uint8_t {
    kLeftToRight = UINT8_C(1),
    kRightToLeft
  };

  struct Op {
    std::vector<real_talk::lexer::TokenInfo> tokens;
    std::vector< std::unique_ptr<ExprNode> > args;
    std::uint8_t prio;
    OpAssoc assoc;
  };

  struct ExprState {
    ExprStateHandler handler;
  };

  std::vector< std::unique_ptr<StmtNode> > ParseStmts(
      real_talk::lexer::Token until_token);
  std::unique_ptr<StmtNode> ParseStmt();
  std::unique_ptr<StmtNode> ParseDef(
      std::unique_ptr<DataTypeNode> simple_data_type);
  std::unique_ptr<StmtNode> ParseFuncDef(
      std::unique_ptr<DataTypeNode> return_data_type,
      const real_talk::lexer::TokenInfo &func_name_token);
  std::unique_ptr<StmtNode> ParseExprStmt();
  std::unique_ptr<StmtNode> ParsePreTestLoop();
  std::unique_ptr<StmtNode> ParseIf();
  std::unique_ptr<StmtNode> ParseImport();
  std::unique_ptr<StmtNode> ParseBreak();
  std::unique_ptr<StmtNode> ParseContinue();
  std::unique_ptr<StmtNode> ParseReturn();
  std::unique_ptr<ExprNode> ParseExpr();
  std::unique_ptr<ExprNode> ParseArrayAlloc();
  std::unique_ptr<ExprNode> ParseId();
  std::unique_ptr<ExprNode> ParseInt();
  std::unique_ptr<ExprNode> ParseLong();
  std::unique_ptr<ExprNode> ParseChar();
  std::unique_ptr<ExprNode> ParseString();
  std::unique_ptr<ExprNode> ParseDouble();
  std::unique_ptr<ExprNode> ParseBool();
  std::unique_ptr<DataTypeNode> ParseIntDataType();
  std::unique_ptr<DataTypeNode> ParseLongDataType();
  std::unique_ptr<DataTypeNode> ParseDoubleDataType();
  std::unique_ptr<DataTypeNode> ParseCharDataType();
  std::unique_ptr<DataTypeNode> ParseStringDataType();
  std::unique_ptr<DataTypeNode> ParseBoolDataType();
  std::unique_ptr<DataTypeNode> ParseVoidDataType();
  std::unique_ptr<DataTypeNode> ParseDataType(
      std::unique_ptr<DataTypeNode> simple_data_type);
  std::unique_ptr<BoundedArrayDataTypeNode> ParseBoundedArrayDataType(
      std::unique_ptr<DataTypeNode> simple_data_type);
  std::unique_ptr<ArgDefNode> ParseArgDef();
  std::unique_ptr<IfNode> DoParseIf();
  std::unique_ptr<ScopeNode> ParseScope();

  real_talk::lexer::TokenInfo ParseName();
  Op ParseCall();
  Op ParseSubscript();
  ExprState BeforeExprState(std::stack<Op> &operators,
                            std::stack< std::unique_ptr<ExprNode> > &operands);
  ExprState AfterExprState(std::stack<Op> &operators,
                           std::stack< std::unique_ptr<ExprNode> > &operands);
  template<typename TExprNode> std::unique_ptr<ExprNode> CreateBinaryExpr(
        std::stack< std::unique_ptr<ExprNode> > &operands, Op op);
  template<typename TExprNode> std::unique_ptr<ExprNode> CreateUnaryExpr(
      std::stack< std::unique_ptr<ExprNode> > &operands, Op op);
  std::unique_ptr<ExprNode> CreateSubscriptExpr(
      std::stack< std::unique_ptr<ExprNode> > &operands, Op op);
  std::unique_ptr<ExprNode> CreateCallExpr(
      std::stack< std::unique_ptr<ExprNode> > &operands, Op op);
  void ReduceExpr(std::stack<Op> &operators,
                  std::stack< std::unique_ptr<ExprNode> > &operands,
                  std::uint8_t next_op_prio,
                  OpAssoc next_op_assoc);
  std::uint8_t GetOpPrio(real_talk::lexer::Token op_token, OpAssoc op_assoc);
  void ConsumeNextToken();
  void AssertNextToken(real_talk::lexer::Token token);
  void UnexpectedToken();

  static const OpPrios &kOpPrios;
  std::unique_ptr<real_talk::lexer::Lexer> lexer_;
  real_talk::lexer::TokenInfo next_token_;
};
}
}
#endif
