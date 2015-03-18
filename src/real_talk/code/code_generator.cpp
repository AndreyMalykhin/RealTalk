
#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>
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
#include "real_talk/semantic/data_type_visitor.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
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
using real_talk::parser::Node;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::VarDefAnalysis;
using real_talk::semantic::LitAnalysis;
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

  void GenerateCmdsSegment();
  void GenerateImportsSegment();
  void GenerateIdsSegment(const vector<string> &ids);
  void GenerateIdAddressesSegment(const vector<IdAddress> &id_addresses);
  void GenerateMetadataSegments(uint32_t segments_metadata_address,
                             uint32_t cmds_address,
                             uint32_t cmds_size);
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
  const NodeSemanticAnalysis &GetNodeAnalysis(const Node &node) const;

  const ProgramNode *program_;
  const SemanticAnalysis *semantic_analysis_;
  Code *code_;
  vector<path> import_file_paths_;
  vector<string> ids_of_global_var_defs_;
  vector<string> ids_of_native_func_defs_;
  vector<IdAddress> id_addresses_of_func_defs_;
  vector<IdAddress> id_addresses_of_global_var_refs_;
  vector<IdAddress> id_addresses_of_func_refs_;
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
  const uint32_t cmds_address = code.GetPosition();
  GenerateCmdsSegment();
  const uint32_t cmds_size = code.GetPosition() - cmds_address;
  GenerateMetadataSegments(segments_metadata_address, cmds_address, cmds_size);

  stream.exceptions(ios::failbit | ios::badbit);
  stream.write(reinterpret_cast<char*>(code.GetData()),
               code.GetSize());

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

void CodeGenerator::Impl::VisitVarDefWithoutInit(
    const VarDefWithoutInitNode &node) {
  const VarDefAnalysis &var_def_analysis =
      static_cast<const VarDefAnalysis&>(GetNodeAnalysis(node));

  if (var_def_analysis.GetStorage() == DataStorage::kGlobal) {
    CreateGlobalVarCmdGenerator().Generate(
        var_def_analysis.GetDataType(), code_);
    ids_of_global_var_defs_.push_back(node.GetNameToken().GetValue());
  } else if (var_def_analysis.GetStorage() == DataStorage::kLocal) {
    CreateLocalVarCmdGenerator().Generate(
        var_def_analysis.GetDataType(), code_);
  } else {
    assert(false);
  }
}

void CodeGenerator::Impl::VisitVarDefWithInit(
    const VarDefWithInitNode&) {}

void CodeGenerator::Impl::VisitExprStmt(const ExprStmtNode &node) {
  node.GetExpr()->Accept(*this);
  // TODO mb it's not always needed
  code_->WriteCmdId(CmdId::kUnload);
}

void CodeGenerator::Impl::VisitReturnValue(const ReturnValueNode&) {}

void CodeGenerator::Impl::VisitReturnWithoutValue(
    const ReturnWithoutValueNode&) {}

void CodeGenerator::Impl::VisitArgDef(const ArgDefNode&) {}

void CodeGenerator::Impl::VisitAnd(const AndNode&) {}

void CodeGenerator::Impl::VisitArrayAllocWithoutInit(
    const ArrayAllocWithoutInitNode&) {}

void CodeGenerator::Impl::VisitArrayAllocWithInit(
    const ArrayAllocWithInitNode&) {}

void CodeGenerator::Impl::VisitAssign(const AssignNode&) {}

void CodeGenerator::Impl::VisitBreak(const BreakNode&) {}

void CodeGenerator::Impl::VisitCall(const CallNode&) {}

void CodeGenerator::Impl::VisitContinue(const ContinueNode&) {}

void CodeGenerator::Impl::VisitDiv(const DivNode&) {}

void CodeGenerator::Impl::VisitEqual(const EqualNode&) {}

void CodeGenerator::Impl::VisitFuncDefWithBody(
    const FuncDefWithBodyNode&) {}

void CodeGenerator::Impl::VisitFuncDefWithoutBody(
    const FuncDefWithoutBodyNode&) {}

void CodeGenerator::Impl::VisitGreater(const GreaterNode&) {}

void CodeGenerator::Impl::VisitGreaterOrEqual(const GreaterOrEqualNode&) {}

void CodeGenerator::Impl::VisitIfElseIfElse(const IfElseIfElseNode&) {}

void CodeGenerator::Impl::VisitIfElseIf(const IfElseIfNode&) {}

void CodeGenerator::Impl::VisitImport(const ImportNode&) {}

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

void CodeGenerator::Impl::VisitPreTestLoop(const PreTestLoopNode&) {}

void CodeGenerator::Impl::VisitSubscript(const SubscriptNode&) {}

void CodeGenerator::Impl::VisitSub(const SubNode&) {}

void CodeGenerator::Impl::VisitSum(const SumNode&) {}

void CodeGenerator::Impl::VisitId(const IdNode&) {}

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
}
}
