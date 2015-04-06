
#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/range/adaptor/reversed.hpp>
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
#include "real_talk/semantic/data_type_visitor.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/scope_analysis.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"
#include "real_talk/semantic/arg_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/char_lit.h"
#include "real_talk/semantic/string_lit.h"
#include "real_talk/semantic/double_lit.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/code.h"

using std::unique_ptr;
using std::ios;
using std::ostream;
using std::streampos;
using std::streamoff;
using std::string;
using std::vector;
using std::numeric_limits;
using boost::numeric_cast;
using boost::filesystem::path;
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
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::VarDefAnalysis;
using real_talk::semantic::LitAnalysis;
using real_talk::semantic::CommonExprAnalysis;
using real_talk::semantic::ScopeAnalysis;
using real_talk::semantic::ControlFlowTransferAnalysis;
using real_talk::semantic::ArgDefAnalysis;
using real_talk::semantic::FuncDefAnalysis;
using real_talk::semantic::ExprAnalysis;
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
using real_talk::semantic::DataStorage;
using real_talk::semantic::IntLit;
using real_talk::semantic::LongLit;
using real_talk::semantic::BoolLit;
using real_talk::semantic::CharLit;
using real_talk::semantic::StringLit;
using real_talk::semantic::DoubleLit;

namespace real_talk {
namespace code {

class CodeGenerator::Impl: private NodeVisitor {
 public:
  void Generate(const ProgramNode &program,
                const SemanticAnalysis &semantic_analysis,
                uint32_t version,
                ostream &stream);

 private:
  class StmtGrouper;
  class CreateGlobalVarCmdGenerator;
  class CreateLocalVarCmdGenerator;
  class CreateAndInitGlobalVarCmdGenerator;
  class CreateAndInitLocalVarCmdGenerator;
  class CreateArrayCmdGenerator;
  class CreateAndInitArrayCmdGenerator;
  class ReturnValueCmdGenerator;
  class Scope;

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
  template<typename TCreateGlobalVarCmdGenerator,
           typename TCreateLocalVarCmdGenerator>
  void VisitVarDef(const VarDefNode &node);
  void VisitIf(const IfNode &node, uint32_t *branch_end_address_placeholder);
  void WriteCurrentCmdAddress(const vector<uint32_t> &address_placeholders);
  void WriteJumpCmdStart(uint32_t local_vars_count);
  const NodeSemanticAnalysis &GetNodeAnalysis(const Node &node) const;
  uint32_t GetCurrentCmdAddress() const;
  Scope *GetCurrentScope();
  void GenerateCmdsSegment();
  void GenerateImportsSegment();
  void GenerateIdsSegment(const vector<string> &ids);
  void GenerateIdAddressesSegment(const vector<IdAddress> &id_addresses);
  void GenerateMetadataSegments(uint32_t segments_metadata_address,
                                uint32_t cmds_address,
                                uint32_t cmds_size);

  const ProgramNode *program_;
  const SemanticAnalysis *semantic_analysis_;
  Code *code_;
  vector<path> import_file_paths_;
  vector<string> ids_of_global_var_defs_;
  vector<string> ids_of_native_func_defs_;
  vector<IdAddress> id_addresses_of_func_defs_;
  vector<IdAddress> id_addresses_of_global_var_refs_;
  vector<IdAddress> id_addresses_of_func_refs_;
  vector<Scope*> scopes_stack_;
  uint32_t cmds_address_;
};

class CodeGenerator::Impl::Scope {
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

class CodeGenerator::Impl::StmtGrouper: private NodeVisitor {
 public:
  struct GroupedStmts {
    vector<const StmtNode*> non_func_defs;
    vector<const FuncDefNode*> func_defs;
  };

