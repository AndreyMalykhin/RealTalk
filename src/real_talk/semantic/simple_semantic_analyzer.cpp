
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <exception>
#include "real_talk/parser/node_visitor.h"
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
#include "real_talk/parser/parser_factory.h"
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
#include "real_talk/semantic/string_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/arg_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/global_data_storage.h"
#include "real_talk/semantic/local_data_storage.h"

using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::unordered_map;
using std::runtime_error;
using std::pair;
using std::make_pair;
using std::move;
using std::exception;
using boost::format;
using boost::adaptors::reverse;
using boost::numeric_cast;
using real_talk::lexer::Token;
using real_talk::parser::Parser;
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
using real_talk::parser::ElseIfNode;
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
using real_talk::parser::IdNode;
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
using real_talk::parser::DefNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::LitNode;
using real_talk::parser::BranchNode;
using real_talk::parser::ParserFactory;

namespace real_talk {
namespace semantic {

class SimpleSemanticAnalyzer::Impl: public real_talk::parser::NodeVisitor {
 public:
  Impl(
      const real_talk::parser::ProgramNode &program,
      const real_talk::parser::ParserFactory &parser_factory);
  SemanticAnalysis Analyze();
  virtual void VisitAnd(const real_talk::parser::AndNode &node) override;
  virtual void VisitArrayAllocWithoutInit(
      const real_talk::parser::ArrayAllocWithoutInitNode &node) override;
  virtual void VisitArrayAllocWithInit(
      const real_talk::parser::ArrayAllocWithInitNode &node) override;
  virtual void VisitAssign(const real_talk::parser::AssignNode &node) override;
  virtual void VisitBool(const real_talk::parser::BoolNode &node) override;
  virtual void VisitBreak(const real_talk::parser::BreakNode &node) override;
  virtual void VisitCall(const real_talk::parser::CallNode &node) override;
  virtual void VisitChar(const real_talk::parser::CharNode &node) override;
  virtual void VisitContinue(
      const real_talk::parser::ContinueNode &node) override;
  virtual void VisitDiv(const real_talk::parser::DivNode &node) override;
  virtual void VisitDouble(const real_talk::parser::DoubleNode &node) override;
  virtual void VisitEqual(const real_talk::parser::EqualNode &node) override;
  virtual void VisitExprStmt(
      const real_talk::parser::ExprStmtNode &node) override;
  virtual void VisitFuncDef(
      const real_talk::parser::FuncDefNode &node) override;
  virtual void VisitGreater(
      const real_talk::parser::GreaterNode &node) override;
  virtual void VisitGreaterOrEqual(
      const real_talk::parser::GreaterOrEqualNode &node) override;
  virtual void VisitIfElseIfElse(
      const real_talk::parser::IfElseIfElseNode &node) override;
  virtual void VisitIfElseIf(
      const real_talk::parser::IfElseIfNode &node) override;
  virtual void VisitImport(const real_talk::parser::ImportNode &node) override;
  virtual void VisitInt(const real_talk::parser::IntNode &node) override;
  virtual void VisitLess(const real_talk::parser::LessNode &node) override;
  virtual void VisitLessOrEqual(
      const real_talk::parser::LessOrEqualNode &node) override;
  virtual void VisitLong(const real_talk::parser::LongNode &node) override;
  virtual void VisitMul(const real_talk::parser::MulNode &node) override;
  virtual void VisitNegative(
      const real_talk::parser::NegativeNode &node) override;
  virtual void VisitNotEqual(
      const real_talk::parser::NotEqualNode &node) override;
  virtual void VisitNot(const real_talk::parser::NotNode &node) override;
  virtual void VisitOr(const real_talk::parser::OrNode &node) override;
  virtual void VisitPreDec(const real_talk::parser::PreDecNode &node) override;
  virtual void VisitPreInc(const real_talk::parser::PreIncNode &node) override;
  virtual void VisitPreTestLoop(
      const real_talk::parser::PreTestLoopNode &node) override;
  virtual void VisitProgram(
      const real_talk::parser::ProgramNode &node) override;
  virtual void VisitString(const real_talk::parser::StringNode &node) override;
  virtual void VisitSubscript(
      const real_talk::parser::SubscriptNode &node) override;
  virtual void VisitSub(const real_talk::parser::SubNode &node) override;
  virtual void VisitSum(const real_talk::parser::SumNode &node) override;
  virtual void VisitVarDefWithoutInit(
      const real_talk::parser::VarDefWithoutInitNode &node) override;
  virtual void VisitVarDefWithInit(
      const real_talk::parser::VarDefWithInitNode &node) override;
  virtual void VisitId(
      const real_talk::parser::IdNode &node) override;
  virtual void VisitIntDataType(
      const real_talk::parser::IntDataTypeNode &node) override;
  virtual void VisitLongDataType(
      const real_talk::parser::LongDataTypeNode &node) override;
  virtual void VisitDoubleDataType(
      const real_talk::parser::DoubleDataTypeNode &node) override;
  virtual void VisitCharDataType(
      const real_talk::parser::CharDataTypeNode &node) override;
  virtual void VisitStringDataType(
      const real_talk::parser::StringDataTypeNode &node) override;
  virtual void VisitBoolDataType(
      const real_talk::parser::BoolDataTypeNode &node) override;
  virtual void VisitVoidDataType(
      const real_talk::parser::VoidDataTypeNode &node) override;
  virtual void VisitArrayDataType(
      const real_talk::parser::ArrayDataTypeNode &node) override;
  virtual void VisitBoundedArrayDataType(
      const real_talk::parser::BoundedArrayDataTypeNode &node) override;
  virtual void VisitReturnValue(
      const real_talk::parser::ReturnValueNode &node) override;
  virtual void VisitReturn(
      const real_talk::parser::ReturnNode &node) override;
  virtual void VisitArgDef(
      const real_talk::parser::ArgDefNode &node) override;

