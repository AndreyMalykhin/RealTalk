
#include <boost/numeric/conversion/cast.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <unordered_map>
#include <cassert>
#include <string>
#include <vector>
#include <limits>
#include "real_talk/parser/node_visitor.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/parser/if_else_if_else_node.h"
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/parser/and_node.h"
#include "real_talk/parser/or_node.h"
#include "real_talk/parser/mul_node.h"
#include "real_talk/parser/div_node.h"
#include "real_talk/parser/sum_node.h"
#include "real_talk/parser/sub_node.h"
#include "real_talk/parser/equal_node.h"
#include "real_talk/parser/not_equal_node.h"
#include "real_talk/parser/greater_node.h"
#include "real_talk/parser/greater_or_equal_node.h"
#include "real_talk/parser/less_node.h"
#include "real_talk/parser/less_or_equal_node.h"
#include "real_talk/parser/not_node.h"
#include "real_talk/parser/pre_dec_node.h"
#include "real_talk/parser/pre_inc_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/semantic/data_type_visitor.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/global_var_def_analysis.h"
#include "real_talk/semantic/local_var_def_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/scope_analysis.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/return_analysis.h"
#include "real_talk/semantic/subscript_analysis.h"
#include "real_talk/semantic/def_analysis_visitor.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/char_lit.h"
#include "real_talk/semantic/string_lit.h"
#include "real_talk/semantic/double_lit.h"
#include "real_talk/code/simple_code_generator.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/cast_cmd_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/data_type_size.h"

using std::unique_ptr;
using std::string;
using std::vector;
using std::numeric_limits;
using std::unordered_map;
using boost::numeric_cast;
using boost::adaptors::reverse;
using real_talk::parser::ProgramNode;
using real_talk::parser::NodeVisitor;
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
using real_talk::parser::StringNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::SubNode;
using real_talk::parser::SumNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::IdNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::StmtNode;
using real_talk::parser::SumNode;
using real_talk::parser::FuncDefNode;
using real_talk::parser::DefNode;
using real_talk::parser::VarDefNode;
using real_talk::parser::Node;
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::ElseIfNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::parser::IfNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::ExprNode;
using real_talk::parser::CallNode;
using real_talk::parser::MulNode;
using real_talk::parser::BinaryExprNode;
using real_talk::parser::UnaryExprNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::LocalVarDefAnalysis;
using real_talk::semantic::GlobalVarDefAnalysis;
using real_talk::semantic::LitAnalysis;
using real_talk::semantic::CommonExprAnalysis;
using real_talk::semantic::ScopeAnalysis;
using real_talk::semantic::ControlFlowTransferAnalysis;
using real_talk::semantic::FuncDefAnalysis;
using real_talk::semantic::ExprAnalysis;
using real_talk::semantic::IdAnalysis;
using real_talk::semantic::DefAnalysis;
using real_talk::semantic::ReturnAnalysis;
using real_talk::semantic::SubscriptAnalysis;
using real_talk::semantic::DefAnalysisVisitor;
using real_talk::semantic::DataTypeVisitor;
using real_talk::semantic::DataType;
using real_talk::semantic::ArrayDataType;
using real_talk::semantic::FuncDataType;
using real_talk::semantic::BoolDataType;
using real_talk::semantic::IntDataType;
using real_talk::semantic::LongDataType;
using real_talk::semantic::DoubleDataType;
using real_talk::semantic::CharDataType;
using real_talk::semantic::StringDataType;
using real_talk::semantic::VoidDataType;
using real_talk::semantic::IntLit;
using real_talk::semantic::LongLit;
using real_talk::semantic::BoolLit;
using real_talk::semantic::CharLit;
using real_talk::semantic::StringLit;
using real_talk::semantic::DoubleLit;

namespace real_talk {
namespace code {

class SimpleCodeGenerator::Impl: private NodeVisitor {
 public:
  explicit Impl(const CastCmdGenerator &cast_cmd_generator);
  unique_ptr<Module> Generate(const ProgramNode &program,
                              const SemanticAnalysis &semantic_analysis,
                              uint32_t version);

 private:
  class StmtGrouper;
  class DataTypeSizeResolver;
  class Scope;
  class IdNodeProcessor;
  template<typename TCreateLocalVarCmdGenerator,
           typename TCreateGlobalVarCmdGenerator> class VarDefNodeProcessor;
  class ArrayCmdGenerator;
  class CreateGlobalVarCmdGenerator;
  class CreateLocalVarCmdGenerator;
  class CreateAndInitGlobalVarCmdGenerator;
  class CreateAndInitGlobalArrayVarCmdGenerator;
  class CreateAndInitLocalVarCmdGenerator;
  class CreateAndInitLocalArrayVarCmdGenerator;
  class CreateArrayCmdGenerator;
  class CreateAndInitArrayCmdGenerator;
  class DestroyLocalVarCmdGenerator;
  class DestroyLocalArrayVarCmdGenerator;
  class LoadGlobalVarValueCmdGenerator;
  class LoadGlobalArrayVarValueCmdGenerator;
  class LoadLocalVarValueCmdGenerator;
  class LoadLocalArrayVarValueCmdGenerator;
  class LoadElementValueCmdGenerator;
  class LoadArrayElementValueCmdGenerator;
  class LoadElementAddressCmdGenerator;
  class LoadArrayElementAddressCmdGenerator;
  class UnloadCmdGenerator;
  class UnloadArrayCmdGenerator;
  class StoreCmdGenerator;
  class StoreArrayCmdGenerator;
  class CallCmdGenerator;
  class ExprCmdGenerator;
  class AndCmdGenerator;
  class OrCmdGenerator;
  class MulCmdGenerator;
  class DivCmdGenerator;
  class SumCmdGenerator;
  class SubCmdGenerator;
  class EqualCmdGenerator;
  class EqualArrayCmdGenerator;
  class NotEqualCmdGenerator;
  class NotEqualArrayCmdGenerator;
  class GreaterCmdGenerator;
  class GreaterOrEqualCmdGenerator;
  class LessCmdGenerator;
  class LessOrEqualCmdGenerator;
  class LogicalNegateCmdGenerator;
  class ArithmeticNegateCmdGenerator;
  class PreDecCmdGenerator;
  class PreIncCmdGenerator;

