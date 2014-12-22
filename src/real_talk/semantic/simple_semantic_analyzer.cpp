
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <unordered_map>
#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <exception>
#include <functional>
#include "real_talk/parser/file_parser.h"
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
#include "real_talk/semantic/simple_semantic_analyzer.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/semantic/array_data_type.h"
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
#include "real_talk/semantic/char_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/arg_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/global_data_storage.h"
#include "real_talk/semantic/local_data_storage.h"
#include "real_talk/semantic/import_file_searcher.h"
#include "real_talk/semantic/lit_parser.h"

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
using boost::filesystem::path;
using boost::filesystem::filesystem_error;
using real_talk::lexer::Token;
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
using real_talk::parser::VarDefNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::LitNode;
using real_talk::parser::BranchNode;
using real_talk::parser::FileParser;

namespace real_talk {
namespace semantic {

class SemanticErrorException: public std::runtime_error {
 public:
  explicit SemanticErrorException(std::unique_ptr<SemanticError> error);
  std::unique_ptr<SemanticError> GetError();
 private:
  std::unique_ptr<SemanticError> error_;
};

class SimpleSemanticAnalyzer::Impl: public real_talk::parser::NodeVisitor {
 public:
  Impl(
      const real_talk::parser::ProgramNode &program,
      const real_talk::parser::FileParser &file_parser,
      const ImportFileSearcher &import_file_searcher,
      const LitParser &lit_parser);
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
  typedef std::unordered_map<boost::filesystem::path,
                             std::shared_ptr<
                               real_talk::parser::ProgramNode>,
                             boost::hash<
                               boost::filesystem::path> > FileAnalyzes;

  struct Scope {
    IdDefs id_defs;
  };

  void VisitBranch(const real_talk::parser::BranchNode &branch_node);
  const DataType &VisitVarDef(
      const real_talk::parser::VarDefNode &var_def_node);
  std::unique_ptr<DataType> CreateDataType(
      const real_talk::parser::DataTypeNode &data_type_node);
  const real_talk::parser::DefNode *GetIdDef(const std::string &id);
  const DataType &GetExprDataType(const real_talk::parser::ExprNode *expr);
  bool IsCurrentScopeGlobal();
  void AddDefAnalyzes(const real_talk::parser::DefNode &def_node,
                      std::unique_ptr<DefAnalysis> def_analysis);
  void AddLitAnalyzes(const real_talk::parser::LitNode *lit_node,
                      std::unique_ptr<DataType> data_type,
                      std::unique_ptr<Lit> lit);

