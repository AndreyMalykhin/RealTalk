
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <cassert>
#include <vector>
#include <stack>
#include <string>
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
#include "real_talk/parser/func_def_node.h"
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
#include "real_talk/parser/bounded_array_data_type_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_node.h"

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
using boost::lexical_cast;
using real_talk::lexer::Lexer;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;

namespace real_talk {
namespace parser {

const SimpleParser::OpPrios &SimpleParser::kOpPrios =
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
    : lexer_(move(lexer)),
      next_token_(TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(0))) {
}

shared_ptr<ProgramNode> SimpleParser::Parse() {
  ConsumeNextToken();
  vector< unique_ptr<StmtNode> > stmts = ParseStmts(Token::kFileEnd);
  return shared_ptr<ProgramNode>(new ProgramNode(move(stmts)));
}

unique_ptr<StmtNode> SimpleParser::ParseStmt() {
  switch (next_token_.GetId()) {
    case Token::kIntType:
      return ParseDef(ParseIntDataType());
    case Token::kCharType:
      return ParseDef(ParseCharDataType());
    case Token::kLongType:
      return ParseDef(ParseLongDataType());
    case Token::kBoolType:
      return ParseDef(ParseBoolDataType());
    case Token::kDoubleType:
      return ParseDef(ParseDoubleDataType());
    case Token::kStringType:
      return ParseDef(ParseStringDataType());
    case Token::kVoidType:
      return ParseDef(ParseVoidDataType());
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

unique_ptr<StmtNode> SimpleParser::ParseReturn() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();

  if (next_token_.GetId() == Token::kStmtEnd) {
    const TokenInfo end_token = next_token_;
    ConsumeNextToken();
    return unique_ptr<StmtNode>(
      new ReturnNode(start_token, end_token));
  }

  unique_ptr<ExprNode> value = ParseExpr();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(
      new ReturnValueNode(start_token, move(value), end_token));
}

unique_ptr<StmtNode> SimpleParser::ParseContinue() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(new ContinueNode(start_token, end_token));
}

unique_ptr<StmtNode> SimpleParser::ParseBreak() {
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(new BreakNode(start_token, end_token));
}

unique_ptr<StmtNode> SimpleParser::ParseImport() {
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

unique_ptr<StmtNode> SimpleParser::ParseIf() {
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

unique_ptr<IfNode> SimpleParser::DoParseIf() {
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

unique_ptr<StmtNode> SimpleParser::ParsePreTestLoop() {
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

unique_ptr<StmtNode> SimpleParser::ParseExprStmt() {
  unique_ptr<ExprNode> expr = ParseExpr();
  AssertNextToken(Token::kStmtEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<StmtNode>(new ExprStmtNode(move(expr), end_token));
}

unique_ptr<StmtNode> SimpleParser::ParseDef(
    unique_ptr<DataTypeNode> simple_data_type) {
  unique_ptr<DataTypeNode> full_data_type =
      ParseDataType(move(simple_data_type));
  const TokenInfo &name_token = ParseName();

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
      return ParseFuncDef(move(full_data_type), name_token);
    }
    default: {
      UnexpectedToken();
    }
  }

  assert(false);
  return unique_ptr<StmtNode>();
}

unique_ptr<StmtNode> SimpleParser::ParseFuncDef(
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
  unique_ptr<ScopeNode> body = ParseScope();
  return unique_ptr<StmtNode>(new FuncDefNode(
      move(return_data_type),
      func_name_token,
      args_start_token,
      move(args),
      arg_separator_tokens,
      args_end_token,
      move(body)));
}

unique_ptr<ScopeNode> SimpleParser::ParseScope() {
  AssertNextToken(Token::kScopeStart);
  const TokenInfo start_token = next_token_;
  ConsumeNextToken();
  vector< unique_ptr<StmtNode> > stmts = ParseStmts(Token::kScopeEnd);
  const TokenInfo end_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ScopeNode>(
      new ScopeNode(start_token, move(stmts), end_token));
}

unique_ptr<ArgDefNode> SimpleParser::ParseArgDef() {
  unique_ptr<DataTypeNode> simple_data_type;

  switch (next_token_.GetId()) {
    case Token::kIntType:
      simple_data_type = ParseIntDataType();
      break;
    case Token::kCharType:
      simple_data_type = ParseCharDataType();
      break;
    case Token::kLongType:
      simple_data_type = ParseLongDataType();
      break;
    case Token::kBoolType:
      simple_data_type = ParseBoolDataType();
      break;
    case Token::kDoubleType:
      simple_data_type = ParseDoubleDataType();
      break;
    case Token::kStringType:
      simple_data_type = ParseStringDataType();
      break;
    case Token::kVoidType:
      simple_data_type = ParseVoidDataType();
      break;
    default:
      UnexpectedToken();
  }

  unique_ptr<DataTypeNode> full_data_type =
      ParseDataType(move(simple_data_type));
  const TokenInfo &name_token = ParseName();
  return unique_ptr<ArgDefNode>(
      new ArgDefNode(move(full_data_type), name_token));
}

vector< unique_ptr<StmtNode> > SimpleParser::ParseStmts(Token until_token) {
  vector< unique_ptr<StmtNode> > stmts;

  while (next_token_.GetId() != until_token) {
    stmts.push_back(ParseStmt());
  }

  return stmts;
}

unique_ptr<ExprNode> SimpleParser::ParseExpr() {
  stack<Op> operators;
  stack< unique_ptr<ExprNode> > operands;
  ExprState next_state = {&SimpleParser::BeforeExprState};

  do {
    next_state = (this->*next_state.handler)(operators, operands);
  } while (next_state.handler != nullptr);

  assert(operands.size() == 1);
  assert(operators.empty());
  return move(operands.top());
}

SimpleParser::ExprState SimpleParser::BeforeExprState(
    stack<Op> &operators,
    stack< unique_ptr<ExprNode> > &operands) {
  ExprState next_state;

  switch (next_token_.GetId()) {
    case Token::kName: {
      operands.push(ParseId());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kStringLit: {
      operands.push(ParseString());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kCharLit: {
      operands.push(ParseChar());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kIntLit: {
      operands.push(ParseInt());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kLongLit: {
      operands.push(ParseLong());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kDoubleLit: {
      operands.push(ParseDouble());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kBoolTrueLit:
    case Token::kBoolFalseLit: {
      operands.push(ParseBool());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kNew: {
      operands.push(ParseArrayAlloc());
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kGroupStart: {
      ConsumeNextToken();
      operands.push(ParseExpr());
      AssertNextToken(Token::kGroupEnd);
      ConsumeNextToken();
      next_state.handler = &SimpleParser::AfterExprState;
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
      next_state.handler = &SimpleParser::BeforeExprState;
      break;
    }
    default: {
      UnexpectedToken();
    }
  }

  return next_state;
}

SimpleParser::ExprState SimpleParser::AfterExprState(
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
      next_state.handler = &SimpleParser::AfterExprState;
      break;
    }
    case Token::kSubscriptStart: {
      const OpAssoc next_op_assoc = OpAssoc::kLeftToRight;
      const uint8_t next_op_prio =
          GetOpPrio(next_token_.GetId(), next_op_assoc);
      ReduceExpr(operators, operands, next_op_prio, next_op_assoc);
      operators.push(ParseSubscript());
      next_state.handler = &SimpleParser::AfterExprState;
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
      next_state.handler = &SimpleParser::BeforeExprState;
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
      next_state.handler = &SimpleParser::BeforeExprState;
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

uint8_t SimpleParser::GetOpPrio(Token op_token, OpAssoc op_assoc) {
  auto op_prio_it = kOpPrios.find(make_pair(op_token, op_assoc));
  assert(op_prio_it != kOpPrios.end());
  return op_prio_it->second;
}

void SimpleParser::ReduceExpr(stack<Op> &operators,
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
SimpleParser::CreateBinaryExpr(stack< unique_ptr<ExprNode> > &operands, Op op) {
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
SimpleParser::CreateUnaryExpr(stack< unique_ptr<ExprNode> > &operands, Op op) {
  assert(operands.size() >= 1);
  unique_ptr<ExprNode> operand(move(operands.top()));
  operands.pop();
  assert(op.tokens.size() == 1);
  return unique_ptr<ExprNode>(new TExprNode(op.tokens.front(), move(operand)));
}

unique_ptr<ExprNode> SimpleParser::CreateSubscriptExpr(
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

unique_ptr<ExprNode> SimpleParser::CreateCallExpr(
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

SimpleParser::Op SimpleParser::ParseCall() {
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

SimpleParser::Op SimpleParser::ParseSubscript() {
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

unique_ptr<ExprNode> SimpleParser::ParseId() {
  const TokenInfo var_name_token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new IdNode(var_name_token));
}

unique_ptr<ExprNode> SimpleParser::ParseChar() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new CharNode(token));
}

unique_ptr<ExprNode> SimpleParser::ParseString() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new StringNode(token));
}

unique_ptr<ExprNode> SimpleParser::ParseInt() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new IntNode(token));
}

unique_ptr<ExprNode> SimpleParser::ParseLong() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new LongNode(token));
}

unique_ptr<ExprNode> SimpleParser::ParseDouble() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new DoubleNode(token));
}

unique_ptr<ExprNode> SimpleParser::ParseBool() {
  const TokenInfo token = next_token_;
  ConsumeNextToken();
  return unique_ptr<ExprNode>(new BoolNode(token));
}

unique_ptr<ExprNode> SimpleParser::ParseArrayAlloc() {
  const TokenInfo op_token = next_token_;
  ConsumeNextToken();
  unique_ptr<BoundedArrayDataTypeNode> array_data_type;
  unique_ptr<DataTypeNode> simple_data_type;

  switch (next_token_.GetId()) {
    case Token::kIntType:
      simple_data_type = ParseIntDataType();
      break;
    case Token::kCharType:
      simple_data_type = ParseCharDataType();
      break;
    case Token::kLongType:
      simple_data_type = ParseLongDataType();
      break;
    case Token::kBoolType:
      simple_data_type = ParseBoolDataType();
      break;
    case Token::kDoubleType:
      simple_data_type = ParseDoubleDataType();
      break;
    case Token::kStringType:
      simple_data_type = ParseStringDataType();
      break;
    case Token::kVoidType:
      simple_data_type = ParseVoidDataType();
      break;
    default:
      UnexpectedToken();
  }

  array_data_type = ParseBoundedArrayDataType(move(simple_data_type));

  if (next_token_.GetId() == Token::kScopeStart) {
    const TokenInfo values_start_token = next_token_;
    ConsumeNextToken();
    vector<TokenInfo> value_separator_tokens;
    vector< unique_ptr<ExprNode> > array_values;

    while (true) {
      array_values.push_back(ParseExpr());

      switch (next_token_.GetId()) {
        case Token::kSeparator: {
          value_separator_tokens.push_back(next_token_);
          ConsumeNextToken();
          break;
        }
        case Token::kScopeEnd: {
          const TokenInfo values_end_token = next_token_;
          ConsumeNextToken();
          return unique_ptr<ExprNode>(new ArrayAllocWithInitNode(
              op_token,
              move(array_data_type),
              values_start_token,
              move(array_values),
              value_separator_tokens,
              values_end_token));
        }
        default: {
          UnexpectedToken();
        }
      }
    };
  }

  return unique_ptr<ExprNode>(new ArrayAllocWithoutInitNode(
      op_token, move(array_data_type)));
}

unique_ptr<DataTypeNode> SimpleParser::ParseIntDataType() {
  unique_ptr<DataTypeNode> data_type(new IntDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseLongDataType() {
  unique_ptr<DataTypeNode> data_type(new LongDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseDoubleDataType() {
  unique_ptr<DataTypeNode> data_type(new DoubleDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseCharDataType() {
  unique_ptr<DataTypeNode> data_type(new CharDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseStringDataType() {
  unique_ptr<DataTypeNode> data_type(new StringDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseBoolDataType() {
  unique_ptr<DataTypeNode> data_type(new BoolDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseVoidDataType() {
  unique_ptr<DataTypeNode> data_type(new VoidDataTypeNode(next_token_));
  ConsumeNextToken();
  return data_type;
}

unique_ptr<DataTypeNode> SimpleParser::ParseDataType(
    unique_ptr<DataTypeNode> simple_data_type) {
  unique_ptr<DataTypeNode> data_type(move(simple_data_type));

  while (next_token_.GetId() == Token::kSubscriptStart) {
    const TokenInfo subscript_start_token = next_token_;
    ConsumeNextToken();

    switch (next_token_.GetId()) {
      case Token::kIntLit: {
        unique_ptr<IntNode> array_size(
            static_cast<IntNode*>(ParseInt().release()));
        AssertNextToken(Token::kSubscriptEnd);
        const TokenInfo subscript_end_token = next_token_;
        ConsumeNextToken();
        data_type.reset(new BoundedArrayDataTypeNode(
            move(data_type),
            subscript_start_token,
            move(array_size),
            subscript_end_token));
        break;
      }
      case Token::kSubscriptEnd: {
        const TokenInfo subscript_end_token = next_token_;
        ConsumeNextToken();
        data_type.reset(new ArrayDataTypeNode(
            move(data_type), subscript_start_token, subscript_end_token));
        break;
      }
      default: {
        UnexpectedToken();
      }
    }
  }

  return data_type;
}

unique_ptr<BoundedArrayDataTypeNode> SimpleParser::ParseBoundedArrayDataType(
    unique_ptr<DataTypeNode> simple_data_type) {
  unique_ptr<DataTypeNode> array_data_type(move(simple_data_type));
  AssertNextToken(Token::kSubscriptStart);

  do {
    const TokenInfo subscript_start_token = next_token_;
    ConsumeNextToken();
    AssertNextToken(Token::kIntLit);
    unique_ptr<IntNode> array_size(
        static_cast<IntNode*>(ParseInt().release()));
    AssertNextToken(Token::kSubscriptEnd);
    const TokenInfo subscript_end_token = next_token_;
    ConsumeNextToken();
    array_data_type.reset(new BoundedArrayDataTypeNode(
        move(array_data_type),
        subscript_start_token,
        move(array_size),
        subscript_end_token));
  } while (next_token_.GetId() == Token::kSubscriptStart);

  return unique_ptr<BoundedArrayDataTypeNode>(
      static_cast<BoundedArrayDataTypeNode*>(array_data_type.release()));
}

TokenInfo SimpleParser::ParseName() {
  AssertNextToken(Token::kName);
  const TokenInfo name = next_token_;
  ConsumeNextToken();
  return name;
}

void SimpleParser::AssertNextToken(Token token) {
  if (next_token_.GetId() != token) {
    UnexpectedToken();
  }
}

void SimpleParser::ConsumeNextToken() {
  next_token_ = lexer_->GetNextToken();
}

void SimpleParser::UnexpectedToken() {
  const string &msg =
      (format("Unexpected token; id=%1%; value=%2%; line=%3%; column=%4%")
       % static_cast<uint32_t>(next_token_.GetId())
       % next_token_.GetValue()
       % next_token_.GetLine()
       % next_token_.GetColumn()).str();
  throw UnexpectedTokenError(next_token_, msg);
}

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