  typedef unordered_map<string, vector<uint32_t>> IdAddresses;

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
  virtual void VisitContinue(const ContinueNode &node) override;
  virtual void VisitDiv(const DivNode &node) override;
  virtual void VisitDouble(const DoubleNode &node) override;
  virtual void VisitEqual(const EqualNode &node) override;
  virtual void VisitExprStmt(const ExprStmtNode &node) override;
  virtual void VisitFuncDefWithBody(const FuncDefWithBodyNode &node) override;
  virtual void VisitFuncDefWithoutBody(
      const FuncDefWithoutBodyNode &node) override;
  virtual void VisitGreater(const GreaterNode &node) override;
  virtual void VisitGreaterOrEqual(const GreaterOrEqualNode &node) override;
  virtual void VisitIfElseIfElse(const IfElseIfElseNode &node) override;
  virtual void VisitIfElseIf(const IfElseIfNode &node) override;
  virtual void VisitImport(const ImportNode &node) override;
  virtual void VisitInt(const IntNode &node) override;
  virtual void VisitLess(const LessNode &node) override;
  virtual void VisitLessOrEqual(const LessOrEqualNode &node) override;
  virtual void VisitLong(const LongNode &node) override;
  virtual void VisitMul(const MulNode &node) override;
  virtual void VisitNegative(const NegativeNode &node) override;
  virtual void VisitNotEqual(const NotEqualNode &node) override;
  virtual void VisitNot(const NotNode &node) override;
  virtual void VisitOr(const OrNode &node) override;
  virtual void VisitPreDec(const PreDecNode &node) override;
  virtual void VisitPreInc(const PreIncNode &node) override;
  virtual void VisitPreTestLoop(const PreTestLoopNode &node) override;
  virtual void VisitProgram(const ProgramNode &node) override;
  virtual void VisitString(const StringNode &node) override;
  virtual void VisitSubscript(const SubscriptNode &node) override;
  virtual void VisitSub(const SubNode &node) override;
  virtual void VisitSum(const SumNode &node) override;
  virtual void VisitVarDefWithoutInit(
      const VarDefWithoutInitNode &node) override;
  virtual void VisitVarDefWithInit(
      const VarDefWithInitNode &node) override;
  virtual void VisitId(const IdNode &node) override;
  virtual void VisitIntDataType(const IntDataTypeNode &node) override;
  virtual void VisitLongDataType(const LongDataTypeNode &node) override;
  virtual void VisitDoubleDataType(const DoubleDataTypeNode &node) override;
  virtual void VisitCharDataType(const CharDataTypeNode &node) override;
  virtual void VisitStringDataType(const StringDataTypeNode &node) override;
  virtual void VisitBoolDataType(const BoolDataTypeNode &node) override;
  virtual void VisitVoidDataType(const VoidDataTypeNode &node) override;
  virtual void VisitArrayDataType(const ArrayDataTypeNode &node) override;
  virtual void VisitReturnValue(const ReturnValueNode &node) override;
  virtual void VisitReturnWithoutValue(
      const ReturnWithoutValueNode &node) override;
  virtual void VisitArgDef(const ArgDefNode &node) override;
  virtual void VisitScope(const ScopeNode &node) override;

  /**
   * @param branch_end_offset_placeholder If not null, generates jump to the end
   * of branch and stores here address of placeholder, that needs to be filled
   * with the offset of branch end
   */
  void VisitIf(const IfNode &node, uint32_t *branch_end_offset_placeholder);

  void VisitBinaryExpr(
      const BinaryExprNode &node, ExprCmdGenerator *cmd_generator);
  void VisitUnaryExpr(
      const UnaryExprNode &node, ExprCmdGenerator *cmd_generator);
  void WriteCurrentCmdOffset(const vector<uint32_t> &offset_placeholders);
  void GenerateJumpCmdStart(
      const vector<const VarDefNode*> &flow_local_var_defs);
  void GenerateCastCmdIfNeeded(const ExprAnalysis &expr_analysis);
  void GenerateDestroyLocalVarCmds(
      const vector<const VarDefNode*> &local_var_defs);
  void GenerateReturnCmd(const vector<const VarDefNode*> &flow_local_var_defs);
  const NodeSemanticAnalysis &GetNodeAnalysis(const Node &node) const;
  const DataType &GetExprDataType(const ExprAnalysis &expr_analysis) const;
  Scope *GetCurrentScope();
  vector<real_talk::code::IdAddresses> TransformIdAddresses(
      const Impl::IdAddresses &id_addresses) const;

  const CastCmdGenerator &cast_cmd_generator_;
  const SemanticAnalysis *semantic_analysis_;
  unique_ptr<Code> code_;
  vector<IdSize> global_var_defs_;
  vector<string> native_func_defs_;
  vector<IdAddress> func_defs_;
  Impl::IdAddresses global_var_refs_;
  Impl::IdAddresses func_refs_;
  Impl::IdAddresses native_func_refs_;
  vector<Scope*> scopes_stack_;
};

class SimpleCodeGenerator::Impl::Scope {
 public:
  Scope(uint32_t start_address, vector<Scope*> &scopes_stack)
      : start_address_(start_address), scopes_stack_(scopes_stack) {
    scopes_stack_.push_back(this);
  }

  ~Scope() {
    scopes_stack_.pop_back();
  }

  uint32_t GetStartAddress() const {
    return start_address_;
  }

  vector<uint32_t> &GetEndAddressPlaceholders() {
    return end_address_placeholders_;
  }

 private:
  uint32_t start_address_;
  vector<uint32_t> end_address_placeholders_;
  vector<Scope*> &scopes_stack_;
};

class SimpleCodeGenerator::Impl::StmtGrouper: private NodeVisitor {
 public:
  struct GroupedStmts {
    vector<const StmtNode*> non_func_defs;
    vector<const FuncDefNode*> func_defs;
  };

  GroupedStmts Group(const ProgramNode &program) {
    program.Accept(*this);
    vector<const StmtNode*> non_func_defs = move(non_func_defs_);
    non_func_defs_.clear();
    vector<const FuncDefNode*> func_defs = move(func_defs_);
    func_defs_.clear();
    return GroupedStmts{move(non_func_defs), move(func_defs)};
  }