  const real_talk::parser::ProgramNode &program_;
  const real_talk::parser::FileParser &file_parser_;
  const ImportFileSearcher &import_file_searcher_;
  const LitParser &lit_parser_;
  std::vector< std::unique_ptr<SemanticProblem> > problems_;
  SemanticAnalysis::DefAnalyzes def_analyzes_;
  SemanticAnalysis::ExprAnalyzes expr_analyzes_;
  SemanticAnalysis::LitAnalyzes lit_analyzes_;
  SemanticAnalysis::ImportAnalyzes import_analyzes_;
  SemanticAnalysis::IdAnalyzes id_analyzes_;
  FileAnalyzes file_analyzes_;
  std::vector<Scope> scopes_stack_;
  std::vector<boost::filesystem::path> file_paths_stack_;
  std::unique_ptr<DataType> current_data_type_;
};

SimpleSemanticAnalyzer::SimpleSemanticAnalyzer(
    const ProgramNode &program,
    const FileParser &file_parser,
    const ImportFileSearcher &import_file_searcher,
    const LitParser &lit_parser)
    : impl_(new SimpleSemanticAnalyzer::Impl(program,
                                             file_parser,
                                             import_file_searcher,
                                             lit_parser)) {
}

SimpleSemanticAnalyzer::~SimpleSemanticAnalyzer() {
}

SemanticAnalysis SimpleSemanticAnalyzer::Analyze() {
  return impl_->Analyze();
}

SimpleSemanticAnalyzer::Impl::Impl(
    const ProgramNode &program,
    const FileParser &file_parser,
    const ImportFileSearcher &import_file_searcher,
    const LitParser &lit_parser)
    : program_(program),
      file_parser_(file_parser),
      import_file_searcher_(import_file_searcher),
      lit_parser_(lit_parser) {
}

SemanticAnalysis SimpleSemanticAnalyzer::Impl::Analyze() {
  const Scope scope = {};
  scopes_stack_.push_back(scope);
  const path file_path;
  file_paths_stack_.push_back(file_path);
  program_.Accept(*this);
  scopes_stack_.pop_back();
  file_paths_stack_.pop_back();
  return SemanticAnalysis(move(problems_),
                          move(def_analyzes_),
                          move(expr_analyzes_),
                          move(lit_analyzes_),
                          move(import_analyzes_),
                          move(id_analyzes_));
}

void SimpleSemanticAnalyzer::Impl::VisitAnd(const AndNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode &alloc_node) {
  unique_ptr<DataType> data_type = CreateDataType(*(alloc_node.GetDataType()));
  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&alloc_node, move(expr_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode &alloc_node) {
  unique_ptr<DataType> data_type = CreateDataType(*(alloc_node.GetDataType()));

  for (const unique_ptr<ExprNode> &value: alloc_node.GetValues()) {
    value->Accept(*this);
  }

  ExprAnalysis expr_analysis(move(data_type));
  expr_analyzes_.insert(make_pair(&alloc_node, move(expr_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitAssign(const AssignNode &assign_node) {
  assign_node.GetLeftOperand()->Accept(*this);
  assign_node.GetRightOperand()->Accept(*this);
  auto left_operand_analysis_it =
      expr_analyzes_.find(assign_node.GetLeftOperand().get());
  assert(left_operand_analysis_it != expr_analyzes_.end());
  auto right_operand_analysis_it =
      expr_analyzes_.find(assign_node.GetRightOperand().get());
  assert(right_operand_analysis_it != expr_analyzes_.end());
  const ExprAnalysis &left_operand_analysis =
      left_operand_analysis_it->second;
  const DataType &left_operand_data_type =
      left_operand_analysis.GetDataType();
  const ExprAnalysis &right_operand_analysis =
      right_operand_analysis_it->second;
  const DataType &right_operand_data_type =
      right_operand_analysis.GetDataType();
  // TODO
  assert(left_operand_data_type == right_operand_data_type);
  ExprAnalysis assign_analysis(left_operand_data_type.Clone());
  expr_analyzes_.insert(make_pair(&assign_node, move(assign_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitEqual(const EqualNode &equal_node) {
  equal_node.GetLeftOperand()->Accept(*this);
  equal_node.GetRightOperand()->Accept(*this);
  auto left_operand_analysis_it =
      expr_analyzes_.find(equal_node.GetLeftOperand().get());
  assert(left_operand_analysis_it != expr_analyzes_.end());
  auto right_operand_analysis_it =
      expr_analyzes_.find(equal_node.GetRightOperand().get());
  assert(right_operand_analysis_it != expr_analyzes_.end());
  const ExprAnalysis &left_operand_analysis =
      left_operand_analysis_it->second;
  const DataType &left_operand_data_type =
      left_operand_analysis.GetDataType();
  const ExprAnalysis &right_operand_analysis =
      right_operand_analysis_it->second;
  const DataType &right_operand_data_type =
      right_operand_analysis.GetDataType();
  // TODO
  assert(left_operand_data_type == right_operand_data_type);
  unique_ptr<DataType> equal_data_type(new BoolDataType());
  ExprAnalysis equal_analysis(move(equal_data_type));
  expr_analyzes_.insert(make_pair(&equal_node, move(equal_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitDiv(const DivNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitCall(const CallNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitReturnValue(
    const ReturnValueNode &return_node) {
  return_node.GetValue()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitReturn(const ReturnNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitBreak(const BreakNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitContinue(const ContinueNode&) {
  
}

void SimpleSemanticAnalyzer::Impl::VisitExprStmt(const ExprStmtNode& stmt) {
  stmt.GetExpr()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &var_def_node) {
  VisitVarDef(var_def_node);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithInit(
    const VarDefWithInitNode &var_def_node) {
  const DataType &var_data_type = VisitVarDef(var_def_node);
  var_def_node.GetValue()->Accept(*this);
  const DataType &value_data_type =
      GetExprDataType(var_def_node.GetValue().get());

  if (var_data_type != value_data_type) {
    const path &current_file_path = file_paths_stack_.back();
    unique_ptr<SemanticError> error(new InitWithIncompatibleTypeError(
        current_file_path, var_def_node, var_data_type, value_data_type));
    throw SemanticErrorException(move(error));
  }
}

const DataType &SimpleSemanticAnalyzer::Impl::VisitVarDef(
    const VarDefNode &var_def_node) {
  unique_ptr<DataType> data_type_ptr =
      CreateDataType(*(var_def_node.GetDataType()));
  const DataType &data_type = *data_type_ptr;
  unique_ptr<DataStorage> data_storage;

  if (IsCurrentScopeGlobal()) {
    data_storage.reset(new GlobalDataStorage());
  } else {
    data_storage.reset(new LocalDataStorage());
  }

  unique_ptr<DefAnalysis> def_analysis(
      new VarDefAnalysis(move(data_type_ptr), move(data_storage)));
  AddDefAnalyzes(var_def_node, move(def_analysis));

  if (data_type.AsPrimitive() == VoidDataType()) {
    const path &current_file_path = file_paths_stack_.back();
    unique_ptr<SemanticError> error(
        new VoidVarDefError(current_file_path, var_def_node));
    throw SemanticErrorException(move(error));
  }

  return data_type;
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
  // TODO
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
  const path relative_file_path(lit.GetValue());
  path absolute_file_path;

  try {
    absolute_file_path = import_file_searcher_.Search(relative_file_path);
  } catch (...) {
    // TODO
    assert(false);
  }

  FileAnalyzes::const_iterator file_analysis_it =
      file_analyzes_.find(absolute_file_path);
  const bool is_file_already_analyzed =
      file_analysis_it != file_analyzes_.end();
  shared_ptr<ProgramNode> program;

  if (is_file_already_analyzed) {
    program = file_analysis_it->second;
  } else {
    try {
      program = file_parser_.Parse(absolute_file_path);
    } catch (...) {
      // TODO
      assert(false);
    }

    file_paths_stack_.push_back(absolute_file_path);
    program->Accept(*this);
    file_paths_stack_.pop_back();
    file_analyzes_.insert(make_pair(absolute_file_path, program));
  }

  const ImportAnalysis import_analysis(program);
  import_analyzes_.insert(make_pair(&import_node, import_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitBool(const BoolNode &bool_node) {
  const bool value = lit_parser_.ParseBool(bool_node.GetToken().GetValue());
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<Lit> lit(new BoolLit(value));
  AddLitAnalyzes(&bool_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitInt(const IntNode &int_node) {
  const int32_t value = lit_parser_.ParseInt(int_node.GetToken().GetValue());
  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<Lit> lit(new IntLit(value));
  AddLitAnalyzes(&int_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitLong(const LongNode &long_node) {
  const int64_t value = lit_parser_.ParseLong(long_node.GetToken().GetValue());
  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<Lit> lit(new LongLit(value));
  AddLitAnalyzes(&long_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitDouble(const DoubleNode &double_node) {
  const double value =
      lit_parser_.ParseDouble(double_node.GetToken().GetValue());
  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<Lit> lit(new DoubleLit(value));
  AddLitAnalyzes(&double_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitChar(const CharNode &char_node) {
  const char value =
      lit_parser_.ParseChar(char_node.GetToken().GetValue());
  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<Lit> lit(new CharLit(value));
  AddLitAnalyzes(&char_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitString(const StringNode &string_node) {
  const string &value =
      lit_parser_.ParseString(string_node.GetToken().GetValue());
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
      problems_.push_back(e.GetError());
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
  current_data_type_.reset(new ArrayDataType(
      move(element_data_type)));
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

  // TODO
  assert(def_node);
  return def_node;
}

const DataType &SimpleSemanticAnalyzer::Impl::GetExprDataType(
    const ExprNode *expr) {
  SemanticAnalysis::ExprAnalyzes::const_iterator expr_analysis_it =
      expr_analyzes_.find(expr);
  assert(expr_analysis_it != expr_analyzes_.end());
  const ExprAnalysis &expr_analysis = expr_analysis_it->second;
  return expr_analysis.GetDataType();
}

void SimpleSemanticAnalyzer::Impl::AddLitAnalyzes(
    const LitNode *lit_node,
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