 private:
  typedef std::unordered_map<std::string,
                             const real_talk::parser::DefNode*> IdDefs;

  struct Scope {
    IdDefs id_defs;
  };

  void VisitBranch(const real_talk::parser::BranchNode &branch_node);
  std::unique_ptr<DataType> CreateDataType(
      const real_talk::parser::DataTypeNode &data_type_node);
  const real_talk::parser::DefNode *GetIdDef(const std::string &id);
  bool IsCurrentScopeGlobal();
  void AddDefAnalyzes(const real_talk::parser::DefNode &def_node,
                      std::unique_ptr<DefAnalysis> def_analysis);
  void AddLitAnalyzes(const real_talk::parser::LitNode *lit_node,
                      std::unique_ptr<DataType> data_type,
                      std::unique_ptr<Lit> lit);

  const real_talk::parser::ProgramNode &program_;
  const real_talk::parser::ParserFactory &parser_factory_;
  std::vector< std::unique_ptr<SemanticError> > errors_;
  SemanticAnalysis::DefAnalyzes def_analyzes_;
  SemanticAnalysis::ExprAnalyzes expr_analyzes_;
  SemanticAnalysis::LitAnalyzes lit_analyzes_;
  SemanticAnalysis::ImportAnalyzes import_analyzes_;
  SemanticAnalysis::IdAnalyzes id_analyzes_;
  std::vector<Scope> scopes_stack_;
  std::unique_ptr<DataType> current_data_type_;
};

SimpleSemanticAnalyzer::SimpleSemanticAnalyzer(
    const ProgramNode &program, const ParserFactory &parser_factory)
    : impl_(new SimpleSemanticAnalyzer::Impl(program, parser_factory)) {
}

SimpleSemanticAnalyzer::~SimpleSemanticAnalyzer() {
}

SemanticAnalysis SimpleSemanticAnalyzer::Analyze() {
  return impl_->Analyze();
}

SimpleSemanticAnalyzer::Impl::Impl(
    const ProgramNode &program,
    const ParserFactory &parser_factory)
    : program_(program),
      parser_factory_(parser_factory) {
}

SemanticAnalysis SimpleSemanticAnalyzer::Impl::Analyze() {
  const Scope scope = {};
  scopes_stack_.push_back(scope);
  program_.Accept(*this);
  scopes_stack_.pop_back();
  return SemanticAnalysis(move(errors_),
                          move(def_analyzes_),
                          move(expr_analyzes_),
                          move(lit_analyzes_),
                          move(import_analyzes_),
                          move(id_analyzes_));
}

void SimpleSemanticAnalyzer::Impl::VisitAnd(const AndNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitAssign(const AssignNode &assign) {
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

void SimpleSemanticAnalyzer::Impl::VisitReturn(const ReturnNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitReturnValue(const ReturnValueNode &return_node) {
  return_node.GetValue()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitBreak(const BreakNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitCall(const CallNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitChar(const CharNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitContinue(const ContinueNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitDiv(const DivNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitEqual(const EqualNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitExprStmt(const ExprStmtNode& stmt) {
  stmt.GetExpr()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &var_def_node) {
  unique_ptr<DataType> data_type =
      CreateDataType(*(var_def_node.GetDataType()));
  unique_ptr<DataStorage> data_storage;

  if (IsCurrentScopeGlobal()) {
    data_storage.reset(new GlobalDataStorage());
  } else {
    data_storage.reset(new LocalDataStorage());
  }

  unique_ptr<DefAnalysis> def_analysis(
      new VarDefAnalysis(move(data_type), move(data_storage)));
  AddDefAnalyzes(var_def_node, move(def_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithInit(
    const VarDefWithInitNode &var_def_node) {
  var_def_node.GetValue()->Accept(*this);
  unique_ptr<DataType> data_type =
      CreateDataType(*(var_def_node.GetDataType()));
  unique_ptr<DataStorage> data_storage;

  if (IsCurrentScopeGlobal()) {
    data_storage.reset(new GlobalDataStorage());
  } else {
    data_storage.reset(new LocalDataStorage());
  }

  unique_ptr<DefAnalysis> def_analysis(
      new VarDefAnalysis(move(data_type), move(data_storage)));
  AddDefAnalyzes(var_def_node, move(def_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitArgDef(const ArgDefNode &arg_def_node) {
  unique_ptr<DataType> data_type =
      CreateDataType(*(arg_def_node.GetDataType()));
  unique_ptr<DefAnalysis> def_analysis(new ArgDefAnalysis(move(data_type)));
  AddDefAnalyzes(arg_def_node, move(def_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitFuncDef(
    const FuncDefNode &func_def_node) {
  unique_ptr<DataType> data_type =
      CreateDataType(*(func_def_node.GetDataType()));
  unique_ptr<DefAnalysis> def_analysis(new FuncDefAnalysis(move(data_type)));
  AddDefAnalyzes(func_def_node, move(def_analysis));
  const Scope scope = {};
  scopes_stack_.push_back(scope);

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    arg_def_node->Accept(*this);
  }

  for (const unique_ptr<StmtNode> &stmt: func_def_node.GetBody()->GetStmts()) {
    stmt->Accept(*this);
  }

  scopes_stack_.pop_back();
}

void SimpleSemanticAnalyzer::Impl::AddDefAnalyzes(
    const DefNode &def_node,
    unique_ptr<DefAnalysis> def_analysis) {
  def_analyzes_.insert(make_pair(&def_node, move(def_analysis)));
  assert(!scopes_stack_.empty());
  Scope &current_scope = scopes_stack_.back();
  const string &name = def_node.GetNameToken().GetValue();
  assert(current_scope.id_defs.count(name) == 0);
  current_scope.id_defs.insert(make_pair(name, &def_node));
}

void SimpleSemanticAnalyzer::Impl::VisitGreater(const GreaterNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitGreaterOrEqual(
    const GreaterOrEqualNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitIfElseIfElse(
    const IfElseIfElseNode &if_else_if_else_node) {
  VisitBranch(if_else_if_else_node);
  const Scope scope = {};
  scopes_stack_.push_back(scope);
  const vector< unique_ptr<StmtNode> > &else_stmts =
      if_else_if_else_node.GetElseBody()->GetStmts();

  for (const unique_ptr<StmtNode> &else_stmt: else_stmts) {
    else_stmt->Accept(*this);
  }

  scopes_stack_.pop_back();
}

void SimpleSemanticAnalyzer::Impl::VisitIfElseIf(
    const IfElseIfNode &if_else_if_node) {
  VisitBranch(if_else_if_node);
}

void SimpleSemanticAnalyzer::Impl::VisitBranch(const BranchNode &branch_node) {
  branch_node.GetIf()->GetCond()->Accept(*this);
  const vector< unique_ptr<StmtNode> > &if_stmts =
      branch_node.GetIf()->GetBody()->GetStmts();
  const Scope scope = {};
  scopes_stack_.push_back(scope);

  for (const unique_ptr<StmtNode> &stmt: if_stmts) {
    stmt->Accept(*this);
  }

  scopes_stack_.pop_back();

  for (const unique_ptr<ElseIfNode> &else_if: branch_node.GetElseIfs()) {
    else_if->GetIf()->GetCond()->Accept(*this);
    const vector< unique_ptr<StmtNode> > &else_if_stmts =
        else_if->GetIf()->GetBody()->GetStmts();
    scopes_stack_.push_back(scope);

    for (const unique_ptr<StmtNode> &stmt: else_if_stmts) {
      stmt->Accept(*this);
    }

    scopes_stack_.pop_back();
  }
}

void SimpleSemanticAnalyzer::Impl::VisitImport(const ImportNode &import_node) {
  import_node.GetFilePath()->Accept(*this);
  SemanticAnalysis::LitAnalyzes::const_iterator lit_analysis_it =
      lit_analyzes_.find(import_node.GetFilePath().get());
  assert(lit_analysis_it != lit_analyzes_.end());
  const LitAnalysis &lit_analysis = lit_analysis_it->second;
  const StringLit &lit = static_cast<const StringLit&>(lit_analysis.GetLit());
  const string &file_path = lit.GetValue();
  unique_ptr<Parser> parser = parser_factory_.Create(file_path);
  shared_ptr<ProgramNode> program = parser->Parse();
  program->Accept(*this);
  ImportAnalysis import_analysis(move(program));
  import_analyzes_.insert(make_pair(&import_node, move(import_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitBool(const BoolNode &bool_node) {
  bool value;

  switch (bool_node.GetToken().GetId()) {
    case Token::kBoolTrueLit:
      value = true;
      break;
    case Token::kBoolFalseLit:
      value = false;
      break;
    default:
      assert(false);
  }

  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<Lit> lit(new BoolLit(value));
  AddLitAnalyzes(&bool_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitInt(const IntNode &int_node) {
  int32_t value;

  try {
    value = numeric_cast<int32_t>(stoi(int_node.GetToken().GetValue()));
  } catch (const exception&) {
    // TODO
    throw;
  }

  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<Lit> lit(new IntLit(value));
  AddLitAnalyzes(&int_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitLong(const LongNode &long_node) {
  int64_t value;

  try {
    value = numeric_cast<int64_t>(stoll(long_node.GetToken().GetValue()));
  } catch (const exception&) {
    // TODO
    throw;
  }

  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<Lit> lit(new LongLit(value));
  AddLitAnalyzes(&long_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitDouble(const DoubleNode &double_node) {
  double value;

  try {
    value = numeric_cast<double>(stod(double_node.GetToken().GetValue()));
  } catch (const exception&) {
    // TODO
    throw;
  }

  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<Lit> lit(new DoubleLit(value));
  AddLitAnalyzes(&double_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitString(const StringNode &string_node) {
  const string &token_value = string_node.GetToken().GetValue();
  assert(token_value.size() > 1);
  auto token_value_end_it = token_value.end() - 1;
  auto token_value_it = token_value.begin() + 1;
  string value;
  value.reserve(static_cast<size_t>(token_value_end_it - token_value_it));
  char previous_char = '\0';
  const char escape_char_ = '\\';

  while (token_value_it != token_value_end_it) {
    const char current_char = *token_value_it;

    if (current_char != escape_char_ || previous_char == escape_char_) {
      value += current_char;
    }

    previous_char = current_char;
    ++token_value_it;
  }

  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<Lit> lit(new StringLit(value));
  AddLitAnalyzes(&string_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitLess(const LessNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitLessOrEqual(const LessOrEqualNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitMul(const MulNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitNegative(const NegativeNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitNotEqual(const NotEqualNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitNot(const NotNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitOr(const OrNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitPreDec(const PreDecNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitPreInc(const PreIncNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitPreTestLoop(const PreTestLoopNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitProgram(const ProgramNode &program) {
  for (const unique_ptr<StmtNode> &stmt: program.GetStmts()) {
    try {
      stmt->Accept(*this);
    } catch (SemanticErrorException &e) {
      errors_.push_back(e.GetError());
    }
  }
}

void SimpleSemanticAnalyzer::Impl::VisitSubscript(const SubscriptNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitSub(const SubNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitSum(const SumNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitId(const IdNode &id_node) {
  const DefNode *def_node = GetIdDef(id_node.GetNameToken().GetValue());
  unique_ptr<DataType> data_type = CreateDataType(*(def_node->GetDataType()));
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&id_node, move(expr_analysis)));
  const IdAnalysis id_analysis(def_node);
  id_analyzes_.insert(make_pair(&id_node, id_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitIntDataType(
    const IntDataTypeNode&) {
  current_data_type_.reset(new IntDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitLongDataType(
    const LongDataTypeNode&) {
  current_data_type_.reset(new LongDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitDoubleDataType(
    const DoubleDataTypeNode&) {
  current_data_type_.reset(new DoubleDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitCharDataType(
    const CharDataTypeNode&) {
  current_data_type_.reset(new CharDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitStringDataType(
    const StringDataTypeNode&) {
  current_data_type_.reset(new StringDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitBoolDataType(
    const BoolDataTypeNode&) {
  current_data_type_.reset(new BoolDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitVoidDataType(
    const VoidDataTypeNode&) {
  current_data_type_.reset(new VoidDataType());
}

void SimpleSemanticAnalyzer::Impl::VisitArrayDataType(
    const ArrayDataTypeNode &data_type_node) {
  unique_ptr<DataType> element_data_type =
      CreateDataType(*(data_type_node.GetElementDataType()));
  current_data_type_.reset(new ArrayDataType(
      move(element_data_type)));
}

void SimpleSemanticAnalyzer::Impl::VisitBoundedArrayDataType(
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

unique_ptr<DataType> SimpleSemanticAnalyzer::Impl::CreateDataType(
    const DataTypeNode &data_type_node) {
  assert(!current_data_type_);
  data_type_node.Accept(*this);
  assert(current_data_type_);
  return move(current_data_type_);
}

const DefNode *SimpleSemanticAnalyzer::Impl::GetIdDef(const string &id) {
  const DefNode *def_node = nullptr;

  for (const Scope &scope: reverse(scopes_stack_)) {
    IdDefs::const_iterator id_def_it = scope.id_defs.find(id);

    if (id_def_it != scope.id_defs.end()) {
      def_node = id_def_it->second;
      break;
    }
  }

  assert(def_node);
  return def_node;
}

void SimpleSemanticAnalyzer::Impl::AddLitAnalyzes(const LitNode *lit_node,
                                            unique_ptr<DataType> data_type,
                                            unique_ptr<Lit> lit) {
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(lit_node, move(expr_analysis)));
  LitAnalysis lit_analysis(move(lit));
  lit_analyzes_.insert(make_pair(lit_node, move(lit_analysis)));
}

bool SimpleSemanticAnalyzer::Impl::IsCurrentScopeGlobal() {
  return scopes_stack_.size() == 1;
}

SemanticErrorException::SemanticErrorException(unique_ptr<SemanticError> error)
    : runtime_error(""), error_(move(error)) {
}

unique_ptr<SemanticError> SemanticErrorException::GetError() {
  return move(error_);
}
}
}