 private:
  virtual void VisitExprStmt(const ExprStmtNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitFuncDefWithBody(const FuncDefWithBodyNode &node) override {
    func_defs_.push_back(&node);
  }

  virtual void VisitFuncDefWithoutBody(
      const FuncDefWithoutBodyNode &node) override {
    func_defs_.push_back(&node);
  }

  virtual void VisitIfElseIfElse(const IfElseIfElseNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitIfElseIf(const IfElseIfNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitImport(const ImportNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitPreTestLoop(const PreTestLoopNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitProgram(const ProgramNode &node) override {
    for (const unique_ptr<StmtNode> &stmt: node.GetStmts()) {
      stmt->Accept(*this);
    }
  }

  virtual void VisitVarDefWithoutInit(
      const VarDefWithoutInitNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitVarDefWithInit(const VarDefWithInitNode &node) override {
    non_func_defs_.push_back(&node);
  }

  virtual void VisitAnd(const AndNode&) override {assert(false);}
  virtual void VisitArrayAllocWithoutInit(
      const ArrayAllocWithoutInitNode&) override {assert(false);}
  virtual void VisitArrayAllocWithInit(
      const ArrayAllocWithInitNode&) override {assert(false);}
  virtual void VisitAssign(const AssignNode&) override {assert(false);}
  virtual void VisitBool(const BoolNode&) override {assert(false);}
  virtual void VisitBreak(const BreakNode&) override {assert(false);}
  virtual void VisitCall(const CallNode&) override {assert(false);}
  virtual void VisitChar(const CharNode&) override {assert(false);}
  virtual void VisitContinue(const ContinueNode&) override {assert(false);}
  virtual void VisitDiv(const DivNode&) override {assert(false);}
  virtual void VisitDouble(const DoubleNode&) override {assert(false);}
  virtual void VisitEqual(const EqualNode&) override {assert(false);}
  virtual void VisitGreater(const GreaterNode&) override {assert(false);}
  virtual void VisitGreaterOrEqual(
      const GreaterOrEqualNode&) override {assert(false);}
  virtual void VisitInt(const IntNode&) override {assert(false);}
  virtual void VisitLess(const LessNode&) override {assert(false);}
  virtual void VisitLessOrEqual(
      const LessOrEqualNode&) override {assert(false);}
  virtual void VisitLong(const LongNode&) override {assert(false);}
  virtual void VisitMul(const MulNode&) override {assert(false);}
  virtual void VisitNegative(
      const NegativeNode&) override {assert(false);}
  virtual void VisitNotEqual(
      const NotEqualNode&) override {assert(false);}
  virtual void VisitNot(const NotNode&) override {assert(false);}
  virtual void VisitOr(const OrNode&) override {assert(false);}
  virtual void VisitPreDec(const PreDecNode&) override {assert(false);}
  virtual void VisitPreInc(const PreIncNode&) override {assert(false);}
  virtual void VisitString(const StringNode&) override {assert(false);}
  virtual void VisitSubscript(
      const SubscriptNode&) override {assert(false);}
  virtual void VisitSub(const SubNode&) override {assert(false);}
  virtual void VisitSum(const SumNode&) override {assert(false);}
  virtual void VisitId(const IdNode&) override {assert(false);}
  virtual void VisitIntDataType(
      const IntDataTypeNode&) override {assert(false);}
  virtual void VisitLongDataType(
      const LongDataTypeNode&) override {assert(false);}
  virtual void VisitDoubleDataType(
      const DoubleDataTypeNode&) override {assert(false);}
  virtual void VisitCharDataType(
      const CharDataTypeNode&) override {assert(false);}
  virtual void VisitStringDataType(
      const StringDataTypeNode&) override {assert(false);}
  virtual void VisitBoolDataType(
      const BoolDataTypeNode&) override {assert(false);}
  virtual void VisitVoidDataType(
      const VoidDataTypeNode&) override {assert(false);}
  virtual void VisitArrayDataType(
      const ArrayDataTypeNode&) override {assert(false);}
  virtual void VisitReturnValue(
      const ReturnValueNode&) override {assert(false);}
  virtual void VisitReturnWithoutValue(
      const ReturnWithoutValueNode&) override {assert(false);}
  virtual void VisitArgDef(const ArgDefNode&) override {assert(false);}
  virtual void VisitScope(const ScopeNode&) override {assert(false);}

  vector<const StmtNode*> non_func_defs_;
  vector<const FuncDefNode*> func_defs_;
};

class SimpleCodeGenerator::Impl::DataTypeSizeResolver: private DataTypeVisitor {
 public:
  DataTypeSize Resolve(const DataType &data_type) {
    data_type.Accept(*this);
    return result_;
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    result_ = DataTypeSize::kArray;
  }

  virtual void VisitBool(const BoolDataType&) override {
    result_ = DataTypeSize::kBool;
  }

  virtual void VisitInt(const IntDataType&) override {
    result_ = DataTypeSize::kInt;
  }

  virtual void VisitLong(const LongDataType&) override {
    result_ = DataTypeSize::kLong;
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    result_ = DataTypeSize::kDouble;
  }

  virtual void VisitChar(const CharDataType&) override {
    result_ = DataTypeSize::kChar;
  }

  virtual void VisitString(const StringDataType&) override {
    result_ = DataTypeSize::kString;
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  DataTypeSize result_;
};

class SimpleCodeGenerator::Impl::ArrayCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const ArrayDataType &data_type, Code *code) {
    code_ = code;
    dimensions_count_ = UINT8_C(0);
    data_type.Accept(*this);
    code_->Write<uint8_t>(dimensions_count_);
  }

 protected:
  Code *code_;

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
    ++dimensions_count_;
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  uint8_t dimensions_count_;
};

class SimpleCodeGenerator::Impl::CreateGlobalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(
      const DataType &data_type, uint32_t *var_index_placeholder, Code *code) {
    assert(var_index_placeholder);
    assert(code);
    code_ = code;
    data_type.Accept(*this);
    *var_index_placeholder = code->GetPosition();
    const uint32_t var_index = numeric_limits<uint32_t>::max();
    code->Write<uint32_t>(var_index);
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalArrayVar);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateGlobalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::CreateAndInitGlobalArrayVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const ArrayDataType &data_type,
                uint32_t *var_index_placeholder,
                Code *code) {
    assert(var_index_placeholder);
    assert(code);
    code_ = code;
    dimensions_count_ = UINT8_C(0);
    data_type.Accept(*this);
    *var_index_placeholder = code->GetPosition();
    const uint32_t var_index = numeric_limits<uint32_t>::max();
    code->Write<uint32_t>(var_index);
    code->Write<uint8_t>(dimensions_count_);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
    ++dimensions_count_;
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitGlobalBoolArrayVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitGlobalIntArrayVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitGlobalLongArrayVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitGlobalDoubleArrayVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitGlobalCharArrayVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitGlobalStringArrayVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  uint8_t dimensions_count_;
  Code *code_;
};

class SimpleCodeGenerator::Impl::CreateAndInitGlobalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(
      const DataType &data_type, uint32_t *var_index_placeholder, Code *code) {
    assert(var_index_placeholder);
    assert(code);
    var_index_placeholder_ = var_index_placeholder;
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    CreateAndInitGlobalArrayVarCmdGenerator().Generate(
        data_type, var_index_placeholder_, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    VisitNonArray(CmdId::kCreateAndInitGlobalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    VisitNonArray(CmdId::kCreateAndInitGlobalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    VisitNonArray(CmdId::kCreateAndInitGlobalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    VisitNonArray(CmdId::kCreateAndInitGlobalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    VisitNonArray(CmdId::kCreateAndInitGlobalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    VisitNonArray(CmdId::kCreateAndInitGlobalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  void VisitNonArray(CmdId cmd_id) {
    code_->Write<CmdId>(cmd_id);
    *var_index_placeholder_ = code_->GetPosition();
    const uint32_t var_index = numeric_limits<uint32_t>::max();
    code_->Write<uint32_t>(var_index);
  }

  uint32_t *var_index_placeholder_;
  Code *code_;
};

class SimpleCodeGenerator::Impl::CreateAndInitLocalArrayVarCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalBoolArrayVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalIntArrayVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalLongArrayVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalDoubleArrayVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalCharArrayVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalStringArrayVar);
  }
};

class SimpleCodeGenerator::Impl::CreateAndInitLocalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    CreateAndInitLocalArrayVarCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLocalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::CreateLocalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalArrayVar);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLocalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::DestroyLocalArrayVarCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalBoolArrayVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalIntArrayVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalLongArrayVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalDoubleArrayVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalCharArrayVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalStringArrayVar);
  }
};

class SimpleCodeGenerator::Impl::DestroyLocalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    DestroyLocalArrayVarCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kDestroyLocalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::CreateArrayCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(
      const DataType &data_type, uint8_t dimensions_count, Code *code) {
    code_ = code;
    data_type.Accept(*this);
    code_->Write<uint8_t>(dimensions_count);
  }

 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateStringArray);
  }

  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::CreateAndInitArrayCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type,
                uint8_t dimensions_count,
                int32_t values_count,
                Code *code) {
    code_ = code;
    data_type.Accept(*this);
    code_->Write<uint8_t>(dimensions_count);
    code_->Write<int32_t>(values_count);
  }

 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kCreateAndInitStringArray);
  }

  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::LoadGlobalArrayVarValueCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const ArrayDataType &data_type, Code *code) {
    assert(code);
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalBoolArrayVarValue);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalIntArrayVarValue);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalLongArrayVarValue);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalDoubleArrayVarValue);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalCharArrayVarValue);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalStringArrayVarValue);
  }

  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::LoadGlobalVarValueCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(
      const DataType &data_type, uint32_t *var_index_placeholder, Code *code) {
    assert(var_index_placeholder);
    assert(code);
    code_ = code;
    data_type.Accept(*this);
    *var_index_placeholder = code_->GetPosition();
    const uint32_t var_index = numeric_limits<uint32_t>::max();
    code_->Write<uint32_t>(var_index);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    LoadGlobalArrayVarValueCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalBoolVarValue);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalIntVarValue);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalLongVarValue);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalDoubleVarValue);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalCharVarValue);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadGlobalStringVarValue);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::LoadLocalArrayVarValueCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const ArrayDataType &data_type, Code *code) {
    assert(code);
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalBoolArrayVarValue);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalIntArrayVarValue);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalLongArrayVarValue);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalDoubleArrayVarValue);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalCharArrayVarValue);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalStringArrayVarValue);
  }

  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::LoadLocalVarValueCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, uint32_t var_index, Code *code) {
    assert(code);
    code_ = code;
    data_type.Accept(*this);
    code_->Write<uint32_t>(var_index);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    LoadLocalArrayVarValueCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalBoolVarValue);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalIntVarValue);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalLongVarValue);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalDoubleVarValue);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalCharVarValue);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLocalStringVarValue);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::UnloadArrayCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadStringArray);
  }
};