  GroupedStmts Group(const ProgramNode *program) {
    program->Accept(*this);
    vector<const StmtNode*> non_func_defs;
    non_func_defs.swap(non_func_defs_);
    vector<const FuncDefNode*> func_defs;
    func_defs.swap(func_defs_);
    const GroupedStmts grouped_stmts = {move(non_func_defs), move(func_defs)};
    return grouped_stmts;
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

class CodeGenerator::Impl::CreateGlobalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
    code_->WriteUint32(numeric_limits<uint32_t>::max());
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalArrayVar);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kCreateGlobalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class CodeGenerator::Impl::CreateAndInitGlobalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
    code_->WriteUint32(numeric_limits<uint32_t>::max());
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalArrayVar);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitGlobalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class CodeGenerator::Impl::CreateAndInitLocalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalArrayVar);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLocalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class CodeGenerator::Impl::CreateLocalVarCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalArrayVar);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalBoolVar);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalIntVar);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalLongVar);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalDoubleVar);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalCharVar);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLocalStringVar);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class CodeGenerator::Impl::CreateArrayCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(
      const DataType &data_type, uint8_t dimensions_count, Code *code) {
    code_ = code;
    data_type.Accept(*this);
    code_->WriteUint8(dimensions_count);
  }

 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kCreateBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kCreateIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kCreateLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kCreateDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kCreateCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kCreateStringArray);
  }

  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class CodeGenerator::Impl::CreateAndInitArrayCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type,
                uint8_t dimensions_count,
                uint32_t values_count,
                Code *code) {
    code_ = code;
    data_type.Accept(*this);
    code_->WriteUint8(dimensions_count);
    code_->WriteUint32(values_count);
  }

 private:
  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitBoolArray);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitIntArray);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitLongArray);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitDoubleArray);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitCharArray);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kCreateAndInitStringArray);
  }

  virtual void VisitArray(const ArrayDataType &data_type) override {
    data_type.GetElementDataType().Accept(*this);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

class CodeGenerator::Impl::ReturnValueCmdGenerator
    : private DataTypeVisitor {
 public:
  void Generate(const DataType &data_type, Code *code) {
    code_ = code;
    data_type.Accept(*this);
  }

 private:
  virtual void VisitArray(const ArrayDataType&) override {
    code_->WriteCmdId(CmdId::kReturnArrayValue);
  }

  virtual void VisitBool(const BoolDataType&) override {
    code_->WriteCmdId(CmdId::kReturnBoolValue);
  }

  virtual void VisitInt(const IntDataType&) override {
    code_->WriteCmdId(CmdId::kReturnIntValue);
  }

  virtual void VisitLong(const LongDataType&) override {
    code_->WriteCmdId(CmdId::kReturnLongValue);
  }

  virtual void VisitDouble(const DoubleDataType&) override {
    code_->WriteCmdId(CmdId::kReturnDoubleValue);
  }

  virtual void VisitChar(const CharDataType&) override {
    code_->WriteCmdId(CmdId::kReturnCharValue);
  }

  virtual void VisitString(const StringDataType&) override {
    code_->WriteCmdId(CmdId::kReturnStringValue);
  }

  virtual void VisitVoid(const VoidDataType&) override {assert(false);}
  virtual void VisitFunc(const FuncDataType&) override {assert(false);}

  Code *code_;
};

CodeGenerator::CodeGenerator(): impl_(new Impl()) {}

CodeGenerator::~CodeGenerator() {}

void CodeGenerator::Generate(const ProgramNode &program,
                             const SemanticAnalysis &semantic_analysis,
                             uint32_t version,
                             ostream &stream) {
  impl_->Generate(program, semantic_analysis, version, stream);
}

void CodeGenerator::Impl::Generate(
    const ProgramNode &program,
    const SemanticAnalysis &semantic_analysis,
    uint32_t version,
    ostream &stream) {
  program_ = &program;
  semantic_analysis_ = &semantic_analysis;
  Code code;
  code_ = &code;

  code.WriteUint32(version);
  const uint32_t segments_metadata_address = code.GetPosition();
  code.Skip(14 * sizeof(uint32_t));
  cmds_address_ = code.GetPosition();
  GenerateCmdsSegment();
  const uint32_t cmds_size = code.GetPosition() - cmds_address_;
  GenerateMetadataSegments(segments_metadata_address, cmds_address_, cmds_size);

  stream.exceptions(ios::failbit | ios::badbit);
  stream.write(reinterpret_cast<char*>(code.GetData()), code.GetSize());

  assert(scopes_stack_.empty());
  import_file_paths_.clear();
  ids_of_global_var_defs_.clear();
  id_addresses_of_func_defs_.clear();
  ids_of_native_func_defs_.clear();
  id_addresses_of_global_var_refs_.clear();
  id_addresses_of_func_refs_.clear();
}

void CodeGenerator::Impl::GenerateCmdsSegment() {
  const StmtGrouper::GroupedStmts &stmts = StmtGrouper().Group(program_);

  for (const StmtNode *non_func_def: stmts.non_func_defs) {
    non_func_def->Accept(*this);
  }

  code_->WriteCmdId(CmdId::kEndMain);

  for (const FuncDefNode *func_def: stmts.func_defs) {
    func_def->Accept(*this);
  }

  code_->WriteCmdId(CmdId::kEndFuncs);
}

void CodeGenerator::Impl::GenerateMetadataSegments(
    uint32_t segments_metadata_address,
    uint32_t cmds_address,
    uint32_t cmds_size) {
  const uint32_t imports_metadata_address = code_->GetPosition();
  GenerateImportsSegment();
  const uint32_t imports_metadata_size =
      code_->GetPosition() - imports_metadata_address;

  const uint32_t global_var_defs_metadata_address = code_->GetPosition();
  GenerateIdsSegment(ids_of_global_var_defs_);
  const uint32_t global_var_defs_metadata_size =
      code_->GetPosition() - global_var_defs_metadata_address;

  const uint32_t func_defs_metadata_address = code_->GetPosition();
  GenerateIdAddressesSegment(id_addresses_of_func_defs_);
  const uint32_t func_defs_metadata_size =
      code_->GetPosition() - func_defs_metadata_address;

  const uint32_t native_func_defs_metadata_address = code_->GetPosition();
  GenerateIdsSegment(ids_of_native_func_defs_);
  const uint32_t native_func_defs_metadata_size =
      code_->GetPosition() - native_func_defs_metadata_address;

  const uint32_t global_var_refs_metadata_address = code_->GetPosition();
  GenerateIdAddressesSegment(id_addresses_of_global_var_refs_);
  const uint32_t global_var_refs_metadata_size =
      code_->GetPosition() - global_var_refs_metadata_address;

  const uint32_t func_refs_metadata_address = code_->GetPosition();
  GenerateIdAddressesSegment(id_addresses_of_func_refs_);
  const uint32_t func_refs_metadata_size =
      code_->GetPosition() - func_refs_metadata_address;

  code_->SetPosition(segments_metadata_address);
  code_->WriteUint32(cmds_address);
  code_->WriteUint32(cmds_size);
  code_->WriteUint32(imports_metadata_address);
  code_->WriteUint32(imports_metadata_size);
  code_->WriteUint32(global_var_defs_metadata_address);
  code_->WriteUint32(global_var_defs_metadata_size);
  code_->WriteUint32(func_defs_metadata_address);
  code_->WriteUint32(func_defs_metadata_size);
  code_->WriteUint32(native_func_defs_metadata_address);
  code_->WriteUint32(native_func_defs_metadata_size);
  code_->WriteUint32(global_var_refs_metadata_address);
  code_->WriteUint32(global_var_refs_metadata_size);
  code_->WriteUint32(func_refs_metadata_address);
  code_->WriteUint32(func_refs_metadata_size);
}

void CodeGenerator::Impl::GenerateImportsSegment() {
  for (const path &import_file_path: import_file_paths_) {
    code_->WriteFilePath(import_file_path);
  }
}

void CodeGenerator::Impl::GenerateIdsSegment(const vector<string> &ids) {
  for (const string &id: ids) {
    code_->WriteString(id);
  }
}

void CodeGenerator::Impl::GenerateIdAddressesSegment(
    const vector<IdAddress> &id_addresses) {
  for (const IdAddress &id_address: id_addresses) {
    code_->WriteIdAddress(id_address);
  }
}

void CodeGenerator::Impl::VisitProgram(const ProgramNode &node) {
  for (const unique_ptr<StmtNode> &stmt: node.GetStmts()) {
    stmt->Accept(*this);
  }
}

void CodeGenerator::Impl::VisitImport(const ImportNode &node) {
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(*node.GetFilePath()));
  const path file_path(
      static_cast<const StringLit&>(lit_analysis.GetLit()).GetValue());
  import_file_paths_.push_back(file_path);
}

