
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <exception>
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
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/long_data_type_node.h"
#include "real_talk/parser/double_data_type_node.h"
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/void_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/bounded_array_data_type_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_node.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/bounded_array_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/double_lit.h"

using std::vector;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::runtime_error;
using std::pair;
using std::make_pair;
using std::move;
using std::exception;
using boost::format;
using boost::adaptors::reverse;
using boost::numeric_cast;
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
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::BoundedArrayDataTypeNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnNode;

namespace real_talk {
namespace semantic {

SimpleSemanticAnalyzer::SimpleSemanticAnalyzer(const ProgramNode &program)
    : program_(program) {
}

SemanticAnalysisResult SimpleSemanticAnalyzer::Analyze() {
  program_.Accept(*this);
  return SemanticAnalysisResult(move(errors_),
                                move(def_analyzes_),
                                move(expr_analyzes_),
                                move(lit_analyzes_),
                                move(id_analyzes_));
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
  auto left_operand_analysis_it =
      expr_analyzes_.find(assign.GetLeftOperand().get());
  assert(left_operand_analysis_it != expr_analyzes_.end());
  auto right_operand_analysis_it =
      expr_analyzes_.find(assign.GetRightOperand().get());
  assert(right_operand_analysis_it != expr_analyzes_.end());
  const ExprAnalysis &left_operand_analysis =
      left_operand_analysis_it->second;
  const DataType &left_operand_data_type =
      left_operand_analysis.GetDataType();
  const ExprAnalysis &right_operand_analysis =
      right_operand_analysis_it->second;
  const DataType &right_operand_data_type =
      right_operand_analysis.GetDataType();

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

  ExprAnalysis assign_analysis(left_operand_data_type.Clone());
  expr_analyzes_.insert(make_pair(&assign, move(assign_analysis)));
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

void SimpleSemanticAnalyzer::VisitInt(const IntNode &int_node) {
  int32_t value;

  try {
    value = numeric_cast<int32_t>(stoi(int_node.GetToken().GetValue()));
  } catch (const exception&) {
    // TODO
    throw;
  }

  unique_ptr<DataType> data_type(new IntDataType());
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&int_node, move(expr_analysis)));
  LitAnalysis lit_analysis(unique_ptr<Lit>(new IntLit(value)));
  lit_analyzes_.insert(make_pair(&int_node, move(lit_analysis)));
}

void SimpleSemanticAnalyzer::VisitLong(const LongNode &long_node) {
  int64_t value;

  try {
    value = numeric_cast<int64_t>(stoll(long_node.GetToken().GetValue()));
  } catch (const exception&) {
    // TODO
    throw;
  }

  unique_ptr<DataType> data_type(new LongDataType());
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&long_node, move(expr_analysis)));
  LitAnalysis lit_analysis(unique_ptr<Lit>(new LongLit(value)));
  lit_analyzes_.insert(make_pair(&long_node, move(lit_analysis)));
}

void SimpleSemanticAnalyzer::VisitDouble(const DoubleNode &double_node) {
  double value;

  try {
    value = numeric_cast<double>(stod(double_node.GetToken().GetValue()));
  } catch (const exception&) {
    // TODO
    throw;
  }

  unique_ptr<DataType> data_type(new DoubleDataType());
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&double_node, move(expr_analysis)));
  LitAnalysis lit_analysis(unique_ptr<Lit>(new DoubleLit(value)));
  lit_analyzes_.insert(make_pair(&double_node, move(lit_analysis)));
}

void SimpleSemanticAnalyzer::VisitLess(const LessNode&) {
  
}

void SimpleSemanticAnalyzer::VisitLessOrEqual(const LessOrEqualNode&) {
  
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

void SimpleSemanticAnalyzer::VisitString(const StringNode &string_node) {
  unique_ptr<DataType> data_type(new StringDataType());
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&string_node, move(expr_analysis)));
}

void SimpleSemanticAnalyzer::VisitSubscript(const SubscriptNode&) {
  
}

void SimpleSemanticAnalyzer::VisitSub(const SubNode&) {
  
}

void SimpleSemanticAnalyzer::VisitSum(const SumNode&) {
  
}