class SimpleCodeGenerator::Impl::UnloadCmdGenerator: private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    assert(code);
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    UnloadArrayCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadBool);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadChar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kUnloadString);
  }

  virtual void VisitVoid(const VoidDataType&) override {}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::IdNodeProcessor: private DefAnalysisVisitor {
 public:
  void Process(const IdNode *id_node,
               const SemanticAnalysis::NodeAnalyzes *node_analyzes,
               Impl::IdAddresses *global_var_refs,
               Impl::IdAddresses *func_refs,
               Impl::IdAddresses *native_func_refs,
               Code *code) {
    assert(id_node);
    assert(node_analyzes);
    assert(global_var_refs);
    assert(func_refs);
    assert(native_func_refs);
    assert(code);
    id_node_ = id_node;
    node_analyzes_ = node_analyzes;
    global_var_refs_ = global_var_refs;
    func_refs_ = func_refs;
    native_func_refs_ = native_func_refs;
    code_ = code;
    id_analysis_ =
        &(static_cast<const IdAnalysis&>(GetNodeAnalysis(*id_node_)));
    const DefNode *def_node = id_analysis_->GetDef();
    const auto &def_analysis =
        static_cast<const DefAnalysis&>(GetNodeAnalysis(*def_node));
    def_analysis.Accept(*this);
  }

 private:
  virtual void VisitLocalVarDef(const LocalVarDefAnalysis &var_def_analysis)
      override {
    auto var_index = UINT32_C(0);
    DataTypeSizeResolver data_type_size_resolver;

    for (const VarDefNode *flow_local_var_def:
             var_def_analysis.GetFlowLocalVarDefs()) {
      const auto &flow_var_def_analysis =
          static_cast<const DefAnalysis&>(GetNodeAnalysis(*flow_local_var_def));
      const auto var_size = static_cast<uint8_t>(
          data_type_size_resolver.Resolve(flow_var_def_analysis.GetDataType()));
      assert(var_index + var_size > var_index);
      var_index += var_size;
    }

    if (id_analysis_->IsAssignee()) {
      code_->Write<CmdId>(CmdId::kLoadLocalVarAddress);
      code_->Write<uint32_t>(var_index);
    } else {
      LoadLocalVarValueCmdGenerator().Generate(
          var_def_analysis.GetDataType(), var_index, code_);
    }
  }

  virtual void VisitGlobalVarDef(const GlobalVarDefAnalysis &var_def_analysis)
      override {
    uint32_t var_index_placeholder;

    if (id_analysis_->IsAssignee()) {
      code_->Write<CmdId>(CmdId::kLoadGlobalVarAddress);
      var_index_placeholder = code_->GetPosition();
      code_->Write<uint32_t>(numeric_limits<uint32_t>::max());
    } else {
      LoadGlobalVarValueCmdGenerator().Generate(
          var_def_analysis.GetDataType(), &var_index_placeholder, code_);
    }

    const string &id = id_node_->GetStartToken().GetValue();
    (*global_var_refs_)[id].push_back(var_index_placeholder);
  }

  virtual void VisitFuncDef(const FuncDefAnalysis &func_def_analysis) override {
    const bool is_native = func_def_analysis.GetDataType().IsNative();
    CmdId cmd;
    Impl::IdAddresses *func_refs;

    if (is_native) {
      cmd = CmdId::kLoadNativeFuncValue;
      func_refs = native_func_refs_;
    } else {
      cmd = CmdId::kLoadFuncValue;
      func_refs = func_refs_;
    }

    code_->Write<CmdId>(cmd);
    const uint32_t func_value_placeholder = code_->GetPosition();
    const auto func_value = numeric_limits<uint32_t>::max();
    code_->Write<uint32_t>(func_value);
    const string &id = id_node_->GetStartToken().GetValue();
    (*func_refs)[id].push_back(func_value_placeholder);
  }

  const NodeSemanticAnalysis &GetNodeAnalysis(const Node &node) const {
    const SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
        node_analyzes_->find(&node);
    assert(node_analysis_it != node_analyzes_->cend());
    return *(node_analysis_it->second);
  }

  const IdNode *id_node_;
  const IdAnalysis *id_analysis_;
  const SemanticAnalysis::NodeAnalyzes *node_analyzes_;
  Impl::IdAddresses *global_var_refs_;
  Impl::IdAddresses *func_refs_;
  Impl::IdAddresses *native_func_refs_;
  Code *code_;
};

template<typename TCreateLocalVarCmdGenerator,
         typename TCreateGlobalVarCmdGenerator>
class SimpleCodeGenerator::Impl::VarDefNodeProcessor
    : private DefAnalysisVisitor {
 public:
  void Process(const VarDefNode *var_def_node,
               const DefAnalysis *var_def_analysis,
               vector<IdSize> *global_var_defs,
               Impl::IdAddresses *global_var_refs,
               Code *code) {
    assert(var_def_node);
    assert(var_def_analysis);
    assert(global_var_defs);
    assert(global_var_refs);
    assert(code);
    var_def_node_ = var_def_node;
    global_var_defs_ = global_var_defs;
    global_var_refs_ = global_var_refs;
    code_ = code;
    var_def_analysis->Accept(*this);
  }

 private:
  virtual void VisitLocalVarDef(const LocalVarDefAnalysis &analysis) override {
    TCreateLocalVarCmdGenerator().Generate(analysis.GetDataType(), code_);
  }

  virtual void VisitGlobalVarDef(const GlobalVarDefAnalysis &analysis)
      override {
    uint32_t var_index_placeholder;
    TCreateGlobalVarCmdGenerator().Generate(
        analysis.GetDataType(), &var_index_placeholder, code_);
    const string &id = var_def_node_->GetNameToken().GetValue();
    const DataTypeSize var_size =
        DataTypeSizeResolver().Resolve(analysis.GetDataType());
    global_var_defs_->push_back(IdSize(id, var_size));
    (*global_var_refs_)[id].push_back(var_index_placeholder);
  }

  virtual void VisitFuncDef(const FuncDefAnalysis&) override {assert(false);}

  const VarDefNode *var_def_node_;
  vector<IdSize> *global_var_defs_;
  Impl::IdAddresses *global_var_refs_;
  Code *code_;
};

class SimpleCodeGenerator::Impl::StoreArrayCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreStringArray);
  }
};

class SimpleCodeGenerator::Impl::StoreCmdGenerator: private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    StoreArrayCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreBool);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreChar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kStoreString);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::CallCmdGenerator: private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitFunc(const FuncDataType &data_type) override {
    code_->Write<CmdId>(data_type.IsNative() ? CmdId::kCallNative : CmdId::kCall);
  }

  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitInt(const IntDataType&) override {assert(false);}
  virtual void VisitLong(const LongDataType&) override {assert(false);}
  virtual void VisitDouble(const DoubleDataType&) override {assert(false);}
  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitVoid(const VoidDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::LoadArrayElementValueCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadBoolArrayElementValue);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadIntArrayElementValue);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLongArrayElementValue);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadDoubleArrayElementValue);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadCharArrayElementValue);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadStringArrayElementValue);
  }
};