void CodeGenerator::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &node) {
  VisitVarDef<CreateGlobalVarCmdGenerator, CreateLocalVarCmdGenerator>(node);
}

void CodeGenerator::Impl::VisitVarDefWithInit(const VarDefWithInitNode &node) {
  node.GetValue()->Accept(*this);
  VisitVarDef<CreateAndInitGlobalVarCmdGenerator,
              CreateAndInitLocalVarCmdGenerator>(node);
}

template<typename TCreateGlobalVarCmdGenerator,
         typename TCreateLocalVarCmdGenerator>
void CodeGenerator::Impl::VisitVarDef(const VarDefNode &node) {
  const VarDefAnalysis &var_def_analysis =
      static_cast<const VarDefAnalysis&>(GetNodeAnalysis(node));

  if (var_def_analysis.GetStorage() == DataStorage::kGlobal) {
    TCreateGlobalVarCmdGenerator().Generate(
        var_def_analysis.GetDataType(), code_);
    ids_of_global_var_defs_.push_back(node.GetNameToken().GetValue());
  } else if (var_def_analysis.GetStorage() == DataStorage::kLocal) {
    TCreateLocalVarCmdGenerator().Generate(
        var_def_analysis.GetDataType(), code_);
  } else {
    assert(false);
  }
}

