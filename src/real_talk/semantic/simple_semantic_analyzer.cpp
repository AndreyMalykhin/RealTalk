
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
#include "real_talk/semantic/arg_def_analysis.h"
#include "real_talk/semantic/global_var_def_analysis.h"
#include "real_talk/semantic/local_var_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/import_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/subscript_analysis.h"
#include "real_talk/semantic/scope_analysis.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"
#include "real_talk/semantic/import_file_searcher.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/data_type_visitor.h"

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
using boost::filesystem::path;
using boost::filesystem::filesystem_error;
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
using real_talk::parser::FileParser;
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::ArrayAllocNode;
using real_talk::parser::FuncDefNode;
using real_talk::parser::BinaryExprNode;
using real_talk::parser::UnaryExprNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::ControlFlowTransferNode;
using real_talk::util::FileNotFoundError;

namespace real_talk {
namespace semantic {

class SimpleSemanticAnalyzer::Impl: private real_talk::parser::NodeVisitor {
 public:
  Impl(
      std::shared_ptr<real_talk::parser::ProgramNode> program,
      const boost::filesystem::path &absolute_file_path,
      const real_talk::parser::FileParser &file_parser,
      const ImportFileSearcher &import_file_searcher,
      const LitParser &lit_parser);
  SemanticAnalysis Analyze();

 private:
  enum class DataTypeId: uint8_t;
  enum class ScopeType: uint8_t;
  class AssigneeContext;
  class Scope;
  class FileScope;
  class SemanticErrorException;
  class DataTypeIdResolver;
  class DataTypeQuery;
  class IsVoidDataType;
  class IsDataTypeSupportedByFuncDef;
  class IsDataTypeSupportedByVarDef;
  class IsDataTypeSupportedBySubscriptIndex;
  class IsDataTypeSupportedByArrayAllocElement;
  class IsDataTypeSupportedByArrayAllocSize;
  class BinaryDataTypeDeductor;
  class UnaryDataTypeDeductor;
  class NotDataTypeDeductor;
  class NegativeDataTypeDeductor;
  class PreIncDataTypeDeductor;
  class PreDecDataTypeDeductor;
  class SubscriptDataTypeDeductor;
  class CallDataTypeDeductor;
  class AssignDataTypeDeductor;
  class LessDataTypeDeductor;
  class LessOrEqualDataTypeDeductor;
  class GreaterDataTypeDeductor;
  class GreaterOrEqualDataTypeDeductor;
  class EqualDataTypeDeductor;
  class NotEqualDataTypeDeductor;
  class AndDataTypeDeductor;
  class OrDataTypeDeductor;
  class MulDataTypeDeductor;
  class DivDataTypeDeductor;
  class SumDataTypeDeductor;
  class SubDataTypeDeductor;

  typedef std::unordered_map<std::string,
                             const real_talk::parser::DefNode*> IdDefs;
  typedef std::unordered_map<boost::filesystem::path,
                             std::shared_ptr<
                               real_talk::parser::ProgramNode>,
                             boost::hash<
                               boost::filesystem::path> > ImportPrograms;

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
  virtual void VisitFuncDefWithBody(
      const real_talk::parser::FuncDefWithBodyNode &node) override;
  virtual void VisitFuncDefWithoutBody(
      const real_talk::parser::FuncDefWithoutBodyNode &node) override;
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
  virtual void VisitReturnValue(
      const real_talk::parser::ReturnValueNode &node) override;
  virtual void VisitReturnWithoutValue(
      const real_talk::parser::ReturnWithoutValueNode &node) override;
  virtual void VisitArgDef(
      const real_talk::parser::ArgDefNode &node) override;
  virtual void VisitScope(
      const real_talk::parser::ScopeNode &node) override;
  void VisitBinaryExpr(const real_talk::parser::BinaryExprNode &expr_node,
                       ValueType expr_value_type,
                       const BinaryDataTypeDeductor &data_type_deductor);
  void VisitUnaryExpr(const real_talk::parser::UnaryExprNode &expr_node,
                      ValueType expr_value_type,
                      UnaryDataTypeDeductor &data_type_deductor,
                      const CommonExprAnalysis &operand_analysis);
  void VisitBranch(const real_talk::parser::BranchNode &branch_node);
  std::unique_ptr<DataType> VisitArrayAlloc(
      const real_talk::parser::ArrayAllocNode &array_alloc_node,
      size_t dimensions_count);
  const DataType &VisitVarDef(
      const real_talk::parser::VarDefNode &var_def_node);
  void VisitFuncDef(const real_talk::parser::FuncDefNode &func_def_node,
                    bool &is_func_native,
                    std::unique_ptr<DataType> &return_data_type);
  const Scope *VisitReturn(const real_talk::parser::ReturnNode &return_node);
  template<typename TError, typename TNode> void VisitControlFlowTransfer(
      const TNode &node);
  std::unique_ptr<DataType> CreateDataType(
      const real_talk::parser::DataTypeNode &data_type_node);
  const CommonExprAnalysis &GetExprAnalysis(
      const real_talk::parser::ExprNode *expr);
  NodeSemanticAnalysis &GetNodeAnalysis(const real_talk::parser::Node *node);
  const DataType &GetExprDataType(const real_talk::parser::ExprNode *expr);
  const DataType &GetFuncReturnDataType(
      const real_talk::parser::FuncDefNode *func_def);
  bool IsCurrentScopeGlobal();
  Scope *GetCurrentScope();

  /**
   * @return Null if not found
   */
  Scope *FindParentScope(ScopeType type);
  bool IsWithinImportProgram();
  bool IsAssigneeContext();
  bool IsDataTypeConvertible(const DataType &dest, const DataType &src);
  const boost::filesystem::path &GetCurrentFilePath();
  void AddDefAnalysis(const real_talk::parser::DefNode &def_node,
                      std::unique_ptr<DefAnalysis> def_analysis);
  void AddLitAnalysis(const real_talk::parser::LitNode *lit_node,
                      std::unique_ptr<DataType> data_type,
                      std::unique_ptr<Lit> lit);
  void AddExprAnalysis(const real_talk::parser::ExprNode &expr_node,
                       std::unique_ptr<DataType> data_type,
                       ValueType value_type);

  std::shared_ptr<real_talk::parser::ProgramNode> program_;
  const boost::filesystem::path file_path_;
  const real_talk::parser::FileParser &file_parser_;
  const ImportFileSearcher &import_file_searcher_;
  const LitParser &lit_parser_;
  std::vector< std::unique_ptr<SemanticProblem> > problems_;
  SemanticAnalysis::NodeAnalyzes node_analyzes_;
  ImportPrograms import_programs_;
  std::vector<Scope*> scopes_stack_;
  std::vector<FileScope*> file_scopes_stack_;
  std::unique_ptr<DataType> current_data_type_;
  size_t dimensions_count_of_current_array_type_;
  size_t non_import_stmts_count_;
  vector<AssigneeContext*> assignee_contexts_stack_;
};

class SimpleSemanticAnalyzer::Impl::SemanticErrorException
    : public std::runtime_error {
 public:
  explicit SemanticErrorException(unique_ptr<SemanticError> error)
      : runtime_error(""), error_(move(error)) {
  }

  unique_ptr<SemanticError> GetError() {
    return move(error_);
  }

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
  vector<Scope*> &scopes_stack_;
};

class SimpleSemanticAnalyzer::Impl::FileScope {
 public:
  FileScope(
      const path &file_path, vector<FileScope*> &file_scopes_stack)
      : file_path_(file_path), file_scopes_stack_(file_scopes_stack) {
    file_scopes_stack_.push_back(this);
  }