class SimpleCodeGenerator::Impl::LoadElementValueCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    LoadArrayElementValueCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitFunc(const FuncDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitInt(const IntDataType&) override {assert(false);}
  virtual void VisitLong(const LongDataType&) override {assert(false);}
  virtual void VisitDouble(const DoubleDataType&) override {assert(false);}
  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitVoid(const VoidDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::LoadArrayElementAddressCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadBoolArrayElementAddress);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadIntArrayElementAddress);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadLongArrayElementAddress);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadDoubleArrayElementAddress);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadCharArrayElementAddress);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kLoadStringArrayElementAddress);
  }
};

class SimpleCodeGenerator::Impl::LoadElementAddressCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    LoadArrayElementAddressCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitFunc(const FuncDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitInt(const IntDataType&) override {assert(false);}
  virtual void VisitLong(const LongDataType&) override {assert(false);}
  virtual void VisitDouble(const DoubleDataType&) override {assert(false);}
  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitVoid(const VoidDataType&) override {assert(false);}

  Code *code_;
};

class SimpleCodeGenerator::Impl::ExprCmdGenerator: private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 protected:
  Code *code_;

 private:
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}
  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::AndCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kAnd);
  }

  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
  virtual void VisitInt(const IntDataType&) override {assert(false);}
  virtual void VisitLong(const LongDataType&) override {assert(false);}
  virtual void VisitDouble(const DoubleDataType&) override {assert(false);}
  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::OrCmdGenerator: public ExprCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kOr);
  }

  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
  virtual void VisitInt(const IntDataType&) override {assert(false);}
  virtual void VisitLong(const LongDataType&) override {assert(false);}
  virtual void VisitDouble(const DoubleDataType&) override {assert(false);}
  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::MulCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kMulInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kMulLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kMulDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kMulChar);
  }

  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::DivCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kDivInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kDivLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kDivDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kDivChar);
  }

  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::SumCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kSumInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kSumLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kSumDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kSumChar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kSumString);
  }

  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::SubCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kSubInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kSubLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kSubDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kSubChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::EqualArrayCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualStringArray);
  }
};

class SimpleCodeGenerator::Impl::EqualCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    EqualArrayCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualBool);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualChar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kEqualString);
  }
};

class SimpleCodeGenerator::Impl::NotEqualArrayCmdGenerator
    : public ArrayCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualStringArray);
  }
};

class SimpleCodeGenerator::Impl::NotEqualCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitArray(const ArrayDataType &data_type) override {
    NotEqualArrayCmdGenerator().Generate(data_type, code_);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualBool);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualChar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->Write<CmdId>(CmdId::kNotEqualString);
  }
};

class SimpleCodeGenerator::Impl::GreaterCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::GreaterOrEqualCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterOrEqualInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterOrEqualLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterOrEqualDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kGreaterOrEqualChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::LessCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLessInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLessLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLessDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLessChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::LessOrEqualCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kLessOrEqualInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kLessOrEqualLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kLessOrEqualDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kLessOrEqualChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::LogicalNegateCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->Write<CmdId>(CmdId::kLogicalNegateBool);
  }

  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
  virtual void VisitInt(const IntDataType&) override {assert(false);}
  virtual void VisitLong(const LongDataType&) override {assert(false);}
  virtual void VisitDouble(const DoubleDataType&) override {assert(false);}
  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::ArithmeticNegateCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kArithmeticNegateInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kArithmeticNegateLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kArithmeticNegateDouble);
  }

  virtual void VisitChar(const CharDataType&) override {assert(false);}
  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::PreDecCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kPreDecInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kPreDecLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kPreDecDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kPreDecChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

class SimpleCodeGenerator::Impl::PreIncCmdGenerator
    : public ExprCmdGenerator {
 private:
  virtual void VisitInt(const IntDataType&) override {
    code_->Write<CmdId>(CmdId::kPreIncInt);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->Write<CmdId>(CmdId::kPreIncLong);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->Write<CmdId>(CmdId::kPreIncDouble);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->Write<CmdId>(CmdId::kPreIncChar);
  }

  virtual void VisitString(const StringDataType&) override {assert(false);}
  virtual void VisitBool(const BoolDataType&) override {assert(false);}
  virtual void VisitArray(const ArrayDataType&) override {assert(false);}
};

SimpleCodeGenerator::SimpleCodeGenerator(
    const CastCmdGenerator &cast_cmd_generator)
    : impl_(new Impl(cast_cmd_generator)) {}

SimpleCodeGenerator::~SimpleCodeGenerator() {}

unique_ptr<Module> SimpleCodeGenerator::Generate(
    const ProgramNode &program,
    const SemanticAnalysis &semantic_analysis,
    uint32_t version) {
  return impl_->Generate(program, semantic_analysis, version);
}

SimpleCodeGenerator::Impl::Impl(
    const CastCmdGenerator &cast_cmd_generator)
    : cast_cmd_generator_(cast_cmd_generator) {}

unique_ptr<Module> SimpleCodeGenerator::Impl::Generate(
    const ProgramNode &program,
    const SemanticAnalysis &semantic_analysis,
    uint32_t version) {
  semantic_analysis_ = &semantic_analysis;
  code_.reset(new Code());
  const StmtGrouper::GroupedStmts &stmts = StmtGrouper().Group(program);

  for (const StmtNode *non_func_def: stmts.non_func_defs) {
    non_func_def->Accept(*this);
  }

  const uint32_t main_cmds_size = code_->GetPosition();

  for (const FuncDefNode *func_def: stmts.func_defs) {
    func_def->Accept(*this);
  }

  assert(scopes_stack_.empty());
  unique_ptr<Module> module(new Module(
      version,
      move(code_),
      main_cmds_size,
      func_defs_,
      global_var_defs_,
      native_func_defs_,
      TransformIdAddresses(func_refs_),
      TransformIdAddresses(native_func_refs_),
      TransformIdAddresses(global_var_refs_)));
  global_var_defs_.clear();
  func_defs_.clear();
  native_func_defs_.clear();
  global_var_refs_.clear();
  func_refs_.clear();
  native_func_refs_.clear();
  return module;
}

vector<real_talk::code::IdAddresses>
SimpleCodeGenerator::Impl::TransformIdAddresses(
    const Impl::IdAddresses &id_addresses) const {
  vector<real_talk::code::IdAddresses> transformed_id_addresses;
  transformed_id_addresses.reserve(id_addresses.size());

  for (const auto &id_addresses_pair: id_addresses) {
    const string &id = id_addresses_pair.first;
    const vector<uint32_t> &addresses = id_addresses_pair.second;
    transformed_id_addresses.push_back(
        real_talk::code::IdAddresses(id, addresses));
  }

  return transformed_id_addresses;
}

void SimpleCodeGenerator::Impl::VisitProgram(const ProgramNode &node) {
  for (const unique_ptr<StmtNode> &stmt: node.GetStmts()) {
    stmt->Accept(*this);
  }
}

void SimpleCodeGenerator::Impl::VisitImport(const ImportNode&) {}