void CodeGenerator::Impl::VisitExprStmt(const ExprStmtNode &node) {
  node.GetExpr()->Accept(*this);
  // TODO mb it's not always needed
  code_->WriteCmdId(CmdId::kUnload);
}

void CodeGenerator::Impl::VisitPreTestLoop(const PreTestLoopNode &node) {
  const uint32_t start_address = GetCurrentCmdAddress();
  node.GetCond()->Accept(*this);
  Scope scope(start_address, scopes_stack_);
  code_->WriteCmdId(CmdId::kJumpIfNot);
  const uint32_t end_address_placeholder = code_->GetPosition();
  scope.GetEndAddressPlaceholders().push_back(end_address_placeholder);
  code_->Skip(sizeof(uint32_t));
  node.GetBody()->Accept(*this);
  const ScopeAnalysis &body_analysis = static_cast<const ScopeAnalysis&>(
      GetNodeAnalysis(*(node.GetBody())));
  WriteJumpCmdStart(body_analysis.GetLocalVarsCount());
  code_->WriteUint32(start_address);
  WriteCurrentCmdAddress(scope.GetEndAddressPlaceholders());
}

void CodeGenerator::Impl::VisitBreak(const BreakNode &node) {
  const ControlFlowTransferAnalysis &break_analysis =
      static_cast<const ControlFlowTransferAnalysis&>(GetNodeAnalysis(node));
  WriteJumpCmdStart(break_analysis.GetFlowLocalVarsCount());
  const uint32_t flow_end_address_placeholder = code_->GetPosition();
  GetCurrentScope()->GetEndAddressPlaceholders().push_back(
      flow_end_address_placeholder);
  code_->Skip(sizeof(uint32_t));
}

void CodeGenerator::Impl::VisitContinue(const ContinueNode &node) {
  const ControlFlowTransferAnalysis &continue_analysis =
      static_cast<const ControlFlowTransferAnalysis&>(GetNodeAnalysis(node));
  WriteJumpCmdStart(continue_analysis.GetFlowLocalVarsCount());
  code_->WriteUint32(GetCurrentScope()->GetStartAddress());
}

void CodeGenerator::Impl::VisitIfElseIfElse(
    const IfElseIfElseNode &if_else_if_else) {
  vector<uint32_t> branch_end_address_placeholders;
  uint32_t branch_end_address_placeholder;
  VisitIf(*(if_else_if_else.GetIf()), &branch_end_address_placeholder);
  branch_end_address_placeholders.push_back(branch_end_address_placeholder);

  for (const unique_ptr<ElseIfNode> &else_if: if_else_if_else.GetElseIfs()) {
    VisitIf(*(else_if->GetIf()), &branch_end_address_placeholder);
    branch_end_address_placeholders.push_back(branch_end_address_placeholder);
  }

  if_else_if_else.GetElseBody()->Accept(*this);
  const ScopeAnalysis &else_body_analysis = static_cast<const ScopeAnalysis&>(
      GetNodeAnalysis(*if_else_if_else.GetElseBody()));

  if (else_body_analysis.GetLocalVarsCount() > 0) {
    code_->WriteCmdId(CmdId::kDestroyLocalVars);
    code_->WriteUint32(else_body_analysis.GetLocalVarsCount());
  }

  WriteCurrentCmdAddress(branch_end_address_placeholders);
}

