
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <cassert>
#include <vector>
#include <string>
#include <utility>
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
#include "real_talk/parser/var_load_node.h"
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
#include "real_talk/parser/simple_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_node.h"
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/int_data_type.h"

using std::vector;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::runtime_error;
using std::pair;
using std::move;
using boost::format;
using boost::adaptors::reverse;
using real_talk::parser::ExprNode;
using real_talk::parser::StmtNode;
using real_talk::parser::AndNode;
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::AssignNode;
using real_talk::parser::BoolNode;
using real_talk::parser::BreakNode;
using real_talk::parser::CallNode;
using real_talk::parser::CharNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::DivNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::EqualNode;
using real_talk::parser::ExprStmtNode;
using real_talk::parser::FuncDefNode;
using real_talk::parser::GreaterNode;
using real_talk::parser::GreaterOrEqualNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::parser::IfElseIfNode;
using real_talk::parser::ImportNode;
using real_talk::parser::IntNode;
using real_talk::parser::LessNode;
using real_talk::parser::LessOrEqualNode;
using real_talk::parser::LongNode;
using real_talk::parser::MulNode;
using real_talk::parser::NegativeNode;
using real_talk::parser::NotEqualNode;
using real_talk::parser::NotNode;
using real_talk::parser::OrNode;
using real_talk::parser::PreDecNode;
using real_talk::parser::PreIncNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::ProgramNode;
using real_talk::parser::StringNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::SubNode;
using real_talk::parser::SumNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::VarLoadNode;
using real_talk::parser::SimpleDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnNode;
using real_talk::lexer::SimpleLexer;
using real_talk::lexer::Token;

namespace real_talk {
namespace semantic {

SimpleSemanticAnalyzer::SimpleSemanticAnalyzer(const ProgramNode &program)
    : program_(program) {
}

SemanticAnalysisResult SimpleSemanticAnalyzer::Analyze() {
  program_.Accept(*this);
  return SemanticAnalysisResult(move(errors_));
}

void SimpleSemanticAnalyzer::VisitAnd(const AndNode&) {
  
}

void SimpleSemanticAnalyzer::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode&) {
  
}

void SimpleSemanticAnalyzer::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode&) {
  
}

void SimpleSemanticAnalyzer::VisitAssign(const AssignNode &assign) {
  assign.GetLeftOperand()->Accept(*this);
  assign.GetRightOperand()->Accept(*this);
  auto left_operand_data_type_it =
      expr_data_types_.find(assign.GetLeftOperand().get());
  assert(left_operand_data_type_it != expr_data_types_.end());
  auto right_operand_data_type_it =
      expr_data_types_.find(assign.GetRightOperand().get());
  assert(right_operand_data_type_it != expr_data_types_.end());
  const DataType &left_operand_data_type =
      *(left_operand_data_type_it->second);
  const DataType &right_operand_data_type =
      *(right_operand_data_type_it->second);

  if (left_operand_data_type != right_operand_data_type) {
    const string error_msg = (format("Assigning '%1%' to '%2%'")
                              % right_operand_data_type.GetName()
                              % left_operand_data_type.GetName()).str();
    const uint32_t error_line_number = assign.GetToken().GetLine();
    const uint32_t error_column_number = assign.GetToken().GetColumn();
    vector< unique_ptr<SemanticHint> > hints;
    unique_ptr<SemanticError> error(new SemanticError(
        error_msg, error_line_number, error_column_number, move(hints)));
    throw SemanticErrorException(move(error));
  }

  expr_data_types_.insert(make_pair(&assign, left_operand_data_type.Clone()));
}

void SimpleSemanticAnalyzer::VisitBool(const BoolNode&) {
  
}

void SimpleSemanticAnalyzer::VisitReturn(const ReturnNode&) {
  
}

void SimpleSemanticAnalyzer::VisitReturnValue(const ReturnValueNode&) {
  
}

void SimpleSemanticAnalyzer::VisitBreak(const BreakNode&) {
  
}

void SimpleSemanticAnalyzer::VisitCall(const CallNode&) {
  
}

void SimpleSemanticAnalyzer::VisitChar(const CharNode&) {
  
}

void SimpleSemanticAnalyzer::VisitContinue(const ContinueNode&) {
  
}

void SimpleSemanticAnalyzer::VisitDiv(const DivNode&) {
  
}

void SimpleSemanticAnalyzer::VisitDouble(const DoubleNode&) {
  
}

void SimpleSemanticAnalyzer::VisitEqual(const EqualNode&) {
  
}

void SimpleSemanticAnalyzer::VisitExprStmt(const ExprStmtNode& stmt) {
  stmt.GetExpr()->Accept(*this);
}

void SimpleSemanticAnalyzer::VisitFuncDef(const FuncDefNode&) {
  
}

void SimpleSemanticAnalyzer::VisitGreater(const GreaterNode&) {
  
}

void SimpleSemanticAnalyzer::VisitGreaterOrEqual(
    const GreaterOrEqualNode&) {
  
}

