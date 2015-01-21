
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
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/arg_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/import_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/import_file_searcher.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/data_type_visitor.h"

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
using boost::format;
using boost::adaptors::reverse;
using boost::filesystem::path;
using boost::filesystem::filesystem_error;
using boost::hash;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::UnexpectedCharError;
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
using real_talk::parser::UnexpectedTokenError;
using real_talk::parser::FuncDefNode;
using real_talk::util::FileNotFoundError;
using real_talk::util::IOError;

namespace real_talk {
namespace semantic {

class SimpleSemanticAnalyzer::Impl: public real_talk::parser::NodeVisitor {
 public:
  Impl(
      std::shared_ptr<real_talk::parser::ProgramNode> program,
      const boost::filesystem::path &absolute_file_path,
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

 private:
  enum class DataTypeId: uint8_t;
  class Scope;
  class FileScope;
  class FuncScope;
  class LoopScope;
  class SemanticErrorException;
  class DataTypeQuery;
  class DataTypesSupportQuery;
  class GetDataTypeId;
  class IsVoidDataType;
  class IsFuncDataType;
  class IsArrayDataType;
  class IsDataTypeSupportedByFuncDef;
  class IsDataTypeSupportedByVarDef;
  class IsDataTypesSupportedByAssign;
  class IsDataTypesSupportedByLess;
  class IsDataTypesSupportedByEqual;
  class IsDataTypeSupportedBySubscriptIndex;
  class IsDataTypeSupportedByArrayAllocElement;
  class IsDataTypeSupportedByArrayAllocSize;
  class IsDataTypeSupportedByNot;

  typedef std::unordered_map<std::string,
                             const real_talk::parser::DefNode*> IdDefs;
  typedef std::unordered_map<boost::filesystem::path,
                             std::shared_ptr<
                               real_talk::parser::ProgramNode>,
                             boost::hash<
                               boost::filesystem::path> > ImportPrograms;

  void VisitBranch(const real_talk::parser::BranchNode &branch_node);
  std::unique_ptr<DataType> VisitArrayAlloc(
      const real_talk::parser::ArrayAllocNode &array_alloc_node);
  const DataType &VisitVarDef(
      const real_talk::parser::VarDefNode &var_def_node);
  void VisitFuncDef(const real_talk::parser::FuncDefNode &func_def_node,
                    bool &is_func_native,
                    std::unique_ptr<DataType> &return_data_type);
  void VisitReturn(const real_talk::parser::ReturnNode &return_node);
  std::unique_ptr<DataType> CreateDataType(
      const real_talk::parser::DataTypeNode &data_type_node);
  const CommonExprAnalysis &GetExprAnalysis(const real_talk::parser::ExprNode *expr);
  const DataType &GetExprDataType(const real_talk::parser::ExprNode *expr);
  const DataType &GetFuncReturnDataType(
      const real_talk::parser::FuncDefNode *func_def);
  bool IsCurrentScopeGlobal();
  bool IsWithinImportProgram();
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
  std::vector<FuncScope*> func_scopes_stack_;
  std::vector<LoopScope*> loop_scopes_stack_;
  std::unique_ptr<DataType> current_data_type_;
  size_t non_import_stmts_count_;
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
  std::unique_ptr<SemanticError> error_;
};

class SimpleSemanticAnalyzer::Impl::Scope {
 public:
  explicit Scope(vector<Scope*> &scopes_stack)
      : scopes_stack_(scopes_stack) {
    scopes_stack_.push_back(this);
  }

  ~Scope() {
    scopes_stack_.pop_back();
  }

  const IdDefs &GetIdDefs() const {
    return id_defs_;
  }

  IdDefs &GetIdDefs() {
    return id_defs_;
  }

 private:
  IdDefs id_defs_;
  std::vector<Scope*> &scopes_stack_;
};

class SimpleSemanticAnalyzer::Impl::FileScope {
 public:
  FileScope(
      vector<FileScope*> &file_scopes_stack, const path &file_path)
      : file_scopes_stack_(file_scopes_stack), file_path_(file_path) {
    file_scopes_stack_.push_back(this);
  }

  ~FileScope() {
    file_scopes_stack_.pop_back();
  }

  const path &GetFilePath() const {
    return file_path_;
  }