void SimpleCodeGenerator::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &node) {
  const DefAnalysis &analysis =
      static_cast<const DefAnalysis&>(GetNodeAnalysis(node));
  VarDefNodeProcessor<CreateLocalVarCmdGenerator, CreateGlobalVarCmdGenerator>()
      .Process(&node,
               &analysis,
               &global_var_defs_,
               &global_var_refs_,
               code_.get());
}

void SimpleCodeGenerator::Impl::VisitVarDefWithInit(
    const VarDefWithInitNode &node) {
  node.GetValue()->Accept(*this);
  const DefAnalysis &var_def_analysis =
      static_cast<const DefAnalysis&>(GetNodeAnalysis(node));
  VarDefNodeProcessor<CreateAndInitLocalVarCmdGenerator,
                      CreateAndInitGlobalVarCmdGenerator>()
      .Process(&node,
               &var_def_analysis,
               &global_var_defs_,
               &global_var_refs_,
               code_.get());
}

void SimpleCodeGenerator::Impl::VisitExprStmt(const ExprStmtNode &node) {
  node.GetExpr()->Accept(*this);
  const ExprAnalysis &analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(*(node.GetExpr())));
  UnloadCmdGenerator().Generate(analysis.GetDataType(), code_.get());
}

void SimpleCodeGenerator::Impl::VisitPreTestLoop(const PreTestLoopNode &node) {
  const uint32_t start_address = code_->GetPosition();
  node.GetCond()->Accept(*this);
  Scope scope(start_address, scopes_stack_);
  code_->Write<CmdId>(CmdId::kJumpIfNot);
  const uint32_t end_offset_placeholder = code_->GetPosition();
  scope.GetEndAddressPlaceholders().push_back(end_offset_placeholder);
  code_->Skip(sizeof(int32_t));
  node.GetBody()->Accept(*this);
  const ScopeAnalysis &body_analysis = static_cast<const ScopeAnalysis&>(
      GetNodeAnalysis(*(node.GetBody())));
  GenerateJumpCmdStart(body_analysis.GetLocalVarDefs());
  const int64_t start_offset = static_cast<int64_t>(start_address)
                               - (static_cast<int64_t>(code_->GetPosition())
                                  + static_cast<int64_t>(sizeof(int32_t)));
  assert(start_offset <= numeric_limits<int32_t>::max());
  assert(start_offset >= numeric_limits<int32_t>::min());
  code_->Write<int32_t>(static_cast<int32_t>(start_offset));
  WriteCurrentCmdOffset(scope.GetEndAddressPlaceholders());
}

void SimpleCodeGenerator::Impl::VisitBreak(const BreakNode &node) {
  const ControlFlowTransferAnalysis &break_analysis =
      static_cast<const ControlFlowTransferAnalysis&>(GetNodeAnalysis(node));
  GenerateJumpCmdStart(break_analysis.GetFlowLocalVarDefs());
  const uint32_t flow_end_address_placeholder = code_->GetPosition();
  GetCurrentScope()->GetEndAddressPlaceholders().push_back(
      flow_end_address_placeholder);
  code_->Skip(sizeof(int32_t));
}

void SimpleCodeGenerator::Impl::VisitContinue(const ContinueNode &node) {
  const ControlFlowTransferAnalysis &continue_analysis =
      static_cast<const ControlFlowTransferAnalysis&>(GetNodeAnalysis(node));
  GenerateJumpCmdStart(continue_analysis.GetFlowLocalVarDefs());
  const int64_t scope_start_offset =
      static_cast<int64_t>(GetCurrentScope()->GetStartAddress())
      - (static_cast<int64_t>(code_->GetPosition())
         + static_cast<int64_t>(sizeof(int32_t)));
  assert(scope_start_offset <= numeric_limits<int32_t>::max());
  assert(scope_start_offset >= numeric_limits<int32_t>::min());
  code_->Write<int32_t>(static_cast<int32_t>(scope_start_offset));
}

void SimpleCodeGenerator::Impl::VisitIfElseIfElse(
    const IfElseIfElseNode &if_else_if_else) {
  vector<uint32_t> branch_end_offset_placeholders;
  uint32_t branch_end_offset_placeholder;
  VisitIf(*(if_else_if_else.GetIf()), &branch_end_offset_placeholder);
  branch_end_offset_placeholders.push_back(branch_end_offset_placeholder);

  for (const unique_ptr<ElseIfNode> &else_if: if_else_if_else.GetElseIfs()) {
    VisitIf(*(else_if->GetIf()), &branch_end_offset_placeholder);
    branch_end_offset_placeholders.push_back(branch_end_offset_placeholder);
  }

  if_else_if_else.GetElseBody()->Accept(*this);
  const ScopeAnalysis &else_body_analysis = static_cast<const ScopeAnalysis&>(
      GetNodeAnalysis(*if_else_if_else.GetElseBody()));
  GenerateDestroyLocalVarCmds(else_body_analysis.GetLocalVarDefs());
  WriteCurrentCmdOffset(branch_end_offset_placeholders);
}

void SimpleCodeGenerator::Impl::VisitIfElseIf(const IfElseIfNode &if_else_if) {
  if (if_else_if.GetElseIfs().empty()) {
    VisitIf(*if_else_if.GetIf(), nullptr);
    return;
  }

  vector<uint32_t> branch_end_offset_placeholders;
  uint32_t branch_end_offset_placeholder;
  VisitIf(*(if_else_if.GetIf()), &branch_end_offset_placeholder);
  branch_end_offset_placeholders.push_back(branch_end_offset_placeholder);
  auto last_else_if_it = if_else_if.GetElseIfs().cend() - 1;

  for (auto else_if_it = if_else_if.GetElseIfs().cbegin();
       else_if_it != last_else_if_it;
       ++else_if_it) {
    const unique_ptr<IfNode> &if_node = (**else_if_it).GetIf();
    VisitIf(*if_node, &branch_end_offset_placeholder);
    branch_end_offset_placeholders.push_back(branch_end_offset_placeholder);
  }

  const unique_ptr<IfNode> &if_node = (**last_else_if_it).GetIf();
  VisitIf(*if_node, nullptr);
  WriteCurrentCmdOffset(branch_end_offset_placeholders);
}

void SimpleCodeGenerator::Impl::VisitIf(
    const IfNode &node, uint32_t *branch_end_offset_placeholder) {
  node.GetCond()->Accept(*this);
  code_->Write<CmdId>(CmdId::kJumpIfNot);
  const uint32_t body_end_offset_placeholder = code_->GetPosition();
  code_->Skip(sizeof(int32_t));
  node.GetBody()->Accept(*this);
  const ScopeAnalysis &body_analysis =
      static_cast<const ScopeAnalysis&>(GetNodeAnalysis(*node.GetBody()));

  if (branch_end_offset_placeholder == nullptr) {
    GenerateDestroyLocalVarCmds(body_analysis.GetLocalVarDefs());
  } else {
    GenerateJumpCmdStart(body_analysis.GetLocalVarDefs());
    *branch_end_offset_placeholder = code_->GetPosition();
    code_->Skip(sizeof(int32_t));
  }

  WriteCurrentCmdOffset(vector<uint32_t>({body_end_offset_placeholder}));
}