  ~FileScope() {
    file_scopes_stack_.pop_back();
  }

  const path &GetFilePath() const {
    return file_path_;
  }

 private:
  const path &file_path_;
  vector<FileScope*> &file_scopes_stack_;
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

  virtual void VisitVoid(const VoidDataType&) override {
    result_ = true;
  }
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

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByArrayAllocSize
    : public DataTypeQuery {
 private:
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
  virtual void VisitLong(const LongDataType&) override {result_ = true;}
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

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedBySubscriptIndex
    : public DataTypeQuery {
 private:
  virtual void VisitInt(const IntDataType&) override {result_ = true;}
};

enum class SimpleSemanticAnalyzer::Impl::DataTypeId: uint8_t {
  kInt = UINT8_C(1),
  kLong,
  kDouble,
  kBool,
  kChar,
  kString,
  kVoid,
  kArray,
  kFunc
};

class SimpleSemanticAnalyzer::Impl::DataTypeIdResolver
    : private DataTypeVisitor {
 public:
  DataTypeId Resolve(const DataType &data_type) {
    data_type.Accept(*this);
    return id_;
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    id_ = DataTypeId::kArray;
  }

  virtual void VisitFunc(const FuncDataType&) override {
    id_ = DataTypeId::kFunc;
  }

  virtual void VisitBool(const BoolDataType&) override {
    id_ = DataTypeId::kBool;
  }

  virtual void VisitInt(const IntDataType&) override {
    id_ = DataTypeId::kInt;
  }

  virtual void VisitLong(const LongDataType&) override {
    id_ = DataTypeId::kLong;
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    id_ = DataTypeId::kDouble;
  }

  virtual void VisitChar(const CharDataType&) override {
    id_ = DataTypeId::kChar;
  }

  virtual void VisitString(const StringDataType&) override {
    id_ = DataTypeId::kString;
  }

  virtual void VisitVoid(const VoidDataType&) override {
    id_ = DataTypeId::kVoid;
  }

  DataTypeId id_;
};

class SimpleSemanticAnalyzer::Impl::UnaryDataTypeDeductor
    : private DataTypeVisitor {
 public:
  virtual ~UnaryDataTypeDeductor() {}

  /**
   * @return nullptr if data type is not supported
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

class SimpleSemanticAnalyzer::Impl::NotDataTypeDeductor
    : public UnaryDataTypeDeductor {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    result_data_type_.reset(new BoolDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::NegativeDataTypeDeductor
    : public UnaryDataTypeDeductor {
 private:
  virtual void VisitInt(const IntDataType&) override {
    result_data_type_.reset(new IntDataType());
  }

  virtual void VisitLong(const LongDataType&) override {
    result_data_type_.reset(new LongDataType());
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    result_data_type_.reset(new DoubleDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::PreIncDataTypeDeductor
    : public UnaryDataTypeDeductor {
 private:
  virtual void VisitInt(const IntDataType&) override {
    result_data_type_.reset(new IntDataType());
  }

  virtual void VisitLong(const LongDataType&) override {
    result_data_type_.reset(new LongDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::PreDecDataTypeDeductor
    : public UnaryDataTypeDeductor {
 private:
  virtual void VisitInt(const IntDataType&) override {
    result_data_type_.reset(new IntDataType());
  }

  virtual void VisitLong(const LongDataType&) override {
    result_data_type_.reset(new LongDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::SubscriptDataTypeDeductor
    : public UnaryDataTypeDeductor {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result_data_type_ = data_type.GetElementDataType().Clone();
  }
};

class SimpleSemanticAnalyzer::Impl::CallDataTypeDeductor
    : public UnaryDataTypeDeductor {
 private:
  virtual void VisitFunc(const FuncDataType &data_type) override {
    result_data_type_ = data_type.GetReturnDataType().Clone();
  }
};

class SimpleSemanticAnalyzer::Impl::BinaryDataTypeDeductor {
 public:
  virtual ~BinaryDataTypeDeductor() {}

  /**
   * @return Null if data types are not supported
   */
  unique_ptr<DataType> Deduct(const DataType &lhs, const DataType &rhs) const {
    const Workers &workers = GetWorkers();
    const DataTypeId lhs_id = DataTypeIdResolver().Resolve(lhs);
    const DataTypeId rhs_id = DataTypeIdResolver().Resolve(rhs);
    Workers::const_iterator worker_it =
        workers.find(make_pair(lhs_id, rhs_id));

    if (worker_it == workers.end()) {
      return unique_ptr<DataType>();
    }

    const Worker &worker = *(worker_it->second);
    return worker(lhs, rhs);
  }

 protected:
  typedef unique_ptr<DataType> (*Worker)(const DataType&, const DataType&);
  typedef pair<DataTypeId, DataTypeId> WorkersKey;
  typedef unordered_map< WorkersKey, Worker, hash<WorkersKey> > Workers;

  static unique_ptr<DataType> LeftWorker(const DataType &lhs, const DataType&) {
    return lhs.Clone();
  }

  static unique_ptr<DataType> RightWorker(
      const DataType&, const DataType &rhs) {
    return rhs.Clone();
  }

 private:
  virtual const Workers &GetWorkers() const = 0;
};

class SimpleSemanticAnalyzer::Impl::AssignDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  virtual const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &AssignDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &AssignDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &AssignDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kBool, DataTypeId::kBool),
              &AssignDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kChar, DataTypeId::kChar),
              &AssignDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kString, DataTypeId::kString),
              &AssignDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kArray, DataTypeId::kArray),
              &AssignDataTypeDeductor::ArrayWorker}
      });
    return workers;
  }

  static unique_ptr<DataType> ArrayWorker(
      const DataType &lhs, const DataType &rhs) {
    return AssignDataTypeDeductor().Deduct(
        static_cast<const ArrayDataType&>(lhs).GetElementDataType(),
        static_cast<const ArrayDataType&>(rhs).GetElementDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::EqualDataTypeDeductor
    : public BinaryDataTypeDeductor {
 public:
  virtual const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &EqualDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &EqualDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &EqualDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kBool, DataTypeId::kBool),
              &EqualDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kChar, DataTypeId::kChar),
              &EqualDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kString, DataTypeId::kString),
              &EqualDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kArray, DataTypeId::kArray),
              &EqualDataTypeDeductor::ArrayWorker}
      });
    return workers;
  }

 private:
  static unique_ptr<DataType> BoolWorker(const DataType&, const DataType&) {
    return unique_ptr<DataType>(new BoolDataType());
  }

  static unique_ptr<DataType> ArrayWorker(
      const DataType &lhs, const DataType &rhs) {
    return EqualDataTypeDeductor().Deduct(
        static_cast<const ArrayDataType&>(lhs).GetElementDataType(),
        static_cast<const ArrayDataType&>(rhs).GetElementDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::NotEqualDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  virtual const Workers &GetWorkers() const override {
    return deductor_.GetWorkers();
  }

  EqualDataTypeDeductor deductor_;
};

class SimpleSemanticAnalyzer::Impl::LessDataTypeDeductor
    : public BinaryDataTypeDeductor {
 public:
  const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &LessDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &LessDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &LessDataTypeDeductor::BoolWorker},
        {make_pair(DataTypeId::kChar, DataTypeId::kChar),
              &LessDataTypeDeductor::BoolWorker}
      });
    return workers;
  }

 private:
  static unique_ptr<DataType> BoolWorker(const DataType&, const DataType&) {
    return unique_ptr<DataType>(new BoolDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::LessOrEqualDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    return deductor_.GetWorkers();
  }

  LessDataTypeDeductor deductor_;
};

