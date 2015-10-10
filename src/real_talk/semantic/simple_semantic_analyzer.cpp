
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <exception>
#include <functional>
#include <limits>
#include "real_talk/lexer/lexer.h"
#include "real_talk/util/errors.h"
#include "real_talk/parser/parser.h"
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
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/void_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
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
#include "real_talk/semantic/global_var_def_analysis.h"
#include "real_talk/semantic/local_var_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/subscript_analysis.h"
#include "real_talk/semantic/scope_analysis.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"
#include "real_talk/semantic/return_analysis.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/data_type_visitor.h"
#include "real_talk/semantic/cast_resolver.h"

using std::numeric_limits;
using std::function;
using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::unordered_map;
using std::unordered_set;
using std::runtime_error;
using std::pair;
using std::make_pair;
using std::move;
using std::exception;
using std::ostream;
using boost::format;
using boost::adaptors::reverse;
using boost::hash;
using real_talk::lexer::Lexer;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;
using real_talk::parser::Parser;
using real_talk::parser::Node;
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
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::FuncDefWithoutBodyNode;
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
using real_talk::parser::DataTypeNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::DefNode;
using real_talk::parser::VarDefNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::LitNode;
using real_talk::parser::BranchNode;
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::ArrayAllocNode;
using real_talk::parser::FuncDefNode;
using real_talk::parser::BinaryExprNode;
using real_talk::parser::UnaryExprNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::ControlFlowTransferNode;
using real_talk::parser::NodeVisitor;
using real_talk::util::FileNotFoundError;

namespace real_talk {
namespace semantic {

class SimpleSemanticAnalyzer::Impl: private NodeVisitor {
 public:
  explicit Impl(const LitParser &lit_parser);
  unique_ptr<SemanticAnalysis> Analyze(const ProgramNode &main_program,
                                       const ImportPrograms &import_programs);

 private:
  enum class ScopeType: uint8_t;
  class AssigneeContext;
  class Scope;
  class SemanticErrorException;
  class DataTypeQuery;
  class IsVoidDataType;
  class IsDataTypeSupportedByFuncDef;
  class IsDataTypeSupportedByVarDef;
  class IsDataTypeSupportedByArrayAllocElement;
  class IsDataTypeSupportedByAssign;
  class IsDataTypeSupportedByLogicalExpr;
  class IsDataTypeSupportedByArithmeticExpr;
  class IsDataTypeSupportedBySum;
  class IsDataTypeSupportedByGreater;
  class IsDataTypeSupportedByEqual;
  class IsDataTypeSupportedByNegative;
  class DataTypeDeductor;
  class SubscriptDataTypeDeductor;
  class CallDataTypeDeductor;

  typedef unordered_map<string, const DefNode*> IdDefs;

  virtual void VisitAnd(const AndNode &node) override;
  virtual void VisitArrayAllocWithoutInit(
      const ArrayAllocWithoutInitNode &node) override;
  virtual void VisitArrayAllocWithInit(
      const ArrayAllocWithInitNode &node) override;
  virtual void VisitAssign(const AssignNode &node) override;
  virtual void VisitBool(const BoolNode &node) override;
  virtual void VisitBreak(const BreakNode &node) override;
  virtual void VisitCall(const CallNode &node) override;
  virtual void VisitChar(const CharNode &node) override;
  virtual void VisitContinue(
      const ContinueNode &node) override;
  virtual void VisitDiv(const DivNode &node) override;
  virtual void VisitDouble(const DoubleNode &node) override;
  virtual void VisitEqual(const EqualNode &node) override;
  virtual void VisitExprStmt(
      const ExprStmtNode &node) override;
  virtual void VisitFuncDefWithBody(
      const FuncDefWithBodyNode &node) override;
  virtual void VisitFuncDefWithoutBody(
      const FuncDefWithoutBodyNode &node) override;
  virtual void VisitGreater(
      const GreaterNode &node) override;
  virtual void VisitGreaterOrEqual(
      const GreaterOrEqualNode &node) override;
  virtual void VisitIfElseIfElse(
      const IfElseIfElseNode &node) override;
  virtual void VisitIfElseIf(
      const IfElseIfNode &node) override;
  virtual void VisitImport(const ImportNode &node) override;
  virtual void VisitInt(const IntNode &node) override;
  virtual void VisitLess(const LessNode &node) override;
  virtual void VisitLessOrEqual(
      const LessOrEqualNode &node) override;
  virtual void VisitLong(const LongNode &node) override;
  virtual void VisitMul(const MulNode &node) override;
  virtual void VisitNegative(
      const NegativeNode &node) override;
  virtual void VisitNotEqual(
      const NotEqualNode &node) override;
  virtual void VisitNot(const NotNode &node) override;
  virtual void VisitOr(const OrNode &node) override;
  virtual void VisitPreDec(const PreDecNode &node) override;
  virtual void VisitPreInc(const PreIncNode &node) override;
  virtual void VisitPreTestLoop(
      const PreTestLoopNode &node) override;
  virtual void VisitProgram(
      const ProgramNode &node) override;
  virtual void VisitString(const StringNode &node) override;
  virtual void VisitSubscript(
      const SubscriptNode &node) override;
  virtual void VisitSub(const SubNode &node) override;
  virtual void VisitSum(const SumNode &node) override;
  virtual void VisitVarDefWithoutInit(
      const VarDefWithoutInitNode &node) override;
  virtual void VisitVarDefWithInit(
      const VarDefWithInitNode &node) override;
  virtual void VisitId(
      const IdNode &node) override;
  virtual void VisitIntDataType(
      const IntDataTypeNode &node) override;
  virtual void VisitLongDataType(
      const LongDataTypeNode &node) override;
  virtual void VisitDoubleDataType(
      const DoubleDataTypeNode &node) override;
  virtual void VisitCharDataType(
      const CharDataTypeNode &node) override;
  virtual void VisitStringDataType(
      const StringDataTypeNode &node) override;
  virtual void VisitBoolDataType(
      const BoolDataTypeNode &node) override;
  virtual void VisitVoidDataType(
      const VoidDataTypeNode &node) override;
  virtual void VisitArrayDataType(
      const ArrayDataTypeNode &node) override;
  virtual void VisitReturnValue(
      const ReturnValueNode &node) override;
  virtual void VisitReturnWithoutValue(
      const ReturnWithoutValueNode &node) override;
  virtual void VisitArgDef(
      const ArgDefNode &node) override;
  virtual void VisitScope(
      const ScopeNode &node) override;

  /**
   * @param expr_data_type If null, will be deduced from operands
   */
  void VisitBinaryExpr(const BinaryExprNode &expr_node,
                       ValueType expr_value_type,
                       unique_ptr<DataType> expr_data_type,
                       DataTypeQuery *data_type_support_query);

  /**
   * @param expr_data_type If null, will be deduced from operand
   */
  void VisitUnaryExpr(const UnaryExprNode &expr_node,
                      ValueType expr_value_type,
                      unique_ptr<DataType> expr_data_type,
                      DataTypeQuery *data_type_support_query);

  void VisitBranch(const BranchNode &branch_node);

  /**
   * @return Element data type
   */
  unique_ptr<DataType> VisitArrayAlloc(
      const ArrayAllocNode &array_alloc_node, size_t dimensions_count);

  const DataType &VisitVarDef(const VarDefNode &var_def_node);
  void VisitFuncDef(const FuncDefNode &func_def_node,
                    bool &is_func_native,
                    unique_ptr<DataType> &return_data_type);
  void VisitReturn(
      const ReturnNode &return_node,
      const Scope **output_current_func_scope,
      vector<const VarDefNode*> *output_flow_local_var_defs);
  template<typename TError, typename TNode> void VisitControlFlowTransfer(
      const TNode &node);
  void VisitPreExpr(const UnaryExprNode &pre_expr_node);