void CodeGenerator::Impl::VisitIfElseIf(const IfElseIfNode &if_else_if) {
  if (if_else_if.GetElseIfs().empty()) {
    VisitIf(*if_else_if.GetIf(), nullptr);
    return;
  }

  vector<uint32_t> branch_end_address_placeholders;
  uint32_t branch_end_address_placeholder;
  VisitIf(*(if_else_if.GetIf()), &branch_end_address_placeholder);
  branch_end_address_placeholders.push_back(branch_end_address_placeholder);
  auto last_else_if_it = if_else_if.GetElseIfs().cend() - 1;

  for (auto else_if_it = if_else_if.GetElseIfs().cbegin();
       else_if_it != last_else_if_it;
       ++else_if_it) {
    const unique_ptr<IfNode> &if_node = (**else_if_it).GetIf();
    VisitIf(*if_node, &branch_end_address_placeholder);
    branch_end_address_placeholders.push_back(branch_end_address_placeholder);
  }

  const unique_ptr<IfNode> &if_node = (**last_else_if_it).GetIf();
  VisitIf(*if_node, nullptr);
  WriteCurrentCmdAddress(branch_end_address_placeholders);
}

/**
 * @param branch_end_address_placeholder If not null, generates jump to the end
 * of branch and stores here address of placeholder, that needs to be filled
 * with the address of branch end
 */
void CodeGenerator::Impl::VisitIf(
    const IfNode &node, uint32_t *branch_end_address_placeholder) {
  node.GetCond()->Accept(*this);
  code_->WriteCmdId(CmdId::kJumpIfNot);
  const uint32_t jump_address_placeholder = code_->GetPosition();
  code_->Skip(sizeof(uint32_t));
  node.GetBody()->Accept(*this);
  const ScopeAnalysis &body_analysis =
      static_cast<const ScopeAnalysis&>(GetNodeAnalysis(*node.GetBody()));

  if (branch_end_address_placeholder == nullptr) {
    if (body_analysis.GetLocalVarsCount() > 0) {
      code_->WriteCmdId(CmdId::kDestroyLocalVars);
      code_->WriteUint32(body_analysis.GetLocalVarsCount());
    }
  } else {
    WriteJumpCmdStart(body_analysis.GetLocalVarsCount());
    *branch_end_address_placeholder = code_->GetPosition();
    code_->Skip(sizeof(uint32_t));
  }

  WriteCurrentCmdAddress(vector<uint32_t>({jump_address_placeholder}));
}

void CodeGenerator::Impl::VisitFuncDefWithBody(
    const FuncDefWithBodyNode &node) {
  const uint32_t start_address = GetCurrentCmdAddress();

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
    code_->WriteCmdId(CmdId::kReturn);
  }

  const string &id = node.GetNameToken().GetValue();
  id_addresses_of_func_defs_.push_back(IdAddress(id, start_address));
}

void CodeGenerator::Impl::VisitFuncDefWithoutBody(
    const FuncDefWithoutBodyNode &node) {
  const string &id = node.GetNameToken().GetValue();
  ids_of_native_func_defs_.push_back(id);
}

void CodeGenerator::Impl::VisitArgDef(const ArgDefNode &node) {
  const ArgDefAnalysis &analysis =
      static_cast<const ArgDefAnalysis&>(GetNodeAnalysis(node));
  CreateAndInitLocalVarCmdGenerator().Generate(analysis.GetDataType(), code_);
}

void CodeGenerator::Impl::VisitReturnValue(const ReturnValueNode &node) {
  node.GetValue()->Accept(*this);
  const ExprAnalysis &analysis =
      static_cast<const ExprAnalysis&>(GetNodeAnalysis(*(node.GetValue())));
  ReturnValueCmdGenerator().Generate(analysis.GetDataType(), code_);
}

void CodeGenerator::Impl::VisitReturnWithoutValue(
    const ReturnWithoutValueNode&) {
  code_->WriteCmdId(CmdId::kReturn);
}