class SimpleSemanticAnalyzer::Impl::GreaterDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    return deductor_.GetWorkers();
  }

  LessDataTypeDeductor deductor_;
};

class SimpleSemanticAnalyzer::Impl::GreaterOrEqualDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    return deductor_.GetWorkers();
  }

  LessDataTypeDeductor deductor_;
};

class SimpleSemanticAnalyzer::Impl::AndDataTypeDeductor
    : public BinaryDataTypeDeductor {
 public:
  virtual const Workers &GetWorkers() const override {
    static const Workers &workers =
        *new Workers({
            {make_pair(DataTypeId::kBool, DataTypeId::kBool),
                  &EqualDataTypeDeductor::LeftWorker}
        });
    return workers;
  }
};

class SimpleSemanticAnalyzer::Impl::OrDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    return deductor_.GetWorkers();
  }

  AndDataTypeDeductor deductor_;
};

class SimpleSemanticAnalyzer::Impl::MulDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kLong),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &LessDataTypeDeductor::LeftWorker}
      });
    return workers;
  }
};

class SimpleSemanticAnalyzer::Impl::DivDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &LessDataTypeDeductor::LeftWorker}
      });
    return workers;
  }
};

class SimpleSemanticAnalyzer::Impl::SumDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kString, DataTypeId::kString),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kLong),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &LessDataTypeDeductor::LeftWorker}
      });
    return workers;
  }
};

class SimpleSemanticAnalyzer::Impl::SubDataTypeDeductor
    : public BinaryDataTypeDeductor {
 private:
  const Workers &GetWorkers() const override {
    static const Workers &workers = *new Workers({
        {make_pair(DataTypeId::kInt, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kLong),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kInt, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kLong, DataTypeId::kDouble),
              &LessDataTypeDeductor::RightWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kInt),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kLong),
              &LessDataTypeDeductor::LeftWorker},
        {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
              &LessDataTypeDeductor::LeftWorker}
      });
    return workers;
  }
};

SimpleSemanticAnalyzer::SimpleSemanticAnalyzer(
    shared_ptr<ProgramNode> program,
    const path &file_path,
    const FileParser &file_parser,
    const ImportFileSearcher &import_file_searcher,
    const LitParser &lit_parser)
    : impl_(new SimpleSemanticAnalyzer::Impl(program,
                                             file_path,
                                             file_parser,
                                             import_file_searcher,
                                             lit_parser)) {
}

SimpleSemanticAnalyzer::~SimpleSemanticAnalyzer() {}

SemanticAnalysis SimpleSemanticAnalyzer::Analyze() {
  return impl_->Analyze();
}

SimpleSemanticAnalyzer::Impl::Impl(
    shared_ptr<ProgramNode> program,
    const path &file_path,
    const FileParser &file_parser,
    const ImportFileSearcher &import_file_searcher,
    const LitParser &lit_parser)
    : program_(program),
      file_path_(file_path),
      file_parser_(file_parser),
      import_file_searcher_(import_file_searcher),
      lit_parser_(lit_parser),
      non_import_stmts_count_(0) {
}