 private:
  std::vector<FileScope*> &file_scopes_stack_;
  const boost::filesystem::path &file_path_;
};

class SimpleSemanticAnalyzer::Impl::FuncScope {
 public:
  FuncScope(
      vector<FuncScope*> &func_scopes_stack,
      const FuncDefNode &func_def)
      : func_scopes_stack_(func_scopes_stack),
        func_def_(func_def),
        has_return_value_(false) {
    func_scopes_stack_.push_back(this);
  }

  ~FuncScope() {
    func_scopes_stack_.pop_back();
  }

  bool HasReturnValue() const {
    return has_return_value_;
  }

  void SetHasReturnValue(bool has_return_value) {
    has_return_value_ = has_return_value;
  }

  const FuncDefNode &GetFuncDef() const {
    return func_def_;
  }

 private:
  std::vector<FuncScope*> &func_scopes_stack_;
  const FuncDefNode &func_def_;
  bool has_return_value_;
};

class SimpleSemanticAnalyzer::Impl::LoopScope {
 public:
  explicit LoopScope(vector<LoopScope*> &scopes_stack)
      : scopes_stack_(scopes_stack) {
    scopes_stack_.push_back(this);
  }

  ~LoopScope() {
    scopes_stack_.pop_back();
  }

 private:
  std::vector<LoopScope*> &scopes_stack_;
};

class SimpleSemanticAnalyzer::Impl::DataTypeQuery: protected DataTypeVisitor {
 public:
  bool Check(const DataType &data_type) {
    data_type.Accept(*this);
    return result;
  }

 protected:
  DataTypeQuery(): result(false) {}
  virtual ~DataTypeQuery() {}
  virtual void VisitArray(const ArrayDataType&) override {}
  virtual void VisitFunc(const FuncDataType&) override {}
  virtual void VisitBool(const BoolDataType&) override {}
  virtual void VisitInt(const IntDataType&) override {}
  virtual void VisitLong(const LongDataType&) override {}
  virtual void VisitDouble(const DoubleDataType&) override {}
  virtual void VisitChar(const CharDataType&) override {}
  virtual void VisitString(const StringDataType&) override {}
  virtual void VisitVoid(const VoidDataType&) override {}

  bool result;
};

class SimpleSemanticAnalyzer::Impl::IsVoidDataType: public DataTypeQuery {
 protected:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {
    result = true;
  }
};

class SimpleSemanticAnalyzer::Impl::IsFuncDataType: public DataTypeQuery {
 protected:
  virtual void VisitFunc(const FuncDataType&) override {
    result = true;
  }
};

class SimpleSemanticAnalyzer::Impl::IsArrayDataType: public DataTypeQuery {
 protected:
  virtual void VisitArray(const ArrayDataType&) override {
    result = true;
  }
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByFuncDef
    : public DataTypeQuery {
 protected:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result = !IsVoidDataType().Check(data_type.GetElementDataType());
  }