void CodeGenerator::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode &node) {
  for (const std::unique_ptr<BoundedArraySizeNode> &size
           : reverse(node.GetSizes())) {
    size->GetValue()->Accept(*this);
  }

  const CommonExprAnalysis &array_alloc_analysis =
      static_cast<const CommonExprAnalysis&>(GetNodeAnalysis(node));
  const size_t dimensions_count = node.GetSizes().size();
  assert(dimensions_count <= numeric_limits<uint8_t>::max());
  CreateArrayCmdGenerator().Generate(array_alloc_analysis.GetDataType(),
                                     static_cast<uint8_t>(dimensions_count),
                                     code_);
}

void CodeGenerator::Impl::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode &node) {
  for (const unique_ptr<ExprNode> &value: reverse(node.GetValues())) {
    value->Accept(*this);
  }

  const CommonExprAnalysis &array_alloc_analysis =
      static_cast<const CommonExprAnalysis&>(GetNodeAnalysis(node));
  const size_t dimensions_count = node.GetSizes().size();
  assert(dimensions_count <= numeric_limits<uint8_t>::max());
  const size_t values_count = node.GetValues().size();
  assert(values_count <= numeric_limits<uint32_t>::max());
  CreateAndInitArrayCmdGenerator().Generate(
      array_alloc_analysis.GetDataType(),
      static_cast<uint8_t>(dimensions_count),
      static_cast<uint32_t>(values_count),
      code_);
}

void CodeGenerator::Impl::VisitId(const IdNode&) {
  // const string &id = node.GetNameToken().GetValue();
  // const IdAnalysis &id_analysis =
  //     static_cast<const IdAnalysis&>(GetNodeAnalysis(id_node));
  // const DefNode *def_node = id_analysis.GetDef();

  // if (def_node->GetType() == DefType::kVar) {
  //   const VarDefAnalysis &var_def_analysis =
  //       static_cast<const VarDefAnalysis&>(GetNodeAnalysis(*def_node));

  //   if (var_def_analysis.GetStorage() == DataStorage::kGlobal) {
  //     if (id_analysis.IsAssignee()) {
  //       LoadGlobalVarValueCmdGenerator().Generate(
  //           var_def_analysis.GetDataType(),
  //           code_,
  //           id_addresses_of_global_var_refs_);
  //     } else {
  //       assert(false);
  //     }
  //   } else if (var_def_analysis.GetStorage() == DataStorage::kLocal) {
  //     assert(false);

  //     if (id_analysis.IsAssignee()) {
  //       assert(false);
  //     } else {
  //       assert(false);
  //     }
  //   } else {
  //     assert(false);
  //   }
  // } else if (def_node->GetType() == DefType::kArg) {
  //   assert(false);

  //   if (id_analysis.IsAssignee()) {
  //     assert(false);
  //   } else {
  //     assert(false);
  //   }
  // } else if (def_node->GetType() == DefType::kFunc) {
  //   assert(false);
  // } else {
  //   assert(false);
  // }
}

void CodeGenerator::Impl::VisitSubscript(const SubscriptNode&) {}

void CodeGenerator::Impl::VisitCall(const CallNode&) {}

void CodeGenerator::Impl::VisitAssign(const AssignNode&) {}

void CodeGenerator::Impl::VisitAnd(const AndNode&) {}

void CodeGenerator::Impl::VisitDiv(const DivNode&) {}

void CodeGenerator::Impl::VisitEqual(const EqualNode&) {}

void CodeGenerator::Impl::VisitGreater(const GreaterNode&) {}

void CodeGenerator::Impl::VisitGreaterOrEqual(const GreaterOrEqualNode&) {}