SemanticAnalysis SimpleSemanticAnalyzer::Impl::Analyze() {
  assert(scopes_stack_.empty());
  assert(file_scopes_stack_.empty());
  import_programs_.insert(make_pair(file_path_, program_));

  {
    FileScope file_scope(file_path_, file_scopes_stack_);
    Scope scope(ScopeType::kGlobal, nullptr, scopes_stack_);
    program_->Accept(*this);
  }

  assert(scopes_stack_.empty());
  assert(file_scopes_stack_.empty());
  assert(assignee_contexts_stack_.empty());
  return SemanticAnalysis(move(problems_), move(node_analyzes_));
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

void SimpleSemanticAnalyzer::Impl::VisitReturnValue(
    const ReturnValueNode &return_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  const Scope *current_func_scope = VisitReturn(return_node);
  return_node.GetValue()->Accept(*this);
  const DataType &func_def_return_data_type =
      GetFuncReturnDataType(current_func_scope->GetFuncDef());
  const DataType &value_data_type =
      GetExprDataType(return_node.GetValue().get());
  const bool is_data_types_compatible = AssignDataTypeDeductor().Deduct(
      func_def_return_data_type, value_data_type).get() != nullptr;

  if (!is_data_types_compatible) {
    unique_ptr<SemanticError> error(new ReturnWithIncompatibleTypeError(
        GetCurrentFilePath(),
        return_node,
        func_def_return_data_type.Clone(),
        value_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }
}

void SimpleSemanticAnalyzer::Impl::VisitReturnWithoutValue(
    const ReturnWithoutValueNode &return_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  const Scope *current_func_scope = VisitReturn(return_node);
  const DataType &func_def_return_data_type =
      GetFuncReturnDataType(current_func_scope->GetFuncDef());

  if (func_def_return_data_type != VoidDataType()) {
    unique_ptr<SemanticError> error(
        new ReturnWithoutValueError(GetCurrentFilePath(), return_node));
    throw SemanticErrorException(move(error));
  }
}

const SimpleSemanticAnalyzer::Impl::Scope
*SimpleSemanticAnalyzer::Impl::VisitReturn(const ReturnNode &return_node) {
  ++non_import_stmts_count_;
  const Scope *current_func_scope = FindParentScope(ScopeType::kFunc);
  const bool is_within_func = current_func_scope != nullptr;

  if (!is_within_func) {
    unique_ptr<SemanticError> error(
        new ReturnNotWithinFuncError(GetCurrentFilePath(), return_node));
    throw SemanticErrorException(move(error));
  }

  GetCurrentScope()->SetHasReturn(true);
  return current_func_scope;
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
  if (IsWithinImportProgram()) {
    return;
  }

  ++non_import_stmts_count_;

  if (FindParentScope(ScopeType::kLoop) == nullptr) {
    unique_ptr<SemanticError> error(new TError(GetCurrentFilePath(), node));
    throw SemanticErrorException(move(error));
  }

  size_t flow_local_vars_count = 0;

  for (const Scope *scope: reverse(scopes_stack_)) {
    flow_local_vars_count += scope->GetIdDefs().size();

    if (scope->GetType() == ScopeType::kLoop) {
      break;
    }
  }

  assert(flow_local_vars_count <= numeric_limits<uint32_t>::max());
  unique_ptr<NodeSemanticAnalysis> analysis(new ControlFlowTransferAnalysis(
      static_cast<uint32_t>(flow_local_vars_count)));
  node_analyzes_.insert(make_pair(&node, move(analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitExprStmt(const ExprStmtNode& stmt) {
  if (IsWithinImportProgram()) {
    return;
  }

  ++non_import_stmts_count_;
  stmt.GetExpr()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &var_def_node) {
  VisitVarDef(var_def_node);
}

void SimpleSemanticAnalyzer::Impl::VisitVarDefWithInit(
    const VarDefWithInitNode &var_def_node) {
  const DataType &var_data_type = VisitVarDef(var_def_node);

  if (IsWithinImportProgram()) {
    return;
  }

  var_def_node.GetValue()->Accept(*this);
  const DataType &value_data_type =
      GetExprDataType(var_def_node.GetValue().get());

  if (!AssignDataTypeDeductor().Deduct(var_data_type, value_data_type)) {
    unique_ptr<SemanticError> error(new VarDefWithIncompatibleValueTypeError(
        GetCurrentFilePath(),
        var_def_node,
        var_data_type.Clone(),
        value_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }
}

const DataType &SimpleSemanticAnalyzer::Impl::VisitVarDef(
    const VarDefNode &var_def_node) {
  if (!IsWithinImportProgram()) {
    ++non_import_stmts_count_;
  }

  unique_ptr<DataType> data_type_ptr =
      CreateDataType(*(var_def_node.GetDataType()));
  const DataType &data_type = *data_type_ptr;
  unique_ptr<DefAnalysis> def_analysis;

  if (IsCurrentScopeGlobal()) {
    def_analysis.reset(new GlobalVarDefAnalysis(move(data_type_ptr)));
  } else {
    const uint32_t index_within_func = UINT32_C(777);
    def_analysis.reset(
        new LocalVarDefAnalysis(move(data_type_ptr), index_within_func));
  }

  AddDefAnalysis(var_def_node, move(def_analysis));

  if (IsWithinImportProgram()) {
    return data_type;
  }

  if (!IsDataTypeSupportedByVarDef().Check(data_type)) {
    unique_ptr<SemanticError> error(new DefWithUnsupportedTypeError(
        GetCurrentFilePath(), var_def_node, data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  return data_type;
}

void SimpleSemanticAnalyzer::Impl::VisitArgDef(const ArgDefNode &arg_def_node) {
  unique_ptr<DataType> data_type_ptr =
      CreateDataType(*(arg_def_node.GetDataType()));
  const DataType &data_type = *data_type_ptr;
  unique_ptr<DefAnalysis> def_analysis(new ArgDefAnalysis(move(data_type_ptr)));
  AddDefAnalysis(arg_def_node, move(def_analysis));

  if (!IsDataTypeSupportedByVarDef().Check(data_type)) {
    unique_ptr<SemanticError> error(new DefWithUnsupportedTypeError(
        GetCurrentFilePath(), arg_def_node, data_type.Clone()));
    throw SemanticErrorException(move(error));
  }
}

void SimpleSemanticAnalyzer::Impl::VisitFuncDefWithBody(
    const FuncDefWithBodyNode &func_def_node) {
  bool is_func_native;
  unique_ptr<DataType> return_data_type;
  VisitFuncDef(func_def_node, is_func_native, return_data_type);

  if (IsWithinImportProgram()) {
    return;
  }

  if (is_func_native) {
    unique_ptr<SemanticError> error(new FuncDefWithBodyIsNativeError(
        GetCurrentFilePath(), func_def_node));
    throw SemanticErrorException(move(error));
  }

  Scope scope(ScopeType::kFunc, &func_def_node, scopes_stack_);

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    arg_def_node->Accept(*this);
  }

  func_def_node.GetBody()->Accept(*this);

  if (*return_data_type != VoidDataType() && !scope.HasReturn()) {
    unique_ptr<SemanticError> error(new FuncDefWithoutReturnValueError(
        GetCurrentFilePath(), func_def_node));
    throw SemanticErrorException(move(error));
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

  if (IsWithinImportProgram()) {
    return;
  }

  if (!is_func_native) {
    unique_ptr<SemanticError> error(new FuncDefWithoutBodyNotNativeError(
        GetCurrentFilePath(), func_def_node));
    throw SemanticErrorException(move(error));
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
  if (!IsWithinImportProgram()) {
    ++non_import_stmts_count_;
  }

  if (!IsCurrentScopeGlobal()) {
    unique_ptr<SemanticError> error(
        new FuncDefWithinNonGlobalScope(GetCurrentFilePath(), func_def_node));
    throw SemanticErrorException(move(error));
  }

  return_data_type = CreateDataType(*(func_def_node.GetDataType()));
  const bool is_return_data_type_supported =
      IsDataTypeSupportedByFuncDef().Check(*return_data_type);

  if (!is_return_data_type_supported) {
    unique_ptr<SemanticError> error(new DefWithUnsupportedTypeError(
        GetCurrentFilePath(), func_def_node, return_data_type->Clone()));
    throw SemanticErrorException(move(error));
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
  }

  is_func_native = modifier_tokens.count(Token::kNative) != 0;
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(return_data_type->Clone(), move(arg_data_types)));
  unique_ptr<DefAnalysis> def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_native, false));
  AddDefAnalysis(func_def_node, move(def_analysis));
}

void SimpleSemanticAnalyzer::Impl::VisitPreTestLoop(
    const PreTestLoopNode &loop_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  ++non_import_stmts_count_;
  loop_node.GetCond()->Accept(*this);
  unique_ptr<DataType> expected_cond_data_type(new BoolDataType());
  const DataType &actual_cond_data_type =
      GetExprDataType(loop_node.GetCond().get());

  if (!IsDataTypeConvertible(*expected_cond_data_type, actual_cond_data_type)) {
    unique_ptr<SemanticError> error(new PreTestLoopWithIncompatibleTypeError(
        GetCurrentFilePath(),
        loop_node,
        move(expected_cond_data_type),
        actual_cond_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  Scope scope(ScopeType::kLoop, nullptr, scopes_stack_);
  loop_node.GetBody()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitIfElseIf(
    const IfElseIfNode &if_else_if_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  VisitBranch(if_else_if_node);
}

void SimpleSemanticAnalyzer::Impl::VisitIfElseIfElse(
    const IfElseIfElseNode &if_else_if_else_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  VisitBranch(if_else_if_else_node);
  Scope scope(ScopeType::kCommon, nullptr, scopes_stack_);
  if_else_if_else_node.GetElseBody()->Accept(*this);
}

void SimpleSemanticAnalyzer::Impl::VisitBranch(const BranchNode &branch_node) {
  ++non_import_stmts_count_;
  const ExprNode &if_cond = *(branch_node.GetIf()->GetCond());
  if_cond.Accept(*this);

  {
    unique_ptr<DataType> expected_cond_data_type(new BoolDataType());
    const DataType &actual_cond_data_type = GetExprDataType(&if_cond);
    const bool is_data_types_compatible = IsDataTypeConvertible(
        *expected_cond_data_type, actual_cond_data_type);

    if (!is_data_types_compatible) {
      unique_ptr<SemanticError> error(new IfWithIncompatibleTypeError(
          GetCurrentFilePath(),
          branch_node,
          *(branch_node.GetIf()),
          move(expected_cond_data_type),
          actual_cond_data_type.Clone()));
      throw SemanticErrorException(move(error));
    }
  }

  {
    Scope scope(ScopeType::kCommon, nullptr, scopes_stack_);
    branch_node.GetIf()->GetBody()->Accept(*this);
  }

  for (const unique_ptr<ElseIfNode> &else_if: branch_node.GetElseIfs()) {
    const ExprNode &else_if_cond = *(else_if->GetIf()->GetCond());
    else_if_cond.Accept(*this);
    unique_ptr<DataType> expected_cond_data_type(new BoolDataType);
    const DataType &actual_cond_data_type = GetExprDataType(&else_if_cond);
    const bool is_data_types_compatible = IsDataTypeConvertible(
        *expected_cond_data_type, actual_cond_data_type);

    if (!is_data_types_compatible) {
      unique_ptr<SemanticError> error(new IfWithIncompatibleTypeError(
          GetCurrentFilePath(),
          branch_node,
          *(else_if->GetIf()),
          move(expected_cond_data_type),
          actual_cond_data_type.Clone()));
      throw SemanticErrorException(move(error));
    }

    Scope scope(ScopeType::kCommon, nullptr, scopes_stack_);
    else_if->GetIf()->GetBody()->Accept(*this);
  }
}

void SimpleSemanticAnalyzer::Impl::VisitImport(const ImportNode &import_node) {
  if (non_import_stmts_count_ != 0) {
    unique_ptr<SemanticError> error(
        new ImportIsNotFirstStmtError(GetCurrentFilePath(), import_node));
    throw SemanticErrorException(move(error));
  }

  assert(IsCurrentScopeGlobal());
  import_node.GetFilePath()->Accept(*this);
  const LitAnalysis &lit_analysis = static_cast<const LitAnalysis&>(
      GetNodeAnalysis(import_node.GetFilePath().get()));
  const StringLit &lit = static_cast<const StringLit&>(lit_analysis.GetLit());
  const path relative_file_path(lit.GetValue());
  path absolute_file_path;

  try {
    absolute_file_path = import_file_searcher_.Search(relative_file_path);
  } catch (const util::IOError&) {
    const string msg = (format("IO error; file_path=%1%")
                        % relative_file_path).str();
    throw SimpleSemanticAnalyzer::IOError(relative_file_path, msg);
  }

  ImportPrograms::const_iterator import_program_it =
      import_programs_.find(absolute_file_path);
  const bool is_file_already_analyzed =
      import_program_it != import_programs_.end();
  shared_ptr<ProgramNode> program;

  if (is_file_already_analyzed) {
    program = import_program_it->second;
  } else {
    try {
      program = file_parser_.Parse(absolute_file_path);
    } catch (const util::IOError&) {
      const string msg = (format("IO error; file_path=%1%")
                          % absolute_file_path).str();
      throw SimpleSemanticAnalyzer::IOError(absolute_file_path, msg);
    } catch (const Parser::UnexpectedTokenError &e) {
      const string msg = (format("Unexpected token; token=%1%; file_path=%2%")
                          % e.GetToken() % absolute_file_path).str();
      throw SimpleSemanticAnalyzer::UnexpectedTokenError(
          e, absolute_file_path, msg);
    } catch (const Lexer::UnexpectedCharError &e) {
      const string msg = (format("Unexpected char; char=%1%; line=%2%; column=%3%; file_path=%4%")
                          % e.GetChar()
                          % e.GetLineNumber()
                          % e.GetColumnNumber()
                          % absolute_file_path).str();
      throw SimpleSemanticAnalyzer::UnexpectedCharError(
          e, absolute_file_path, msg);
    }

    import_programs_.insert(make_pair(absolute_file_path, program));
    FileScope file_scope(absolute_file_path, file_scopes_stack_);
    program->Accept(*this);
  }

  unique_ptr<NodeSemanticAnalysis> import_analysis(new ImportAnalysis(program));
  node_analyzes_.insert(make_pair(&import_node, move(import_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode &alloc_node) {
  unique_ptr<DataType> element_data_type =
      VisitArrayAlloc(alloc_node, alloc_node.GetSizes().size());
  unique_ptr<DataType> &array_data_type = element_data_type;
  assert(!alloc_node.GetSizes().empty());

  for (const unique_ptr<BoundedArraySizeNode> &size: alloc_node.GetSizes()) {
    array_data_type.reset(new ArrayDataType(move(array_data_type)));
    const ExprNode &size_value = *(size->GetValue());
    size_value.Accept(*this);
    const DataType &size_data_type = GetExprDataType(&size_value);

    if (!IsDataTypeSupportedByArrayAllocSize().Check(size_data_type)) {
      unique_ptr<SemanticError> error(
          new ArrayAllocWithUnsupportedSizeTypeError(
              GetCurrentFilePath(),
              alloc_node,
              *size,
              size_data_type.Clone()));
      throw SemanticErrorException(move(error));
    }
  }

  AddExprAnalysis(alloc_node, move(array_data_type), ValueType::kRight);
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
  assert(values.size() <= numeric_limits<uint32_t>::max());
  auto value_end_it = values.end();

  for (auto value_it = values.begin();
       value_it != value_end_it;
       ++value_it) {
    const ExprNode &value = **value_it;
    value.Accept(*this);
    const DataType &expected_value_data_type =
        static_cast<const ArrayDataType&>(
            *array_data_type).GetElementDataType();
    const DataType &actual_value_data_type = GetExprDataType(&value);
    const bool is_data_types_compatible = AssignDataTypeDeductor().Deduct(
        expected_value_data_type, actual_value_data_type).get() != nullptr;

    if (!is_data_types_compatible) {
      size_t value_index = static_cast<size_t>(value_it - values.begin());
      unique_ptr<SemanticError> error(
          new ArrayAllocWithIncompatibleValueTypeError(
              GetCurrentFilePath(),
              alloc_node,
              value_index,
              expected_value_data_type.Clone(),
              actual_value_data_type.Clone()));
      throw SemanticErrorException(move(error));
    }
  }

  AddExprAnalysis(alloc_node, move(array_data_type), ValueType::kRight);
}

unique_ptr<DataType> SimpleSemanticAnalyzer::Impl::VisitArrayAlloc(
    const ArrayAllocNode &alloc_node, size_t dimensions_count) {
  const uint8_t max_dimensions_count = numeric_limits<uint8_t>::max();

  if (dimensions_count > max_dimensions_count) {
    unique_ptr<SemanticError> error(new ArrayAllocWithTooManyDimensionsError(
        GetCurrentFilePath(), alloc_node, max_dimensions_count));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> element_data_type =
      CreateDataType(*(alloc_node.GetDataType()));

  if (!IsDataTypeSupportedByArrayAllocElement().Check(*element_data_type)) {
    unique_ptr<SemanticError> error(
        new ArrayAllocWithUnsupportedElementTypeError(
            GetCurrentFilePath(), alloc_node, element_data_type->Clone()));
    throw SemanticErrorException(move(error));
  }

  return element_data_type;
}

void SimpleSemanticAnalyzer::Impl::VisitSubscript(
    const SubscriptNode &subscript_node) {
  {
    AssigneeContext assignee_context(false, assignee_contexts_stack_);
    subscript_node.GetOperand()->Accept(*this);
  }

  const CommonExprAnalysis &operand_analysis =
      GetExprAnalysis(subscript_node.GetOperand().get());
  const DataType &operand_data_type = operand_analysis.GetDataType();
  unique_ptr<DataType> subscript_data_type =
      SubscriptDataTypeDeductor().Deduct(operand_data_type);

  if (!subscript_data_type) {
    unique_ptr<SemanticError> error(
        new SubscriptWithNonArrayError(GetCurrentFilePath(), subscript_node));
    throw SemanticErrorException(move(error));
  }

  {
    AssigneeContext assignee_context(false, assignee_contexts_stack_);
    subscript_node.GetIndex()->Accept(*this);
  }

  const DataType &index_data_type =
      GetExprDataType(subscript_node.GetIndex().get());

  if (!IsDataTypeSupportedBySubscriptIndex().Check(index_data_type)) {
    unique_ptr<SemanticError> error(new SubscriptWithUnsupportedIndexTypeError(
        GetCurrentFilePath(),
        subscript_node,
        index_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<NodeSemanticAnalysis> subscript_analysis(new SubscriptAnalysis(
      move(subscript_data_type),
      operand_analysis.GetValueType(),
      IsAssigneeContext()));
  node_analyzes_.insert(make_pair(&subscript_node, move(subscript_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitId(const IdNode &id_node) {
  const DefNode *def_node = nullptr;
  const string &id = id_node.GetNameToken().GetValue();

  for (const Scope* scope: reverse(scopes_stack_)) {
    const IdDefs &scope_id_defs = scope->GetIdDefs();
    IdDefs::const_iterator scope_id_def_it = scope_id_defs.find(id);

    if (scope_id_def_it != scope_id_defs.end()) {
      def_node = scope_id_def_it->second;
      break;
    }
  }

  if (def_node == nullptr) {
    unique_ptr<SemanticError> error(
        new IdWithoutDefError(GetCurrentFilePath(), id_node));
    throw SemanticErrorException(move(error));
  }

  const DefAnalysis &def_analysis =
      static_cast<const DefAnalysis&>(GetNodeAnalysis(def_node));
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      def_analysis.GetDataType().Clone(),
      def_analysis.GetValueType(),
      def_node,
      IsAssigneeContext()));
  node_analyzes_.insert(make_pair(&id_node, move(id_analysis)));
}

void SimpleSemanticAnalyzer::Impl::VisitCall(const CallNode &call_node) {
  call_node.GetOperand()->Accept(*this);
  const DataType &operand_data_type =
      GetExprDataType(call_node.GetOperand().get());
  unique_ptr<DataType> call_data_type =
      CallDataTypeDeductor().Deduct(operand_data_type);

  if (!call_data_type) {
    unique_ptr<SemanticError> error(
        new CallWithNonFuncError(GetCurrentFilePath(), call_node));
    throw SemanticErrorException(move(error));
  }

  AddExprAnalysis(call_node, move(call_data_type), ValueType::kRight);
  const FuncDataType &func_data_type =
      static_cast<const FuncDataType&>(operand_data_type);
  size_t expected_args_count = func_data_type.GetArgDataTypes().size();
  size_t actual_args_count = call_node.GetArgs().size();

  if (expected_args_count != actual_args_count) {
    unique_ptr<SemanticError> error(new CallWithInvalidArgsCount(
        GetCurrentFilePath(),
        call_node,
        expected_args_count,
        actual_args_count));
    throw SemanticErrorException(move(error));
  }

  assert(expected_args_count == actual_args_count);
  vector< unique_ptr<ExprNode> >::const_iterator call_arg_it =
      call_node.GetArgs().begin();

  for (const unique_ptr<DataType> &arg_def_data_type_ptr:
           func_data_type.GetArgDataTypes()) {
    const ExprNode &call_arg = **call_arg_it;
    call_arg.Accept(*this);
    const DataType &call_arg_data_type = GetExprDataType(&call_arg);
    const DataType &arg_def_data_type = *arg_def_data_type_ptr;
    const bool is_data_types_compatible = AssignDataTypeDeductor().Deduct(
        arg_def_data_type, call_arg_data_type).get() != nullptr;

    if (!is_data_types_compatible) {
      const size_t arg_index =
          static_cast<size_t>(call_arg_it - call_node.GetArgs().begin());
      unique_ptr<SemanticError> error(new CallWithIncompatibleArgTypeError(
          GetCurrentFilePath(),
          call_node,
          arg_index,
          arg_def_data_type.Clone(),
          call_arg_data_type.Clone()));
      throw SemanticErrorException(move(error));
    }

    ++call_arg_it;
  }
}

void SimpleSemanticAnalyzer::Impl::VisitAssign(const AssignNode &assign_node) {
  {
    AssigneeContext assignee_context(true, assignee_contexts_stack_);
    assign_node.GetLeftOperand()->Accept(*this);
  }

  const CommonExprAnalysis &left_operand_expr_analysis =
      GetExprAnalysis(assign_node.GetLeftOperand().get());

  if (left_operand_expr_analysis.GetValueType() == ValueType::kRight) {
    unique_ptr<SemanticError> error(new AssignWithRightValueAssigneeError(
        GetCurrentFilePath(), assign_node));
    throw SemanticErrorException(move(error));
  }

  assign_node.GetRightOperand()->Accept(*this);
  const DataType &left_operand_data_type =
      left_operand_expr_analysis.GetDataType();
  const DataType &right_operand_data_type =
      GetExprDataType(assign_node.GetRightOperand().get());
  unique_ptr<DataType> assign_data_type = AssignDataTypeDeductor().Deduct(
      left_operand_data_type, right_operand_data_type);

  if (!assign_data_type) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
        GetCurrentFilePath(),
        assign_node,
        left_operand_data_type.Clone(),
        right_operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  AddExprAnalysis(
      assign_node, move(assign_data_type), ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitAnd(const AndNode &and_node) {
  VisitBinaryExpr(and_node, ValueType::kRight, AndDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitOr(const OrNode &or_node) {
  VisitBinaryExpr(or_node, ValueType::kRight, OrDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitGreater(
    const GreaterNode &greater_node) {
  VisitBinaryExpr(greater_node, ValueType::kRight, GreaterDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitGreaterOrEqual(
    const GreaterOrEqualNode &greater_or_equal_node) {
    VisitBinaryExpr(greater_or_equal_node,
                    ValueType::kRight,
                    GreaterOrEqualDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitEqual(const EqualNode &equal_node) {
  VisitBinaryExpr(equal_node, ValueType::kRight, EqualDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitNotEqual(
    const NotEqualNode &not_equal_node) {
  VisitBinaryExpr(
      not_equal_node, ValueType::kRight, NotEqualDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitLess(const LessNode &less_node) {
  VisitBinaryExpr(less_node, ValueType::kRight, LessDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitLessOrEqual(
    const LessOrEqualNode &less_or_equal_node) {
  VisitBinaryExpr(
      less_or_equal_node, ValueType::kRight, LessOrEqualDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitDiv(const DivNode &div_node) {
  VisitBinaryExpr(div_node, ValueType::kRight, DivDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitMul(const MulNode &mul_node) {
  VisitBinaryExpr(mul_node, ValueType::kRight, MulDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitSub(const SubNode &sub_node) {
  VisitBinaryExpr(sub_node, ValueType::kRight, SubDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitSum(const SumNode &sum_node) {
  VisitBinaryExpr(sum_node, ValueType::kRight, SumDataTypeDeductor());
}

void SimpleSemanticAnalyzer::Impl::VisitNot(const NotNode &not_node) {
  not_node.GetOperand()->Accept(*this);
  const CommonExprAnalysis &operand_analysis =
      GetExprAnalysis(not_node.GetOperand().get());
  NotDataTypeDeductor data_type_deductor;
  VisitUnaryExpr(
      not_node, ValueType::kRight, data_type_deductor, operand_analysis);
}

void SimpleSemanticAnalyzer::Impl::VisitPreDec(const PreDecNode &pre_dec_node) {
  pre_dec_node.GetOperand()->Accept(*this);
  const CommonExprAnalysis &operand_analysis =
      GetExprAnalysis(pre_dec_node.GetOperand().get());
  PreDecDataTypeDeductor data_type_deductor;
  VisitUnaryExpr(pre_dec_node,
                 ValueType::kRight,
                 data_type_deductor,
                 operand_analysis);
}

void SimpleSemanticAnalyzer::Impl::VisitPreInc(const PreIncNode &pre_inc_node) {
  pre_inc_node.GetOperand()->Accept(*this);
  const CommonExprAnalysis &operand_analysis =
      GetExprAnalysis(pre_inc_node.GetOperand().get());
  PreIncDataTypeDeductor data_type_deductor;
  VisitUnaryExpr(pre_inc_node,
                 ValueType::kRight,
                 data_type_deductor,
                 operand_analysis);
}

void SimpleSemanticAnalyzer::Impl::VisitNegative(
    const NegativeNode &negative_node) {
  negative_node.GetOperand()->Accept(*this);
  const CommonExprAnalysis &operand_analysis =
      GetExprAnalysis(negative_node.GetOperand().get());
  NegativeDataTypeDeductor data_type_deductor;
  VisitUnaryExpr(negative_node,
                 ValueType::kRight,
                 data_type_deductor,
                 operand_analysis);
}

void SimpleSemanticAnalyzer::Impl::VisitUnaryExpr(
    const UnaryExprNode &expr_node,
    ValueType expr_value_type,
    UnaryDataTypeDeductor &data_type_deductor,
    const CommonExprAnalysis &operand_analysis) {
  const DataType &operand_data_type = operand_analysis.GetDataType();
  unique_ptr<DataType> expr_data_type =
      data_type_deductor.Deduct(operand_data_type);

  if (!expr_data_type) {
    unique_ptr<SemanticError> error(new UnaryExprWithUnsupportedTypeError(
        GetCurrentFilePath(), expr_node, operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  AddExprAnalysis(expr_node, move(expr_data_type), expr_value_type);
}

void SimpleSemanticAnalyzer::Impl::VisitBinaryExpr(
    const BinaryExprNode &expr_node,
    ValueType expr_value_type,
    const BinaryDataTypeDeductor &data_type_deductor) {
  expr_node.GetLeftOperand()->Accept(*this);
  expr_node.GetRightOperand()->Accept(*this);
  const DataType &left_operand_data_type =
      GetExprDataType(expr_node.GetLeftOperand().get());
  const DataType &right_operand_data_type =
      GetExprDataType(expr_node.GetRightOperand().get());
  unique_ptr<DataType> expr_data_type = data_type_deductor.Deduct(
      left_operand_data_type, right_operand_data_type);

  if (!expr_data_type) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
        GetCurrentFilePath(),
        expr_node,
        left_operand_data_type.Clone(),
        right_operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  AddExprAnalysis(expr_node, move(expr_data_type), expr_value_type);
}

void SimpleSemanticAnalyzer::Impl::VisitBool(const BoolNode &bool_node) {
  const bool value = lit_parser_.ParseBool(bool_node.GetToken().GetValue());
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<Lit> lit(new BoolLit(value));
  AddLitAnalysis(&bool_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitInt(const IntNode &int_node) {
  int32_t value;

  try {
    value = lit_parser_.ParseInt(int_node.GetToken().GetValue());
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(new IntWithOutOfRangeValueError(
        GetCurrentFilePath(), int_node));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<Lit> lit(new IntLit(value));
  AddLitAnalysis(&int_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitLong(const LongNode &long_node) {
  int64_t value;

  try {
    value = lit_parser_.ParseLong(long_node.GetToken().GetValue());
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(new LongWithOutOfRangeValueError(
        GetCurrentFilePath(), long_node));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<Lit> lit(new LongLit(value));
  AddLitAnalysis(&long_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitDouble(const DoubleNode &double_node) {
  double value;

  try {
    value = lit_parser_.ParseDouble(double_node.GetToken().GetValue());
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(new DoubleWithOutOfRangeValueError(
        GetCurrentFilePath(), double_node));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<Lit> lit(new DoubleLit(value));
  AddLitAnalysis(&double_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitChar(const CharNode &char_node) {
  char value;

  try {
    value = lit_parser_.ParseChar(char_node.GetToken().GetValue());
  } catch (const LitParser::EmptyHexValueError&) {
    unique_ptr<SemanticError> error(new CharWithEmptyHexValueError(
        GetCurrentFilePath(), char_node));
    throw SemanticErrorException(move(error));
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(new CharWithOutOfRangeHexValueError(
        GetCurrentFilePath(), char_node));
    throw SemanticErrorException(move(error));
  } catch (const LitParser::MultipleCharsError&) {
    unique_ptr<SemanticError> error(new CharWithMultipleCharsError(
        GetCurrentFilePath(), char_node));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<Lit> lit(new CharLit(value));
  AddLitAnalysis(&char_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitString(const StringNode &string_node) {
  string value;

  try {
    value = lit_parser_.ParseString(string_node.GetToken().GetValue());
  } catch (const LitParser::EmptyHexValueError&) {
    unique_ptr<SemanticError> error(new StringWithEmptyHexValueError(
        GetCurrentFilePath(), string_node));
    throw SemanticErrorException(move(error));
  } catch (const LitParser::OutOfRangeError&) {
    unique_ptr<SemanticError> error(new StringWithOutOfRangeHexValueError(
        GetCurrentFilePath(), string_node));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<Lit> lit(new StringLit(value));
  AddLitAnalysis(&string_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitScope(const ScopeNode &scope_node) {
  for (const unique_ptr<StmtNode> &stmt: scope_node.GetStmts()) {
    stmt->Accept(*this);
  }

  const size_t local_vars_count = GetCurrentScope()->GetIdDefs().size();
  assert(local_vars_count <= numeric_limits<uint32_t>::max());
  unique_ptr<NodeSemanticAnalysis> scope_analysis(
      new ScopeAnalysis(static_cast<uint32_t>(local_vars_count)));
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
        GetCurrentFilePath(), data_type_node, max_dimensions_count));
    throw SemanticErrorException(move(error));
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
  assert(node_analysis_it != node_analyzes_.end());
  return *(node_analysis_it->second);
}

const CommonExprAnalysis &SimpleSemanticAnalyzer::Impl::GetExprAnalysis(
    const ExprNode *expr) {
  return static_cast<const CommonExprAnalysis&>(GetNodeAnalysis(expr));
}

const DataType &SimpleSemanticAnalyzer::Impl::GetExprDataType(
    const ExprNode *expr) {
  return GetExprAnalysis(expr).GetDataType();
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
    ValueType value_type) {
  unique_ptr<NodeSemanticAnalysis> expr_analysis(
      new CommonExprAnalysis(move(data_type), value_type));
  node_analyzes_.insert(make_pair(&expr_node, move(expr_analysis)));
}

void SimpleSemanticAnalyzer::Impl::AddLitAnalysis(
    const LitNode *lit_node,
    unique_ptr<DataType> data_type,
    unique_ptr<Lit> lit) {
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(data_type), ValueType::kRight, move(lit)));
  node_analyzes_.insert(make_pair(lit_node, move(lit_analysis)));
}

void SimpleSemanticAnalyzer::Impl::AddDefAnalysis(
    const DefNode &def_node, unique_ptr<DefAnalysis> def_analysis) {
  node_analyzes_.insert(make_pair(&def_node, move(def_analysis)));
  Scope *current_scope = GetCurrentScope();
  const string &id = def_node.GetNameToken().GetValue();
  const bool is_id_already_exists =
      !current_scope->GetIdDefs().insert(make_pair(id, &def_node)).second;

  if (is_id_already_exists) {
    unique_ptr<SemanticError> error(
        new DuplicateDefError(GetCurrentFilePath(), def_node));
    throw SemanticErrorException(move(error));
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

bool SimpleSemanticAnalyzer::Impl::IsWithinImportProgram() {
  return file_scopes_stack_.size() > 1;
}

bool SimpleSemanticAnalyzer::Impl::IsDataTypeConvertible(
    const DataType &dest, const DataType &src) {
  return dest == src;
}

bool SimpleSemanticAnalyzer::Impl::IsAssigneeContext() {
  return !assignee_contexts_stack_.empty()
      && assignee_contexts_stack_.back()->IsActive();
}

const path &SimpleSemanticAnalyzer::Impl::GetCurrentFilePath() {
  assert(!file_scopes_stack_.empty());
  return file_scopes_stack_.back()->GetFilePath();
}

SimpleSemanticAnalyzer::UnexpectedTokenError::UnexpectedTokenError(
    const Parser::UnexpectedTokenError &error,
    const path &file_path,
    const string &msg)
    : runtime_error(msg),
      error_(error),
      file_path_(file_path) {
}

const Parser::UnexpectedTokenError
&SimpleSemanticAnalyzer::UnexpectedTokenError::GetError() const {
  return error_;
}

const path &SimpleSemanticAnalyzer::UnexpectedTokenError::GetFilePath() const {
  return file_path_;
}

bool operator==(
    const SimpleSemanticAnalyzer::UnexpectedTokenError &lhs,
    const SimpleSemanticAnalyzer::UnexpectedTokenError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.error_ == rhs.error_
      && lhs.file_path_ == rhs.file_path_;
}

ostream &operator<<(
    ostream &stream,
    const SimpleSemanticAnalyzer::UnexpectedTokenError &error) {
  return stream << "error=" << error.error_ << "; file_path="
                << error.file_path_ << "; msg=" << error.what();
}

SimpleSemanticAnalyzer::UnexpectedCharError::UnexpectedCharError(
    const Lexer::UnexpectedCharError &error,
    const path &file_path,
    const string &msg)
    : runtime_error(msg), error_(error), file_path_(file_path) {
}

const Lexer::UnexpectedCharError
&SimpleSemanticAnalyzer::UnexpectedCharError::GetError() const {
  return error_;
}

const path &SimpleSemanticAnalyzer::UnexpectedCharError::GetFilePath() const {
  return file_path_;
}

bool operator==(
    const SimpleSemanticAnalyzer::UnexpectedCharError &lhs,
    const SimpleSemanticAnalyzer::UnexpectedCharError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.file_path_ == rhs.file_path_
      && lhs.error_ == rhs.error_;
}

ostream &operator<<(ostream &stream,
                    const SimpleSemanticAnalyzer::UnexpectedCharError &error) {
  return stream << "error=" << error.error_ << "; file_path="
                << error.file_path_ << "; msg=" << error.what();
}

SimpleSemanticAnalyzer::IOError::IOError(
    const path &file_path, const string &msg)
    : runtime_error(msg), file_path_(file_path) {
}

const path &SimpleSemanticAnalyzer::IOError::GetFilePath() const {
  return file_path_;
}

bool operator==(
    const SimpleSemanticAnalyzer::IOError &lhs,
    const SimpleSemanticAnalyzer::IOError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.file_path_ == rhs.file_path_;
}

ostream &operator<<(
    ostream &stream, const SimpleSemanticAnalyzer::IOError &error) {
  return stream << "file_path=" << error.file_path_ << "; msg=" << error.what();
}
}
}
