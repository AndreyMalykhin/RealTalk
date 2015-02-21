
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <vector>
#include <stack>
#include <string>
#include <stdexcept>
#include <utility>
#include "real_talk/lexer/lexer.h"
#include "real_talk/parser/simple_parser.h"
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/sum_node.h"
#include "real_talk/parser/sub_node.h"
#include "real_talk/parser/not_node.h"
#include "real_talk/parser/and_node.h"
#include "real_talk/parser/or_node.h"
#include "real_talk/parser/pre_inc_node.h"
#include "real_talk/parser/pre_dec_node.h"
#include "real_talk/parser/mul_node.h"
#include "real_talk/parser/div_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/arg_def_node.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/greater_or_equal_node.h"
#include "real_talk/parser/less_or_equal_node.h"
#include "real_talk/parser/less_node.h"
#include "real_talk/parser/greater_node.h"
#include "real_talk/parser/equal_node.h"
#include "real_talk/parser/not_equal_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/parser/if_else_if_else_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/long_data_type_node.h"
#include "real_talk/parser/double_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/void_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"

using std::pair;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::move;
using std::vector;
using std::unordered_map;
using std::stack;
using std::runtime_error;
using std::make_pair;
using boost::format;
using real_talk::lexer::Lexer;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;

namespace real_talk {
namespace parser {

class SimpleParser::Impl {
 public:
  explicit Impl(std::unique_ptr<real_talk::lexer::Lexer> lexer);
  std::shared_ptr<ProgramNode> Parse();

 private:
  struct ExprState;

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

  typedef ExprState (SimpleParser::Impl::*ExprStateHandler)(
      std::stack<Op> &operators,
      std::stack< std::unique_ptr<ExprNode> > &operands);
  typedef std::pair<real_talk::lexer::Token, OpAssoc> OpPriosKey;
  typedef std::unordered_map<OpPriosKey,
                             std::uint8_t,
                             boost::hash<OpPriosKey> > OpPrios;

  struct ExprState {
    ExprStateHandler handler;
  };