void CodeGenerator::Impl::VisitInt(const IntNode &node) {
  code_->WriteCmdId(CmdId::kLoadIntValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const int32_t value =
      static_cast<const IntLit&>(lit_analysis.GetLit()).GetValue();
  code_->WriteInt32(value);
}

void CodeGenerator::Impl::VisitLong(const LongNode &node) {
  code_->WriteCmdId(CmdId::kLoadLongValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const int64_t value =
      static_cast<const LongLit&>(lit_analysis.GetLit()).GetValue();
  code_->WriteInt64(value);
}

void CodeGenerator::Impl::VisitBool(const BoolNode &node) {
  code_->WriteCmdId(CmdId::kLoadBoolValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const bool value =
      static_cast<const BoolLit&>(lit_analysis.GetLit()).GetValue();
  code_->WriteBool(value);
}

void CodeGenerator::Impl::VisitChar(const CharNode &node) {
  code_->WriteCmdId(CmdId::kLoadCharValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const char value =
      static_cast<const CharLit&>(lit_analysis.GetLit()).GetValue();
  code_->WriteChar(value);
}

void CodeGenerator::Impl::VisitString(const StringNode &node) {
  code_->WriteCmdId(CmdId::kLoadStringValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const string &value =
      static_cast<const StringLit&>(lit_analysis.GetLit()).GetValue();
  code_->WriteString(value);
}

void CodeGenerator::Impl::VisitDouble(const DoubleNode &node) {
  code_->WriteCmdId(CmdId::kLoadDoubleValue);
  const LitAnalysis &lit_analysis =
      static_cast<const LitAnalysis&>(GetNodeAnalysis(node));
  const double &value =
      static_cast<const DoubleLit&>(lit_analysis.GetLit()).GetValue();
  code_->WriteDouble(value);
}

void CodeGenerator::Impl::VisitLess(const LessNode&) {}

void CodeGenerator::Impl::VisitLessOrEqual(const LessOrEqualNode&) {}

void CodeGenerator::Impl::VisitMul(const MulNode&) {}

void CodeGenerator::Impl::VisitNegative(const NegativeNode&) {}

void CodeGenerator::Impl::VisitNotEqual(const NotEqualNode&) {}

void CodeGenerator::Impl::VisitNot(const NotNode&) {}

void CodeGenerator::Impl::VisitOr(const OrNode&) {}

void CodeGenerator::Impl::VisitPreDec(const PreDecNode&) {}

void CodeGenerator::Impl::VisitPreInc(const PreIncNode&) {}

void CodeGenerator::Impl::VisitSub(const SubNode&) {}

void CodeGenerator::Impl::VisitSum(const SumNode&) {}

void CodeGenerator::Impl::VisitScope(const ScopeNode &node) {
  for (const unique_ptr<StmtNode> &stmt: node.GetStmts()) {
    stmt->Accept(*this);
  }
}

void CodeGenerator::Impl::VisitIntDataType(const IntDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitLongDataType(const LongDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitDoubleDataType(const DoubleDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitCharDataType(const CharDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitStringDataType(const StringDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitBoolDataType(const BoolDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitVoidDataType(const VoidDataTypeNode&) {
  assert(false);
}

void CodeGenerator::Impl::VisitArrayDataType(const ArrayDataTypeNode&) {
  assert(false);
}

const NodeSemanticAnalysis &CodeGenerator::Impl::GetNodeAnalysis(
    const Node &node) const {
  SemanticAnalysis::NodeAnalyzes::const_iterator node_analysis_it =
      semantic_analysis_->GetNodeAnalyzes().find(&node);
  assert(node_analysis_it != semantic_analysis_->GetNodeAnalyzes().cend());
  return *(node_analysis_it->second);
}

uint32_t CodeGenerator::Impl::GetCurrentCmdAddress() const {
  assert(code_->GetPosition() >= cmds_address_);
  return code_->GetPosition() - cmds_address_;
}

void CodeGenerator::Impl::WriteCurrentCmdAddress(
    const vector<uint32_t> &address_placeholders) {
  const uint32_t current_address = GetCurrentCmdAddress();
  const uint32_t continue_address = code_->GetPosition();

  for (const uint32_t address_placeholder: address_placeholders) {
    code_->SetPosition(address_placeholder);
    code_->WriteUint32(current_address);
  }

  code_->SetPosition(continue_address);
}

void CodeGenerator::Impl::WriteJumpCmdStart(uint32_t local_vars_count) {
  if (local_vars_count > UINT32_C(0)) {
    code_->WriteCmdId(CmdId::kDestroyLocalVarsAndJump);
    code_->WriteUint32(local_vars_count);
  } else {
    code_->WriteCmdId(CmdId::kDirectJump);
  }
}

CodeGenerator::Impl::Scope *CodeGenerator::Impl::GetCurrentScope() {
  assert(!scopes_stack_.empty());
  return scopes_stack_.back();
}
}
}