  /**
   * @param output_scope Null if scope not found
   */
  void GetLocalVarDefsUpToScope(
      ScopeType scope_type,
      const Scope **output_scope,
      vector<const VarDefNode*> *output_local_var_defs);

  unique_ptr<DataType> CreateDataType(const DataTypeNode &data_type_node);
  ExprAnalysis &GetExprAnalysis(const ExprNode *expr);
  NodeSemanticAnalysis &GetNodeAnalysis(const Node *node);
  const DataType &GetFuncReturnDataType(const FuncDefNode *func_def);
  bool IsCurrentScopeGlobal();
  Scope *GetCurrentScope();

  /**
   * @return Null if not found
   */
  Scope *FindParentScope(ScopeType type);

  bool IsAssigneeContext();
  void AddDefAnalysis(const DefNode &def_node,
                      unique_ptr<DefAnalysis> def_analysis);
  void AddLitAnalysis(const LitNode *lit_node,
                      unique_ptr<DataType> data_type,
                      unique_ptr<DataType> casted_data_type,
                      unique_ptr<Lit> lit);
  void AddExprAnalysis(const ExprNode &expr_node,
                       unique_ptr<DataType> data_type,
                       unique_ptr<DataType> casted_data_type,
                       ValueType value_type);
  bool CanCastTo(const DataType &dest_data_type, const DataType &src_data_type);
  void ThrowError(unique_ptr<SemanticError> error);

  const LitParser &lit_parser_;
  SemanticAnalysis::ProgramProblems problems_;
  SemanticAnalysis::NodeAnalyzes node_analyzes_;
  vector<Scope*> scopes_stack_;
  vector<AssigneeContext*> assignee_contexts_stack_;
  CastResolver cast_resolver_;
  unique_ptr<DataType> current_data_type_;
  size_t dimensions_count_of_current_array_type_;
  const ProgramNode *program_;
  bool is_program_import_;
  bool has_non_import_stmts_;
};

class SimpleSemanticAnalyzer::Impl::SemanticErrorException
    : public std::runtime_error {
 public:
  explicit SemanticErrorException(unique_ptr<SemanticError> error)
      : runtime_error(""), error_(move(error)) {
  }

  unique_ptr<SemanticError> GetError() {return move(error_);}

 private:
  unique_ptr<SemanticError> error_;
};

enum class SimpleSemanticAnalyzer::Impl::ScopeType: uint8_t {
  kCommon = UINT8_C(1),
  kGlobal = UINT8_C(2),
  kLoop = UINT8_C(3),
  kFunc = UINT8_C(4)
};

class SimpleSemanticAnalyzer::Impl::Scope {
 public:
  /**
   * @param func_def Null if it's not a function scope
   */
  Scope(ScopeType type,
        const FuncDefNode *func_def,
        vector<Scope*> &scopes_stack)
      : type_(type),
        has_return_(false),
        func_def_(func_def),
        scopes_stack_(scopes_stack) {
    scopes_stack_.push_back(this);
  }

  ~Scope() {
    scopes_stack_.pop_back();
  }

  bool HasReturn() const {
    return has_return_;
  }

  void SetHasReturn(bool has_return) {
    has_return_ = has_return;
  }

  const IdDefs &GetIdDefs() const {
    return id_defs_;
  }

  IdDefs &GetIdDefs() {
    return id_defs_;
  }

  const vector<const VarDefNode*> &GetLocalVarDefs() const {
    return local_var_defs_;
  }

  vector<const VarDefNode*> &GetLocalVarDefs() {
    return local_var_defs_;
  }

  const FuncDefNode *GetFuncDef() const {
    return func_def_;
  }

  ScopeType GetType() const {
    return type_;
  }

 private:
  ScopeType type_;
  bool has_return_;
  const FuncDefNode *func_def_;
  IdDefs id_defs_;
  vector<const VarDefNode*> local_var_defs_;
  vector<Scope*> &scopes_stack_;
};

class SimpleSemanticAnalyzer::Impl::AssigneeContext {
 public:
  AssigneeContext(bool is_active, vector<AssigneeContext*> &contexts_stack)
      : is_active_(is_active), contexts_stack_(contexts_stack) {
    contexts_stack_.push_back(this);
  }

  ~AssigneeContext() {
    contexts_stack_.pop_back();
  }

  bool IsActive() const {
    return is_active_;
  }

 private:
  bool is_active_;
  vector<AssigneeContext*> &contexts_stack_;
};

class SimpleSemanticAnalyzer::Impl::DataTypeQuery: protected DataTypeVisitor {
 public:
  virtual ~DataTypeQuery() {}

  bool Check(const DataType &data_type) {
    data_type.Accept(*this);
    return result_;
  }

 protected:
  DataTypeQuery(): result_(false) {}

  bool result_;

 private:
  virtual void VisitArray(const ArrayDataType&) override {}
  virtual void VisitFunc(const FuncDataType&) override {}
  virtual void VisitBool(const BoolDataType&) override {}
  virtual void VisitInt(const IntDataType&) override {}
  virtual void VisitLong(const LongDataType&) override {}
  virtual void VisitDouble(const DoubleDataType&) override {}
  virtual void VisitChar(const CharDataType&) override {}
  virtual void VisitString(const StringDataType&) override {}
  virtual void VisitVoid(const VoidDataType&) override {}
};

class SimpleSemanticAnalyzer::Impl::IsVoidDataType: public DataTypeQuery {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByAssign
    : public DataTypeQuery {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result_ = !IsVoidDataType().Check(data_type);
  }