void SimpleCodeGenerator::Impl::VisitFuncDefWithBody(
    const FuncDefWithBodyNode &node) {
  const uint32_t start_address = code_->GetPosition();

  for (const unique_ptr<ArgDefNode> &arg: node.GetArgs()) {
    arg->Accept(*this);
  }

  for (const unique_ptr<StmtNode> &stmt: node.GetBody()->GetStmts()) {
    stmt->Accept(*this);
  }

  const FuncDefAnalysis &func_analysis =
      static_cast<const FuncDefAnalysis&>(GetNodeAnalysis(node));

  if (func_analysis.GetDataType().GetReturnDataType() == VoidDataType()
      && !func_analysis.HasReturn()) {
    const auto &body_analysis =
        static_cast<const ScopeAnalysis&>(GetNodeAnalysis(*(node.GetBody())));
    GenerateReturnCmd(body_analysis.GetLocalVarDefs());
  }

  const string &id = node.GetNameToken().GetValue();
  func_defs_.push_back(IdAddress(id, start_address));
}

void SimpleCodeGenerator::Impl::VisitFuncDefWithoutBody(
    const FuncDefWithoutBodyNode &node) {
  const string &id = node.GetNameToken().GetValue();
  native_func_defs_.push_back(id);
}

void SimpleCodeGenerator::Impl::VisitArgDef(const ArgDefNode &node) {
  const DefAnalysis &analysis =
      static_cast<const DefAnalysis&>(GetNodeAnalysis(node));
  CreateAndInitLocalVarCmdGenerator().Generate(
      analysis.GetDataType(), code_.get());
}

void SimpleCodeGenerator::Impl::VisitReturnValue(const ReturnValueNode &node) {
  node.GetValue()->Accept(*this);
  const ReturnAnalysis &return_analysis = static_cast<const ReturnAnalysis&>(
      GetNodeAnalysis(node));
  GenerateReturnCmd(return_analysis.GetFlowLocalVarDefs());
}

void SimpleCodeGenerator::Impl::VisitReturnWithoutValue(
    const ReturnWithoutValueNode &node) {
  const ReturnAnalysis &analysis = static_cast<const ReturnAnalysis&>(
      GetNodeAnalysis(node));
  GenerateReturnCmd(analysis.GetFlowLocalVarDefs());
}

void SimpleCodeGenerator::Impl::VisitScope(const ScopeNode &node) {
  for (const unique_ptr<StmtNode> &stmt: node.GetStmts()) {
    stmt->Accept(*this);
  }
}

void SimpleCodeGenerator::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode &node) {
  for (const std::unique_ptr<BoundedArraySizeNode> &size
           : reverse(node.GetSizes())) {
    size->GetValue()->Accept(*this);
  }

  const ExprAnalysis &array_alloc_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  const size_t dimensions_count = node.GetSizes().size();
  assert(dimensions_count <= numeric_limits<uint8_t>::max());
  CreateArrayCmdGenerator().Generate(array_alloc_analysis.GetDataType(),
                                     static_cast<uint8_t>(dimensions_count),
                                     code_.get());
  GenerateCastCmdIfNeeded(array_alloc_analysis);
}

void SimpleCodeGenerator::Impl::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode &node) {
  for (const unique_ptr<ExprNode> &value: reverse(node.GetValues())) {
    value->Accept(*this);
  }

  const ExprAnalysis &array_alloc_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  const size_t dimensions_count = node.GetSizes().size();
  assert(dimensions_count <= numeric_limits<uint8_t>::max());
  const size_t values_count = node.GetValues().size();
  assert(values_count <= numeric_limits<int32_t>::max());
  CreateAndInitArrayCmdGenerator().Generate(
      array_alloc_analysis.GetDataType(),
      static_cast<uint8_t>(dimensions_count),
      static_cast<int32_t>(values_count),
      code_.get());
  GenerateCastCmdIfNeeded(array_alloc_analysis);
}

void SimpleCodeGenerator::Impl::VisitId(const IdNode &node) {
  IdNodeProcessor().Process(&node,
                            &(semantic_analysis_->GetNodeAnalyzes()),
                            &global_var_refs_,
                            &func_refs_,
                            &native_func_refs_,
                            code_.get());
  const auto &id_analysis =
      static_cast<const IdAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(id_analysis);
}

void SimpleCodeGenerator::Impl::VisitCall(const CallNode &node) {
  for (const unique_ptr<ExprNode> &arg: reverse(node.GetArgs())) {
    arg->Accept(*this);
  }

  node.GetOperand()->Accept(*this);
  const ExprAnalysis &operand_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(*(node.GetOperand())));
  CallCmdGenerator().Generate(GetExprDataType(operand_analysis), code_.get());
  const ExprAnalysis &call_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(call_analysis);
}

void SimpleCodeGenerator::Impl::VisitAssign(const AssignNode &node) {
  node.GetRightOperand()->Accept(*this);
  node.GetLeftOperand()->Accept(*this);
  const ExprAnalysis &left_operand_analysis = static_cast<const ExprAnalysis&>(
      GetNodeAnalysis(*(node.GetLeftOperand())));
  StoreCmdGenerator().Generate(
      left_operand_analysis.GetDataType(), code_.get());
  const ExprAnalysis &assign_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(assign_analysis);
}

void SimpleCodeGenerator::Impl::VisitSubscript(const SubscriptNode &node) {
  node.GetIndex()->Accept(*this);
  node.GetOperand()->Accept(*this);
  const SubscriptAnalysis &subscript_analysis =
      static_cast<const SubscriptAnalysis&>(GetNodeAnalysis(node));
  const ExprAnalysis &operand_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(*(node.GetOperand())));

  if (subscript_analysis.IsAssignee()) {
    LoadElementAddressCmdGenerator().Generate(
        GetExprDataType(operand_analysis), code_.get());
  } else {
    LoadElementValueCmdGenerator().Generate(
        GetExprDataType(operand_analysis), code_.get());
  }

  GenerateCastCmdIfNeeded(subscript_analysis);
}

void SimpleCodeGenerator::Impl::VisitAnd(const AndNode &node) {
  node.GetLeftOperand()->Accept(*this);
  code_->Write<CmdId>(CmdId::kImplicitJumpIfNot);
  const uint32_t end_address_placeholder = code_->GetPosition();
  code_->Skip(sizeof(int32_t));
  node.GetRightOperand()->Accept(*this);
  const ExprAnalysis &left_operand_analysis = static_cast<const ExprAnalysis&>(
      GetNodeAnalysis(*(node.GetLeftOperand())));
  AndCmdGenerator().Generate(GetExprDataType(
      left_operand_analysis), code_.get());
  const ExprAnalysis &and_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(and_analysis);
  WriteCurrentCmdOffset(vector<uint32_t>({end_address_placeholder}));
}

void SimpleCodeGenerator::Impl::VisitOr(const OrNode &node) {
  node.GetLeftOperand()->Accept(*this);
  code_->Write<CmdId>(CmdId::kImplicitJumpIf);
  const uint32_t end_address_placeholder = code_->GetPosition();
  code_->Skip(sizeof(int32_t));
  node.GetRightOperand()->Accept(*this);
  const ExprAnalysis &left_operand_analysis = static_cast<const ExprAnalysis&>(
      GetNodeAnalysis(*(node.GetLeftOperand())));
  OrCmdGenerator().Generate(GetExprDataType(
      left_operand_analysis), code_.get());
  const ExprAnalysis &or_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(or_analysis);
  WriteCurrentCmdOffset(vector<uint32_t>({end_address_placeholder}));
}