  virtual void VisitBool(const BoolDataType&) override {result = true;}
  virtual void VisitInt(const IntDataType&) override {result = true;}
  virtual void VisitLong(const LongDataType&) override {result = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result = true;}
  virtual void VisitChar(const CharDataType&) override {result = true;}
  virtual void VisitString(const StringDataType&) override {result = true;}
  virtual void VisitVoid(const VoidDataType&) override {result = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByVarDef
    : public DataTypeQuery {
 protected:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    result = !IsVoidDataType().Check(data_type.GetElementDataType());
  }

  virtual void VisitBool(const BoolDataType&) override {result = true;}
  virtual void VisitInt(const IntDataType&) override {result = true;}
  virtual void VisitLong(const LongDataType&) override {result = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result = true;}
  virtual void VisitChar(const CharDataType&) override {result = true;}
  virtual void VisitString(const StringDataType&) override {result = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByArrayAllocSize
    : public DataTypeQuery {
 protected:
  virtual void VisitInt(const IntDataType&) override {result = true;}
  virtual void VisitLong(const LongDataType&) override {result = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedBySubscriptIndex
    : public DataTypeQuery {
 protected:
  virtual void VisitInt(const IntDataType&) override {result = true;}
  virtual void VisitLong(const LongDataType&) override {result = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByArrayAllocElement
    : public DataTypeQuery {
 protected:
  virtual void VisitBool(const BoolDataType&) override {result = true;}
  virtual void VisitInt(const IntDataType&) override {result = true;}
  virtual void VisitLong(const LongDataType&) override {result = true;}
  virtual void VisitDouble(const DoubleDataType&) override {result = true;}
  virtual void VisitChar(const CharDataType&) override {result = true;}
  virtual void VisitString(const StringDataType&) override {result = true;}
};

class SimpleSemanticAnalyzer::Impl::IsDataTypeSupportedByNot
    : public DataTypeQuery {
 protected:
  virtual void VisitBool(const BoolDataType&) override {result = true;}
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

class SimpleSemanticAnalyzer::Impl::GetDataTypeId: private DataTypeVisitor {
 public:
  DataTypeId Get(const DataType &data_type) {
    data_type.Accept(*this);
    return id;
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    id = DataTypeId::kArray;
  }

  virtual void VisitFunc(const FuncDataType&) override {
    id = DataTypeId::kFunc;
  }

  virtual void VisitBool(const BoolDataType&) override {
    id = DataTypeId::kBool;
  }

  virtual void VisitInt(const IntDataType&) override {
    id = DataTypeId::kInt;
  }

  virtual void VisitLong(const LongDataType&) override {
    id = DataTypeId::kLong;
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    id = DataTypeId::kDouble;
  }

  virtual void VisitChar(const CharDataType&) override {
    id = DataTypeId::kChar;
  }

  virtual void VisitString(const StringDataType&) override {
    id = DataTypeId::kString;
  }

  virtual void VisitVoid(const VoidDataType&) override {
    id = DataTypeId::kVoid;
  }

  DataTypeId id;
};

class SimpleSemanticAnalyzer::Impl::DataTypesSupportQuery {
 public:
  virtual ~DataTypesSupportQuery() {}

  bool Check(const DataType &lhs, const DataType &rhs) const {
    const SupportedDataTypes &supported_data_types = GetSupportedDataTypes();
    DataTypeId lhs_id = GetDataTypeId().Get(lhs);
    DataTypeId rhs_id = GetDataTypeId().Get(rhs);
    SupportedDataTypes::const_iterator support_checker_it =
        supported_data_types.find(make_pair(lhs_id, rhs_id));

    if (support_checker_it == supported_data_types.end()) {
      return false;
    }

    const DataTypeSupportChecker &support_checker =
        *(support_checker_it->second);
    return support_checker(lhs, rhs);
  }

 protected:
  typedef bool (*DataTypeSupportChecker)(const DataType&, const DataType&);
  typedef pair<DataTypeId, DataTypeId> SupportedDataTypesKey;
  typedef unordered_map<SupportedDataTypesKey,
                        DataTypeSupportChecker,
                        hash<SupportedDataTypesKey> > SupportedDataTypes;

  static bool TrueChecker(const DataType&, const DataType&) {
    return true;
  }

 private:
  virtual const SupportedDataTypes &GetSupportedDataTypes() const = 0;
};

class SimpleSemanticAnalyzer::Impl::IsDataTypesSupportedByAssign
    : public DataTypesSupportQuery {
 private:
  virtual const SupportedDataTypes &GetSupportedDataTypes() const override {
    static const SupportedDataTypes &supported_data_types =
        *new SupportedDataTypes({
            {make_pair(DataTypeId::kInt, DataTypeId::kInt),
                  &IsDataTypesSupportedByAssign::TrueChecker},
            {make_pair(DataTypeId::kLong, DataTypeId::kLong),
                  &IsDataTypesSupportedByAssign::TrueChecker},
            {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
                  &IsDataTypesSupportedByAssign::TrueChecker},
            {make_pair(DataTypeId::kBool, DataTypeId::kBool),
                  &IsDataTypesSupportedByAssign::TrueChecker},
            {make_pair(DataTypeId::kChar, DataTypeId::kChar),
                  &IsDataTypesSupportedByAssign::TrueChecker},
            {make_pair(DataTypeId::kString, DataTypeId::kString),
                  &IsDataTypesSupportedByAssign::TrueChecker},
            {make_pair(DataTypeId::kArray, DataTypeId::kArray),
                  &IsDataTypesSupportedByAssign::ArrayChecker}
        });
    return supported_data_types;
  }

  static bool ArrayChecker(const DataType &lhs, const DataType &rhs) {
    return IsDataTypesSupportedByAssign().Check(
        static_cast<const ArrayDataType&>(lhs).GetElementDataType(),
        static_cast<const ArrayDataType&>(rhs).GetElementDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::IsDataTypesSupportedByEqual
    : public DataTypesSupportQuery {
 private:
  virtual const SupportedDataTypes &GetSupportedDataTypes() const override {
    static const SupportedDataTypes &supported_data_types =
        *new SupportedDataTypes({
            {make_pair(DataTypeId::kInt, DataTypeId::kInt),
                  &IsDataTypesSupportedByEqual::TrueChecker},
            {make_pair(DataTypeId::kLong, DataTypeId::kLong),
                  &IsDataTypesSupportedByEqual::TrueChecker},
            {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
                  &IsDataTypesSupportedByEqual::TrueChecker},
            {make_pair(DataTypeId::kBool, DataTypeId::kBool),
                  &IsDataTypesSupportedByEqual::TrueChecker},
            {make_pair(DataTypeId::kChar, DataTypeId::kChar),
                  &IsDataTypesSupportedByEqual::TrueChecker},
            {make_pair(DataTypeId::kString, DataTypeId::kString),
                  &IsDataTypesSupportedByEqual::TrueChecker},
            {make_pair(DataTypeId::kArray, DataTypeId::kArray),
                  &IsDataTypesSupportedByEqual::ArrayChecker}
        });
    return supported_data_types;
  }

  static bool ArrayChecker(const DataType &lhs, const DataType &rhs) {
    return IsDataTypesSupportedByEqual().Check(
        static_cast<const ArrayDataType&>(lhs).GetElementDataType(),
        static_cast<const ArrayDataType&>(rhs).GetElementDataType());
  }
};

class SimpleSemanticAnalyzer::Impl::IsDataTypesSupportedByLess
    : public DataTypesSupportQuery {
 private:
  const SupportedDataTypes &GetSupportedDataTypes() const {
    static const SupportedDataTypes &supported_data_types =
        *new SupportedDataTypes({
            {make_pair(DataTypeId::kInt, DataTypeId::kInt),
                  &IsDataTypesSupportedByLess::TrueChecker},
            {make_pair(DataTypeId::kLong, DataTypeId::kLong),
                  &IsDataTypesSupportedByLess::TrueChecker},
            {make_pair(DataTypeId::kDouble, DataTypeId::kDouble),
                  &IsDataTypesSupportedByLess::TrueChecker},
            {make_pair(DataTypeId::kChar, DataTypeId::kChar),
                  &IsDataTypesSupportedByLess::TrueChecker}
        });
    return supported_data_types;
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
  import_programs_.insert(make_pair(file_path_, program_));

  {
    Scope scope(scopes_stack_);
    FileScope file_scope(file_scopes_stack_, file_path_);
    program_->Accept(*this);
  }

  assert(scopes_stack_.empty());
  assert(file_scopes_stack_.empty());
  assert(func_scopes_stack_.empty());
  assert(loop_scopes_stack_.empty());
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

  VisitReturn(return_node);
  return_node.GetValue()->Accept(*this);
  FuncScope &current_func_scope = *(func_scopes_stack_.back());
  current_func_scope.SetHasReturnValue(true);
  const DataType &func_def_return_data_type =
      GetFuncReturnDataType(&current_func_scope.GetFuncDef());
  const DataType &value_data_type =
      GetExprDataType(return_node.GetValue().get());
  const bool is_data_types_compatible = IsDataTypesSupportedByAssign().Check(
      func_def_return_data_type, value_data_type);

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

  VisitReturn(return_node);
  FuncScope &current_func_scope = *(func_scopes_stack_.back());
  const DataType &func_def_return_data_type =
      GetFuncReturnDataType(&current_func_scope.GetFuncDef());

  if (func_def_return_data_type != VoidDataType()) {
    unique_ptr<SemanticError> error(
        new ReturnWithoutValueError(GetCurrentFilePath(), return_node));
    throw SemanticErrorException(move(error));
  }
}

void SimpleSemanticAnalyzer::Impl::VisitReturn(const ReturnNode &return_node) {
  ++non_import_stmts_count_;

  if (func_scopes_stack_.empty()) {
    unique_ptr<SemanticError> error(
        new ReturnNotWithinFuncError(GetCurrentFilePath(), return_node));
    throw SemanticErrorException(move(error));
  }
}

void SimpleSemanticAnalyzer::Impl::VisitBreak(const BreakNode &break_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  ++non_import_stmts_count_;

  if (loop_scopes_stack_.empty()) {
    unique_ptr<SemanticError> error(
        new BreakNotWithinLoopError(GetCurrentFilePath(), break_node));
    throw SemanticErrorException(move(error));
  }
}

void SimpleSemanticAnalyzer::Impl::VisitContinue(
    const ContinueNode &continue_node) {
  if (IsWithinImportProgram()) {
    return;
  }

  ++non_import_stmts_count_;

  if (loop_scopes_stack_.empty()) {
    unique_ptr<SemanticError> error(
        new ContinueNotWithinLoopError(GetCurrentFilePath(), continue_node));
    throw SemanticErrorException(move(error));
  }
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

  if (!IsDataTypesSupportedByAssign().Check(var_data_type, value_data_type)) {
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
  DataStorage data_storage =
      IsCurrentScopeGlobal() ? DataStorage::kGlobal : DataStorage::kLocal;
  unique_ptr<DefAnalysis> def_analysis(
      new VarDefAnalysis(move(data_type_ptr), data_storage));
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

  FuncScope func_scope(func_scopes_stack_, func_def_node);
  Scope scope(scopes_stack_);

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    arg_def_node->Accept(*this);
  }

  for (const unique_ptr<StmtNode> &stmt: func_def_node.GetBody()->GetStmts()) {
    stmt->Accept(*this);
  }

  if (*return_data_type != VoidDataType() && !func_scope.HasReturnValue()) {
    unique_ptr<SemanticError> error(new FuncDefWithoutReturnValueError(
        GetCurrentFilePath(), func_def_node));
    throw SemanticErrorException(move(error));
  }
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

  FuncScope func_scope(func_scopes_stack_, func_def_node);
  Scope scope(scopes_stack_);

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

  return_data_type = CreateDataType(*(func_def_node.GetDataType()));
  vector< unique_ptr<DataType> > arg_data_types;

  for (const unique_ptr<ArgDefNode> &arg_def_node: func_def_node.GetArgs()) {
    unique_ptr<DataType> arg_data_type =
        CreateDataType(*(arg_def_node->GetDataType()));
    arg_data_types.push_back(move(arg_data_type));
  }

  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(return_data_type->Clone(), move(arg_data_types)));
  unordered_set<Token> modifier_tokens;

  for (const TokenInfo &modifier_token: func_def_node.GetModifierTokens()) {
    const bool is_duplicate_modifier =
        !modifier_tokens.insert(modifier_token.GetId()).second;
    assert(!is_duplicate_modifier);
  }

  is_func_native = modifier_tokens.count(Token::kNative) != 0;
  unique_ptr<DefAnalysis> def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_native));
  AddDefAnalysis(func_def_node, move(def_analysis));

  if (IsWithinImportProgram()) {
    return;
  }

  if (!IsCurrentScopeGlobal()) {
    unique_ptr<SemanticError> error(
        new FuncDefWithinNonGlobalScope(GetCurrentFilePath(), func_def_node));
    throw SemanticErrorException(move(error));
  }

  const bool is_return_data_type_supported =
      IsDataTypeSupportedByFuncDef().Check(*return_data_type);

  if (!is_return_data_type_supported) {
    unique_ptr<SemanticError> error(new DefWithUnsupportedTypeError(
        GetCurrentFilePath(), func_def_node, return_data_type->Clone()));
    throw SemanticErrorException(move(error));
  }
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

  LoopScope loop_scope(loop_scopes_stack_);
  Scope scope(scopes_stack_);

  for (const unique_ptr<StmtNode> &stmt: loop_node.GetBody()->GetStmts()) {
    stmt->Accept(*this);
  }
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
  const vector< unique_ptr<StmtNode> > &else_stmts =
      if_else_if_else_node.GetElseBody()->GetStmts();
  Scope scope(scopes_stack_);

  for (const unique_ptr<StmtNode> &else_stmt: else_stmts) {
    else_stmt->Accept(*this);
  }
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

  const vector< unique_ptr<StmtNode> > &if_stmts =
      branch_node.GetIf()->GetBody()->GetStmts();

  {
    Scope scope(scopes_stack_);

    for (const unique_ptr<StmtNode> &stmt: if_stmts) {
      stmt->Accept(*this);
    }
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

    const vector< unique_ptr<StmtNode> > &else_if_stmts =
        else_if->GetIf()->GetBody()->GetStmts();
    Scope scope(scopes_stack_);

    for (const unique_ptr<StmtNode> &stmt: else_if_stmts) {
      stmt->Accept(*this);
    }
  }
}

void SimpleSemanticAnalyzer::Impl::VisitImport(const ImportNode &import_node) {
  import_node.GetFilePath()->Accept(*this);
  SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
      node_analyzes_.find(import_node.GetFilePath().get());
  assert(node_analysis_it != node_analyzes_.end());
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(*(node_analysis_it->second));
  const StringLit &lit = static_cast<const StringLit&>(lit_analysis.GetLit());
  const path relative_file_path(lit.GetValue());
  path absolute_file_path;

  try {
    absolute_file_path = import_file_searcher_.Search(relative_file_path);
  } catch (const FileNotFoundError&) {
    unique_ptr<SemanticError> error(new ImportWithNotExistingFileError(
        GetCurrentFilePath(), import_node, relative_file_path));
    throw SemanticErrorException(move(error));
  } catch (const IOError&) {
    unique_ptr<SemanticError> error(new ImportWithIOError(
        GetCurrentFilePath(), import_node, relative_file_path));
    throw SemanticErrorException(move(error));
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
    } catch (const IOError&) {
      unique_ptr<SemanticError> error(new ImportWithIOError(
          GetCurrentFilePath(), import_node, absolute_file_path));
      throw SemanticErrorException(move(error));
    } catch (const UnexpectedTokenError &e) {
      unique_ptr<SemanticError> error(new ImportWithUnexpectedTokenError(
          GetCurrentFilePath(), import_node, absolute_file_path, e.GetToken()));
      throw SemanticErrorException(move(error));
    } catch (const UnexpectedCharError &e) {
      unique_ptr<SemanticError> error(new ImportWithUnexpectedCharError(
          GetCurrentFilePath(),
          import_node,
          absolute_file_path,
          e.GetChar(),
          e.GetLineNumber(),
          e.GetColumnNumber()));
      throw SemanticErrorException(move(error));
    }

    import_programs_.insert(make_pair(absolute_file_path, program));
    FileScope file_scope(file_scopes_stack_, absolute_file_path);
    program->Accept(*this);
  }

  unique_ptr<NodeSemanticAnalysis> import_analysis(new ImportAnalysis(program));
  node_analyzes_.insert(make_pair(&import_node, move(import_analysis)));

  if (non_import_stmts_count_ != 0) {
    unique_ptr<SemanticError> error(
        new ImportIsNotFirstStmtError(GetCurrentFilePath(), import_node));
    throw SemanticErrorException(move(error));
  }
}

void SimpleSemanticAnalyzer::Impl::VisitAnd(const AndNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode &alloc_node) {
  unique_ptr<DataType> element_data_type = VisitArrayAlloc(alloc_node);
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
  unique_ptr<DataType> array_data_type = VisitArrayAlloc(alloc_node);
  assert(!alloc_node.GetSizes().empty());

  for (const unique_ptr<UnboundedArraySizeNode> &size: alloc_node.GetSizes()) {
    array_data_type.reset(new ArrayDataType(move(array_data_type)));
    (void) size;
  }

  const vector< unique_ptr<ExprNode> > &values = alloc_node.GetValues();
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
    const bool is_data_types_compatible = IsDataTypesSupportedByAssign().Check(
        expected_value_data_type, actual_value_data_type);

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
    const ArrayAllocNode &alloc_node) {
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

void SimpleSemanticAnalyzer::Impl::VisitAssign(const AssignNode &assign_node) {
  assign_node.GetLeftOperand()->Accept(*this);
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
  const bool is_data_types_supported = IsDataTypesSupportedByAssign().Check(
      left_operand_data_type, right_operand_data_type);

  if (!is_data_types_supported) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
        GetCurrentFilePath(),
        assign_node,
        left_operand_data_type.Clone(),
        right_operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  AddExprAnalysis(
      assign_node, left_operand_data_type.Clone(), ValueType::kLeft);
}

void SimpleSemanticAnalyzer::Impl::VisitDiv(const DivNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitCall(const CallNode &call_node) {
  call_node.GetOperand()->Accept(*this);
  const DataType &operand_data_type =
      GetExprDataType(call_node.GetOperand().get());

  if (!IsFuncDataType().Check(operand_data_type)) {
    unique_ptr<SemanticError> error(
        new CallWithNonFuncError(GetCurrentFilePath(), call_node));
    throw SemanticErrorException(move(error));
  }

  const FuncDataType &func_data_type =
      static_cast<const FuncDataType&>(operand_data_type);
  AddExprAnalysis(
      call_node, func_data_type.GetReturnDataType().Clone(), ValueType::kRight);
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
    const bool is_data_types_compatible = IsDataTypesSupportedByAssign().Check(
        arg_def_data_type, call_arg_data_type);

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

void SimpleSemanticAnalyzer::Impl::VisitGreater(const GreaterNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitGreaterOrEqual(
    const GreaterOrEqualNode&) {
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
  } catch (...) {
    // TODO
    assert(false);
  }

  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<Lit> lit(new IntLit(value));
  AddLitAnalysis(&int_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitLong(const LongNode &long_node) {
  int64_t value;

  try {
    value = lit_parser_.ParseLong(long_node.GetToken().GetValue());
  } catch (...) {
    // TODO
    assert(false);
  }

  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<Lit> lit(new LongLit(value));
  AddLitAnalysis(&long_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitDouble(const DoubleNode &double_node) {
  double value;

  try {
    value = lit_parser_.ParseDouble(double_node.GetToken().GetValue());
  } catch (...) {
    // TODO
    assert(false);
  }

  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<Lit> lit(new DoubleLit(value));
  AddLitAnalysis(&double_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitChar(const CharNode &char_node) {
  char value;

  try {
    value = lit_parser_.ParseChar(char_node.GetToken().GetValue());
  } catch (...) {
    // TODO
    assert(false);
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
  } catch (const LitParser::OutOfRange&) {
    unique_ptr<SemanticError> error(new StringWithOutOfRangeHexValueError(
        GetCurrentFilePath(), string_node));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<Lit> lit(new StringLit(value));
  AddLitAnalysis(&string_node, move(data_type), move(lit));
}

void SimpleSemanticAnalyzer::Impl::VisitEqual(const EqualNode &equal_node) {
  equal_node.GetLeftOperand()->Accept(*this);
  equal_node.GetRightOperand()->Accept(*this);
  const DataType &left_operand_data_type =
      GetExprDataType(equal_node.GetLeftOperand().get());
  const DataType &right_operand_data_type =
      GetExprDataType(equal_node.GetRightOperand().get());
  const bool is_data_types_supported = IsDataTypesSupportedByEqual().Check(
      left_operand_data_type, right_operand_data_type);

  if (!is_data_types_supported) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
        GetCurrentFilePath(),
        equal_node,
        left_operand_data_type.Clone(),
        right_operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> equal_data_type(new BoolDataType());
  AddExprAnalysis(equal_node, move(equal_data_type), ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitLess(const LessNode &less_node) {
  less_node.GetLeftOperand()->Accept(*this);
  less_node.GetRightOperand()->Accept(*this);
  const DataType &left_operand_data_type =
      GetExprDataType(less_node.GetLeftOperand().get());
  const DataType &right_operand_data_type =
      GetExprDataType(less_node.GetRightOperand().get());
  const bool is_data_types_supported = IsDataTypesSupportedByLess().Check(
      left_operand_data_type, right_operand_data_type);

  if (!is_data_types_supported) {
    unique_ptr<SemanticError> error(new BinaryExprWithUnsupportedTypesError(
        GetCurrentFilePath(),
        less_node,
        left_operand_data_type.Clone(),
        right_operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> less_data_type(new BoolDataType());
  AddExprAnalysis(less_node, move(less_data_type), ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitLessOrEqual(const LessOrEqualNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitMul(const MulNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitNegative(const NegativeNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitNotEqual(const NotEqualNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitNot(const NotNode &not_node) {
  not_node.GetOperand()->Accept(*this);
  const DataType &operand_data_type =
      GetExprDataType(not_node.GetOperand().get());

  if (!IsDataTypeSupportedByNot().Check(operand_data_type)) {
    unique_ptr<SemanticError> error(new UnaryExprWithUnsupportedTypeError(
        GetCurrentFilePath(), not_node, operand_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  unique_ptr<DataType> not_data_type(new BoolDataType());
  AddExprAnalysis(not_node, move(not_data_type), ValueType::kRight);
}

void SimpleSemanticAnalyzer::Impl::VisitOr(const OrNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitPreDec(const PreDecNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitPreInc(const PreIncNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitSubscript(
    const SubscriptNode &subscript) {
  subscript.GetOperand()->Accept(*this);
  const CommonExprAnalysis &operand_expr_analysis =
      GetExprAnalysis(subscript.GetOperand().get());
  const DataType &operand_data_type = operand_expr_analysis.GetDataType();

  if (!IsArrayDataType().Check(operand_data_type)) {
    unique_ptr<SemanticError> error(
        new SubscriptWithNonArrayError(GetCurrentFilePath(), subscript));
    throw SemanticErrorException(move(error));
  }

  subscript.GetIndex()->Accept(*this);
  const DataType &index_data_type = GetExprDataType(subscript.GetIndex().get());

  if (!IsDataTypeSupportedBySubscriptIndex().Check(index_data_type)) {
    unique_ptr<SemanticError> error(new SubscriptWithUnsupportedIndexTypeError(
        GetCurrentFilePath(),
        subscript,
        index_data_type.Clone()));
    throw SemanticErrorException(move(error));
  }

  const ArrayDataType &array_data_type =
      static_cast<const ArrayDataType&>(operand_data_type);
  AddExprAnalysis(subscript,
                  array_data_type.GetElementDataType().Clone(),
                  operand_expr_analysis.GetValueType());
}

void SimpleSemanticAnalyzer::Impl::VisitSub(const SubNode&) {
}

void SimpleSemanticAnalyzer::Impl::VisitSum(const SumNode&) {
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

  SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
      node_analyzes_.find(def_node);
  assert(node_analysis_it != node_analyzes_.end());
  const DefAnalysis &def_analysis =
      static_cast<const DefAnalysis&>(*(node_analysis_it->second));
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      def_analysis.GetDataType().Clone(),
      def_analysis.GetValueType(),
      def_node));
  node_analyzes_.insert(make_pair(&id_node, move(id_analysis)));
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

unique_ptr<DataType> SimpleSemanticAnalyzer::Impl::CreateDataType(
    const DataTypeNode &data_type_node) {
  assert(!current_data_type_);
  data_type_node.Accept(*this);
  assert(current_data_type_);
  return move(current_data_type_);
}

const CommonExprAnalysis &SimpleSemanticAnalyzer::Impl::GetExprAnalysis(
    const ExprNode *expr) {
  SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
      node_analyzes_.find(expr);
  assert(node_analysis_it != node_analyzes_.end());
  return static_cast<const CommonExprAnalysis&>(*(node_analysis_it->second));
}

const DataType &SimpleSemanticAnalyzer::Impl::GetExprDataType(
    const ExprNode *expr) {
  return GetExprAnalysis(expr).GetDataType();
}

const DataType &SimpleSemanticAnalyzer::Impl::GetFuncReturnDataType(
    const FuncDefNode *func_def) {
  SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
      node_analyzes_.find(func_def);
  assert(node_analysis_it != node_analyzes_.end());
  const FuncDefAnalysis &func_def_analysis =
      static_cast<const FuncDefAnalysis&>(*(node_analysis_it->second));
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
  assert(!scopes_stack_.empty());
  Scope &current_scope = *(scopes_stack_.back());
  const string &id = def_node.GetNameToken().GetValue();
  const bool is_id_already_exists =
      !current_scope.GetIdDefs().insert(make_pair(id, &def_node)).second;

  if (is_id_already_exists) {
    unique_ptr<SemanticError> error(
        new DuplicateDefError(GetCurrentFilePath(), def_node));
    throw SemanticErrorException(move(error));
  }
}

bool SimpleSemanticAnalyzer::Impl::IsCurrentScopeGlobal() {
  return scopes_stack_.size() == 1;
}

bool SimpleSemanticAnalyzer::Impl::IsWithinImportProgram() {
  return file_scopes_stack_.size() > 1;
}

bool SimpleSemanticAnalyzer::Impl::IsDataTypeConvertible(
    const DataType &dest, const DataType &src) {
  return dest == src;
}

const path &SimpleSemanticAnalyzer::Impl::GetCurrentFilePath() {
  assert(!file_scopes_stack_.empty());
  return file_scopes_stack_.back()->GetFilePath();
}
}
}