void SimpleSemanticAnalyzer::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &var_def_node) {
  unique_ptr<DataType> data_type =
      CreateDataType(*(var_def_node.GetDataType()));
  DefAnalysis def_analysis(data_type->Clone());
  def_analyzes_.insert(make_pair(&var_def_node, move(def_analysis)));
  const string &var_name = var_def_node.GetNameToken().GetValue();
  assert(!scopes_stack_.empty());
  Scope &current_scope = scopes_stack_.back();
  current_scope.id_data_types.insert(
      make_pair(var_name, move(data_type)));
}

void SimpleSemanticAnalyzer::VisitVarDefWithInit(
    const VarDefWithInitNode &var_def_node) {
  var_def_node.GetValue()->Accept(*this);
  unique_ptr<DataType> data_type =
      CreateDataType(*(var_def_node.GetDataType()));
  DefAnalysis def_analysis(data_type->Clone());
  def_analyzes_.insert(make_pair(&var_def_node, move(def_analysis)));
  const string &var_name = var_def_node.GetNameToken().GetValue();
  assert(!scopes_stack_.empty());
  Scope &current_scope = scopes_stack_.back();
  current_scope.id_data_types.insert(
      make_pair(var_name, move(data_type)));
}

void SimpleSemanticAnalyzer::VisitVarLoad(const VarLoadNode &var_node) {
  unique_ptr<DataType> data_type = GetDataTypeOfId(var_node);
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&var_node, move(expr_analysis)));
}

void SimpleSemanticAnalyzer::VisitIntDataType(
    const IntDataTypeNode&) {
  current_data_type_.reset(new IntDataType());
}

void SimpleSemanticAnalyzer::VisitLongDataType(
    const LongDataTypeNode&) {
  current_data_type_.reset(new LongDataType());
}

void SimpleSemanticAnalyzer::VisitDoubleDataType(
    const DoubleDataTypeNode&) {
  current_data_type_.reset(new DoubleDataType());
}

void SimpleSemanticAnalyzer::VisitCharDataType(
    const CharDataTypeNode&) {
  current_data_type_.reset(new CharDataType());
}

void SimpleSemanticAnalyzer::VisitStringDataType(
    const StringDataTypeNode&) {
  current_data_type_.reset(new StringDataType());
}

void SimpleSemanticAnalyzer::VisitBoolDataType(
    const BoolDataTypeNode&) {
  current_data_type_.reset(new BoolDataType());
}

void SimpleSemanticAnalyzer::VisitVoidDataType(
    const VoidDataTypeNode&) {
  current_data_type_.reset(new VoidDataType());
}

void SimpleSemanticAnalyzer::VisitArrayDataType(
    const ArrayDataTypeNode &data_type_node) {
  unique_ptr<DataType> element_data_type =
      CreateDataType(*(data_type_node.GetElementDataType()));
  current_data_type_.reset(new ArrayDataType(
      move(element_data_type)));
}

void SimpleSemanticAnalyzer::VisitBoundedArrayDataType(
    const BoundedArrayDataTypeNode &data_type_node) {
  data_type_node.GetSize()->Accept(*this);
  unique_ptr<DataType> element_data_type =
      CreateDataType(*(data_type_node.GetElementDataType()));
  auto lit_analysis_it = lit_analyzes_.find(data_type_node.GetSize().get());
  assert(lit_analysis_it != lit_analyzes_.end());
  const LitAnalysis &lit_analysis = lit_analysis_it->second;
  const IntLit &size = static_cast<const IntLit&>(lit_analysis.GetLit());
  current_data_type_.reset(new BoundedArrayDataType(
      move(element_data_type), size.Clone()));
}

unique_ptr<DataType> SimpleSemanticAnalyzer::CreateDataType(
    const DataTypeNode &data_type_node) {
  assert(!current_data_type_);
  data_type_node.Accept(*this);
  assert(current_data_type_);
  return move(current_data_type_);
}

unique_ptr<DataType> SimpleSemanticAnalyzer::GetDataTypeOfId(
    const VarLoadNode &var_node) {
  unique_ptr<DataType> data_type;

  for (const Scope &scope: reverse(scopes_stack_)) {
    IdDataTypes::const_iterator data_type_it =
        scope.id_data_types.find(var_node.GetNameToken().GetValue());

    if (data_type_it != scope.id_data_types.end()) {
      data_type = data_type_it->second->Clone();
      break;
    }
  }

  assert(data_type);
  return data_type;
}

SemanticErrorException::SemanticErrorException(unique_ptr<SemanticError> error)
    : runtime_error(""), error_(move(error)) {
}

unique_ptr<SemanticError> SemanticErrorException::GetError() {
  return move(error_);
}
}
}