void SimpleCodeGenerator::Impl::VisitEqual(const EqualNode &node) {
  EqualCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitNotEqual(const NotEqualNode &node) {
  NotEqualCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitGreater(const GreaterNode &node) {
  GreaterCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitGreaterOrEqual(
    const GreaterOrEqualNode &node) {
  GreaterOrEqualCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitLess(const LessNode &node) {
  LessCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitLessOrEqual(const LessOrEqualNode &node) {
  LessOrEqualCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitDiv(const DivNode &node) {
  DivCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitMul(const MulNode &node) {
  MulCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitSub(const SubNode &node) {
  SubCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitSum(const SumNode &node) {
  SumCmdGenerator cmd_generator;
  VisitBinaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitBinaryExpr(
    const BinaryExprNode &node, ExprCmdGenerator *cmd_generator) {
  node.GetLeftOperand()->Accept(*this);
  node.GetRightOperand()->Accept(*this);
  const ExprAnalysis &left_operand_analysis = static_cast<const ExprAnalysis&>(
      GetNodeAnalysis(*(node.GetLeftOperand())));
  cmd_generator->Generate(GetExprDataType(left_operand_analysis), code_.get());
  const ExprAnalysis &expr_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(expr_analysis);
}

void SimpleCodeGenerator::Impl::VisitNot(const NotNode &node) {
  LogicalNegateCmdGenerator cmd_generator;
  VisitUnaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitNegative(const NegativeNode &node) {
  ArithmeticNegateCmdGenerator cmd_generator;
  VisitUnaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitPreDec(const PreDecNode &node) {
  PreDecCmdGenerator cmd_generator;
  VisitUnaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitPreInc(const PreIncNode &node) {
  PreIncCmdGenerator cmd_generator;
  VisitUnaryExpr(node, &cmd_generator);
}

void SimpleCodeGenerator::Impl::VisitUnaryExpr(
    const UnaryExprNode &node, ExprCmdGenerator *cmd_generator) {
  node.GetOperand()->Accept(*this);
  const auto &operand_analysis = static_cast<const ExprAnalysis&>(
      GetNodeAnalysis(*(node.GetOperand())));
  cmd_generator->Generate(GetExprDataType(operand_analysis), code_.get());
  const auto &expr_analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(node));
  GenerateCastCmdIfNeeded(expr_analysis);
}

void SimpleCodeGenerator::Impl::VisitInt(const IntNode &node) {
  code_->Write<CmdId>(CmdId::kLoadIntValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const int32_t value =
      static_cast<const IntLit&>(lit_analysis.GetLit()).GetValue();
  code_->Write<int32_t>(value);
  GenerateCastCmdIfNeeded(lit_analysis);
}

void SimpleCodeGenerator::Impl::VisitLong(const LongNode &node) {
  code_->Write<CmdId>(CmdId::kLoadLongValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const int64_t value =
      static_cast<const LongLit&>(lit_analysis.GetLit()).GetValue();
  code_->Write<int64_t>(value);
  GenerateCastCmdIfNeeded(lit_analysis);
}

void SimpleCodeGenerator::Impl::VisitBool(const BoolNode &node) {
  code_->Write<CmdId>(CmdId::kLoadBoolValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const bool value =
      static_cast<const BoolLit&>(lit_analysis.GetLit()).GetValue();
  code_->Write<bool>(value);
  GenerateCastCmdIfNeeded(lit_analysis);
}

void SimpleCodeGenerator::Impl::VisitChar(const CharNode &node) {
  code_->Write<CmdId>(CmdId::kLoadCharValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const char value =
      static_cast<const CharLit&>(lit_analysis.GetLit()).GetValue();
  code_->Write<char>(value);
  GenerateCastCmdIfNeeded(lit_analysis);
}

void SimpleCodeGenerator::Impl::VisitString(const StringNode &node) {
  code_->Write<CmdId>(CmdId::kLoadStringValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const string &value =
      static_cast<const StringLit&>(lit_analysis.GetLit()).GetValue();
  code_->Write<string>(value);
  GenerateCastCmdIfNeeded(lit_analysis);
}

void SimpleCodeGenerator::Impl::VisitDouble(const DoubleNode &node) {
  code_->Write<CmdId>(CmdId::kLoadDoubleValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const double &value =
      static_cast<const DoubleLit&>(lit_analysis.GetLit()).GetValue();
  code_->Write<double>(value);
  GenerateCastCmdIfNeeded(lit_analysis);
}

void SimpleCodeGenerator::Impl::VisitIntDataType(const IntDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitLongDataType(const LongDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitDoubleDataType(const DoubleDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitCharDataType(const CharDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitStringDataType(const StringDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitBoolDataType(const BoolDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitVoidDataType(const VoidDataTypeNode&) {
  assert(false);
}

void SimpleCodeGenerator::Impl::VisitArrayDataType(const ArrayDataTypeNode&) {
  assert(false);
}

const NodeSemanticAnalysis &SimpleCodeGenerator::Impl::GetNodeAnalysis(
    const Node &node) const {
  SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
      semantic_analysis_->GetNodeAnalyzes().find(&node);
  assert(node_analysis_it != semantic_analysis_->GetNodeAnalyzes().cend());
  return *(node_analysis_it->second);
}

void SimpleCodeGenerator::Impl::WriteCurrentCmdOffset(
    const vector<uint32_t> &offset_placeholders) {
  const uint32_t current_address = code_->GetPosition();

  for (const uint32_t offset_placeholder: offset_placeholders) {
    code_->SetPosition(offset_placeholder);
    const int64_t offset = static_cast<int64_t>(current_address)
                           - (static_cast<int64_t>(offset_placeholder)
                              + static_cast<int64_t>(sizeof(int32_t)));
    assert(offset <= numeric_limits<int32_t>::max());
    assert(offset >= numeric_limits<int32_t>::min());
    code_->Write<int32_t>(static_cast<int32_t>(offset));
  }

  code_->SetPosition(current_address);
}

void SimpleCodeGenerator::Impl::GenerateJumpCmdStart(
    const vector<const VarDefNode*> &flow_local_var_defs) {
  GenerateDestroyLocalVarCmds(flow_local_var_defs);
  code_->Write<CmdId>(CmdId::kDirectJump);
}

SimpleCodeGenerator::Impl::Scope *SimpleCodeGenerator::Impl::GetCurrentScope() {
  assert(!scopes_stack_.empty());
  return scopes_stack_.back();
}

void SimpleCodeGenerator::Impl::GenerateCastCmdIfNeeded(
    const ExprAnalysis &expr_analysis) {
  if (!expr_analysis.GetCastedDataType()) {
    return;
  }

  const CmdId cmd_id = cast_cmd_generator_.Generate(
      *(expr_analysis.GetCastedDataType()), expr_analysis.GetDataType());
  code_->Write<CmdId>(cmd_id);
}

void SimpleCodeGenerator::Impl::GenerateDestroyLocalVarCmds(
    const vector<const VarDefNode*> &local_var_defs) {
  DestroyLocalVarCmdGenerator cmd_generator;

  for (const VarDefNode *var_def: reverse(local_var_defs)) {
    assert(var_def);
    const auto &def_analysis =
        static_cast<const DefAnalysis&>(GetNodeAnalysis(*var_def));
    cmd_generator.Generate(def_analysis.GetDataType(), code_.get());
  }
}

void SimpleCodeGenerator::Impl::GenerateReturnCmd(
    const vector<const VarDefNode*> &flow_local_var_defs) {
  GenerateDestroyLocalVarCmds(flow_local_var_defs);
  code_->Write<CmdId>(CmdId::kReturn);
}

const DataType &SimpleCodeGenerator::Impl::GetExprDataType(
    const ExprAnalysis &expr_analysis) const {
  return expr_analysis.GetCastedDataType()
      ? *(expr_analysis.GetCastedDataType())
      : expr_analysis.GetDataType();
}
}
}