void SimpleSemanticAnalyzer::VisitIfElseIfElse(const IfElseIfElseNode&) {
  
}

void SimpleSemanticAnalyzer::VisitIfElseIf(const IfElseIfNode&) {
  
}

void SimpleSemanticAnalyzer::VisitImport(const ImportNode&) {
  
}

void SimpleSemanticAnalyzer::VisitInt(const IntNode&) {
  
}

void SimpleSemanticAnalyzer::VisitLess(const LessNode&) {
  
}

void SimpleSemanticAnalyzer::VisitLessOrEqual(const LessOrEqualNode&) {
  
}

void SimpleSemanticAnalyzer::VisitLong(const LongNode&) {
  
}

void SimpleSemanticAnalyzer::VisitMul(const MulNode&) {
  
}

void SimpleSemanticAnalyzer::VisitNegative(const NegativeNode&) {
  
}

void SimpleSemanticAnalyzer::VisitNotEqual(const NotEqualNode&) {
  
}

void SimpleSemanticAnalyzer::VisitNot(const NotNode&) {
  
}

void SimpleSemanticAnalyzer::VisitOr(const OrNode&) {
  
}

void SimpleSemanticAnalyzer::VisitPreDec(const PreDecNode&) {
  
}

void SimpleSemanticAnalyzer::VisitPreInc(const PreIncNode&) {
  
}

void SimpleSemanticAnalyzer::VisitPreTestLoop(const PreTestLoopNode&) {
  
}

void SimpleSemanticAnalyzer::VisitProgram(const ProgramNode &program) {
  Scope scope = {};
  scopes_stack_.push_back(move(scope));

  for (const unique_ptr<StmtNode> &stmt: program.GetStmts()) {
    try {
      stmt->Accept(*this);
    } catch (SemanticErrorException &e) {
      errors_.push_back(e.GetError());
    }
  }
}

void SimpleSemanticAnalyzer::VisitString(const StringNode &str) {
  unique_ptr<DataType> data_type(new StringDataType());
  expr_data_types_.insert(make_pair(&str, move(data_type)));
}

void SimpleSemanticAnalyzer::VisitSubscript(const SubscriptNode&) {
  
}

void SimpleSemanticAnalyzer::VisitSub(const SubNode&) {
  
}

void SimpleSemanticAnalyzer::VisitSum(const SumNode&) {
  
}

void SimpleSemanticAnalyzer::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &var_def) {
  assert(!scopes_stack_.empty());
  unique_ptr<DataType> data_type =
      CreateDataType(*(var_def.GetVarDataType()));
  const string &var_name = var_def.GetVarNameToken().GetValue();
  Scope &current_scope = scopes_stack_.back();
  current_scope.var_data_types.insert(
      make_pair(var_name, move(data_type)));
}

void SimpleSemanticAnalyzer::VisitVarDefWithInit(
    const VarDefWithInitNode&) {
}

void SimpleSemanticAnalyzer::VisitVarLoad(const VarLoadNode &var) {
  unique_ptr<DataType> var_data_type = GetVarDataType(var);
  expr_data_types_.insert(make_pair(&var, move(var_data_type)));
}

void SimpleSemanticAnalyzer::VisitSimpleDataType(
    const SimpleDataTypeNode &data_type_node) {
  switch (data_type_node.GetNameToken().GetId()) {
    case Token::kIntType:
      current_data_type_.reset(new IntDataType());
      break;
    case Token::kStringType:
      current_data_type_.reset(new StringDataType());
      break;
    default:
      assert(false);
      break;
  }
}

void SimpleSemanticAnalyzer::VisitArrayDataType(
    const ArrayDataTypeNode &data_type_node) {
  unique_ptr<DataType> element_data_type =
      CreateDataType(*(data_type_node.GetElementDataType()));
  current_data_type_.reset(new ArrayDataType(
      move(current_data_type_)));
}

unique_ptr<DataType> SimpleSemanticAnalyzer::CreateDataType(
    const DataTypeNode &data_type_node) {
  assert(!current_data_type_);
  data_type_node.Accept(*this);
  assert(current_data_type_);
  return move(current_data_type_);
}

unique_ptr<DataType> SimpleSemanticAnalyzer::GetVarDataType(
    const VarLoadNode &var) {
  unique_ptr<DataType> var_data_type;

  for (const Scope &scope: reverse(scopes_stack_)) {
    VarDataTypes::const_iterator var_data_type_it =
        scope.var_data_types.find(var.GetNameToken().GetValue());

    if (var_data_type_it != scope.var_data_types.end()) {
      var_data_type = var_data_type_it->second->Clone();
      break;
    }
  }

  assert(var_data_type);
  return var_data_type;
}

SemanticErrorException::SemanticErrorException(unique_ptr<SemanticError> error)
    : runtime_error(""), error_(move(error)) {
}

unique_ptr<SemanticError> SemanticErrorException::GetError() {
  return move(error_);
}
}
}