  std::vector< std::unique_ptr<StmtNode> > ParseStmts(
      real_talk::lexer::Token until_token);
  std::unique_ptr<StmtNode> ParseStmt();
  std::unique_ptr<StmtNode> ParseDef(
      const std::vector<real_talk::lexer::TokenInfo> &modifier_tokens,
      std::unique_ptr<PrimitiveDataTypeNode> primitive_data_type);
  std::unique_ptr<StmtNode> ParseFuncDef(
      const std::vector<real_talk::lexer::TokenInfo> &modifier_tokens,
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
  std::unique_ptr<PrimitiveDataTypeNode> ParseIntDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParseLongDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParseDoubleDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParseCharDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParseStringDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParseBoolDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParseVoidDataType();
  std::unique_ptr<PrimitiveDataTypeNode> ParsePrimitiveDataType();
  std::unique_ptr<DataTypeNode> ParseDataType(
      std::unique_ptr<PrimitiveDataTypeNode> primitive_data_type);
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

const SimpleParser::Impl::OpPrios &SimpleParser::Impl::kOpPrios =
    *new OpPrios({
        {{Token::kGroupStart, OpAssoc::kLeftToRight}, UINT8_C(9)},
        {{Token::kSubscriptStart, OpAssoc::kLeftToRight}, UINT8_C(9)},
        {{Token::kNotOp, OpAssoc::kRightToLeft}, UINT8_C(8)},
        {{Token::kPreIncOp, OpAssoc::kRightToLeft}, UINT8_C(8)},
        {{Token::kPreDecOp, OpAssoc::kRightToLeft}, UINT8_C(8)},
        {{Token::kSubOp, OpAssoc::kRightToLeft}, UINT8_C(8)},
        {{Token::kMulOp, OpAssoc::kLeftToRight}, UINT8_C(7)},
        {{Token::kDivOp, OpAssoc::kLeftToRight}, UINT8_C(7)},
        {{Token::kSumOp, OpAssoc::kLeftToRight}, UINT8_C(6)},
        {{Token::kSubOp, OpAssoc::kLeftToRight}, UINT8_C(6)},
        {{Token::kGreaterOrEqualOp, OpAssoc::kLeftToRight}, UINT8_C(5)},
        {{Token::kLessOrEqualOp, OpAssoc::kLeftToRight}, UINT8_C(5)},
        {{Token::kLessOp, OpAssoc::kLeftToRight}, UINT8_C(5)},
        {{Token::kGreaterOp, OpAssoc::kLeftToRight}, UINT8_C(5)},
        {{Token::kEqualOp, OpAssoc::kLeftToRight}, UINT8_C(4)},
        {{Token::kNotEqualOp, OpAssoc::kLeftToRight}, UINT8_C(4)},
        {{Token::kAndOp, OpAssoc::kLeftToRight}, UINT8_C(3)},
        {{Token::kOrOp, OpAssoc::kLeftToRight}, UINT8_C(2)},
        {{Token::kAssignOp, OpAssoc::kRightToLeft}, UINT8_C(1)}
    });

SimpleParser::SimpleParser(unique_ptr<Lexer> lexer)
    : impl_(new Impl(move(lexer))) {
}

SimpleParser::~SimpleParser() {}

shared_ptr<ProgramNode> SimpleParser::Parse() {
  return impl_->Parse();
}

SimpleParser::Impl::Impl(unique_ptr<Lexer> lexer)
    : lexer_(move(lexer)),
      next_token_(TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(0))) {
  assert(lexer_);
}

shared_ptr<ProgramNode> SimpleParser::Impl::Parse() {
  ConsumeNextToken();
  vector< unique_ptr<StmtNode> > stmts = ParseStmts(Token::kFileEnd);
  return shared_ptr<ProgramNode>(new ProgramNode(move(stmts)));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseStmt() {
  switch (next_token_.GetId()) {
    case Token::kIntType:
      return ParseDef(vector<TokenInfo>(), ParseIntDataType());
    case Token::kCharType:
      return ParseDef(vector<TokenInfo>(), ParseCharDataType());
    case Token::kLongType:
      return ParseDef(vector<TokenInfo>(), ParseLongDataType());
    case Token::kBoolType:
      return ParseDef(vector<TokenInfo>(), ParseBoolDataType());
    case Token::kDoubleType:
      return ParseDef(vector<TokenInfo>(), ParseDoubleDataType());
    case Token::kStringType:
      return ParseDef(vector<TokenInfo>(), ParseStringDataType());
    case Token::kVoidType:
      return ParseDef(vector<TokenInfo>(), ParseVoidDataType());
    case Token::kNative: {
      const TokenInfo native_token = next_token_;
      ConsumeNextToken();
      const vector<TokenInfo> modifier_tokens = {native_token};
      return ParseDef(modifier_tokens, ParsePrimitiveDataType());
    }
    case Token::kWhile:
      return ParsePreTestLoop();
    case Token::kIf:
      return ParseIf();
    case Token::kImport:
      return ParseImport();
    case Token::kBreak:
      return ParseBreak();
    case Token::kContinue:
      return ParseContinue();
    case Token::kReturn:
      return ParseReturn();
    default:
      return ParseExprStmt();
  }

  assert(false);
  return unique_ptr<StmtNode>();
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseReturn() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();

  if (next_token_.GetId() == Token::kStmtEnd) {
    const TokenInfo end_token = next_token_;
    ConsumeNextToken();
    return unique_ptr<StmtNode>(
      new ReturnWithoutValueNode(start_token, end_token));
  }

  unique_ptr<ExprNode> value = ParseExpr();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(
      new ReturnValueNode(start_token, move(value), end_token));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseContinue() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(new ContinueNode(start_token, end_token));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseBreak() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(new BreakNode(start_token, end_token));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseImport() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kStringLit);
  unique_ptr<StringNode> file_path(
      static_cast<StringNode*>(ParseString().release()));
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(
      new ImportNode(start_token, move(file_path), end_token));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseIf() {
  unique_ptr<IfNode> first_if_node = DoParseIf();
  vector< unique_ptr<ElseIfNode> > else_ifs;

  while (next_token_.GetId() == Token::kElse) {
    const TokenInfo else_token = next_token_;
    ConsumeNextToken();

    switch (next_token_.GetId()) {
      case Token::kIf: {
        unique_ptr<IfNode> if_node = DoParseIf();
        unique_ptr<ElseIfNode> else_if(
            new ElseIfNode(else_token, move(if_node)));
        else_ifs.push_back(move(else_if));
        break;
      }
      case Token::kScopeStart: {
        unique_ptr<ScopeNode> else_body = ParseScope();
        return unique_ptr<StmtNode>(new IfElseIfElseNode(
            move(first_if_node),
            move(else_ifs),
            else_token,
            move(else_body)));
      }
      default: {
        UnexpectedToken();
      }
    }
  }

  return unique_ptr<StmtNode>(
      new IfElseIfNode(move(first_if_node), move(else_ifs)));
}

unique_ptr<IfNode> SimpleParser::Impl::DoParseIf() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kGroupStart);
  const TokenInfo cond_start_token = next_token_;
  ConsumeNextToken();
  unique_ptr<ExprNode> cond = ParseExpr();
  AssertNextToken(Token::kGroupEnd);
  const TokenInfo cond_end_token = next_token_;
  ConsumeNextToken();
  unique_ptr<ScopeNode> body = ParseScope();
  return unique_ptr<IfNode>(new IfNode(
      start_token, cond_start_token, move(cond), cond_end_token, move(body)));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParsePreTestLoop() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kGroupStart);
  const TokenInfo cond_start_token = next_token_;
  ConsumeNextToken();
  unique_ptr<ExprNode> cond = ParseExpr();
  AssertNextToken(Token::kGroupEnd);
  const TokenInfo cond_end_token = next_token_;
  ConsumeNextToken();
  unique_ptr<ScopeNode> body = ParseScope();
  return unique_ptr<StmtNode>(new PreTestLoopNode(
      start_token,
      cond_start_token,
      move(cond),
      cond_end_token,
      move(body)));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseExprStmt() {
  unique_ptr<ExprNode> expr = ParseExpr();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(new ExprStmtNode(move(expr), end_token));
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseDef(
    const vector<TokenInfo> &modifier_tokens,
    unique_ptr<PrimitiveDataTypeNode> primitive_data_type) {
  unique_ptr<DataTypeNode> full_data_type =
      ParseDataType(move(primitive_data_type));
  const TokenInfo &name_token = ParseName();

  if (!modifier_tokens.empty()) {
    return ParseFuncDef(modifier_tokens, move(full_data_type), name_token);
  }

  switch (next_token_.GetId()) {
    case Token::kStmtEnd: {
      const TokenInfo end_token = next_token_;
      ConsumeNextToken();
      return unique_ptr<StmtNode>(new VarDefWithoutInitNode(
          move(full_data_type), name_token, end_token));
    }
    case Token::kAssignOp: {
      const TokenInfo assign_token = next_token_;
      ConsumeNextToken();
      unique_ptr<ExprNode> value = ParseExpr();
      AssertNextToken(Token::kStmtEnd);
      const TokenInfo end_token = next_token_;
      ConsumeNextToken();
      return unique_ptr<StmtNode>(new VarDefWithInitNode(
          move(full_data_type),
          name_token,
          assign_token,
          move(value),
          end_token));
    }
    case Token::kGroupStart: {
      return ParseFuncDef(modifier_tokens, move(full_data_type), name_token);
    }
    default: {
      UnexpectedToken();
    }
  }

  assert(false);
  return unique_ptr<StmtNode>();
}

unique_ptr<StmtNode> SimpleParser::Impl::ParseFuncDef(
    const vector<TokenInfo> &modifier_tokens,
    unique_ptr<DataTypeNode> return_data_type,
    const TokenInfo &func_name_token) {
  const TokenInfo args_start_token = next_token_;
  ConsumeNextToken();
  vector< unique_ptr<ArgDefNode> > args;
  vector<TokenInfo> arg_separator_tokens;

  if (next_token_.GetId() != Token::kGroupEnd) {
    while (true) {
      args.push_back(ParseArgDef());

      if (next_token_.GetId() != Token::kSeparator) {
        break;
      }

      arg_separator_tokens.push_back(next_token_);
      ConsumeNextToken();
    }
  }

  AssertNextToken(Token::kGroupEnd);
  const TokenInfo args_end_token = next_token_;
  ConsumeNextToken();

  if (next_token_.GetId() == Token::kStmtEnd) {
    const TokenInfo end_token = next_token_;
    ConsumeNextToken();
    return unique_ptr<StmtNode>(new FuncDefWithoutBodyNode(
        modifier_tokens,
        move(return_data_type),
        func_name_token,
        args_start_token,
        move(args),
        arg_separator_tokens,
        args_end_token,
        end_token));
  }

  unique_ptr<ScopeNode> body = ParseScope();
  return unique_ptr<StmtNode>(new FuncDefWithBodyNode(
      modifier_tokens,
      move(return_data_type),
      func_name_token,
      args_start_token,
      move(args),
      arg_separator_tokens,
      args_end_token,
      move(body)));
}

unique_ptr<ScopeNode> SimpleParser::Impl::ParseScope() {
  AssertNextToken(Token::kScopeStart);
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  vector< unique_ptr<StmtNode> > stmts = ParseStmts(Token::kScopeEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ScopeNode>(
      new ScopeNode(start_token, move(stmts), end_token));
}

unique_ptr<ArgDefNode> SimpleParser::Impl::ParseArgDef() {
  unique_ptr<PrimitiveDataTypeNode> primitive_data_type =
      ParsePrimitiveDataType();
  unique_ptr<DataTypeNode> full_data_type =
      ParseDataType(move(primitive_data_type));
  const TokenInfo &name_token = ParseName();
  return unique_ptr<ArgDefNode>(
      new ArgDefNode(move(full_data_type), name_token));
}

vector< unique_ptr<StmtNode> > SimpleParser::Impl::ParseStmts(Token until_token) {
  vector< unique_ptr<StmtNode> > stmts;

  while (next_token_.GetId() != until_token) {
    stmts.push_back(ParseStmt());
  }

  return stmts;
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseExpr() {
  stack<Op> operators;
  stack< unique_ptr<ExprNode> > operands;
  ExprState next_state = {&SimpleParser::Impl::BeforeExprState};

  do {
    next_state = (this->*next_state.handler)(operators, operands);
  } while (next_state.handler != nullptr);

  assert(operands.size() == 1);
  assert(operators.empty());
  return move(operands.top());
}

SimpleParser::Impl::ExprState SimpleParser::Impl::BeforeExprState(
    stack<Op> &operators,
    stack< unique_ptr<ExprNode> > &operands) {
  ExprState next_state;

  switch (next_token_.GetId()) {
    case Token::kName: {
      operands.push(ParseId());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kStringLit: {
      operands.push(ParseString());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kCharLit: {
      operands.push(ParseChar());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kIntLit: {
      operands.push(ParseInt());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kLongLit: {
      operands.push(ParseLong());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kDoubleLit: {
      operands.push(ParseDouble());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kBoolTrueLit:
    case Token::kBoolFalseLit: {
      operands.push(ParseBool());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kNew: {
      operands.push(ParseArrayAlloc());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kGroupStart: {
      ConsumeNextToken();
      operands.push(ParseExpr());
      AssertNextToken(Token::kGroupEnd);
      ConsumeNextToken();
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kSubOp:
    case Token::kNotOp:
    case Token::kPreDecOp:
    case Token::kPreIncOp: {
      const OpAssoc next_op_assoc = OpAssoc::kRightToLeft;
      const uint8_t next_op_prio =
          GetOpPrio(next_token_.GetId(), next_op_assoc);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      const vector<TokenInfo> op_tokens = {next_token_};
      ConsumeNextToken();
      vector< unique_ptr<ExprNode> > op_args;
      Op op = {op_tokens, move(op_args), next_op_prio, next_op_assoc};
      operators.push(move(op));
      next_state.handler = &SimpleParser::Impl::BeforeExprState;
      break;
    }
    default: {
      UnexpectedToken();
    }
  }

  return next_state;
}

SimpleParser::Impl::ExprState SimpleParser::Impl::AfterExprState(
    stack<Op> &operators,
    stack< unique_ptr<ExprNode> > &operands) {
  ExprState next_state;

  switch (next_token_.GetId()) {
    case Token::kGroupStart: {
      const OpAssoc next_op_assoc = OpAssoc::kLeftToRight;
      const uint8_t next_op_prio =
          GetOpPrio(next_token_.GetId(), next_op_assoc);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      operators.push(ParseCall());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kSubscriptStart: {
      const OpAssoc next_op_assoc = OpAssoc::kLeftToRight;
      const uint8_t next_op_prio =
          GetOpPrio(next_token_.GetId(), next_op_assoc);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      operators.push(ParseSubscript());
      next_state.handler = &SimpleParser::Impl::AfterExprState;
      break;
    }
    case Token::kAssignOp: {
      const OpAssoc next_op_assoc = OpAssoc::kRightToLeft;
      const uint8_t next_op_prio =
          GetOpPrio(next_token_.GetId(), next_op_assoc);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      const vector<TokenInfo> op_tokens = {next_token_};
      ConsumeNextToken();
      vector< unique_ptr<ExprNode> > op_args;
      Op op = {op_tokens, move(op_args), next_op_prio, next_op_assoc};
      operators.push(move(op));
      next_state.handler = &SimpleParser::Impl::BeforeExprState;
      break;
    }
    case Token::kAndOp:
    case Token::kOrOp:
    case Token::kGreaterOrEqualOp:
    case Token::kLessOrEqualOp:
    case Token::kLessOp:
    case Token::kGreaterOp:
    case Token::kEqualOp:
    case Token::kNotEqualOp:
    case Token::kSumOp:
    case Token::kSubOp:
    case Token::kDivOp:
    case Token::kMulOp: {
      const OpAssoc next_op_assoc = OpAssoc::kLeftToRight;
      const uint8_t next_op_prio =
          GetOpPrio(next_token_.GetId(), next_op_assoc);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      const vector<TokenInfo> op_tokens = {next_token_};
      ConsumeNextToken();
      vector< unique_ptr<ExprNode> > op_args;
      Op op = {op_tokens, move(op_args), next_op_prio, next_op_assoc};
      operators.push(move(op));
      next_state.handler = &SimpleParser::Impl::BeforeExprState;
      break;
    }
    default: {
      const OpAssoc next_op_assoc = OpAssoc::kLeftToRight;
      const uint8_t next_op_prio = UINT8_C(0);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      next_state.handler = nullptr;
      break;
    }
  }

  return next_state;
}

uint8_t SimpleParser::Impl::GetOpPrio(Token op_token, OpAssoc op_assoc) {
  auto op_prio_it = kOpPrios.find(make_pair(op_token, op_assoc));
  assert(op_prio_it != kOpPrios.end());
  return op_prio_it->second;
}

void SimpleParser::Impl::ReduceExpr(stack<Op> &operators,
                              stack< unique_ptr<ExprNode> > &operands,
                              uint8_t next_op_prio,
                              OpAssoc next_op_assoc) {
  while (!operators.empty()) {
    Op &previous_op = operators.top();

    if (next_op_prio > previous_op.prio) {
      break;
    } else if (next_op_prio == previous_op.prio) {
      assert(next_op_assoc == previous_op.assoc);

      if (next_op_assoc == OpAssoc::kRightToLeft
          && previous_op.assoc == OpAssoc::kRightToLeft) {
        break;
      }
    }

    unique_ptr<ExprNode> expr;
    assert(!previous_op.tokens.empty());
    const Token previous_op_start_token = previous_op.tokens.front().GetId();

    switch (previous_op_start_token) {
      case Token::kAssignOp: {
        expr = CreateBinaryExpr<AssignNode>(operands, move(previous_op));
        break;
      }
      case Token::kGreaterOrEqualOp: {
        expr = CreateBinaryExpr<GreaterOrEqualNode>(
            operands, move(previous_op));
        break;
      }
      case Token::kLessOrEqualOp: {
        expr = CreateBinaryExpr<LessOrEqualNode>(
            operands, move(previous_op));
        break;
      }
      case Token::kLessOp: {
        expr = CreateBinaryExpr<LessNode>(operands, move(previous_op));
        break;
      }
      case Token::kGreaterOp: {
        expr = CreateBinaryExpr<GreaterNode>(operands, move(previous_op));
        break;
      }
      case Token::kEqualOp: {
        expr = CreateBinaryExpr<EqualNode>(operands, move(previous_op));
        break;
      }
      case Token::kNotEqualOp: {
        expr = CreateBinaryExpr<NotEqualNode>(operands, move(previous_op));
        break;
      }
      case Token::kSumOp: {
        expr = CreateBinaryExpr<SumNode>(operands, move(previous_op));
        break;
      }
      case Token::kSubOp: {
        if (previous_op.assoc == OpAssoc::kLeftToRight) {
          expr = CreateBinaryExpr<SubNode>(operands, move(previous_op));
        } else {
          expr = CreateUnaryExpr<NegativeNode>(operands, move(previous_op));
        }

        break;
      }
      case Token::kMulOp: {
        expr = CreateBinaryExpr<MulNode>(operands, move(previous_op));
        break;
      }
      case Token::kDivOp: {
        expr = CreateBinaryExpr<DivNode>(operands, move(previous_op));
        break;
      }
      case Token::kAndOp: {
        expr = CreateBinaryExpr<AndNode>(operands, move(previous_op));
        break;
      }
      case Token::kOrOp: {
        expr = CreateBinaryExpr<OrNode>(operands, move(previous_op));
        break;
      }
      case Token::kNotOp: {
        expr = CreateUnaryExpr<NotNode>(operands, move(previous_op));
        break;
      }
      case Token::kPreIncOp: {
        expr = CreateUnaryExpr<PreIncNode>(operands, move(previous_op));
        break;
      }
      case Token::kPreDecOp: {
        expr = CreateUnaryExpr<PreDecNode>(operands, move(previous_op));
        break;
      }
      case Token::kSubscriptStart: {
        expr = CreateSubscriptExpr(operands, move(previous_op));
        break;
      }
      case Token::kGroupStart: {
        expr = CreateCallExpr(operands, move(previous_op));
        break;
      }
      default: {
        assert(false);
      }
    }

    operators.pop();
    operands.push(move(expr));
  }
}

template<typename TExprNode> unique_ptr<ExprNode>
SimpleParser::Impl::CreateBinaryExpr(
    stack< unique_ptr<ExprNode> > &operands, Op op) {
  assert(operands.size() >= 2);
  unique_ptr<ExprNode> right_operand(move(operands.top()));
  operands.pop();
  unique_ptr<ExprNode> left_operand(move(operands.top()));
  operands.pop();
  assert(op.tokens.size() == 1);
  return unique_ptr<ExprNode>(new TExprNode(
      op.tokens.front(), move(left_operand), move(right_operand)));
}

template<typename TExprNode> unique_ptr<ExprNode>
SimpleParser::Impl::CreateUnaryExpr(
    stack< unique_ptr<ExprNode> > &operands, Op op) {
  assert(operands.size() >= 1);
  unique_ptr<ExprNode> operand(move(operands.top()));
  operands.pop();
  assert(op.tokens.size() == 1);
  return unique_ptr<ExprNode>(new TExprNode(op.tokens.front(), move(operand)));
}

unique_ptr<ExprNode> SimpleParser::Impl::CreateSubscriptExpr(
    stack< unique_ptr<ExprNode> > &operands, Op op) {
  assert(operands.size() >= 1);
  unique_ptr<ExprNode> operand(move(operands.top()));
  operands.pop();
  assert(op.args.size() == 1);
  unique_ptr<ExprNode> &index = op.args.front();
  assert(op.tokens.size() == 2);
  const TokenInfo &start_token = op.tokens.front();
  const TokenInfo &end_token = op.tokens.back();
  return unique_ptr<ExprNode>(
      new SubscriptNode(start_token, end_token, move(operand), move(index)));
}

unique_ptr<ExprNode> SimpleParser::Impl::CreateCallExpr(
    stack< unique_ptr<ExprNode> > &operands, Op op) {
  assert(operands.size() >= 1);
  unique_ptr<ExprNode> operand(move(operands.top()));
  operands.pop();
  assert(op.tokens.size() >= 2);
  const TokenInfo &start_token = op.tokens.front();
  const vector<TokenInfo> arg_separator_tokens(
      op.tokens.begin() + 1, op.tokens.end() - 1);
  const TokenInfo &end_token = op.tokens.back();
  return unique_ptr<ExprNode>(new CallNode(
      move(operand),
      start_token,
      move(op.args),
      arg_separator_tokens,
      end_token));
}

SimpleParser::Impl::Op SimpleParser::Impl::ParseCall() {
  vector<TokenInfo> op_tokens;
  op_tokens.push_back(next_token_);
  ConsumeNextToken();
  vector< unique_ptr<ExprNode> > op_args;

  if (next_token_.GetId() != Token::kGroupEnd) {
    while (true) {
      op_args.push_back(ParseExpr());

      if (next_token_.GetId() != Token::kSeparator) {
        break;
      }

      op_tokens.push_back(next_token_);
      ConsumeNextToken();
    }
  }

  AssertNextToken(Token::kGroupEnd);
  op_tokens.push_back(next_token_);
  ConsumeNextToken();
  const OpAssoc op_assoc = OpAssoc::kLeftToRight;
  Op op = {op_tokens,
           move(op_args),
           GetOpPrio(Token::kGroupStart, op_assoc),
           op_assoc};
  return op;
}

SimpleParser::Impl::Op SimpleParser::Impl::ParseSubscript() {
  vector<TokenInfo> op_tokens;
  op_tokens.push_back(next_token_);
  ConsumeNextToken();
  unique_ptr<ExprNode> index = ParseExpr();
  vector< unique_ptr<ExprNode> > op_args;
  op_args.push_back(move(index));
  AssertNextToken(Token::kSubscriptEnd);
  op_tokens.push_back(next_token_);
  ConsumeNextToken();
  const OpAssoc op_assoc = OpAssoc::kLeftToRight;
  Op op = {op_tokens,
           move(op_args),
           GetOpPrio(Token::kSubscriptStart, op_assoc),
           op_assoc};
  return op;
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseId() {
  const TokenInfo var_name_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new IdNode(var_name_token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseChar() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new CharNode(token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseString() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new StringNode(token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseInt() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new IntNode(token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseLong() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new LongNode(token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseDouble() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new DoubleNode(token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseBool() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new BoolNode(token));
}

unique_ptr<ExprNode> SimpleParser::Impl::ParseArrayAlloc() {
  const TokenInfo alloc_token = next_token_;
  ConsumeNextToken();
  unique_ptr<PrimitiveDataTypeNode> data_type = ParsePrimitiveDataType();
  AssertNextToken(Token::kSubscriptStart);
  TokenInfo array_size_start_token = next_token_;
  ConsumeNextToken();

  if (next_token_.GetId() == Token::kSubscriptEnd) {
    TokenInfo array_size_end_token = next_token_;
    ConsumeNextToken();
    unique_ptr<UnboundedArraySizeNode> array_size(new UnboundedArraySizeNode(
        array_size_start_token, array_size_end_token));
    vector< unique_ptr<UnboundedArraySizeNode> > array_sizes;
    array_sizes.push_back(move(array_size));

    while (next_token_.GetId() == Token::kSubscriptStart) {
      array_size_start_token = next_token_;
      ConsumeNextToken();
      AssertNextToken(Token::kSubscriptEnd);
      array_size_end_token = next_token_;
      ConsumeNextToken();
      array_size.reset(new UnboundedArraySizeNode(
          array_size_start_token, array_size_end_token));
      array_sizes.push_back(move(array_size));
    }

    AssertNextToken(Token::kScopeStart);
    const TokenInfo values_start_token = next_token_;
    ConsumeNextToken();
    vector<TokenInfo> value_separator_tokens;
    vector< unique_ptr<ExprNode> > array_values;

    if (next_token_.GetId() != Token::kScopeEnd) {
      bool stop = false;

      while (!stop) {
        array_values.push_back(ParseExpr());

        switch (next_token_.GetId()) {
          case Token::kSeparator: {
            value_separator_tokens.push_back(next_token_);
            ConsumeNextToken();
            break;
          }
          case Token::kScopeEnd: {
            stop = true;
            break;
          }
          default: {
            UnexpectedToken();
          }
        }
      }
    }

    const TokenInfo values_end_token = next_token_;
    ConsumeNextToken();
    return unique_ptr<ExprNode>(new ArrayAllocWithInitNode(
        alloc_token,
        move(data_type),
        move(array_sizes),
        values_start_token,
        move(array_values),
        value_separator_tokens,
        values_end_token));
  } else {
    unique_ptr<ExprNode> array_size_value = ParseExpr();
    AssertNextToken(Token::kSubscriptEnd);
    TokenInfo array_size_end_token = next_token_;
    ConsumeNextToken();
    unique_ptr<BoundedArraySizeNode> array_size(new BoundedArraySizeNode(
        array_size_start_token,
        move(array_size_value),
        array_size_end_token));
    vector< unique_ptr<BoundedArraySizeNode> > array_sizes;
    array_sizes.push_back(move(array_size));

    while (next_token_.GetId() == Token::kSubscriptStart) {
      array_size_start_token = next_token_;
      ConsumeNextToken();
      array_size_value = ParseExpr();
      AssertNextToken(Token::kSubscriptEnd);
      array_size_end_token = next_token_;
      ConsumeNextToken();
      array_size.reset(new BoundedArraySizeNode(
          array_size_start_token,
          move(array_size_value),
          array_size_end_token));
      array_sizes.push_back(move(array_size));
    }

    return unique_ptr<ExprNode>(new ArrayAllocWithoutInitNode(
        alloc_token, move(data_type), move(array_sizes)));
  }
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseIntDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(new IntDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseLongDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(
      new LongDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseDoubleDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(
      new DoubleDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseCharDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(
      new CharDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseStringDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(
      new StringDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseBoolDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(
      new BoolDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParseVoidDataType() {
  unique_ptr<PrimitiveDataTypeNode> data_type(
      new VoidDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<PrimitiveDataTypeNode> SimpleParser::Impl::ParsePrimitiveDataType() {
  switch (next_token_.GetId()) {
    case Token::kIntType:
      return ParseIntDataType();
    case Token::kCharType:
      return ParseCharDataType();
    case Token::kLongType:
      return ParseLongDataType();
    case Token::kBoolType:
      return ParseBoolDataType();
    case Token::kDoubleType:
      return ParseDoubleDataType();
    case Token::kStringType:
      return ParseStringDataType();
    case Token::kVoidType:
      return ParseVoidDataType();
    default:
      UnexpectedToken();
  }

  assert(false);
}

unique_ptr<DataTypeNode> SimpleParser::Impl::ParseDataType(
    unique_ptr<PrimitiveDataTypeNode> primitive_data_type) {
  unique_ptr<DataTypeNode> data_type(move(primitive_data_type));

  while (next_token_.GetId() == Token::kSubscriptStart) {
    const TokenInfo subscript_start_token = next_token_;
    ConsumeNextToken();
    AssertNextToken(Token::kSubscriptEnd);
    const TokenInfo subscript_end_token = next_token_;
    ConsumeNextToken();
    data_type.reset(new ArrayDataTypeNode(
        move(data_type), subscript_start_token, subscript_end_token));
  }

  return data_type;
}

TokenInfo SimpleParser::Impl::ParseName() {
  AssertNextToken(Token::kName);
  const TokenInfo name = next_token_;
  ConsumeNextToken();
  return name;
}

void SimpleParser::Impl::AssertNextToken(Token token) {
  if (next_token_.GetId() != token) {
    UnexpectedToken();
  }
}

void SimpleParser::Impl::ConsumeNextToken() {
  next_token_ = lexer_->GetNextToken();
}

void SimpleParser::Impl::UnexpectedToken() {
  const string &msg = (format("Unexpected token: %1%") % next_token_).str();
  throw UnexpectedTokenError(next_token_, msg);
}
}
}