  virtual void VisitBool(const BoolDataType&) override {result_ = true;}
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
  virtual void VisitString(const StringDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByLogicalExpr
    : public DataTypeQuery {
 private:
  virtual void VisitBool(const BoolDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByGreater
    : public DataTypeQuery {
 private:
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByEqual
    : public DataTypeQuery {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result_ = !IsVoidDataType().Check(data_type);
  }

  virtual void VisitBool(const BoolDataType&) override {result_ = true;}
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
  virtual void VisitString(const StringDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByArithmeticExpr
    : public DataTypeQuery {
 private:
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedBySum
    : public DataTypeQuery {
 private:
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
  virtual void VisitString(const StringDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByNegative
    : public DataTypeQuery {
 private:
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByFuncDef
    : public DataTypeQuery {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result_ = !IsVoidDataType().Check(data_type.GetElementDataType());
  }

  virtual void VisitBool(const BoolDataType&) override {result_ = true;}
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
  virtual void VisitString(const StringDataType&) override {result_ = true;}
  virtual void VisitVoid(const VoidDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByVarDef
    : public DataTypeQuery {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result_ = !IsVoidDataType().Check(data_type.GetElementDataType());
  }

  virtual void VisitBool(const BoolDataType&) override {result_ = true;}
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
  virtual void VisitString(const StringDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByArrayAllocElement
    : public DataTypeQuery {
 private:
  virtual void VisitBool(const BoolDataType&) override {result_ = true;}
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result_ = true;}
  virtual void VisitChar(const CharDataType&) override {result_ = true;}
  virtual void VisitString(const StringDataType&) override {result_ = true;}
};

class SimpleSemanticAnalyzer::Impl::DataTypeDeductor
    : private DataTypeVisitor {
 public:
  virtual ~DataTypeDeductor() {}

  /**
   * @return Null if data type is not supported
   */
  unique_ptr<DataType> Deduct(const DataType &data_type) {
    data_type.Accept(*this);
    return move(result_data_type_);
  }

 protected:
  unique_ptr<DataType> result_data_type_;

 private:
  virtual void VisitArray(const ArrayDataType&) override {}
  virtual void VisitFunc(const FuncDataType&) override {}
  virtual void VisitBool(const BoolDataType&) override {}
  virtual void VisitInt(const IntDataType&) override {}
  virtual void VisitLong(const LongDataType&) override {}
  virtual void VisitDouble(const DoubleDataType&) override {}
  virtual void VisitChar(const CharDataType&) override {}
  virtual void VisitString(const StringDataType&) override {}
  virtual void VisitVoid(const VoidDataType&) override {}
};

class SimpleSemanticAnalyzer::Impl::SubscriptDataTypeDeductor
    : public DataTypeDeductor {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result_data_type_ = data_type.GetElementDataType().Clone();
  }
};

class SimpleSemanticAnalyzer::Impl::CallDataTypeDeductor
    : public DataTypeDeductor {
 private:
  virtual void VisitFunc(const FuncDataType &data_type) override {
    result_data_type_ = data_type.GetReturnDataType().Clone();
  }
};

SimpleSemanticAnalyzer::SimpleSemanticAnalyzer(const LitParser &lit_parser)
    : impl_(new SimpleSemanticAnalyzer::Impl(lit_parser)) {}

SimpleSemanticAnalyzer::~SimpleSemanticAnalyzer() {}

unique_ptr<SemanticAnalysis> SimpleSemanticAnalyzer::Analyze(
    const ProgramNode &main_program,
    const ImportPrograms &import_programs) {
  return impl_->Analyze(main_program, import_programs);
}

SimpleSemanticAnalyzer::Impl::Impl(const LitParser &lit_parser)
    : lit_parser_(lit_parser) {}

unique_ptr<SemanticAnalysis> SimpleSemanticAnalyzer::Impl::Analyze(
    const ProgramNode &main_program, const ImportPrograms &import_programs) {
  assert(problems_.empty());
  assert(node_analyzes_.empty());
  assert(scopes_stack_.empty());
  assert(assignee_contexts_stack_.empty());

  {
    Scope global_scope(ScopeType::kGlobal, nullptr, scopes_stack_);

    for (const unique_ptr<ProgramNode> &import_program
             : reverse(import_programs)) {
      is_program_import_ = true;
      import_program->Accept(*this);
    }

    is_program_import_ = false;
    main_program.Accept(*this);
    assert(global_scope.GetLocalVarDefs().empty());
  }

  assert(scopes_stack_.empty());
  assert(assignee_contexts_stack_.empty());
  unique_ptr<SemanticAnalysis> analysis(
      new SemanticAnalysis(move(problems_), move(node_analyzes_)));
  problems_.clear();
  node_analyzes_.clear();
  return analysis;
}

void SimpleSemanticAnalyzer::Impl::VisitProgram(const ProgramNode &program) {
  program_ = &program;
  problems_.insert(make_pair(program_, SemanticAnalysis::Problems()));
  has_non_import_stmts_ = false;

  for (const unique_ptr<StmtNode> &stmt: program.GetStmts()) {
    try {
      stmt->Accept(*this);
    } catch (SemanticErrorException &e) {
      SemanticAnalysis::ProgramProblems::iterator problems_it =
          problems_.find(program_);
      assert(problems_it != problems_.end());
      problems_it->second.push_back(e.GetError());
    }
  }
}

void SimpleSemanticAnalyzer::Impl::VisitReturnValue(
    const ReturnValueNode &return_node) {
  if (is_program_import_) {
    return;
  }

  const Scope *current_func_scope;
  vector<const VarDefNode*> flow_local_var_defs;
  VisitReturn(return_node, &current_func_scope, &flow_local_var_defs);
  return_node.GetValue()->Accept(*this);
  const DataType &expected_data_type =
      GetFuncReturnDataType(current_func_scope->GetFuncDef());
  ExprAnalysis &value_analysis = GetExprAnalysis(return_node.GetValue().get());
  const DataType &actual_data_type = value_analysis.GetDataType();

  if (!IsDataTypeSupportedByAssign().Check(expected_data_type)) {
      unique_ptr<SemanticError> error(new ReturnWithIncompatibleTypeError(
          return_node,
          expected_data_type.Clone(),
          actual_data_type.Clone()));
      ThrowError(move(error));
  }

  if (expected_data_type != actual_data_type) {
    const bool is_value_castable = CanCastTo(
        expected_data_type, actual_data_type);

    if (!is_value_castable) {
      unique_ptr<SemanticError> error(new ReturnWithIncompatibleTypeError(
          return_node,
          expected_data_type.Clone(),
          actual_data_type.Clone()));
      ThrowError(move(error));
    }

    value_analysis.SetCastedDataType(expected_data_type.Clone());
  }

  unique_ptr<NodeSemanticAnalysis> return_analysis(new ReturnAnalysis(
      current_func_scope->GetFuncDef(), flow_local_var_defs));
  node_analyzes_.insert(make_pair(&return_node, move(return_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitReturnWithoutValue(
    const ReturnWithoutValueNode &return_node) {
  if (is_program_import_) {
    return;
  }

  const Scope *current_func_scope;
  vector<const VarDefNode*> flow_local_var_defs;
  VisitReturn(return_node, &current_func_scope, &flow_local_var_defs);
  const DataType &func_def_return_data_type =
      GetFuncReturnDataType(current_func_scope->GetFuncDef());

  if (func_def_return_data_type != VoidDataType()) {
    unique_ptr<SemanticError> error(new ReturnWithoutValueError(return_node));
    ThrowError(move(error));
  }

  unique_ptr<NodeSemanticAnalysis> return_analysis(new ReturnAnalysis(
      current_func_scope->GetFuncDef(), flow_local_var_defs));
  node_analyzes_.insert(make_pair(&return_node, move(return_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitReturn(
    const ReturnNode &return_node,
    const SimpleSemanticAnalyzer::Impl::Scope **output_current_func_scope,
    vector<const VarDefNode*> *output_flow_local_var_defs) {
  assert(output_current_func_scope);
  assert(output_flow_local_var_defs);
  has_non_import_stmts_ = true;
  GetLocalVarDefsUpToScope(
      ScopeType::kFunc, output_current_func_scope, output_flow_local_var_defs);
  const bool is_within_func = *output_current_func_scope != nullptr;

  if (!is_within_func) {
    unique_ptr<SemanticError> error(new ReturnNotWithinFuncError(return_node));
    ThrowError(move(error));
  }

  GetCurrentScope()->SetHasReturn(true);
}

void SimpleSemanticAnalyzer::Impl::VisitBreak(const BreakNode &break_node) {
  VisitControlFlowTransfer<BreakNotWithinLoopError>(break_node);
}

void SimpleSemanticAnalyzer::Impl::VisitContinue(
    const ContinueNode &continue_node) {
  VisitControlFlowTransfer<ContinueNotWithinLoopError>(continue_node);
}

template<typename TError, typename TNode>
void SimpleSemanticAnalyzer::Impl::VisitControlFlowTransfer(const TNode &node) {
  has_non_import_stmts_ = true;

  if (is_program_import_) {
    return;
  }

  const Scope *loop_scope;
  vector<const VarDefNode*> flow_local_var_defs;
  GetLocalVarDefsUpToScope(ScopeType::kLoop, &loop_scope, &flow_local_var_defs);
  const bool is_within_loop = loop_scope != nullptr;

  if (!is_within_loop) {
    unique_ptr<SemanticError> error(new TError(node));
    ThrowError(move(error));
  }

  unique_ptr<NodeSemanticAnalysis> break_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes_.insert(make_pair(&node, move(break_analysis)));
}

void SimpleSemanticAnalyzer::Impl::GetLocalVarDefsUpToScope(
    ScopeType scope_type,
    const Scope **output_scope,
    vector<const VarDefNode*> *output_local_var_defs) {
  assert(output_scope);
  assert(output_local_var_defs);
  const auto output_local_var_defs_end_it = output_local_var_defs->cend();

  for (const Scope *scope: reverse(scopes_stack_)) {
    output_local_var_defs->insert(output_local_var_defs_end_it,
                                  scope->GetLocalVarDefs().cbegin(),
                                  scope->GetLocalVarDefs().cend());

    if (scope->GetType() == scope_type) {
      *output_scope = scope;
      return;
    }
  }

  *output_scope = nullptr;
}

void SimpleSemanticAnalyzer::Impl::VisitExprStmt(const ExprStmtNode& stmt) {
  has_non_import_stmts_ = true;

  if (is_program_import_) {
    return;
  }

  stmt.GetExpr()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &var_def_node) {
  VisitVarDef(var_def_node);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithInit(
    const VarDefWithInitNode &var_def_node) {
  const DataType &var_data_type = VisitVarDef(var_def_node);

  if (is_program_import_) {
    return;
  }

  var_def_node.GetValue()->Accept(*this);
  ExprAnalysis &value_analysis = GetExprAnalysis(var_def_node.GetValue().get());
  const DataType &value_data_type = value_analysis.GetDataType();

  if (!IsDataTypeSupportedByAssign().Check(var_data_type)) {
    unique_ptr<SemanticError> error(new VarDefWithIncompatibleValueTypeError(
        var_def_node,
        var_data_type.Clone(),
        value_data_type.Clone()));
    ThrowError(move(error));
  }

  if (var_data_type != value_data_type) {
    const bool is_value_castable =
        CanCastTo(var_data_type, value_data_type);

    if (!is_value_castable) {
      unique_ptr<SemanticError> error(new VarDefWithIncompatibleValueTypeError(
          var_def_node,
          var_data_type.Clone(),
          value_data_type.Clone()));
      ThrowError(move(error));
    }

    value_analysis.SetCastedDataType(var_data_type.Clone());
  }
}

const DataType &SimpleSemanticAnalyzer::Impl::VisitVarDef(
    const VarDefNode &var_def_node) {
  has_non_import_stmts_ = true;
  unique_ptr<DataType> data_type_ptr =
      CreateDataType(*(var_def_node.GetDataType()));
  const DataType &data_type = *data_type_ptr;
  unique_ptr<DefAnalysis> def_analysis;

  if (IsCurrentScopeGlobal()) {
    def_analysis.reset(new GlobalVarDefAnalysis(move(data_type_ptr)));
    AddDefAnalysis(var_def_node, move(def_analysis));
  } else {
    vector<const VarDefNode*> flow_local_var_defs;
    const Scope *func_scope;
    GetLocalVarDefsUpToScope(
        ScopeType::kFunc, &func_scope, &flow_local_var_defs);
    def_analysis.reset(new LocalVarDefAnalysis(
        move(data_type_ptr), flow_local_var_defs));
    AddDefAnalysis(var_def_node, move(def_analysis));
    GetCurrentScope()->GetLocalVarDefs().push_back(&var_def_node);
  }

  if (is_program_import_) {
    return data_type;
  }

  if (!IsDataTypeSupportedByVarDef().Check(data_type)) {
    unique_ptr<SemanticError> error(new DefWithUnsupportedTypeError(
        var_def_node, data_type.Clone()));
    ThrowError(move(error));
  }

  return data_type;
}

void SimpleSemanticAnalyzer::Impl::VisitArgDef(const ArgDefNode &arg_def_node) {
  VisitVarDef(arg_def_node);
}

void SimpleSemanticAnalyzer::Impl::VisitFuncDefWithBody(
    const FuncDefWithBodyNode &func_def_node) {
  bool is_func_native;
  unique_ptr<DataType> return_data_type;
  VisitFuncDef(func_def_node, is_func_native, return_data_type);

  if (is_program_import_) {
    return;
  }

  if (is_func_native) {
    unique_ptr<SemanticError> error(
        new FuncDefWithBodyIsNativeError(func_def_node));
    ThrowError(move(error));
  }

  Scope scope(ScopeType::kFunc, &func_def_node, scopes_stack_);

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    arg_def_node->Accept(*this);
  }

  func_def_node.GetBody()->Accept(*this);

  if (*return_data_type != VoidDataType() && !scope.HasReturn()) {
    unique_ptr<SemanticError> error(
        new FuncDefWithoutReturnValueError(func_def_node));
    ThrowError(move(error));
  }

  FuncDefAnalysis &def_analysis =
      static_cast<FuncDefAnalysis&>(GetNodeAnalysis(&func_def_node));
  def_analysis.SetHasReturn(scope.HasReturn());
}

void SimpleSemanticAnalyzer::Impl::VisitFuncDefWithoutBody(
    const FuncDefWithoutBodyNode &func_def_node) {
  bool is_func_native;
  unique_ptr<DataType> return_data_type;
  VisitFuncDef(func_def_node, is_func_native, return_data_type);

  if (is_program_import_) {
    return;
  }

  if (!is_func_native) {
    unique_ptr<SemanticError> error(
        new FuncDefWithoutBodyNotNativeError(func_def_node));
    ThrowError(move(error));
  }

  Scope scope(ScopeType::kFunc, &func_def_node, scopes_stack_);

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    arg_def_node->Accept(*this);
  }
}

void SimpleSemanticAnalyzer::Impl::VisitFuncDef(
    const FuncDefNode &func_def_node,
    bool &is_func_native,
    unique_ptr<DataType> &return_data_type) {
  has_non_import_stmts_ = true;

  if (!IsCurrentScopeGlobal()) {
    unique_ptr<SemanticError> error(
        new FuncDefWithinNonGlobalScopeError(func_def_node));
    ThrowError(move(error));
  }

  return_data_type = CreateDataType(*(func_def_node.GetDataType()));
  const bool is_return_data_type_supported =
      IsDataTypeSupportedByFuncDef().Check(*return_data_type);

  if (!is_return_data_type_supported) {
    unique_ptr<SemanticError> error(new DefWithUnsupportedTypeError(
        func_def_node, return_data_type->Clone()));
    ThrowError(move(error));
  }

  vector< unique_ptr<DataType> > arg_data_types;

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    unique_ptr<DataType> arg_data_type =
        CreateDataType(*(arg_def_node->GetDataType()));
    arg_data_types.push_back(move(arg_data_type));
  }

  unordered_set<Token> modifier_tokens;

  for (const TokenInfo &modifier_token: func_def_node.GetModifierTokens()) {
    const bool is_duplicate_modifier =
        !modifier_tokens.insert(modifier_token.GetId()).second;
    assert(!is_duplicate_modifier);
    (void) is_duplicate_modifier;
  }

  is_func_native = modifier_tokens.count(Token::kNative) != 0;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      return_data_type->Clone(), move(arg_data_types), is_func_native));
  const bool is_func_has_return = false;
  unique_ptr<DefAnalysis> def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  AddDefAnalysis(func_def_node, move(def_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitPreTestLoop(
    const PreTestLoopNode &loop_node) {
  has_non_import_stmts_ = true;

  if (is_program_import_) {
    return;
  }

  loop_node.GetCond()->Accept(*this);
  BoolDataType expected_cond_data_type;
  ExprAnalysis &cond_analysis = GetExprAnalysis(loop_node.GetCond().get());
  const DataType &actual_cond_data_type = cond_analysis.GetDataType();

  if (expected_cond_data_type != actual_cond_data_type) {
    const bool is_actual_cond_castable =
        CanCastTo(expected_cond_data_type, actual_cond_data_type);

    if (!is_actual_cond_castable) {
      unique_ptr<SemanticError> error(new PreTestLoopWithIncompatibleTypeError(
          loop_node,
          expected_cond_data_type.Clone(),
          actual_cond_data_type.Clone()));
      ThrowError(move(error));
    }

    cond_analysis.SetCastedDataType(expected_cond_data_type.Clone());
  }

  Scope scope(ScopeType::kLoop, nullptr, scopes_stack_);
  loop_node.GetBody()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitIfElseIf(
    const IfElseIfNode &if_else_if_node) {
  has_non_import_stmts_ = true;

  if (is_program_import_) {
    return;
  }

  VisitBranch(if_else_if_node);
}

void SimpleSemanticAnalyzer::Impl::VisitIfElseIfElse(
    const IfElseIfElseNode &if_else_if_else_node) {
  has_non_import_stmts_ = true;

  if (is_program_import_) {
    return;
  }

  VisitBranch(if_else_if_else_node);
  Scope scope(ScopeType::kCommon, nullptr, scopes_stack_);
  if_else_if_else_node.GetElseBody()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitBranch(const BranchNode &branch_node) {
  const ExprNode &if_cond = *(branch_node.GetIf()->GetCond());
  if_cond.Accept(*this);
  BoolDataType expected_cond_data_type;

  {
    ExprAnalysis &cond_analysis = GetExprAnalysis(&if_cond);
    const DataType &actual_cond_data_type = cond_analysis.GetDataType();

    if (expected_cond_data_type != actual_cond_data_type) {
      const bool is_cond_castable =
          CanCastTo(expected_cond_data_type, actual_cond_data_type);

      if (!is_cond_castable) {
        unique_ptr<SemanticError> error(new IfWithIncompatibleTypeError(
            branch_node,
            *(branch_node.GetIf()),
            expected_cond_data_type.Clone(),
            actual_cond_data_type.Clone()));
        ThrowError(move(error));
      }

      cond_analysis.SetCastedDataType(expected_cond_data_type.Clone());
    }
  }

  {
    Scope scope(ScopeType::kCommon, nullptr, scopes_stack_);
    branch_node.GetIf()->GetBody()->Accept(*this);
  }

  for (const unique_ptr<ElseIfNode> &else_if: branch_node.GetElseIfs()) {
    const ExprNode &else_if_cond = *(else_if->GetIf()->GetCond());
    else_if_cond.Accept(*this);
    ExprAnalysis &cond_analysis = GetExprAnalysis(&else_if_cond);
    const DataType &actual_cond_data_type = cond_analysis.GetDataType();

    if (expected_cond_data_type != actual_cond_data_type) {
      const bool is_cond_castable =
          CanCastTo(expected_cond_data_type, actual_cond_data_type);

      if (!is_cond_castable) {
        unique_ptr<SemanticError> error(new IfWithIncompatibleTypeError(
            branch_node,
            *(else_if->GetIf()),
            expected_cond_data_type.Clone(),
            actual_cond_data_type.Clone()));
        ThrowError(move(error));
      }

      cond_analysis.SetCastedDataType(expected_cond_data_type.Clone());
    }

    Scope scope(ScopeType::kCommon, nullptr, scopes_stack_);
    else_if->GetIf()->GetBody()->Accept(*this);
  }
}

void SimpleSemanticAnalyzer::Impl::VisitImport(const ImportNode &import_node) {
  if (is_program_import_) {
    return;
  }

  if (has_non_import_stmts_) {
    unique_ptr<SemanticError> error(new ImportIsNotFirstStmtError(import_node));
    ThrowError(move(error));
  }

  import_node.GetFilePath()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode &alloc_node) {
  unique_ptr<DataType> element_data_type =
      VisitArrayAlloc(alloc_node, alloc_node.GetSizes().size());
  unique_ptr<DataType> &array_data_type = element_data_type;
  assert(!alloc_node.GetSizes().empty());
  IntDataType expected_size_data_type;

  for (const unique_ptr<BoundedArraySizeNode> &size: alloc_node.GetSizes()) {
    array_data_type.reset(new ArrayDataType(move(array_data_type)));
    const ExprNode &size_value = *(size->GetValue());
    size_value.Accept(*this);
    ExprAnalysis &size_value_analysis = GetExprAnalysis(&size_value);
    const DataType &actual_size_data_type = size_value_analysis.GetDataType();

    if (expected_size_data_type != actual_size_data_type) {
      const bool is_size_castable =
          CanCastTo(expected_size_data_type, actual_size_data_type);

      if (!is_size_castable) {
        unique_ptr<SemanticError> error(
            new ArrayAllocWithUnsupportedSizeTypeError(
                alloc_node,
                *size,
                actual_size_data_type.Clone()));
        ThrowError(move(error));
      }

      size_value_analysis.SetCastedDataType(expected_size_data_type.Clone());
    }
  }

  unique_ptr<DataType> array_casted_data_type;
  AddExprAnalysis(alloc_node,
                  move(array_data_type),
                  move(array_casted_data_type),
                  ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode &alloc_node) {
  unique_ptr<DataType> array_data_type =
      VisitArrayAlloc(alloc_node, alloc_node.GetSizes().size());
  assert(!alloc_node.GetSizes().empty());

  for (size_t i = 0; i != alloc_node.GetSizes().size(); ++i) {
    array_data_type.reset(new ArrayDataType(move(array_data_type)));
  }

  const vector< unique_ptr<ExprNode> > &values = alloc_node.GetValues();
  assert(values.size() <= numeric_limits<int32_t>::max());
  auto value_end_it = values.cend();

  for (auto value_it = values.cbegin();
       value_it != value_end_it;
       ++value_it) {
    const ExprNode &value = **value_it;
    value.Accept(*this);
    const DataType &expected_value_data_type =
        static_cast<const ArrayDataType&>(
            *array_data_type).GetElementDataType();
    ExprAnalysis &value_analysis = GetExprAnalysis(&value);
    const DataType &actual_value_data_type = value_analysis.GetDataType();
    const size_t value_index = static_cast<size_t>(value_it - values.begin());

    if (!IsDataTypeSupportedByAssign().Check(expected_value_data_type)) {
      unique_ptr<SemanticError> error(
          new ArrayAllocWithIncompatibleValueTypeError(
              alloc_node,
              value_index,
              expected_value_data_type.Clone(),
              actual_value_data_type.Clone()));
      ThrowError(move(error));
    }

    if (expected_value_data_type != actual_value_data_type) {
      const bool is_actual_value_castable = CanCastTo(
          expected_value_data_type, actual_value_data_type);

      if (!is_actual_value_castable) {
        unique_ptr<SemanticError> error(
            new ArrayAllocWithIncompatibleValueTypeError(
                alloc_node,
                value_index,
                expected_value_data_type.Clone(),
                actual_value_data_type.Clone()));
        ThrowError(move(error));
      }

      value_analysis.SetCastedDataType(expected_value_data_type.Clone());
    }
  }

  unique_ptr<DataType> array_casted_data_type;
  AddExprAnalysis(alloc_node,
                  move(array_data_type),
                  move(array_casted_data_type),
                  ValueType::kRight);
}

unique_ptr<DataType> SimpleSemanticAnalyzer::Impl::VisitArrayAlloc(
    const ArrayAllocNode &alloc_node, size_t dimensions_count) {
  const uint8_t max_dimensions_count = numeric_limits<uint8_t>::max();

  if (dimensions_count > max_dimensions_count) {
    unique_ptr<SemanticError> error(new ArrayAllocWithTooManyDimensionsError(
        alloc_node, max_dimensions_count));
    ThrowError(move(error));
  }

  unique_ptr<DataType> element_data_type =
      CreateDataType(*(alloc_node.GetDataType()));

  if (!IsDataTypeSupportedByArrayAllocElement().Check(*element_data_type)) {
    unique_ptr<SemanticError> error(
        new ArrayAllocWithUnsupportedElementTypeError(
            alloc_node, element_data_type->Clone()));
    ThrowError(move(error));
  }

  return element_data_type;
}

void SimpleSemanticAnalyzer::Impl::VisitSubscript(
    const SubscriptNode &subscript_node) {
  {
    AssigneeContext assignee_context(false, assignee_contexts_stack_);
    subscript_node.GetOperand()->Accept(*this);
  }

  const ExprAnalysis &operand_analysis =
      GetExprAnalysis(subscript_node.GetOperand().get());
  const DataType &operand_data_type = operand_analysis.GetDataType();
  unique_ptr<DataType> subscript_data_type =
      SubscriptDataTypeDeductor().Deduct(operand_data_type);

  if (!subscript_data_type) {
    unique_ptr<SemanticError> error(
        new SubscriptWithUnsupportedOperandTypeError(
            subscript_node, operand_data_type.Clone()));
    ThrowError(move(error));
  }

  {
    AssigneeContext assignee_context(false, assignee_contexts_stack_);
    subscript_node.GetIndex()->Accept(*this);
  }

  IntDataType expected_index_data_type;
  ExprAnalysis &index_analysis =
      GetExprAnalysis(subscript_node.GetIndex().get());
  const DataType &actual_index_data_type = index_analysis.GetDataType();

  if (expected_index_data_type != actual_index_data_type) {
    const bool is_index_castable =
        CanCastTo(expected_index_data_type, actual_index_data_type);

    if (!is_index_castable) {
      unique_ptr<SemanticError> error(
          new SubscriptWithUnsupportedIndexTypeError(
              subscript_node, actual_index_data_type.Clone()));
      ThrowError(move(error));
    }

    index_analysis.SetCastedDataType(expected_index_data_type.Clone());
  }

  unique_ptr<DataType> subscript_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> subscript_analysis(new SubscriptAnalysis(
      move(subscript_data_type),
      move(subscript_casted_data_type),
      operand_analysis.GetValueType(),
      IsAssigneeContext()));
  node_analyzes_.insert(make_pair(&subscript_node, move(subscript_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitId(const IdNode &id_node) {
  const DefNode *def_node = nullptr;
  const string &id = id_node.GetStartToken().GetValue();

  for (const Scope* scope: reverse(scopes_stack_)) {
    const IdDefs &scope_id_defs = scope->GetIdDefs();
    const auto scope_id_defs_it = scope_id_defs.find(id);

    if (scope_id_defs_it != scope_id_defs.cend()) {
      def_node = scope_id_defs_it->second;
      break;
    }
  }

  if (def_node == nullptr) {
    unique_ptr<SemanticError> error(new IdWithoutDefError(id_node));
    ThrowError(move(error));
  }

  const DefAnalysis &def_analysis =
      static_cast<const DefAnalysis&>(GetNodeAnalysis(def_node));
  unique_ptr<DataType> casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      def_analysis.GetDataType().Clone(),
      move(casted_data_type),
      def_analysis.GetValueType(),
      def_node,
      IsAssigneeContext()));
  node_analyzes_.insert(make_pair(&id_node, move(id_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitCall(const CallNode &call_node) {
  call_node.GetOperand()->Accept(*this);
  const DataType &operand_data_type =
      GetExprAnalysis(call_node.GetOperand().get()).GetDataType();
  unique_ptr<DataType> call_data_type =
      CallDataTypeDeductor().Deduct(operand_data_type);

  if (!call_data_type) {
    unique_ptr<SemanticError> error(
        new CallWithUnsupportedTypeError(call_node, operand_data_type.Clone()));
    ThrowError(move(error));
  }

  unique_ptr<DataType> call_casted_data_type;
  AddExprAnalysis(call_node,
                  move(call_data_type),
                  move(call_casted_data_type),
                  ValueType::kRight);
  const FuncDataType &func_data_type =
      static_cast<const FuncDataType&>(operand_data_type);
  size_t expected_args_count = func_data_type.GetArgDataTypes().size();
  size_t actual_args_count = call_node.GetArgs().size();

  if (expected_args_count != actual_args_count) {
    unique_ptr<SemanticError> error(new CallWithInvalidArgsCountError(
        call_node,
        expected_args_count,
        actual_args_count));
    ThrowError(move(error));
  }

  assert(expected_args_count == actual_args_count);
  vector< unique_ptr<ExprNode> >::const_iterator args_it =
      call_node.GetArgs().cbegin();

  for (const unique_ptr<DataType> &arg_def_data_type_ptr:
           func_data_type.GetArgDataTypes()) {
    const ExprNode &arg = **args_it;
    arg.Accept(*this);
    ExprAnalysis &arg_analysis = GetExprAnalysis(&arg);
    const DataType &actual_arg_data_type = arg_analysis.GetDataType();
    const DataType &expected_arg_data_type = *arg_def_data_type_ptr;
    const size_t arg_index =
        static_cast<size_t>(args_it - call_node.GetArgs().begin());

    if (!IsDataTypeSupportedByAssign().Check(expected_arg_data_type)) {
      unique_ptr<SemanticError> error(new CallWithIncompatibleArgTypeError(
          call_node,
          arg_index,
          expected_arg_data_type.Clone(),
          actual_arg_data_type.Clone()));
      ThrowError(move(error));
    }

    if (actual_arg_data_type != expected_arg_data_type) {
      const bool is_actual_arg_castable = CanCastTo(
          expected_arg_data_type, actual_arg_data_type);

      if (!is_actual_arg_castable) {
        unique_ptr<SemanticError> error(new CallWithIncompatibleArgTypeError(
            call_node,
            arg_index,
            expected_arg_data_type.Clone(),
            actual_arg_data_type.Clone()));
        ThrowError(move(error));
      }

      arg_analysis.SetCastedDataType(expected_arg_data_type.Clone());
    }

    ++args_it;
  }
}

void SimpleSemanticAnalyzer::Impl::VisitAssign(const AssignNode &assign_node) {
  {
    AssigneeContext assignee_context(true, assignee_contexts_stack_);
    assign_node.GetLeftOperand()->Accept(*this);
  }

  const ExprAnalysis &assignee_analysis =
      GetExprAnalysis(assign_node.GetLeftOperand().get());

  if (assignee_analysis.GetValueType() == ValueType::kRight) {
    unique_ptr<SemanticError> error(
        new AssignWithRightValueAssigneeError(assign_node));
    ThrowError(move(error));
  }

  assign_node.GetRightOperand()->Accept(*this);
  const DataType &assignee_data_type =
      assignee_analysis.GetDataType();
  ExprAnalysis &value_analysis =
      GetExprAnalysis(assign_node.GetRightOperand().get());
  const DataType &value_data_type = value_analysis.GetDataType();

  if (!IsDataTypeSupportedByAssign().Check(assignee_data_type)) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
          assign_node,
          assignee_data_type.Clone(),
          value_data_type.Clone()));
      ThrowError(move(error));
  }

  if (assignee_data_type != value_data_type) {
    const bool is_value_castable =
        CanCastTo(assignee_data_type, value_data_type);

    if (!is_value_castable) {
      unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
          assign_node,
          assignee_data_type.Clone(),
          value_data_type.Clone()));
      ThrowError(move(error));
    }

    value_analysis.SetCastedDataType(assignee_data_type.Clone());
  }

  unique_ptr<DataType> assign_data_type(new VoidDataType());
  unique_ptr<DataType> assign_casted_data_type;
  AddExprAnalysis(assign_node,
                  move(assign_data_type),
                  move(assign_casted_data_type),
                  ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitAnd(const AndNode &and_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByLogicalExpr data_type_support_query;
  VisitBinaryExpr(and_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitOr(const OrNode &or_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByLogicalExpr data_type_support_query;
  VisitBinaryExpr(or_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitGreater(
    const GreaterNode &greater_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByGreater data_type_support_query;
  VisitBinaryExpr(greater_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitGreaterOrEqual(
    const GreaterOrEqualNode &greater_or_equal_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByGreater data_type_support_query;
  VisitBinaryExpr(greater_or_equal_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitEqual(const EqualNode &equal_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByEqual data_type_support_query;
  VisitBinaryExpr(equal_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitNotEqual(
    const NotEqualNode &not_equal_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByEqual data_type_support_query;
  VisitBinaryExpr(not_equal_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitLess(const LessNode &less_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByGreater data_type_support_query;
  VisitBinaryExpr(less_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitLessOrEqual(
    const LessOrEqualNode &less_or_equal_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByGreater data_type_support_query;
  VisitBinaryExpr(less_or_equal_node,
                  ValueType::kRight,
                  move(data_type),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitDiv(const DivNode &div_node) {
  IsDataTypeSupportedByArithmeticExpr data_type_support_query;
  VisitBinaryExpr(div_node,
                  ValueType::kRight,
                  unique_ptr<DataType>(),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitMul(const MulNode &mul_node) {
  IsDataTypeSupportedByArithmeticExpr data_type_support_query;
  VisitBinaryExpr(mul_node,
                  ValueType::kRight,
                  unique_ptr<DataType>(),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitSub(const SubNode &sub_node) {
  IsDataTypeSupportedByArithmeticExpr data_type_support_query;
  VisitBinaryExpr(sub_node,
                  ValueType::kRight,
                  unique_ptr<DataType>(),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitSum(const SumNode &sum_node) {
  IsDataTypeSupportedBySum data_type_support_query;
  VisitBinaryExpr(sum_node,
                  ValueType::kRight,
                  unique_ptr<DataType>(),
                  &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitNot(const NotNode &not_node) {
  unique_ptr<DataType> data_type(new BoolDataType());
  IsDataTypeSupportedByLogicalExpr data_type_support_query;
  VisitUnaryExpr(not_node,
                 ValueType::kRight,
                 move(data_type),
                 &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitPreInc(const PreIncNode &pre_inc_node) {
  VisitPreExpr(pre_inc_node);
}

void SimpleSemanticAnalyzer::Impl::VisitPreDec(const PreDecNode &pre_dec_node) {
  VisitPreExpr(pre_dec_node);
}

void SimpleSemanticAnalyzer::Impl::VisitPreExpr(
    const UnaryExprNode &expr_node) {
  {
    AssigneeContext assignee_context(true, assignee_contexts_stack_);
    expr_node.GetOperand()->Accept(*this);
  }

  const ExprAnalysis &operand_analysis =
      GetExprAnalysis(expr_node.GetOperand().get());

  if (operand_analysis.GetValueType() == ValueType::kRight) {
    unique_ptr<SemanticError> error(
        new UnaryExprWithRightValueOperandError(expr_node));
    ThrowError(move(error));
  }

  const DataType &operand_data_type = operand_analysis.GetDataType();

  if (!IsDataTypeSupportedByArithmeticExpr().Check(operand_data_type)) {
    unique_ptr<SemanticError> error(new UnaryExprWithUnsupportedTypeError(
        expr_node, operand_data_type.Clone()));
    ThrowError(move(error));
  }

  unique_ptr<DataType> expr_data_type(new VoidDataType());
  unique_ptr<DataType> expr_casted_data_type;
  AddExprAnalysis(expr_node,
                  move(expr_data_type),
                  move(expr_casted_data_type),
                  ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitNegative(
    const NegativeNode &negative_node) {
  IsDataTypeSupportedByNegative data_type_support_query;
  VisitUnaryExpr(negative_node,
                 ValueType::kRight,
                 unique_ptr<DataType>(),
                 &data_type_support_query);
}

void SimpleSemanticAnalyzer::Impl::VisitUnaryExpr(
    const UnaryExprNode &expr_node,
    ValueType expr_value_type,
    unique_ptr<DataType> expr_data_type,
    DataTypeQuery *data_type_support_query) {
  expr_node.GetOperand()->Accept(*this);
  const ExprAnalysis &operand_analysis =
      GetExprAnalysis(expr_node.GetOperand().get());
  const DataType &operand_data_type = operand_analysis.GetDataType();

  if (!data_type_support_query->Check(operand_data_type)) {
    unique_ptr<SemanticError> error(new UnaryExprWithUnsupportedTypeError(
        expr_node, operand_data_type.Clone()));
    ThrowError(move(error));
  }

  if (!expr_data_type) {
    expr_data_type = operand_data_type.Clone();
  }

  unique_ptr<DataType> expr_casted_data_type;
  AddExprAnalysis(expr_node,
                  move(expr_data_type),
                  move(expr_casted_data_type),
                  expr_value_type);
}

void SimpleSemanticAnalyzer::Impl::VisitBinaryExpr(
    const BinaryExprNode &expr_node,
    ValueType expr_value_type,
    unique_ptr<DataType> expr_data_type,
    DataTypeQuery *data_type_support_query) {
  expr_node.GetLeftOperand()->Accept(*this);
  expr_node.GetRightOperand()->Accept(*this);
  ExprAnalysis &left_operand_analysis =
      GetExprAnalysis(expr_node.GetLeftOperand().get());
  const DataType &left_operand_data_type = left_operand_analysis.GetDataType();
  ExprAnalysis &right_operand_analysis =
      GetExprAnalysis(expr_node.GetRightOperand().get());
  const DataType &right_operand_data_type =
      right_operand_analysis.GetDataType();
  unique_ptr<DataType> final_operand_data_type;

  if (left_operand_data_type != right_operand_data_type) {
    const CastResolver::ResolvedCast resolved_cast = cast_resolver_.Resolve(
        left_operand_data_type, right_operand_data_type);

    if (!resolved_cast.IsSuccess()) {
      unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
          expr_node,
          left_operand_data_type.Clone(),
          right_operand_data_type.Clone()));
      ThrowError(move(error));
    }

    final_operand_data_type = resolved_cast.GetFinalDataType()->Clone();

    if (resolved_cast.GetLeftDataType()) {
      left_operand_analysis.SetCastedDataType(
          resolved_cast.GetLeftDataType()->Clone());
    } else if (resolved_cast.GetRightDataType()) {
      right_operand_analysis.SetCastedDataType(
          resolved_cast.GetRightDataType()->Clone());
    }
  } else {
    final_operand_data_type = left_operand_data_type.Clone();
  }

  assert(final_operand_data_type);

  if (!data_type_support_query->Check(*final_operand_data_type)) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
        expr_node,
        left_operand_data_type.Clone(),
        right_operand_data_type.Clone()));
    ThrowError(move(error));
  }

  if (!expr_data_type) {
    expr_data_type = final_operand_data_type->Clone();
  }

  unique_ptr<DataType> expr_casted_data_type;
  AddExprAnalysis(expr_node,
                  move(expr_data_type),
                  move(expr_casted_data_type),
                  expr_value_type);
}

void SimpleSemanticAnalyzer::Impl::VisitBool(const BoolNode &bool_node) {
  const bool value =
      lit_parser_.ParseBool(bool_node.GetStartToken().GetValue());
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<Lit> lit(new BoolLit(value));
  unique_ptr<DataType> casted_data_type;
  AddLitAnalysis(
      &bool_node, move(data_type), move(casted_data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitInt(const IntNode &int_node) {
  int32_t value;

  try {
    value = lit_parser_.ParseInt(int_node.GetStartToken().GetValue());
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(
        new IntWithOutOfRangeValueError(int_node));
    ThrowError(move(error));
  }

  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<Lit> lit(new IntLit(value));
  unique_ptr<DataType> casted_data_type;
  AddLitAnalysis(&int_node, move(data_type), move(casted_data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitLong(const LongNode &long_node) {
  int64_t value;

  try {
    value = lit_parser_.ParseLong(long_node.GetStartToken().GetValue());
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(
        new LongWithOutOfRangeValueError(long_node));
    ThrowError(move(error));
  }

  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<Lit> lit(new LongLit(value));
  unique_ptr<DataType> casted_data_type;
  AddLitAnalysis(
      &long_node, move(data_type), move(casted_data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitDouble(const DoubleNode &double_node) {
  double value;

  try {
    value = lit_parser_.ParseDouble(double_node.GetStartToken().GetValue());
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(
        new DoubleWithOutOfRangeValueError(double_node));
    ThrowError(move(error));
  }

  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<Lit> lit(new DoubleLit(value));
  unique_ptr<DataType> casted_data_type;
  AddLitAnalysis(
      &double_node, move(data_type), move(casted_data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitChar(const CharNode &char_node) {
  char value;

  try {
    value = lit_parser_.ParseChar(char_node.GetStartToken().GetValue());
  } catch (const LitParser::EmptyHexValueError&) {
    unique_ptr<SemanticError> error(
        new CharWithEmptyHexValueError(char_node));
    ThrowError(move(error));
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(
        new CharWithOutOfRangeHexValueError(char_node));
    ThrowError(move(error));
  } catch (const LitParser::MultipleCharsError&) {
    unique_ptr<SemanticError> error(
        new CharWithMultipleCharsError(char_node));
    ThrowError(move(error));
  }

  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<Lit> lit(new CharLit(value));
  unique_ptr<DataType> casted_data_type;
  AddLitAnalysis(
      &char_node, move(data_type), move(casted_data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitString(const StringNode &string_node) {
  string value;

  try {
    value = lit_parser_.ParseString(string_node.GetStartToken().GetValue());
  } catch (const LitParser::EmptyHexValueError&) {
    unique_ptr<SemanticError> error(
        new StringWithEmptyHexValueError(string_node));
    ThrowError(move(error));
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(
        new StringWithOutOfRangeHexValueError(string_node));
    ThrowError(move(error));
  }

  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<Lit> lit(new StringLit(value));
  unique_ptr<DataType> casted_data_type;
  AddLitAnalysis(
      &string_node, move(data_type), move(casted_data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitScope(const ScopeNode &scope_node) {
  for (const unique_ptr<StmtNode> &stmt: scope_node.GetStmts()) {
    stmt->Accept(*this);
  }

  const vector<const VarDefNode*> &local_var_defs =
      GetCurrentScope()->GetLocalVarDefs();
  unique_ptr<NodeSemanticAnalysis> scope_analysis(
      new ScopeAnalysis(local_var_defs));
  node_analyzes_.insert(make_pair(&scope_node, move(scope_analysis)));
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
  ++dimensions_count_of_current_array_type_;
  const uint8_t max_dimensions_count = numeric_limits<uint8_t>::max();

  if (dimensions_count_of_current_array_type_ > max_dimensions_count) {
    unique_ptr<SemanticError> error(new ArrayTypeWithTooManyDimensionsError(
        data_type_node, max_dimensions_count));
    ThrowError(move(error));
  }
}

unique_ptr<DataType> SimpleSemanticAnalyzer::Impl::CreateDataType(
    const DataTypeNode &data_type_node) {
  assert(!current_data_type_);
  dimensions_count_of_current_array_type_ = 0;
  data_type_node.Accept(*this);
  assert(current_data_type_);
  return move(current_data_type_);
}

NodeSemanticAnalysis &SimpleSemanticAnalyzer::Impl::GetNodeAnalysis(
    const Node *node) {
  SemanticAnalysis::NodeAnalyzes::iterator node_analysis_it =
      node_analyzes_.find(node);
  assert(node_analysis_it != node_analyzes_.cend());
  return *(node_analysis_it->second);
}

ExprAnalysis &SimpleSemanticAnalyzer::Impl::GetExprAnalysis(
    const ExprNode *expr) {
  return static_cast<ExprAnalysis&>(GetNodeAnalysis(expr));
}

const DataType &SimpleSemanticAnalyzer::Impl::GetFuncReturnDataType(
    const FuncDefNode *func_def) {
  const FuncDefAnalysis &func_def_analysis =
      static_cast<const FuncDefAnalysis&>(GetNodeAnalysis(func_def));
  return func_def_analysis.GetDataType().GetReturnDataType();
}

void SimpleSemanticAnalyzer::Impl::AddExprAnalysis(
    const ExprNode &expr_node,
    unique_ptr<DataType> data_type,
    unique_ptr<DataType> casted_data_type,
    ValueType value_type) {
  unique_ptr<NodeSemanticAnalysis> expr_analysis(new CommonExprAnalysis(
      move(data_type), move(casted_data_type), value_type));
  node_analyzes_.insert(make_pair(&expr_node, move(expr_analysis)));
}

void SimpleSemanticAnalyzer::Impl::AddLitAnalysis(
    const LitNode *lit_node,
    unique_ptr<DataType> data_type,
    unique_ptr<DataType> casted_data_type,
    unique_ptr<Lit> lit) {
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(data_type), move(casted_data_type), ValueType::kRight, move(lit)));
  node_analyzes_.insert(make_pair(lit_node, move(lit_analysis)));
}

void SimpleSemanticAnalyzer::Impl::AddDefAnalysis(
    const DefNode &def_node, unique_ptr<DefAnalysis> def_analysis) {
  node_analyzes_.insert(make_pair(&def_node, move(def_analysis)));
  Scope *current_scope = GetCurrentScope();
  const string &id = def_node.GetNameToken().GetValue();
  const bool is_id_already_exists = !current_scope->GetIdDefs().insert(
      make_pair(id, &def_node)).second;

  if (is_id_already_exists) {
    unique_ptr<SemanticError> error(new DuplicateDefError(def_node));
    ThrowError(move(error));
  }
}

SimpleSemanticAnalyzer::Impl::Scope
*SimpleSemanticAnalyzer::Impl::GetCurrentScope() {
  assert(!scopes_stack_.empty());
  return scopes_stack_.back();
}

bool SimpleSemanticAnalyzer::Impl::IsCurrentScopeGlobal() {
  return GetCurrentScope()->GetType() == ScopeType::kGlobal;
}

SimpleSemanticAnalyzer::Impl::Scope
*SimpleSemanticAnalyzer::Impl::FindParentScope(ScopeType type) {
  Scope *found_scope = nullptr;

  for (Scope *scope: reverse(scopes_stack_)) {
    if (scope->GetType() == type) {
      found_scope = scope;
      break;
    }
  }

  return found_scope;
}

bool SimpleSemanticAnalyzer::Impl::IsAssigneeContext() {
  return !assignee_contexts_stack_.empty()
      && assignee_contexts_stack_.back()->IsActive();
}

bool SimpleSemanticAnalyzer::Impl::CanCastTo(
    const DataType &dest_data_type, const DataType &src_data_type) {
  CastResolver::ResolvedCast resolved_cast =
      cast_resolver_.Resolve(dest_data_type, src_data_type);
  return resolved_cast.IsSuccess()
      && *(resolved_cast.GetFinalDataType()) == dest_data_type;
}

void SimpleSemanticAnalyzer::Impl::ThrowError(unique_ptr<SemanticError> error) {
  throw SemanticErrorException(move(error));
}
}
}
