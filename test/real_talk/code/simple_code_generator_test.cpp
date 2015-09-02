
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <limits>
#include <iterator>
#include "real_talk/parser/if_else_if_else_node.h"
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/long_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/double_data_type_node.h"
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/void_data_type_node.h"
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
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/local_var_def_analysis.h"
#include "real_talk/semantic/global_var_def_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/scope_analysis.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/return_analysis.h"
#include "real_talk/semantic/subscript_analysis.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/func_data_type.h"
#include "real_talk/semantic/value_type.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/string_lit.h"
#include "real_talk/semantic/double_lit.h"
#include "real_talk/semantic/char_lit.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/module.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/simple_code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/code/cast_cmd_generator.h"
#include "real_talk/code/data_type_size.h"

using std::make_move_iterator;
using std::move;
using std::numeric_limits;
using std::memcmp;
using std::unique_ptr;
using std::vector;
using std::ostringstream;
using std::stringstream;
using std::string;
using std::istream;
using std::ostream;
using std::ios;
using std::streampos;
using std::streamoff;
using std::equal;
using testing::Test;
using testing::Throw;
using testing::_;
using testing::Return;
using testing::Ref;
using testing::ByRef;
using testing::Eq;
using boost::numeric_cast;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;
using real_talk::parser::StmtNode;
using real_talk::parser::VarDefNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::ProgramNode;
using real_talk::parser::IntNode;
using real_talk::parser::LongNode;
using real_talk::parser::BoolNode;
using real_talk::parser::StringNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::CharNode;
using real_talk::parser::ExprStmtNode;
using real_talk::parser::LitNode;
using real_talk::parser::ImportNode;
using real_talk::parser::ExprNode;
using real_talk::parser::PrimitiveDataTypeNode;
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::IfNode;
using real_talk::parser::ElseIfNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::parser::IfElseIfNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::BreakNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::FuncDefWithoutBodyNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::IdNode;
using real_talk::parser::AssignNode;
using real_talk::parser::CallNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::AndNode;
using real_talk::parser::OrNode;
using real_talk::parser::MulNode;
using real_talk::parser::DivNode;
using real_talk::parser::SumNode;
using real_talk::parser::SubNode;
using real_talk::parser::EqualNode;
using real_talk::parser::NotEqualNode;
using real_talk::parser::GreaterNode;
using real_talk::parser::GreaterOrEqualNode;
using real_talk::parser::LessNode;
using real_talk::parser::LessOrEqualNode;
using real_talk::parser::NotNode;
using real_talk::parser::PreDecNode;
using real_talk::parser::PreIncNode;
using real_talk::parser::NegativeNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::LocalVarDefAnalysis;
using real_talk::semantic::GlobalVarDefAnalysis;
using real_talk::semantic::LitAnalysis;
using real_talk::semantic::ExprAnalysis;
using real_talk::semantic::CommonExprAnalysis;
using real_talk::semantic::ScopeAnalysis;
using real_talk::semantic::ControlFlowTransferAnalysis;
using real_talk::semantic::FuncDefAnalysis;
using real_talk::semantic::IdAnalysis;
using real_talk::semantic::ReturnAnalysis;
using real_talk::semantic::SubscriptAnalysis;
using real_talk::semantic::DataType;
using real_talk::semantic::IntDataType;
using real_talk::semantic::LongDataType;
using real_talk::semantic::ArrayDataType;
using real_talk::semantic::DoubleDataType;
using real_talk::semantic::CharDataType;
using real_talk::semantic::StringDataType;
using real_talk::semantic::BoolDataType;
using real_talk::semantic::VoidDataType;
using real_talk::semantic::FuncDataType;
using real_talk::semantic::ValueType;
using real_talk::semantic::Lit;
using real_talk::semantic::IntLit;
using real_talk::semantic::LongLit;
using real_talk::semantic::BoolLit;
using real_talk::semantic::StringLit;
using real_talk::semantic::DoubleLit;
using real_talk::semantic::CharLit;

namespace real_talk {
namespace code {
namespace {

struct TestCast {
  unique_ptr<DataType> dest_data_type;
  unique_ptr<DataType> src_data_type;
  CmdId cmd_id;
};

class CastCmdGeneratorMock: public CastCmdGenerator {
 public:
  MOCK_CONST_METHOD2(Generate, CmdId(const DataType&, const DataType&));
};
}

class SimpleCodeGeneratorTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  string PrintModule(Module &module) {
    stringstream stream;
    module.GetCmdsCode().SetPosition(UINT32_C(0));
    stream << module;
    return stream.str();
  }

  void TestGenerate(vector<TestCast> test_casts,
                    const ProgramNode &program_node,
                    const SemanticAnalysis &semantic_analysis,
                    uint32_t version,
                    Module &expected_module) {
    CastCmdGeneratorMock cast_cmd_generator;

    for (const TestCast &test_cast: test_casts) {
      EXPECT_CALL(cast_cmd_generator,
                  Generate(Eq(ByRef(*(test_cast.dest_data_type))),
                           Eq(ByRef(*(test_cast.src_data_type)))))
          .Times(1)
          .WillOnce(Return(test_cast.cmd_id))
          .RetiresOnSaturation();
    }

    SimpleCodeGenerator generator(cast_cmd_generator);
    Code actual_code;
    unique_ptr<Module> actual_module =
        generator.Generate(program_node, semantic_analysis, version);
    ASSERT_TRUE(actual_module.get());
    ASSERT_EQ(expected_module, *actual_module)
        << "[expected]\n" << PrintModule(expected_module)
        << "\n[actual]\n" << PrintModule(*actual_module);
  }

  void TestGlobalVarDefWithoutInit(unique_ptr<DataTypeNode> data_type_node,
                                   unique_ptr<DataType> data_type,
                                   DataTypeSize expected_var_size,
                                   CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new GlobalVarDefAnalysis(move(data_type)));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(expected_cmd_id);
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->Write<uint32_t>(var_index);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", expected_var_size}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs = {{"var", {var_index_placeholder}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestGlobalVarDefWithInit(
      unique_ptr<DataTypeNode> data_type_node,
      unique_ptr<DataType> data_type,
      unique_ptr<ExprNode> value_node,
      SemanticAnalysis::NodeAnalyzes value_node_analyzes,
      const Code &expected_value_code,
      unique_ptr<TestCast> test_cast,
      DataTypeSize expected_var_size,
      CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
        move(value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(make_move_iterator(value_node_analyzes.begin()),
                         make_move_iterator(value_node_analyzes.end()));
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new GlobalVarDefAnalysis(move(data_type)));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));
    vector<TestCast> test_casts;

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write(expected_value_code.GetData(),
                          expected_value_code.GetSize());

    if (test_cast) {
      cmds_code->Write<CmdId>(test_cast->cmd_id);
      test_casts.push_back(move(*(test_cast.release())));
    }

    cmds_code->Write<CmdId>(expected_cmd_id);
    uint32_t var_index = numeric_limits<uint32_t>::max();
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    cmds_code->Write<uint32_t>(var_index);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", expected_var_size}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs = {{"var", {var_index_placeholder}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestGlobalArrayVarDefWithInit(
      unique_ptr<DataTypeNode> def_data_type_node,
      unique_ptr<PrimitiveDataTypeNode> value_data_type_node,
      unique_ptr<DataType> data_type,
      CmdId create_array_cmd_id,
      CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
    IntNode *size_expr_node_ptr1 = new IntNode(
        TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(9)));
    unique_ptr<ExprNode> size_expr_node1(size_expr_node_ptr1);
    unique_ptr<BoundedArraySizeNode> size_node1(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(8), UINT32_C(8)),
        move(size_expr_node1),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(10), UINT32_C(10))));
    size_nodes.push_back(move(size_node1));
    IntNode *size_expr_node_ptr2 = new IntNode(
        TokenInfo(Token::kIntLit, "2", UINT32_C(12), UINT32_C(12)));
    unique_ptr<ExprNode> size_expr_node2(size_expr_node_ptr2);
    unique_ptr<BoundedArraySizeNode> size_node2(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(11), UINT32_C(11)),
        move(size_expr_node2),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(13), UINT32_C(13))));
    size_nodes.push_back(move(size_node2));
    ArrayAllocWithoutInitNode *value_node_ptr(
        new ArrayAllocWithoutInitNode(
            TokenInfo(Token::kNew, "fresh", UINT32_C(6), UINT32_C(6)),
            move(value_data_type_node),
            move(size_nodes)));
    unique_ptr<ExprNode> value_node(value_node_ptr);

    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(def_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
    VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
        move(array_data_type_node2),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
        move(value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> array_data_type1(
        new ArrayDataType(move(data_type)));
    unique_ptr<DataType> array_data_type2(
        new ArrayDataType(move(array_data_type1)));
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new GlobalVarDefAnalysis(array_data_type2->Clone()));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    unique_ptr<DataType> size_lit_casted_data_type1;
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis1(new LitAnalysis(
        unique_ptr<DataType>(new IntDataType()),
        move(size_lit_casted_data_type1),
        ValueType::kRight,
        unique_ptr<Lit>(new IntLit(INT32_C(1)))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr1, move(size_lit_analysis1)));
    unique_ptr<DataType> size_lit_casted_data_type2;
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis2(new LitAnalysis(
        unique_ptr<DataType>(new IntDataType()),
        move(size_lit_casted_data_type2),
        ValueType::kRight,
        unique_ptr<Lit>(new IntLit(INT32_C(2)))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr2, move(size_lit_analysis2)));
    unique_ptr<DataType> value_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
        array_data_type2->Clone(),
        move(value_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(
        make_pair(value_node_ptr, move(value_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
    cmds_code->Write<int32_t>(INT32_C(2));
    cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
    cmds_code->Write<int32_t>(INT32_C(1));
    cmds_code->Write<CmdId>(create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    cmds_code->Write<uint8_t>(dimensions_count);
    cmds_code->Write<CmdId>(expected_cmd_id);
    uint32_t var_index = numeric_limits<uint32_t>::max();
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", DataTypeSize::kArray}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs = {{"var", {var_index_placeholder}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestLocalArrayVarDefWithInit(
      unique_ptr<DataTypeNode> def_data_type_node,
      unique_ptr<PrimitiveDataTypeNode> value_data_type_node,
      unique_ptr<DataType> data_type,
      CmdId create_array_cmd_id,
      CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
    IntNode *size_expr_node_ptr1 = new IntNode(
        TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(9)));
    unique_ptr<ExprNode> size_expr_node1(size_expr_node_ptr1);
    unique_ptr<BoundedArraySizeNode> size_node1(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(8), UINT32_C(8)),
        move(size_expr_node1),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(10), UINT32_C(10))));
    size_nodes.push_back(move(size_node1));
    IntNode *size_expr_node_ptr2 = new IntNode(
        TokenInfo(Token::kIntLit, "2", UINT32_C(12), UINT32_C(12)));
    unique_ptr<ExprNode> size_expr_node2(size_expr_node_ptr2);
    unique_ptr<BoundedArraySizeNode> size_node2(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(11), UINT32_C(11)),
        move(size_expr_node2),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(13), UINT32_C(13))));
    size_nodes.push_back(move(size_node2));
    ArrayAllocWithoutInitNode *value_node_ptr(
        new ArrayAllocWithoutInitNode(
            TokenInfo(Token::kNew, "fresh", UINT32_C(6), UINT32_C(6)),
            move(value_data_type_node),
            move(size_nodes)));
    unique_ptr<ExprNode> value_node(value_node_ptr);

    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(def_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
    VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
        move(array_data_type_node2),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
        move(value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> array_data_type1(
        new ArrayDataType(move(data_type)));
    unique_ptr<DataType> array_data_type2(
        new ArrayDataType(move(array_data_type1)));
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
        array_data_type2->Clone(), flow_local_var_defs));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    unique_ptr<DataType> size_lit_casted_data_type1;
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis1(new LitAnalysis(
        unique_ptr<DataType>(new IntDataType()),
        move(size_lit_casted_data_type1),
        ValueType::kRight,
        unique_ptr<Lit>(new IntLit(INT32_C(1)))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr1, move(size_lit_analysis1)));
    unique_ptr<DataType> size_lit_casted_data_type2;
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis2(new LitAnalysis(
        unique_ptr<DataType>(new IntDataType()),
        move(size_lit_casted_data_type2),
        ValueType::kRight,
        unique_ptr<Lit>(new IntLit(INT32_C(2)))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr2, move(size_lit_analysis2)));
    unique_ptr<DataType> value_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
        array_data_type2->Clone(),
        move(value_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(
        make_pair(value_node_ptr, move(value_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
    cmds_code->Write<int32_t>(INT32_C(2));
    cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
    cmds_code->Write<int32_t>(INT32_C(1));
    cmds_code->Write<CmdId>(create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    cmds_code->Write<uint8_t>(dimensions_count);
    cmds_code->Write<CmdId>(expected_cmd_id);
    cmds_code->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestLocalVarDefWithInit(
      unique_ptr<DataTypeNode> data_type_node,
      unique_ptr<DataType> data_type,
      unique_ptr<ExprNode> value_node,
      SemanticAnalysis::NodeAnalyzes value_node_analyzes,
      const Code &expected_value_code,
      unique_ptr<TestCast> test_cast,
      CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
        move(value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(make_move_iterator(value_node_analyzes.begin()),
                         make_move_iterator(value_node_analyzes.end()));
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new LocalVarDefAnalysis(move(data_type), flow_local_var_defs));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));
    vector<TestCast> test_casts;

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write(expected_value_code.GetData(),
                          expected_value_code.GetSize());

    if (test_cast) {
      cmds_code->Write<CmdId>(test_cast->cmd_id);
      test_casts.push_back(move(*(test_cast.release())));
    }

    cmds_code->Write<CmdId>(expected_cmd_id);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestLocalVarDefWithoutInit(unique_ptr<DataTypeNode> data_type_node,
                                  unique_ptr<DataType> data_type,
                                  CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new LocalVarDefAnalysis(move(data_type), flow_local_var_defs));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(expected_cmd_id);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestLit(unique_ptr<LitNode> lit_node,
               unique_ptr<LitAnalysis> lit_analysis,
               unique_ptr<Code> expected_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    LitNode *lit_node_ptr = lit_node.get();
    unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
        move(lit_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
    program_stmt_nodes.push_back(move(int_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    uint32_t main_cmds_code_size = expected_code->GetPosition();

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(expected_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestArrayAllocWithoutInit(
      unique_ptr<PrimitiveDataTypeNode> primitive_data_type_node,
      unique_ptr<DataType> primitive_data_type,
      CmdId expected_unload_cmd_id,
      CmdId expected_alloc_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
    CharNode *size_expr_node_ptr1 = new CharNode(
        TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3)));
    unique_ptr<ExprNode> size_expr_node1(size_expr_node_ptr1);
    unique_ptr<BoundedArraySizeNode> size_node1(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
        move(size_expr_node1),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
    size_nodes.push_back(move(size_node1));
    CharNode *size_expr_node_ptr2 = new CharNode(
        TokenInfo(Token::kCharLit, "'b'", UINT32_C(6), UINT32_C(6)));
    unique_ptr<ExprNode> size_expr_node2(size_expr_node_ptr2);
    unique_ptr<BoundedArraySizeNode> size_node2(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(5), UINT32_C(5)),
        move(size_expr_node2),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
    size_nodes.push_back(move(size_node2));
    ArrayAllocWithoutInitNode *array_alloc_node_ptr(
        new ArrayAllocWithoutInitNode(
            TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
            move(primitive_data_type_node),
            move(size_nodes)));
    unique_ptr<ExprNode> array_alloc_node(array_alloc_node_ptr);
    unique_ptr<StmtNode> stmt_node(new ExprStmtNode(
        move(array_alloc_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8))));
    program_stmt_nodes.push_back(move(stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> size_lit_casted_data_type1(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis1(new LitAnalysis(
        unique_ptr<DataType>(new CharDataType()),
        move(size_lit_casted_data_type1),
        ValueType::kRight,
        unique_ptr<Lit>(new CharLit('a'))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr1, move(size_lit_analysis1)));

    unique_ptr<DataType> size_lit_casted_data_type2(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis2(new LitAnalysis(
        unique_ptr<DataType>(new CharDataType()),
        move(size_lit_casted_data_type2),
        ValueType::kRight,
        unique_ptr<Lit>(new CharLit('b'))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr2, move(size_lit_analysis2)));

    unique_ptr<DataType> array_data_type1(
        new ArrayDataType(move(primitive_data_type)));
    unique_ptr<DataType> array_data_type2(
        new ArrayDataType(move(array_data_type1)));
    unique_ptr<DataType> array_alloc_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> array_alloc_analysis(
        new CommonExprAnalysis(move(array_data_type2),
                               move(array_alloc_casted_data_type),
                               ValueType::kRight));
    node_analyzes.insert(
        make_pair(array_alloc_node_ptr, move(array_alloc_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kLoadCharValue);
    cmds_code->Write<char>('b');
    cmds_code->Write<CmdId>(CmdId::kCastCharToInt);
    cmds_code->Write<CmdId>(CmdId::kLoadCharValue);
    cmds_code->Write<char>('a');
    cmds_code->Write<CmdId>(CmdId::kCastCharToInt);
    cmds_code->Write<CmdId>(expected_alloc_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    cmds_code->Write<uint8_t>(dimensions_count);
    cmds_code->Write<CmdId>(expected_unload_cmd_id);
    cmds_code->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<TestCast> test_casts;
    unique_ptr<DataType> dest_data_type(new IntDataType());
    unique_ptr<DataType> src_data_type(new CharDataType());
    TestCast test_cast =
        {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
    test_casts.push_back(move(test_cast));
    unique_ptr<DataType> dest_data_type2(new IntDataType());
    unique_ptr<DataType> src_data_type2(new CharDataType());
    TestCast test_cast2 =
        {move(dest_data_type2), move(src_data_type2), CmdId::kCastCharToInt};
    test_casts.push_back(move(test_cast2));

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestArrayAllocWithInit(
      unique_ptr<PrimitiveDataTypeNode> primitive_data_type_node,
      unique_ptr<DataType> primitive_data_type,
      vector< unique_ptr<ExprNode> > value_nodes,
      SemanticAnalysis::NodeAnalyzes value_analyzes,
      const Code &values_code,
      vector<TestCast> test_casts,
      CmdId expected_unload_cmd_id,
      CmdId expected_alloc_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
    unique_ptr<UnboundedArraySizeNode> size_node1(new UnboundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3))));
    size_nodes.push_back(move(size_node1));
    vector<TokenInfo> value_separator_tokens;

    for (size_t i = 1; i < value_nodes.size(); ++i) {
      value_separator_tokens.push_back(TokenInfo(
          Token::kSeparator,
          ",",
          numeric_cast<uint32_t>(i),
          numeric_cast<uint32_t>(i)));
    }

    int32_t values_count = numeric_cast<int32_t>(value_nodes.size());
    ArrayAllocWithInitNode *array_alloc_node_ptr(new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
        move(primitive_data_type_node),
        move(size_nodes),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
        move(value_nodes),
        value_separator_tokens,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(100), UINT32_C(100))));
    unique_ptr<ExprNode> array_alloc_node(array_alloc_node_ptr);
    unique_ptr<StmtNode> stmt_node(new ExprStmtNode(
        move(array_alloc_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(101), UINT32_C(101))));
    program_stmt_nodes.push_back(move(stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes(move(value_analyzes));
    unique_ptr<DataType> array_data_type(
        new ArrayDataType(move(primitive_data_type)));
    unique_ptr<DataType> array_alloc_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> array_alloc_analysis(
        new CommonExprAnalysis(move(array_data_type),
                               move(array_alloc_casted_data_type),
                               ValueType::kRight));
    node_analyzes.insert(
        make_pair(array_alloc_node_ptr, move(array_alloc_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write(values_code.GetData(), values_code.GetSize());
    cmds_code->Write<CmdId>(expected_alloc_cmd_id);
    uint8_t dimensions_count = UINT8_C(1);
    cmds_code->Write<uint8_t>(dimensions_count);
    cmds_code->Write<int32_t>(values_count);
    cmds_code->Write<CmdId>(expected_unload_cmd_id);
    cmds_code->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestReturnValue(unique_ptr<ExprNode> value_node,
                       unique_ptr<DataTypeNode> return_data_type_node,
                       unique_ptr<DataType> return_data_type,
                       SemanticAnalysis::NodeAnalyzes value_analyzes,
                       vector<TestCast> test_casts,
                       const Code &expected_value_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<StmtNode> > body_stmt_nodes;
    unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
        TokenInfo(Token::kBoolType, "bool", UINT32_C(5), UINT32_C(5))));
    auto *var_def_node_ptr = new VarDefWithoutInitNode(
        move(var_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    body_stmt_nodes.push_back(move(var_def_node));
    unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(8), UINT32_C(8))));
    auto *var_def_node_ptr2 = new VarDefWithoutInitNode(
        move(var_data_type_node2),
        TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
    unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
    body_stmt_nodes.push_back(move(var_def_node2));
    ReturnValueNode *return_node_ptr = new ReturnValueNode(
        TokenInfo(Token::kReturn, "return", UINT32_C(11), UINT32_C(11)),
        move(value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
    unique_ptr<StmtNode> return_node(return_node_ptr);
    body_stmt_nodes.push_back(move(return_node));
    unique_ptr<DataTypeNode> var_data_type_node3(new LongDataTypeNode(
        TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(13))));
    auto *var_def_node_ptr3 = new VarDefWithoutInitNode(
        move(var_data_type_node3),
        TokenInfo(Token::kName, "var3", UINT32_C(14), UINT32_C(14)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)));
    unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
    body_stmt_nodes.push_back(move(var_def_node3));
    ScopeNode *body_node_ptr = new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
        move(body_stmt_nodes),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
    unique_ptr<ScopeNode> body_node(body_node_ptr);

    vector<TokenInfo> modifier_tokens;
    vector<TokenInfo> arg_separator_tokens;
    FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
        modifier_tokens,
        move(return_data_type_node),
        TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
        vector< unique_ptr<ArgDefNode> >(),
        arg_separator_tokens,
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
        move(body_node));
    unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
    program_stmt_nodes.push_back(move(func_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes(move(value_analyzes));
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
        unique_ptr<DataType>(new BoolDataType()), flow_local_var_defs));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
    unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
        unique_ptr<DataType>(new IntDataType()), flow_local_var_defs2));
    node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
    vector<const VarDefNode*> flow_local_var_defs3 =
        {var_def_node_ptr, var_def_node_ptr2};
    unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
        unique_ptr<DataType>(new LongDataType()), flow_local_var_defs3));
    node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
    vector<const VarDefNode*> body_local_vars_defs =
        {var_def_node_ptr, var_def_node_ptr2, var_def_node_ptr3};
    unique_ptr<NodeSemanticAnalysis> body_analysis(
        new ScopeAnalysis(body_local_vars_defs));
    node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));
    vector< unique_ptr<DataType> > arg_data_types;
    bool is_func_native = false;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types), is_func_native));
    bool is_func_has_return = true;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
    vector<const VarDefNode*> flow_local_var_defs4 =
        {var_def_node_ptr, var_def_node_ptr2};
    unique_ptr<NodeSemanticAnalysis> return_analysis(
        new ReturnAnalysis(func_def_node_ptr, flow_local_var_defs4));
    node_analyzes.insert(make_pair(return_node_ptr, move(return_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    uint32_t func_def_address = cmds_code->GetPosition();
    cmds_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
    cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
    cmds_code->Write(expected_value_code.GetData(),
                          expected_value_code.GetSize());
    cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
    cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
    cmds_code->Write<CmdId>(CmdId::kReturn);
    cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs = {{"func", func_def_address}};
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestIdAsNotAssigneeGlobalVar(unique_ptr<DataTypeNode> data_type_node,
                                    const DataType &data_type,
                                    DataTypeSize expected_var_size,
                                    CmdId expected_create_var_cmd_id,
                                    CmdId expected_load_var_cmd_id,
                                    const Code &expected_unload_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    IdNode *id_node_ptr = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
    unique_ptr<ExprNode> id_node(id_node_ptr);
    unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
        move(id_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4))));
    program_stmt_nodes.push_back(move(id_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        data_type.Clone()));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    bool is_id_assignee = false;
    unique_ptr<DataType> casted_data_type;
    unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
        data_type.Clone(),
        move(casted_data_type),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(expected_create_var_cmd_id);
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write<CmdId>(expected_load_var_cmd_id);
    uint32_t var_index_placeholder2 = cmds_code->GetPosition();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(expected_unload_code.GetData(),
                     expected_unload_code.GetSize());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", expected_var_size}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs =
        {{"var", {var_index_placeholder, var_index_placeholder2}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestIdAsNotAssigneeGlobalArrayVar(
      unique_ptr<DataTypeNode> data_type_node,
      const DataType &data_type,
      CmdId expected_load_var_cmd_id,
      CmdId expected_unload_cmd_id) {
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
    unique_ptr<DataType> array_data_type(new ArrayDataType(data_type.Clone()));
    ArrayDataType array_data_type2(move(array_data_type));
    Code expected_unload_code;
    expected_unload_code.Write<CmdId>(expected_unload_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    expected_unload_code.Write<uint8_t>(dimensions_count);
    TestIdAsNotAssigneeGlobalVar(
        move(array_data_type_node2),
        array_data_type2,
        DataTypeSize::kArray,
        CmdId::kCreateGlobalArrayVar,
        expected_load_var_cmd_id,
        expected_unload_code);
  }

  void TestIdAsNotAssigneeLocalVar(unique_ptr<DataTypeNode> data_type_node,
                                   unique_ptr<DataTypeNode> data_type_node2,
                                   const DataType &data_type,
                                   unique_ptr<Code> expected_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(2), UINT32_C(2)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
        move(data_type_node2),
        TokenInfo(Token::kName, "var2", UINT32_C(5), UINT32_C(5)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)));
    unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
    program_stmt_nodes.push_back(move(var_def_node2));
    IdNode *id_node_ptr = new IdNode(
        TokenInfo(Token::kName, "var2", UINT32_C(7), UINT32_C(7)));
    unique_ptr<ExprNode> id_node(id_node_ptr);
    unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
        move(id_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8))));
    program_stmt_nodes.push_back(move(id_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
        data_type.Clone(), flow_local_var_defs));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
    unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
        data_type.Clone(), flow_local_var_defs2));
    node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
    bool is_id_assignee = false;
    unique_ptr<DataType> casted_data_type;
    unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
        data_type.Clone(),
        move(casted_data_type),
        ValueType::kLeft,
        var_def_node_ptr2,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    uint32_t main_cmds_code_size = expected_code->GetPosition();
    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(expected_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestIdAsNotAssigneeLocalArrayVar(
      unique_ptr<DataTypeNode> data_type_node,
      unique_ptr<DataTypeNode> data_type_node2,
      const DataType &data_type,
      CmdId unload_value_cmd_id,
      CmdId expected_load_var_cmd_id) {
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
    unique_ptr<DataTypeNode> array_data_type_node3(new ArrayDataTypeNode(
        move(data_type_node2),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
    unique_ptr<DataTypeNode> array_data_type_node4(new ArrayDataTypeNode(
        move(array_data_type_node3),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(8), UINT32_C(8)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(9), UINT32_C(9))));
    unique_ptr<Code> expected_code(new Code());
    expected_code->Write<CmdId>(CmdId::kCreateLocalArrayVar);
    expected_code->Write<CmdId>(CmdId::kCreateLocalArrayVar);
    expected_code->Write<CmdId>(expected_load_var_cmd_id);
    uint32_t var_index = UINT32_C(1);
    expected_code->Write<uint32_t>(var_index);
    expected_code->Write<CmdId>(unload_value_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    expected_code->Write<uint8_t>(dimensions_count);
    unique_ptr<DataType> array_data_type(new ArrayDataType(data_type.Clone()));
    ArrayDataType array_data_type2(move(array_data_type));
    TestIdAsNotAssigneeLocalVar(
        move(array_data_type_node2),
        move(array_data_type_node4),
        array_data_type2,
        move(expected_code));
  }

  void TestIdAsAssigneeGlobalVar(unique_ptr<DataTypeNode> data_type_node,
                                 unique_ptr<ExprNode> value_node,
                                 const DataType &data_type,
                                 unique_ptr<ExprAnalysis> value_analysis,
                                 vector<TestCast> test_casts,
                                 DataTypeSize expected_var_size,
                                 CmdId expected_create_var_cmd_id,
                                 const Code &expected_value_code,
                                 const Code &expected_store_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    IdNode *id_node_ptr = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
    unique_ptr<ExprNode> id_node(id_node_ptr);
    ExprNode *value_node_ptr = value_node.get();
    AssignNode *assign_node_ptr = new AssignNode(
        TokenInfo(Token::kAssignOp, "=", UINT32_C(4), UINT32_C(4)),
        move(id_node),
        move(value_node));
    unique_ptr<ExprNode> assign_node(assign_node_ptr);
    unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
        move(assign_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
    program_stmt_nodes.push_back(move(id_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new GlobalVarDefAnalysis(data_type.Clone()));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    bool is_id_assignee = true;
    unique_ptr<DataType> id_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
        data_type.Clone(),
        move(id_casted_data_type),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
    unique_ptr<DataType> assign_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> assign_analysis(new CommonExprAnalysis(
        unique_ptr<DataType>(new VoidDataType()),
        move(assign_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(make_pair(assign_node_ptr, move(assign_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(expected_create_var_cmd_id);
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(expected_value_code.GetData(),
                          expected_value_code.GetSize());
    cmds_code->Write<CmdId>(CmdId::kLoadGlobalVarAddress);
    uint32_t var_index_placeholder2 = cmds_code->GetPosition();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(expected_store_code.GetData(),
                          expected_store_code.GetSize());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", expected_var_size}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs =
        {{"var", {var_index_placeholder, var_index_placeholder2}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestIdAsAssigneeLocalVar(unique_ptr<DataTypeNode> data_type_node,
                                unique_ptr<ExprNode> value_node,
                                const DataType &data_type,
                                unique_ptr<ExprAnalysis> value_analysis,
                                vector<TestCast> test_casts,
                                CmdId expected_create_var_cmd_id,
                                const Code &expected_value_code,
                                const Code &expected_store_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    IdNode *id_node_ptr = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
    unique_ptr<ExprNode> id_node(id_node_ptr);
    ExprNode *value_node_ptr = value_node.get();
    AssignNode *assign_node_ptr = new AssignNode(
        TokenInfo(Token::kAssignOp, "=", UINT32_C(4), UINT32_C(4)),
        move(id_node),
        move(value_node));
    unique_ptr<ExprNode> assign_node(assign_node_ptr);
    unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
        move(assign_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
    program_stmt_nodes.push_back(move(id_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new LocalVarDefAnalysis(data_type.Clone(), flow_local_var_defs));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    bool is_id_assignee = true;
    unique_ptr<DataType> id_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
        data_type.Clone(),
        move(id_casted_data_type),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
    unique_ptr<DataType> assign_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> assign_analysis(new CommonExprAnalysis(
        unique_ptr<DataType>(new VoidDataType()),
        move(assign_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(make_pair(assign_node_ptr, move(assign_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(expected_create_var_cmd_id);
    cmds_code->Write(expected_value_code.GetData(),
                          expected_value_code.GetSize());
    cmds_code->Write<CmdId>(CmdId::kLoadLocalVarAddress);
    uint32_t var_index = UINT32_C(0);
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(expected_store_code.GetData(),
                          expected_store_code.GetSize());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestIdAsAssigneeGlobalArrayVar(
      unique_ptr<DataTypeNode> def_data_type_node,
      unique_ptr<PrimitiveDataTypeNode> value_data_type_node,
      unique_ptr<DataType> data_type,
      CmdId expected_value_cmd_id,
      CmdId expected_store_cmd_id) {
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(def_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));

    vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
    unique_ptr<UnboundedArraySizeNode> array_size_node(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
    array_size_nodes.push_back(move(array_size_node));
    unique_ptr<UnboundedArraySizeNode> array_size_node2(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(8), UINT32_C(8)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(9), UINT32_C(9))));
    array_size_nodes.push_back(move(array_size_node2));
    vector< unique_ptr<ExprNode> > array_value_nodes;
    vector<TokenInfo> array_value_separator_tokens;
    unique_ptr<ExprNode> value_node(new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
        move(value_data_type_node),
        move(array_size_nodes),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(11), UINT32_C(11)),
        move(array_value_nodes),
        array_value_separator_tokens,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(12), UINT32_C(12))));

    unique_ptr<DataType> array_data_type(new ArrayDataType(move(data_type)));
    ArrayDataType array_data_type2(move(array_data_type));
    unique_ptr<DataType> value_casted_data_type;
    unique_ptr<ExprAnalysis> value_analysis(new CommonExprAnalysis(
        array_data_type2.Clone(),
        move(value_casted_data_type),
        ValueType::kRight));

    Code expected_value_code;
    expected_value_code.Write<CmdId>(expected_value_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    expected_value_code.Write<uint8_t>(dimensions_count);
    int32_t values_count = INT32_C(0);
    expected_value_code.Write<int32_t>(values_count);
    Code expected_store_code;
    expected_store_code.Write<CmdId>(expected_store_cmd_id);
    expected_store_code.Write<uint8_t>(dimensions_count);
    TestIdAsAssigneeGlobalVar(move(array_data_type_node2),
                              move(value_node),
                              array_data_type2,
                              move(value_analysis),
                              vector<TestCast>(),
                              DataTypeSize::kArray,
                              CmdId::kCreateGlobalArrayVar,
                              expected_value_code,
                              expected_store_code);
  }

  void TestIdAsAssigneeLocalArrayVar(
      unique_ptr<DataTypeNode> def_data_type_node,
      unique_ptr<PrimitiveDataTypeNode> value_data_type_node,
      unique_ptr<DataType> data_type,
      CmdId expected_value_cmd_id,
      CmdId expected_store_cmd_id) {
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(def_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));

    vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
    unique_ptr<UnboundedArraySizeNode> array_size_node(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
    array_size_nodes.push_back(move(array_size_node));
    unique_ptr<UnboundedArraySizeNode> array_size_node2(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(8), UINT32_C(8)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(9), UINT32_C(9))));
    array_size_nodes.push_back(move(array_size_node2));
    vector< unique_ptr<ExprNode> > array_value_nodes;
    vector<TokenInfo> array_value_separator_tokens;
    unique_ptr<ExprNode> value_node(new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
        move(value_data_type_node),
        move(array_size_nodes),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(11), UINT32_C(11)),
        move(array_value_nodes),
        array_value_separator_tokens,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(12), UINT32_C(12))));

    unique_ptr<DataType> array_data_type(new ArrayDataType(move(data_type)));
    ArrayDataType array_data_type2(move(array_data_type));
    unique_ptr<DataType> value_casted_data_type;
    unique_ptr<ExprAnalysis> value_analysis(new CommonExprAnalysis(
        array_data_type2.Clone(),
        move(value_casted_data_type),
        ValueType::kRight));

    Code expected_value_code;
    expected_value_code.Write<CmdId>(expected_value_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    expected_value_code.Write<uint8_t>(dimensions_count);
    int32_t values_count = INT32_C(0);
    expected_value_code.Write<int32_t>(values_count);
    Code expected_store_code;
    expected_store_code.Write<CmdId>(expected_store_cmd_id);
    expected_store_code.Write<uint8_t>(dimensions_count);
    TestIdAsAssigneeLocalVar(move(array_data_type_node2),
                             move(value_node),
                             array_data_type2,
                             move(value_analysis),
                             vector<TestCast>(),
                             CmdId::kCreateLocalArrayVar,
                             expected_value_code,
                             expected_store_code);
  }

  void TestNotAssigneeSubscriptWithArray(
      unique_ptr<DataTypeNode> element_data_type_node,
      const DataType &element_data_type,
      const Code &expected_code,
      CmdId expected_load_var_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(array_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    IdNode *id_node_ptr = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
    unique_ptr<ExprNode> id_node(id_node_ptr);
    CharNode *char_node_ptr = new CharNode(
        TokenInfo(Token::kCharLit, "'a'", UINT32_C(7), UINT32_C(7)));
    unique_ptr<ExprNode> char_node(char_node_ptr);
    SubscriptNode *subscript_expr_node_ptr = new SubscriptNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(8), UINT32_C(8)),
        move(id_node),
        move(char_node));
    unique_ptr<ExprNode> subscript_expr_node(subscript_expr_node_ptr);
    unique_ptr<StmtNode> subscript_stmt_node(new ExprStmtNode(
        move(subscript_expr_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
    program_stmt_nodes.push_back(move(subscript_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> var_data_type(
        new ArrayDataType(element_data_type.Clone()));
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        move(var_data_type)));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    bool is_id_assignee = false;
    unique_ptr<DataType> id_data_type(
        new ArrayDataType(element_data_type.Clone()));
    unique_ptr<DataType> id_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
        move(id_data_type),
        move(id_casted_data_type),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
    bool is_subscript_assignee = false;
    unique_ptr<DataType> subscript_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> subscript_analysis(new SubscriptAnalysis(
        element_data_type.Clone(),
        move(subscript_casted_data_type),
        ValueType::kLeft,
        is_subscript_assignee));
    node_analyzes.insert(
        make_pair(subscript_expr_node_ptr, move(subscript_analysis)));
    unique_ptr<DataType> char_casted_data_type(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> char_analysis(new LitAnalysis(
        unique_ptr<DataType>(new CharDataType()),
        move(char_casted_data_type),
        ValueType::kRight,
        unique_ptr<Lit>(new CharLit('a'))));
    node_analyzes.insert(make_pair(char_node_ptr, move(char_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    vector<TestCast> test_casts;
    unique_ptr<DataType> dest_data_type(new IntDataType());
    unique_ptr<DataType> src_data_type(new CharDataType());
    TestCast test_cast =
        {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
    test_casts.push_back(move(test_cast));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kCreateGlobalArrayVar);
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write<CmdId>(CmdId::kLoadCharValue);
    cmds_code->Write<char>('a');
    cmds_code->Write<CmdId>(CmdId::kCastCharToInt);
    cmds_code->Write<CmdId>(expected_load_var_cmd_id);
    uint32_t var_index_placeholder2 = cmds_code->GetPosition();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(expected_code.GetData(), expected_code.GetSize());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", DataTypeSize::kArray}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs =
        {{"var", {var_index_placeholder, var_index_placeholder2}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestAssigneeSubscriptWithArray(
      unique_ptr<DataTypeNode> element_data_type_node,
      unique_ptr<ExprNode> value_node,
      const DataType &element_data_type,
      unique_ptr<NodeSemanticAnalysis> value_analysis,
      const Code &expected_value_code,
      const Code &expected_assign_code,
      CmdId expected_load_var_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(array_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    program_stmt_nodes.push_back(move(var_def_node));
    IdNode *id_node_ptr = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
    unique_ptr<ExprNode> id_node(id_node_ptr);
    CharNode *char_node_ptr = new CharNode(
        TokenInfo(Token::kCharLit, "'a'", UINT32_C(7), UINT32_C(7)));
    unique_ptr<ExprNode> char_node(char_node_ptr);
    SubscriptNode *subscript_node_ptr = new SubscriptNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(8), UINT32_C(8)),
        move(id_node),
        move(char_node));
    unique_ptr<ExprNode> subscript_node(subscript_node_ptr);
    ExprNode *value_node_ptr = value_node.get();
    AssignNode *assign_expr_node_ptr = new AssignNode(
        TokenInfo(Token::kAssignOp, "=", UINT32_C(9), UINT32_C(9)),
        move(subscript_node),
        move(value_node));
    unique_ptr<ExprNode> assign_expr_node(assign_expr_node_ptr);
    unique_ptr<StmtNode> assign_stmt_node(new ExprStmtNode(
        move(assign_expr_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(11), UINT32_C(11))));
    program_stmt_nodes.push_back(move(assign_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> var_data_type(
        new ArrayDataType(element_data_type.Clone()));
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        move(var_data_type)));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    bool is_id_assignee = false;
    unique_ptr<DataType> id_data_type(
        new ArrayDataType(element_data_type.Clone()));
    unique_ptr<DataType> id_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
        move(id_data_type),
        move(id_casted_data_type),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
    bool is_subscript_assignee = true;
    unique_ptr<DataType> subscript_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> subscript_analysis(new SubscriptAnalysis(
        element_data_type.Clone(),
        move(subscript_casted_data_type),
        ValueType::kLeft,
        is_subscript_assignee));
    node_analyzes.insert(
        make_pair(subscript_node_ptr, move(subscript_analysis)));
    unique_ptr<DataType> char_casted_data_type(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> char_analysis(new LitAnalysis(
        unique_ptr<DataType>(new CharDataType()),
        move(char_casted_data_type),
        ValueType::kRight,
        unique_ptr<Lit>(new CharLit('a'))));
    node_analyzes.insert(make_pair(char_node_ptr, move(char_analysis)));
    node_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
    unique_ptr<DataType> assign_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> assign_analysis(new CommonExprAnalysis(
        unique_ptr<DataType>(new VoidDataType()),
        move(assign_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(
        make_pair(assign_expr_node_ptr, move(assign_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    vector<TestCast> test_casts;
    unique_ptr<DataType> dest_data_type(new IntDataType());
    unique_ptr<DataType> src_data_type(new CharDataType());
    TestCast test_cast =
        {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
    test_casts.push_back(move(test_cast));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kCreateGlobalArrayVar);
    uint32_t var_index_placeholder = cmds_code->GetPosition();
    uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(expected_value_code.GetData(),
                     expected_value_code.GetSize());
    cmds_code->Write<CmdId>(CmdId::kLoadCharValue);
    cmds_code->Write<char>('a');
    cmds_code->Write<CmdId>(CmdId::kCastCharToInt);
    cmds_code->Write<CmdId>(expected_load_var_cmd_id);
    uint32_t var_index_placeholder2 = cmds_code->GetPosition();
    cmds_code->Write<uint32_t>(var_index);
    cmds_code->Write(
        expected_assign_code.GetData(), expected_assign_code.GetSize());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();

    vector<IdSize> global_var_defs = {{"var", DataTypeSize::kArray}};
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs =
        {{"var", {var_index_placeholder, var_index_placeholder2}}};
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestBinaryExpr(const ExprNode *left_operand_node,
                      const ExprNode *right_operand_node,
                      unique_ptr<ExprNode> expr_node,
                      unique_ptr<NodeSemanticAnalysis> left_operand_analysis,
                      unique_ptr<NodeSemanticAnalysis> right_operand_analysis,
                      unique_ptr<DataType> expr_data_type,
                      vector<TestCast> test_casts,
                      unique_ptr<Code> expected_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    ExprNode *expr_node_ptr = expr_node.get();
    unique_ptr<StmtNode> expr_stmt_node(new ExprStmtNode(
        move(expr_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
    program_stmt_nodes.push_back(move(expr_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(
        make_pair(left_operand_node, move(left_operand_analysis)));
    node_analyzes.insert(
        make_pair(right_operand_node, move(right_operand_analysis)));
    unique_ptr<DataType> expr_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> expr_analysis(new CommonExprAnalysis(
        move(expr_data_type),
        move(expr_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(make_pair(expr_node_ptr, move(expr_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    uint32_t main_cmds_code_size = expected_code->GetPosition();
    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(expected_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestUnaryExpr(const ExprNode *operand_node,
                     unique_ptr<ExprNode> expr_node,
                     unique_ptr<NodeSemanticAnalysis> operand_analysis,
                     unique_ptr<DataType> expr_data_type,
                     vector<TestCast> test_casts,
                     unique_ptr<Code> expected_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    ExprNode *expr_node_ptr = expr_node.get();
    unique_ptr<StmtNode> expr_stmt_node(new ExprStmtNode(
        move(expr_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
    program_stmt_nodes.push_back(move(expr_stmt_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    node_analyzes.insert(make_pair(operand_node, move(operand_analysis)));
    unique_ptr<DataType> expr_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> expr_analysis(new CommonExprAnalysis(
        move(expr_data_type),
        move(expr_casted_data_type),
        ValueType::kRight));
    node_analyzes.insert(make_pair(expr_node_ptr, move(expr_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    uint32_t main_cmds_code_size = expected_code->GetPosition();

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs;
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(expected_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(move(test_casts),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestFuncDefWithBodyAndArg(unique_ptr<DataTypeNode> arg_data_type_node,
                                 const DataType &arg_data_type,
                                 const Code &expected_create_arg_code,
                                 const Code &expected_destroy_arg_code) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<ArgDefNode> > arg_def_nodes;
    ArgDefNode *arg_def_node_ptr = new ArgDefNode(
        move(arg_data_type_node),
        TokenInfo(Token::kName, "arg", UINT32_C(4), UINT32_C(4)));
    unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
    arg_def_nodes.push_back(move(arg_def_node));

    vector< unique_ptr<StmtNode> > body_stmt_nodes;
    unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(10), UINT32_C(10))));
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(var_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(11), UINT32_C(11)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    body_stmt_nodes.push_back(move(var_def_node));
    ScopeNode *body_node_ptr = new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(9)),
        move(body_stmt_nodes),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(13), UINT32_C(13)));
    unique_ptr<ScopeNode> body_node(body_node_ptr);

    unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    vector<TokenInfo> modifier_tokens;
    vector<TokenInfo> arg_separator_tokens;
    FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
        modifier_tokens,
        move(return_data_type_node),
        TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
        move(arg_def_nodes),
        arg_separator_tokens,
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(8), UINT32_C(8)),
        move(body_node));
    unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
    program_stmt_nodes.push_back(move(func_def_node));
    ProgramNode program_node(move(program_stmt_nodes));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    vector<const VarDefNode*> body_local_var_defs =
        {arg_def_node_ptr, var_def_node_ptr};
    unique_ptr<NodeSemanticAnalysis> body_analysis(
        new ScopeAnalysis(body_local_var_defs));
    node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));
    vector<const VarDefNode*> flow_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> arg_def_analysis(new LocalVarDefAnalysis(
        arg_data_type.Clone(), flow_local_var_defs));
    node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));
    vector<const VarDefNode*> flow_local_var_defs2 = {arg_def_node_ptr};
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
        unique_ptr<DataType>(new IntDataType()), flow_local_var_defs2));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    arg_data_types.push_back(arg_data_type.Clone());
    bool is_func_native = false;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types), is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::ProgramProblems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    uint32_t func_def_address = cmds_code->GetPosition();
    cmds_code->Write(expected_create_arg_code.GetData(),
                          expected_create_arg_code.GetSize());
    cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
    cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
    cmds_code->Write(expected_destroy_arg_code.GetData(),
                          expected_destroy_arg_code.GetSize());
    cmds_code->Write<CmdId>(CmdId::kReturn);

    vector<IdSize> global_var_defs;
    vector<IdAddress> func_defs = {{"func", func_def_address}};
    vector<string> native_func_defs;
    vector<IdAddresses> global_var_refs;
    vector<IdAddresses> func_refs;
    vector<IdAddresses> native_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           main_cmds_code_size,
                           func_defs,
                           global_var_defs,
                           native_func_defs,
                           func_refs,
                           native_func_refs,
                           global_var_refs);
    TestGenerate(vector<TestCast>(),
                 program_node,
                 semantic_analysis,
                 version,
                 expected_module);
  }

  void TestEqualArray(unique_ptr<PrimitiveDataTypeNode> left_data_type_node,
                      unique_ptr<PrimitiveDataTypeNode> right_data_type_node,
                      unique_ptr<DataType> data_type,
                      CmdId expected_create_array_cmd_id,
                      CmdId expected_equal_cmd_id) {
    vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
    unique_ptr<UnboundedArraySizeNode> array_size_node(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3))));
    array_size_nodes.push_back(move(array_size_node));
    unique_ptr<UnboundedArraySizeNode> array_size_node2(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
    array_size_nodes.push_back(move(array_size_node2));
    vector< unique_ptr<ExprNode> > array_value_nodes;
    vector<TokenInfo> array_value_separator_tokens;
    ArrayAllocWithInitNode *left_operand_node_ptr = new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "new", UINT32_C(0), UINT32_C(0)),
        move(left_data_type_node),
        move(array_size_nodes),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
        move(array_value_nodes),
        array_value_separator_tokens,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
    unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);

    vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes2;
    unique_ptr<UnboundedArraySizeNode> array_size_node3(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(11), UINT32_C(11)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(12), UINT32_C(12))));
    array_size_nodes2.push_back(move(array_size_node3));
    unique_ptr<UnboundedArraySizeNode> array_size_node4(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(13), UINT32_C(13)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(14), UINT32_C(14))));
    array_size_nodes2.push_back(move(array_size_node4));
    vector< unique_ptr<ExprNode> > array_value_nodes2;
    vector<TokenInfo> array_value_separator_tokens2;
    ArrayAllocWithInitNode *right_operand_node_ptr = new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "new", UINT32_C(9), UINT32_C(9)),
        move(right_data_type_node),
        move(array_size_nodes2),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(15), UINT32_C(15)),
        move(array_value_nodes2),
        array_value_separator_tokens2,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
    unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
    unique_ptr<ExprNode> expr_node(new EqualNode(
        TokenInfo(Token::kEqualOp, "==", UINT32_C(8), UINT32_C(8)),
        move(left_operand_node),
        move(right_operand_node)));

    unique_ptr<DataType> array_data_type(new ArrayDataType(move(data_type)));
    ArrayDataType array_data_type2(move(array_data_type));
    unique_ptr<DataType> left_operand_data_type = array_data_type2.Clone();
    unique_ptr<DataType> left_operand_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> left_operand_analysis(
        new CommonExprAnalysis(
            move(left_operand_data_type),
            move(left_operand_casted_data_type),
            ValueType::kRight));
    unique_ptr<DataType> right_operand_data_type = array_data_type2.Clone();
    unique_ptr<DataType> right_operand_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> right_operand_analysis(
        new CommonExprAnalysis(
            move(right_operand_data_type),
            move(right_operand_casted_data_type),
            ValueType::kRight));
    unique_ptr<DataType> expr_data_type(new BoolDataType());

    unique_ptr<Code> expected_code(new Code());
    expected_code->Write<CmdId>(expected_create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    expected_code->Write<uint8_t>(dimensions_count);
    int32_t values_count = INT32_C(0);
    expected_code->Write<int32_t>(values_count);
    expected_code->Write<CmdId>(expected_create_array_cmd_id);
    expected_code->Write<uint8_t>(dimensions_count);
    expected_code->Write<int32_t>(values_count);
    expected_code->Write<CmdId>(expected_equal_cmd_id);
    expected_code->Write<uint8_t>(dimensions_count);
    expected_code->Write<CmdId>(CmdId::kUnloadBool);
    TestBinaryExpr(left_operand_node_ptr,
                   right_operand_node_ptr,
                   move(expr_node),
                   move(left_operand_analysis),
                   move(right_operand_analysis),
                   move(expr_data_type),
                   vector<TestCast>(),
                   move(expected_code));
  }

  void TestNotEqualArray(unique_ptr<PrimitiveDataTypeNode> left_data_type_node,
                         unique_ptr<PrimitiveDataTypeNode> right_data_type_node,
                         unique_ptr<DataType> data_type,
                         CmdId expected_create_array_cmd_id,
                         CmdId expected_equal_cmd_id) {
    vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
    unique_ptr<UnboundedArraySizeNode> array_size_node(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3))));
    array_size_nodes.push_back(move(array_size_node));
    unique_ptr<UnboundedArraySizeNode> array_size_node2(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
    array_size_nodes.push_back(move(array_size_node2));
    vector< unique_ptr<ExprNode> > array_value_nodes;
    vector<TokenInfo> array_value_separator_tokens;
    ArrayAllocWithInitNode *left_operand_node_ptr = new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "new", UINT32_C(0), UINT32_C(0)),
        move(left_data_type_node),
        move(array_size_nodes),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
        move(array_value_nodes),
        array_value_separator_tokens,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
    unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);

    vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes2;
    unique_ptr<UnboundedArraySizeNode> array_size_node3(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(11), UINT32_C(11)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(12), UINT32_C(12))));
    array_size_nodes2.push_back(move(array_size_node3));
    unique_ptr<UnboundedArraySizeNode> array_size_node4(
        new UnboundedArraySizeNode(
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(13), UINT32_C(13)),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(14), UINT32_C(14))));
    array_size_nodes2.push_back(move(array_size_node4));
    vector< unique_ptr<ExprNode> > array_value_nodes2;
    vector<TokenInfo> array_value_separator_tokens2;
    ArrayAllocWithInitNode *right_operand_node_ptr = new ArrayAllocWithInitNode(
        TokenInfo(Token::kNew, "new", UINT32_C(9), UINT32_C(9)),
        move(right_data_type_node),
        move(array_size_nodes2),
        TokenInfo(Token::kScopeStart, "{", UINT32_C(15), UINT32_C(15)),
        move(array_value_nodes2),
        array_value_separator_tokens2,
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
    unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
    unique_ptr<ExprNode> expr_node(new NotEqualNode(
        TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(8), UINT32_C(8)),
        move(left_operand_node),
        move(right_operand_node)));

    unique_ptr<DataType> array_data_type(new ArrayDataType(move(data_type)));
    ArrayDataType array_data_type2(move(array_data_type));
    unique_ptr<DataType> left_operand_data_type = array_data_type2.Clone();
    unique_ptr<DataType> left_operand_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> left_operand_analysis(
        new CommonExprAnalysis(
            move(left_operand_data_type),
            move(left_operand_casted_data_type),
            ValueType::kRight));
    unique_ptr<DataType> right_operand_data_type = array_data_type2.Clone();
    unique_ptr<DataType> right_operand_casted_data_type;
    unique_ptr<NodeSemanticAnalysis> right_operand_analysis(
        new CommonExprAnalysis(
            move(right_operand_data_type),
            move(right_operand_casted_data_type),
            ValueType::kRight));
    unique_ptr<DataType> expr_data_type(new BoolDataType());

    unique_ptr<Code> expected_code(new Code());
    expected_code->Write<CmdId>(expected_create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    expected_code->Write<uint8_t>(dimensions_count);
    int32_t values_count = INT32_C(0);
    expected_code->Write<int32_t>(values_count);
    expected_code->Write<CmdId>(expected_create_array_cmd_id);
    expected_code->Write<uint8_t>(dimensions_count);
    expected_code->Write<int32_t>(values_count);
    expected_code->Write<CmdId>(expected_equal_cmd_id);
    expected_code->Write<uint8_t>(dimensions_count);
    expected_code->Write<CmdId>(CmdId::kUnloadBool);
    TestBinaryExpr(left_operand_node_ptr,
                   right_operand_node_ptr,
                   move(expr_node),
                   move(left_operand_analysis),
                   move(right_operand_analysis),
                   move(expr_data_type),
                   vector<TestCast>(),
                   move(expected_code));
  }
};

TEST_F(SimpleCodeGeneratorTest, GlobalIntVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestGlobalVarDefWithoutInit(move(data_type_node),
                              move(data_type),
                              DataTypeSize::kInt,
                              CmdId::kCreateGlobalIntVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalArrayVarDefWithoutInit) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> array_data_type_node1(new ArrayDataTypeNode(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
      move(array_data_type_node1),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));

  unique_ptr<DataType> int_data_type(new IntDataType());
  unique_ptr<DataType> array_data_type1(
      new ArrayDataType(move(int_data_type)));
  unique_ptr<DataType> array_data_type2(
      new ArrayDataType(move(array_data_type1)));

  TestGlobalVarDefWithoutInit(move(array_data_type_node2),
                              move(array_data_type2),
                              DataTypeSize::kArray,
                              CmdId::kCreateGlobalArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalLongVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestGlobalVarDefWithoutInit(move(data_type_node),
                              move(data_type),
                              DataTypeSize::kLong,
                              CmdId::kCreateGlobalLongVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalDoubleVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestGlobalVarDefWithoutInit(move(data_type_node),
                              move(data_type),
                              DataTypeSize::kDouble,
                              CmdId::kCreateGlobalDoubleVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalCharVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestGlobalVarDefWithoutInit(move(data_type_node),
                              move(data_type),
                              DataTypeSize::kChar,
                              CmdId::kCreateGlobalCharVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalStringVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestGlobalVarDefWithoutInit(move(data_type_node),
                              move(data_type),
                              DataTypeSize::kString,
                              CmdId::kCreateGlobalStringVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalBoolVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestGlobalVarDefWithoutInit(move(data_type_node),
                              move(data_type),
                              DataTypeSize::kBool,
                              CmdId::kCreateGlobalBoolVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalIntVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestLocalVarDefWithoutInit(
      move(data_type_node), move(data_type), CmdId::kCreateLocalIntVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalArrayVarDefWithoutInit) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> array_data_type_node1(new ArrayDataTypeNode(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  unique_ptr<DataTypeNode> array_data_type_node2(new ArrayDataTypeNode(
      move(array_data_type_node1),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));

  unique_ptr<DataType> int_data_type(new IntDataType());
  unique_ptr<DataType> array_data_type1(
      new ArrayDataType(move(int_data_type)));
  unique_ptr<DataType> array_data_type2(
      new ArrayDataType(move(array_data_type1)));

  TestLocalVarDefWithoutInit(move(array_data_type_node2),
                             move(array_data_type2),
                             CmdId::kCreateLocalArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalLongVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestLocalVarDefWithoutInit(
      move(data_type_node), move(data_type), CmdId::kCreateLocalLongVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalDoubleVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestLocalVarDefWithoutInit(
      move(data_type_node), move(data_type), CmdId::kCreateLocalDoubleVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalCharVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestLocalVarDefWithoutInit(
      move(data_type_node), move(data_type), CmdId::kCreateLocalCharVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalStringVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestLocalVarDefWithoutInit(
      move(data_type_node), move(data_type), CmdId::kCreateLocalStringVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalBoolVarDefWithoutInit) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestLocalVarDefWithoutInit(
      move(data_type_node), move(data_type), CmdId::kCreateLocalBoolVar);
}

TEST_F(SimpleCodeGeneratorTest, ExprStmt) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<IntNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  program_stmt_nodes.push_back(move(int_stmt_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> int_lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_lit_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(7));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, Int) {
  unique_ptr<LitNode> lit_node(new IntNode(
      TokenInfo(Token::kIntLit, "-7", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(-7)))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(-7));
  expected_code->Write<CmdId>(CmdId::kUnloadInt);
  TestLit(move(lit_node), move(lit_analysis), move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, Long) {
  unique_ptr<LitNode> lit_node(new LongNode(
      TokenInfo(Token::kLongLit, "-77L", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new LongDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(-77)))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(-77));
  expected_code->Write<CmdId>(CmdId::kUnloadLong);
  TestLit(move(lit_node), move(lit_analysis), move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, Bool) {
  unique_ptr<LitNode> lit_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadBoolValue);
  expected_code->Write<bool>(false);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);
  TestLit(move(lit_node), move(lit_analysis), move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, String) {
  unique_ptr<LitNode> lit_node(new StringNode(
      TokenInfo(Token::kStringLit, "\"swagger\"", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("swagger"))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadStringValue);
  expected_code->Write<string>("swagger");
  expected_code->Write<CmdId>(CmdId::kUnloadString);
  TestLit(move(lit_node), move(lit_analysis), move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, Double) {
  unique_ptr<LitNode> lit_node(new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.77777777777", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new DoubleDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.77777777777))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(7.77777777777);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);
  TestLit(move(lit_node), move(lit_analysis), move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, Char) {
  unique_ptr<LitNode> lit_node(new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kUnloadChar);
  TestLit(move(lit_node), move(lit_analysis), move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, Import) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  unique_ptr<ImportNode> import_node(new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
  program_stmt_nodes.push_back(move(import_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("file.rt"))));
  node_analyzes.insert(
      make_pair(file_path_node_ptr, move(lit_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, GlobalIntVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'7'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('7'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('7');

  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt}));

  TestGlobalVarDefWithInit(move(data_type_node),
                           unique_ptr<DataType>(new IntDataType()),
                           move(value_node),
                           move(value_node_analyzes),
                           move(expected_value_code),
                           move(test_cast),
                           DataTypeSize::kInt,
                           CmdId::kCreateAndInitGlobalIntVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalLongVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new LongDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));

  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong}));

  TestGlobalVarDefWithInit(move(data_type_node),
                           move(data_type),
                           move(value_node),
                           move(value_node_analyzes),
                           move(expected_value_code),
                           move(test_cast),
                           DataTypeSize::kLong,
                           CmdId::kCreateAndInitGlobalLongVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalDoubleVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new DoubleDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));

  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble}));

  TestGlobalVarDefWithInit(move(data_type_node),
                           move(data_type),
                           move(value_node),
                           move(value_node_analyzes),
                           move(expected_value_code),
                           move(test_cast),
                           DataTypeSize::kDouble,
                           CmdId::kCreateAndInitGlobalDoubleVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalCharVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  TestGlobalVarDefWithInit(move(data_type_node),
                           move(data_type),
                           move(value_node),
                           move(value_node_analyzes),
                           move(expected_value_code),
                           unique_ptr<TestCast>(),
                           DataTypeSize::kChar,
                           CmdId::kCreateAndInitGlobalCharVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalStringVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new StringDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');

  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString}));

  TestGlobalVarDefWithInit(move(data_type_node),
                           move(data_type),
                           move(value_node),
                           move(value_node_analyzes),
                           move(expected_value_code),
                           move(test_cast),
                           DataTypeSize::kString,
                           CmdId::kCreateAndInitGlobalStringVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalBoolVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<ExprNode> value_node(new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadBoolValue);
  expected_value_code.Write<bool>(true);
  TestGlobalVarDefWithInit(move(data_type_node),
                           move(data_type),
                           move(value_node),
                           move(value_node_analyzes),
                           move(expected_value_code),
                           unique_ptr<TestCast>(),
                           DataTypeSize::kBool,
                           CmdId::kCreateAndInitGlobalBoolVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalIntArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(7))));
  TestGlobalArrayVarDefWithInit(move(def_data_type_node),
                                move(value_data_type_node),
                                unique_ptr<DataType>(new IntDataType()),
                                CmdId::kCreateIntArray,
                                CmdId::kCreateAndInitGlobalIntArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalLongArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(7), UINT32_C(7))));
  TestGlobalArrayVarDefWithInit(move(def_data_type_node),
                                move(value_data_type_node),
                                unique_ptr<DataType>(new LongDataType()),
                                CmdId::kCreateLongArray,
                                CmdId::kCreateAndInitGlobalLongArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalDoubleArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(7), UINT32_C(7))));
  TestGlobalArrayVarDefWithInit(move(def_data_type_node),
                                move(value_data_type_node),
                                unique_ptr<DataType>(new DoubleDataType()),
                                CmdId::kCreateDoubleArray,
                                CmdId::kCreateAndInitGlobalDoubleArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalCharArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(7), UINT32_C(7))));
  TestGlobalArrayVarDefWithInit(move(def_data_type_node),
                                move(value_data_type_node),
                                unique_ptr<DataType>(new CharDataType()),
                                CmdId::kCreateCharArray,
                                CmdId::kCreateAndInitGlobalCharArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalBoolArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(7), UINT32_C(7))));
  TestGlobalArrayVarDefWithInit(move(def_data_type_node),
                                move(value_data_type_node),
                                unique_ptr<DataType>(new BoolDataType()),
                                CmdId::kCreateBoolArray,
                                CmdId::kCreateAndInitGlobalBoolArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, GlobalStringArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(7), UINT32_C(7))));
  TestGlobalArrayVarDefWithInit(move(def_data_type_node),
                                move(value_data_type_node),
                                unique_ptr<DataType>(new StringDataType()),
                                CmdId::kCreateStringArray,
                                CmdId::kCreateAndInitGlobalStringArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalIntVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');

  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt}));

  TestLocalVarDefWithInit(move(data_type_node),
                          unique_ptr<DataType>(new IntDataType()),
                          move(value_node),
                          move(value_node_analyzes),
                          move(expected_value_code),
                          move(test_cast),
                          CmdId::kCreateAndInitLocalIntVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalLongVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new LongDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));

  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong}));

  TestLocalVarDefWithInit(move(data_type_node),
                          move(data_type),
                          move(value_node),
                          move(value_node_analyzes),
                          move(expected_value_code),
                          move(test_cast),
                          CmdId::kCreateAndInitLocalLongVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalDoubleVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new DoubleDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));

  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble}));

  TestLocalVarDefWithInit(move(data_type_node),
                          move(data_type),
                          move(value_node),
                          move(value_node_analyzes),
                          move(expected_value_code),
                          move(test_cast),
                          CmdId::kCreateAndInitLocalDoubleVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalCharVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  TestLocalVarDefWithInit(move(data_type_node),
                          move(data_type),
                          move(value_node),
                          move(value_node_analyzes),
                          move(expected_value_code),
                          unique_ptr<TestCast>(),
                          CmdId::kCreateAndInitLocalCharVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalStringVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type(new StringDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');

  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  unique_ptr<TestCast> test_cast(new TestCast(
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString}));

  TestLocalVarDefWithInit(move(data_type_node),
                          move(data_type),
                          move(value_node),
                          move(value_node_analyzes),
                          move(expected_value_code),
                          move(test_cast),
                          CmdId::kCreateAndInitLocalStringVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalBoolVarDefWithInit) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<ExprNode> value_node(new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadBoolValue);
  expected_value_code.Write<bool>(true);
  TestLocalVarDefWithInit(move(data_type_node),
                          move(data_type),
                          move(value_node),
                          move(value_node_analyzes),
                          move(expected_value_code),
                          unique_ptr<TestCast>(),
                          CmdId::kCreateAndInitLocalBoolVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalIntArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(7))));
  TestLocalArrayVarDefWithInit(move(def_data_type_node),
                               move(value_data_type_node),
                               unique_ptr<DataType>(new IntDataType()),
                               CmdId::kCreateIntArray,
                               CmdId::kCreateAndInitLocalIntArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalLongArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(7), UINT32_C(7))));
  TestLocalArrayVarDefWithInit(move(def_data_type_node),
                               move(value_data_type_node),
                               unique_ptr<DataType>(new LongDataType()),
                               CmdId::kCreateLongArray,
                               CmdId::kCreateAndInitLocalLongArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalDoubleArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(7), UINT32_C(7))));
  TestLocalArrayVarDefWithInit(move(def_data_type_node),
                               move(value_data_type_node),
                               unique_ptr<DataType>(new DoubleDataType()),
                               CmdId::kCreateDoubleArray,
                               CmdId::kCreateAndInitLocalDoubleArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalCharArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(7), UINT32_C(7))));
  TestLocalArrayVarDefWithInit(move(def_data_type_node),
                               move(value_data_type_node),
                               unique_ptr<DataType>(new CharDataType()),
                               CmdId::kCreateCharArray,
                               CmdId::kCreateAndInitLocalCharArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalBoolArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(7), UINT32_C(7))));
  TestLocalArrayVarDefWithInit(move(def_data_type_node),
                               move(value_data_type_node),
                               unique_ptr<DataType>(new BoolDataType()),
                               CmdId::kCreateBoolArray,
                               CmdId::kCreateAndInitLocalBoolArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, LocalStringArrayVarDefWithInit) {
  unique_ptr<DataTypeNode> def_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(7), UINT32_C(7))));
  TestLocalArrayVarDefWithInit(move(def_data_type_node),
                               move(value_data_type_node),
                               unique_ptr<DataType>(new StringDataType()),
                               CmdId::kCreateStringArray,
                               CmdId::kCreateAndInitLocalStringArrayVar);
}

TEST_F(SimpleCodeGeneratorTest, IntArrayAllocWithoutInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestArrayAllocWithoutInit(move(data_type_node),
                            move(data_type),
                            CmdId::kUnloadIntArray,
                            CmdId::kCreateIntArray);
}

TEST_F(SimpleCodeGeneratorTest, LongArrayAllocWithoutInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestArrayAllocWithoutInit(move(data_type_node),
                            move(data_type),
                            CmdId::kUnloadLongArray,
                            CmdId::kCreateLongArray);
}

TEST_F(SimpleCodeGeneratorTest, DoubleArrayAllocWithoutInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestArrayAllocWithoutInit(move(data_type_node),
                            move(data_type),
                            CmdId::kUnloadDoubleArray,
                            CmdId::kCreateDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, BoolArrayAllocWithoutInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestArrayAllocWithoutInit(move(data_type_node),
                            move(data_type),
                            CmdId::kUnloadBoolArray,
                            CmdId::kCreateBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, CharArrayAllocWithoutInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestArrayAllocWithoutInit(move(data_type_node),
                            move(data_type),
                            CmdId::kUnloadCharArray,
                            CmdId::kCreateCharArray);
}

TEST_F(SimpleCodeGeneratorTest, StringArrayAllocWithoutInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestArrayAllocWithoutInit(move(data_type_node),
                            move(data_type),
                            CmdId::kUnloadStringArray,
                            CmdId::kCreateStringArray);
}

TEST_F(SimpleCodeGeneratorTest, IntArrayAllocWithInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new IntDataType());

  vector< unique_ptr<ExprNode> > value_nodes;
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  CharNode *value_node_ptr2 = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node2(value_node_ptr2);
  value_nodes.push_back(move(value_node2));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
  unique_ptr<DataType> value_casted_data_type2(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  value_analyzes.insert(make_pair(value_node_ptr2, move(value_analysis2)));

  Code values_code;
  values_code.Write<CmdId>(CmdId::kLoadCharValue);
  values_code.Write<char>('b');
  values_code.Write<CmdId>(CmdId::kCastCharToInt);
  values_code.Write<CmdId>(CmdId::kLoadCharValue);
  values_code.Write<char>('a');
  values_code.Write<CmdId>(CmdId::kCastCharToInt);

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));
  unique_ptr<DataType> dest_data_type2(new IntDataType());
  unique_ptr<DataType> src_data_type2(new CharDataType());
  TestCast test_cast2 =
      {move(dest_data_type2), move(src_data_type2), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast2));

  TestArrayAllocWithInit(move(data_type_node),
                         move(data_type),
                         move(value_nodes),
                         move(value_analyzes),
                         values_code,
                         move(test_casts),
                         CmdId::kUnloadIntArray,
                         CmdId::kCreateAndInitIntArray);
}

TEST_F(SimpleCodeGeneratorTest, LongArrayAllocWithInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new LongDataType());

  vector< unique_ptr<ExprNode> > value_nodes;
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  IntNode *value_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node2(value_node_ptr2);
  value_nodes.push_back(move(value_node2));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
  unique_ptr<DataType> value_casted_data_type2(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  value_analyzes.insert(make_pair(value_node_ptr2, move(value_analysis2)));

  Code values_code;
  values_code.Write<CmdId>(CmdId::kLoadIntValue);
  values_code.Write<int32_t>(INT32_C(2));
  values_code.Write<CmdId>(CmdId::kCastIntToLong);
  values_code.Write<CmdId>(CmdId::kLoadIntValue);
  values_code.Write<int32_t>(INT32_C(1));
  values_code.Write<CmdId>(CmdId::kCastIntToLong);

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));
  unique_ptr<DataType> dest_data_type2(new LongDataType());
  unique_ptr<DataType> src_data_type2(new IntDataType());
  TestCast test_cast2 =
      {move(dest_data_type2), move(src_data_type2), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast2));

  TestArrayAllocWithInit(move(data_type_node),
                         move(data_type),
                         move(value_nodes),
                         move(value_analyzes),
                         values_code,
                         move(test_casts),
                         CmdId::kUnloadLongArray,
                         CmdId::kCreateAndInitLongArray);
}

TEST_F(SimpleCodeGeneratorTest, DoubleArrayAllocWithInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new DoubleDataType());

  vector< unique_ptr<ExprNode> > value_nodes;
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  IntNode *value_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node2(value_node_ptr2);
  value_nodes.push_back(move(value_node2));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
  unique_ptr<DataType> value_casted_data_type2(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  value_analyzes.insert(make_pair(value_node_ptr2, move(value_analysis2)));

  Code values_code;
  values_code.Write<CmdId>(CmdId::kLoadIntValue);
  values_code.Write<int32_t>(INT32_C(2));
  values_code.Write<CmdId>(CmdId::kCastIntToDouble);
  values_code.Write<CmdId>(CmdId::kLoadIntValue);
  values_code.Write<int32_t>(INT32_C(1));
  values_code.Write<CmdId>(CmdId::kCastIntToDouble);

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));
  unique_ptr<DataType> dest_data_type2(new DoubleDataType());
  unique_ptr<DataType> src_data_type2(new IntDataType());
  TestCast test_cast2 =
      {move(dest_data_type2), move(src_data_type2), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast2));

  TestArrayAllocWithInit(move(data_type_node),
                         move(data_type),
                         move(value_nodes),
                         move(value_analyzes),
                         values_code,
                         move(test_casts),
                         CmdId::kUnloadDoubleArray,
                         CmdId::kCreateAndInitDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, BoolArrayAllocWithInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new BoolDataType());

  vector< unique_ptr<ExprNode> > value_nodes;
  BoolNode *value_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  BoolNode *value_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node2(value_node_ptr2);
  value_nodes.push_back(move(value_node2));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
  unique_ptr<DataType> value_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> value_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(value_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  value_analyzes.insert(make_pair(value_node_ptr2, move(value_analysis2)));

  Code values_code;
  values_code.Write<CmdId>(CmdId::kLoadBoolValue);
  values_code.Write<bool>(false);
  values_code.Write<CmdId>(CmdId::kLoadBoolValue);
  values_code.Write<bool>(true);

  TestArrayAllocWithInit(move(data_type_node),
                         move(data_type),
                         move(value_nodes),
                         move(value_analyzes),
                         values_code,
                         vector<TestCast>(),
                         CmdId::kUnloadBoolArray,
                         CmdId::kCreateAndInitBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, CharArrayAllocWithInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new CharDataType());

  vector< unique_ptr<ExprNode> > value_nodes;
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  CharNode *value_node_ptr2 = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node2(value_node_ptr2);
  value_nodes.push_back(move(value_node2));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
  unique_ptr<DataType> value_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> value_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  value_analyzes.insert(make_pair(value_node_ptr2, move(value_analysis2)));

  Code values_code;
  values_code.Write<CmdId>(CmdId::kLoadCharValue);
  values_code.Write<char>('b');
  values_code.Write<CmdId>(CmdId::kLoadCharValue);
  values_code.Write<char>('a');

  TestArrayAllocWithInit(move(data_type_node),
                         move(data_type),
                         move(value_nodes),
                         move(value_analyzes),
                         values_code,
                         vector<TestCast>(),
                         CmdId::kUnloadCharArray,
                         CmdId::kCreateAndInitCharArray);
}

TEST_F(SimpleCodeGeneratorTest, StringArrayAllocWithInit) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new StringDataType());

  vector< unique_ptr<ExprNode> > value_nodes;
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  CharNode *value_node_ptr2 = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node2(value_node_ptr2);
  value_nodes.push_back(move(value_node2));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new StringDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));
  unique_ptr<DataType> value_casted_data_type2(new StringDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  value_analyzes.insert(make_pair(value_node_ptr2, move(value_analysis2)));

  Code values_code;
  values_code.Write<CmdId>(CmdId::kLoadCharValue);
  values_code.Write<char>('b');
  values_code.Write<CmdId>(CmdId::kCastCharToString);
  values_code.Write<CmdId>(CmdId::kLoadCharValue);
  values_code.Write<char>('a');
  values_code.Write<CmdId>(CmdId::kCastCharToString);

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));
  unique_ptr<DataType> dest_data_type2(new StringDataType());
  unique_ptr<DataType> src_data_type2(new CharDataType());
  TestCast test_cast2 =
      {move(dest_data_type2), move(src_data_type2), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast2));

  TestArrayAllocWithInit(move(data_type_node),
                         move(data_type),
                         move(value_nodes),
                         move(value_analyzes),
                         values_code,
                         move(test_casts),
                         CmdId::kUnloadStringArray,
                         CmdId::kCreateAndInitStringArray);
}

TEST_F(SimpleCodeGeneratorTest, IfElseIfElseWithoutVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  if_body_stmt_nodes.push_back(move(int_stmt_node));
  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(if_body_node)));

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  vector< unique_ptr<StmtNode> > else_if_body_stmt_nodes;
  IntNode *int_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(14), UINT32_C(14)));
  unique_ptr<ExprNode> int_node2(int_node_ptr2);
  unique_ptr<StmtNode> int_stmt_node2(new ExprStmtNode(
      move(int_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15))));
  else_if_body_stmt_nodes.push_back(move(int_stmt_node2));
  ScopeNode *else_if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      move(else_if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> else_if_body_node(else_if_body_node_ptr);
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(10), UINT32_C(10)),
      move(bool_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(12), UINT32_C(12)),
      move(else_if_body_node)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(8), UINT32_C(8)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  vector< unique_ptr<StmtNode> > else_body_stmt_nodes;
  IntNode *int_node_ptr3 = new IntNode(
      TokenInfo(Token::kIntLit, "3", UINT32_C(19), UINT32_C(19)));
  unique_ptr<ExprNode> int_node3(int_node_ptr3);
  unique_ptr<StmtNode> int_stmt_node3(new ExprStmtNode(
      move(int_node3),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(20), UINT32_C(20))));
  else_body_stmt_nodes.push_back(move(int_stmt_node3));
  ScopeNode *else_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(18), UINT32_C(18)),
      move(else_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21)));
  unique_ptr<ScopeNode> else_body_node(else_body_node_ptr);

  unique_ptr<StmtNode> if_else_if_else_node(new IfElseIfElseNode(
      move(if_node),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(17), UINT32_C(17)),
      move(else_body_node)));
  program_stmt_nodes.push_back(move(if_else_if_else_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));
  vector<const VarDefNode*> else_if_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> else_if_body_analysis(
      new ScopeAnalysis(else_if_body_local_var_defs));
  node_analyzes.insert(
      make_pair(else_if_body_node_ptr, move(else_if_body_analysis)));
  vector<const VarDefNode*> else_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> else_body_analysis(
      new ScopeAnalysis(else_body_local_var_defs));
  node_analyzes.insert(
      make_pair(else_body_node_ptr, move(else_body_analysis)));
  unique_ptr<DataType> int_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> int_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_analysis)));
  unique_ptr<DataType> int_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> int_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(int_node_ptr2, move(int_analysis2)));
  unique_ptr<DataType> int_casted_data_type3;
  unique_ptr<NodeSemanticAnalysis> int_analysis3(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type3),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(3)))));
  node_analyzes.insert(make_pair(int_node_ptr3, move(int_analysis3)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<DataType> bool_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t else_if_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(1));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t branch_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));

  uint32_t else_if_address = cmds_code->GetPosition();
  int32_t else_if_offset = static_cast<int32_t>(else_if_address)
                           - (static_cast<int32_t>(else_if_offset_placeholder)
                              + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(else_if_offset_placeholder);
  cmds_code->Write<int32_t>(else_if_offset);
  cmds_code->SetPosition(else_if_address);
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(false);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t else_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(2));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t branch_end_offset_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));

  uint32_t else_address = cmds_code->GetPosition();
  int32_t else_offset = static_cast<int32_t>(else_address)
                        - (static_cast<int32_t>(else_offset_placeholder)
                           + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(else_offset_placeholder);
  cmds_code->Write<int32_t>(else_offset);
  cmds_code->SetPosition(else_address);
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(3));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);

  uint32_t branch_end_address = cmds_code->GetPosition();
  int32_t branch_end_offset = static_cast<int32_t>(branch_end_address)
                              - (static_cast<int32_t>(branch_end_offset_placeholder)
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder);
  cmds_code->Write<int32_t>(branch_end_offset);
  cmds_code->SetPosition(branch_end_offset_placeholder2);
  int32_t branch_end_offset2 = static_cast<int32_t>(branch_end_address)
                               - (static_cast<int32_t>(branch_end_offset_placeholder2)
                                  + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(branch_end_offset2);
  cmds_code->SetPosition(branch_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, IfElseIfElseWithLocalVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  if_body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  if_body_stmt_nodes.push_back(move(var_def_node2));
  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(if_body_node)));

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  vector< unique_ptr<StmtNode> > else_if_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(18), UINT32_C(18))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(19), UINT32_C(19)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(20), UINT32_C(20)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  else_if_body_stmt_nodes.push_back(move(var_def_node3));
  unique_ptr<DataTypeNode> data_type_node4(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(21), UINT32_C(21))));
  VarDefWithoutInitNode *var_def_node_ptr4 = new VarDefWithoutInitNode(
      move(data_type_node4),
      TokenInfo(Token::kName, "var4", UINT32_C(22), UINT32_C(22)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(23), UINT32_C(23)));
  unique_ptr<StmtNode> var_def_node4(var_def_node_ptr4);
  else_if_body_stmt_nodes.push_back(move(var_def_node4));
  ScopeNode *else_if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(17), UINT32_C(17)),
      move(else_if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(24), UINT32_C(24)));
  unique_ptr<ScopeNode> else_if_body_node(else_if_body_node_ptr);
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(15), UINT32_C(15)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(13), UINT32_C(13)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(14), UINT32_C(14)),
      move(bool_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(16), UINT32_C(16)),
      move(else_if_body_node)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(12), UINT32_C(12)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  vector< unique_ptr<StmtNode> > else_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node5(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(27), UINT32_C(27))));
  VarDefWithoutInitNode *var_def_node_ptr5 = new VarDefWithoutInitNode(
      move(data_type_node5),
      TokenInfo(Token::kName, "var5", UINT32_C(28), UINT32_C(28)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(29), UINT32_C(29)));
  unique_ptr<StmtNode> var_def_node5(var_def_node_ptr5);
  else_body_stmt_nodes.push_back(move(var_def_node5));
  unique_ptr<DataTypeNode> data_type_node6(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(30), UINT32_C(30))));
  VarDefWithoutInitNode *var_def_node_ptr6 = new VarDefWithoutInitNode(
      move(data_type_node6),
      TokenInfo(Token::kName, "var6", UINT32_C(31), UINT32_C(31)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(32), UINT32_C(32)));
  unique_ptr<StmtNode> var_def_node6(var_def_node_ptr6);
  else_body_stmt_nodes.push_back(move(var_def_node6));
  ScopeNode *else_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(26), UINT32_C(26)),
      move(else_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(33), UINT32_C(33)));
  unique_ptr<ScopeNode> else_body_node(else_body_node_ptr);
  unique_ptr<StmtNode> if_else_if_else_node(new IfElseIfElseNode(
      move(if_node),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(25), UINT32_C(25)),
      move(else_body_node)));
  program_stmt_nodes.push_back(move(if_else_if_else_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));
  vector<const VarDefNode*> else_if_body_local_var_defs =
      {var_def_node_ptr3, var_def_node_ptr4};
  unique_ptr<NodeSemanticAnalysis> else_if_body_analysis(
      new ScopeAnalysis(else_if_body_local_var_defs));
  node_analyzes.insert(
      make_pair(else_if_body_node_ptr, move(else_if_body_analysis)));
  vector<const VarDefNode*> else_body_local_var_defs =
      {var_def_node_ptr5, var_def_node_ptr6};
  unique_ptr<NodeSemanticAnalysis> else_body_analysis(
      new ScopeAnalysis(else_body_local_var_defs));
  node_analyzes.insert(
      make_pair(else_body_node_ptr, move(else_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new BoolDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  vector<const VarDefNode*> flow_local_var_defs3;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs3));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  vector<const VarDefNode*> flow_local_var_defs4 = {var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis4(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new CharDataType()), flow_local_var_defs4));
  node_analyzes.insert(make_pair(var_def_node_ptr4, move(var_def_analysis4)));
  vector<const VarDefNode*> flow_local_var_defs5;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis5(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new DoubleDataType()), flow_local_var_defs5));
  node_analyzes.insert(make_pair(var_def_node_ptr5, move(var_def_analysis5)));
  vector<const VarDefNode*> flow_local_var_defs6 = {var_def_node_ptr5};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis6(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new StringDataType()), flow_local_var_defs6));
  node_analyzes.insert(make_pair(var_def_node_ptr6, move(var_def_analysis6)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<DataType> bool_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t else_if_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t branch_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));

  uint32_t else_if_address = cmds_code->GetPosition();
  int32_t else_if_offset = static_cast<int32_t>(else_if_address)
                           - (static_cast<int32_t>(else_if_offset_placeholder)
                              + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(else_if_offset_placeholder);
  cmds_code->Write<int32_t>(else_if_offset);
  cmds_code->SetPosition(else_if_address);
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(false);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t else_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalCharVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalCharVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t branch_end_offset_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));

  uint32_t else_address = cmds_code->GetPosition();
  int32_t else_offset = static_cast<int32_t>(else_address)
                        - (static_cast<int32_t>(else_offset_placeholder)
                           + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(else_offset_placeholder);
  cmds_code->Write<int32_t>(else_offset);
  cmds_code->SetPosition(else_address);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalStringVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalStringVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalDoubleVar);

  uint32_t branch_end_address = cmds_code->GetPosition();
  int32_t branch_end_offset =
      static_cast<int32_t>(branch_end_address)
      - (static_cast<int32_t>(branch_end_offset_placeholder)
         + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder);
  cmds_code->Write<int32_t>(branch_end_offset);
  int32_t branch_end_offset2 =
      static_cast<int32_t>(branch_end_address)
      - (static_cast<int32_t>(branch_end_offset_placeholder2)
         + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder2);
  cmds_code->Write<int32_t>(branch_end_offset2);
  cmds_code->SetPosition(branch_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, IfElseIfWithoutVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  if_body_stmt_nodes.push_back(move(int_stmt_node));
  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(if_body_node)));

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  vector< unique_ptr<StmtNode> > else_if_body_stmt_nodes;
  IntNode *int_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(14), UINT32_C(14)));
  unique_ptr<ExprNode> int_node2(int_node_ptr2);
  unique_ptr<StmtNode> int_stmt_node2(new ExprStmtNode(
      move(int_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15))));
  else_if_body_stmt_nodes.push_back(move(int_stmt_node2));
  ScopeNode *else_if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      move(else_if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> else_if_body_node(else_if_body_node_ptr);
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(10), UINT32_C(10)),
      move(bool_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(12), UINT32_C(12)),
      move(else_if_body_node)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(8), UINT32_C(8)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  unique_ptr<StmtNode> if_else_if_node(new IfElseIfNode(
      move(if_node), move(else_if_nodes)));
  program_stmt_nodes.push_back(move(if_else_if_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));
  vector<const VarDefNode*> else_if_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> else_if_body_analysis(
      new ScopeAnalysis(else_if_body_local_var_defs));
  node_analyzes.insert(
      make_pair(else_if_body_node_ptr, move(else_if_body_analysis)));
  unique_ptr<DataType> int_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> int_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_analysis)));
  unique_ptr<DataType> int_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> int_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(int_node_ptr2, move(int_analysis2)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<DataType> bool_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t else_if_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(1));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t branch_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));

  uint32_t else_if_address = cmds_code->GetPosition();
  int32_t else_if_offset = static_cast<int32_t>(else_if_address)
                           - (static_cast<int32_t>(else_if_offset_placeholder)
                              + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(else_if_offset_placeholder);
  cmds_code->Write<int32_t>(else_if_offset);
  cmds_code->SetPosition(else_if_address);
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(false);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t branch_end_offset_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(2));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);

  uint32_t branch_end_address = cmds_code->GetPosition();
  int32_t branch_end_offset = static_cast<int32_t>(branch_end_address)
                              - (static_cast<int32_t>(branch_end_offset_placeholder)
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder);
  cmds_code->Write<int32_t>(branch_end_offset);
  int32_t branch_end_offset2 = static_cast<int32_t>(branch_end_address)
                               - (static_cast<int32_t>(branch_end_offset_placeholder2)
                                  + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder2);
  cmds_code->Write<int32_t>(branch_end_offset2);
  cmds_code->SetPosition(branch_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, IfElseIfWithLocalVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  if_body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  if_body_stmt_nodes.push_back(move(var_def_node2));
  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(if_body_node)));

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  vector< unique_ptr<StmtNode> > else_if_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(18), UINT32_C(18))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(19), UINT32_C(19)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(20), UINT32_C(20)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  else_if_body_stmt_nodes.push_back(move(var_def_node3));
  unique_ptr<DataTypeNode> data_type_node4(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(21), UINT32_C(21))));
  VarDefWithoutInitNode *var_def_node_ptr4 = new VarDefWithoutInitNode(
      move(data_type_node4),
      TokenInfo(Token::kName, "var4", UINT32_C(22), UINT32_C(22)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(23), UINT32_C(23)));
  unique_ptr<StmtNode> var_def_node4(var_def_node_ptr4);
  else_if_body_stmt_nodes.push_back(move(var_def_node4));
  ScopeNode *else_if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(17), UINT32_C(17)),
      move(else_if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(24), UINT32_C(24)));
  unique_ptr<ScopeNode> else_if_body_node(else_if_body_node_ptr);
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(15), UINT32_C(15)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(13), UINT32_C(13)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(14), UINT32_C(14)),
      move(bool_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(16), UINT32_C(16)),
      move(else_if_body_node)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(12), UINT32_C(12)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));
  unique_ptr<StmtNode> if_else_if_node(new IfElseIfNode(
      move(if_node), move(else_if_nodes)));
  program_stmt_nodes.push_back(move(if_else_if_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));
  vector<const VarDefNode*> else_if_body_local_var_defs =
      {var_def_node_ptr3, var_def_node_ptr4};
  unique_ptr<NodeSemanticAnalysis> else_if_body_analysis(
      new ScopeAnalysis(else_if_body_local_var_defs));
  node_analyzes.insert(
      make_pair(else_if_body_node_ptr, move(else_if_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new BoolDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  vector<const VarDefNode*> flow_local_var_defs3;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs3));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  vector<const VarDefNode*> flow_local_var_defs4 = {var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis4(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new DoubleDataType()), flow_local_var_defs4));
  node_analyzes.insert(make_pair(var_def_node_ptr4, move(var_def_analysis4)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<DataType> bool_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t else_if_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t branch_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));

  uint32_t else_if_address = cmds_code->GetPosition();
  int32_t else_if_offset = static_cast<int32_t>(else_if_address)
                           - (static_cast<int32_t>(else_if_offset_placeholder)
                              + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(else_if_offset_placeholder);
  cmds_code->Write<int32_t>(else_if_offset);
  cmds_code->SetPosition(else_if_address);
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(false);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t branch_end_offset_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalDoubleVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);

  uint32_t branch_end_address = cmds_code->GetPosition();
  int32_t branch_end_offset =
      static_cast<int32_t>(branch_end_address)
      - (static_cast<int32_t>(branch_end_offset_placeholder)
         + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder);
  cmds_code->Write<int32_t>(branch_end_offset);
  int32_t branch_end_offset2 =
      static_cast<int32_t>(branch_end_address)
      - (static_cast<int32_t>(branch_end_offset_placeholder2)
         + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder2);
  cmds_code->Write<int32_t>(branch_end_offset2);
  cmds_code->SetPosition(branch_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, IfWithoutVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  if_body_stmt_nodes.push_back(move(int_stmt_node));
  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(if_body_node)));
  unique_ptr<StmtNode> if_else_if_node(new IfElseIfNode(
      move(if_node), vector< unique_ptr<ElseIfNode> >()));
  program_stmt_nodes.push_back(move(if_else_if_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));
  unique_ptr<DataType> int_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> int_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_analysis)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t branch_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(1));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);

  uint32_t branch_end_address = cmds_code->GetPosition();
  int32_t branch_end_offset =
      static_cast<int32_t>(branch_end_address)
      - (static_cast<int32_t>(branch_end_offset_placeholder)
         + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder);
  cmds_code->Write<int32_t>(branch_end_offset);
  cmds_code->SetPosition(branch_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, IfWithLocalVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  if_body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  if_body_stmt_nodes.push_back(move(var_def_node2));
  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(if_body_node)));
  unique_ptr<StmtNode> if_else_if_node(new IfElseIfNode(
      move(if_node), vector< unique_ptr<ElseIfNode> >()));
  program_stmt_nodes.push_back(move(if_else_if_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t branch_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);

  uint32_t branch_end_address = cmds_code->GetPosition();
  int32_t branch_end_offset = static_cast<int32_t>(branch_end_address)
                              - (static_cast<int32_t>(branch_end_offset_placeholder)
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(branch_end_offset_placeholder);
  cmds_code->Write<int32_t>(branch_end_offset);
  cmds_code->SetPosition(branch_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, PreTestLoopWithoutVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > loop_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  loop_body_stmt_nodes.push_back(move(int_stmt_node));
  ScopeNode *loop_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(loop_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> loop_body_node(loop_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(loop_body_node)));
  program_stmt_nodes.push_back(move(loop_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> loop_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> loop_body_analysis(
      new ScopeAnalysis(loop_body_local_var_defs));
  node_analyzes.insert(make_pair(loop_body_node_ptr, move(loop_body_analysis)));
  unique_ptr<DataType> int_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> int_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_analysis)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t loop_start_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t loop_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(1));
  cmds_code->Write<CmdId>(CmdId::kUnloadInt);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset = static_cast<int32_t>(loop_start_address)
                              - (static_cast<int32_t>(cmds_code->GetPosition())
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset);

  uint32_t loop_end_address = cmds_code->GetPosition();
  int32_t loop_end_offset = static_cast<int32_t>(loop_end_address)
                            - (static_cast<int32_t>(loop_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder);
  cmds_code->Write<int32_t>(loop_end_offset);
  cmds_code->SetPosition(loop_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, PreTestLoopWithLocalVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > loop_body_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  loop_body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  loop_body_stmt_nodes.push_back(move(var_def_node2));
  ScopeNode *loop_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(loop_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> loop_body_node(loop_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(loop_body_node)));
  program_stmt_nodes.push_back(move(loop_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> loop_body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> loop_body_analysis(
      new ScopeAnalysis(loop_body_local_var_defs));
  node_analyzes.insert(make_pair(loop_body_node_ptr, move(loop_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t loop_start_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t loop_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset = static_cast<int32_t>(loop_start_address)
                              - (static_cast<int32_t>(cmds_code->GetPosition())
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset);

  uint32_t loop_end_address = cmds_code->GetPosition();
  int32_t loop_end_offset = static_cast<int32_t>(loop_end_address)
                            - (static_cast<int32_t>(loop_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder);
  cmds_code->Write<int32_t>(loop_end_offset);
  cmds_code->SetPosition(loop_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, BreakWithinLoopWithoutVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > loop_body_stmt_nodes;
  BreakNode *break_node_ptr = new BreakNode(
      TokenInfo(Token::kBreak, "break", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)));
  unique_ptr<StmtNode> break_node(break_node_ptr);
  loop_body_stmt_nodes.push_back(move(break_node));
  ScopeNode *loop_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(loop_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> loop_body_node(loop_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(loop_body_node)));
  program_stmt_nodes.push_back(move(loop_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> loop_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> loop_body_analysis(
      new ScopeAnalysis(loop_body_local_var_defs));
  node_analyzes.insert(make_pair(loop_body_node_ptr, move(loop_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> break_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes.insert(make_pair(break_node_ptr, move(break_analysis)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));

  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t loop_start_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t loop_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t loop_end_offset_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset = static_cast<int32_t>(loop_start_address)
                              - (static_cast<int32_t>(cmds_code->GetPosition())
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset);

  uint32_t loop_end_address = cmds_code->GetPosition();
  int32_t loop_end_offset = static_cast<int32_t>(loop_end_address)
                            - (static_cast<int32_t>(loop_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder);
  cmds_code->Write<int32_t>(loop_end_offset);
  int32_t loop_end_offset2 = static_cast<int32_t>(loop_end_address)
                             - (static_cast<int32_t>(loop_end_offset_placeholder2)
                                + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder2);
  cmds_code->Write<int32_t>(loop_end_offset2);
  cmds_code->SetPosition(loop_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, BreakWithinLoopWithLocalVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > loop_body_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(5), UINT32_C(5))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  loop_body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  loop_body_stmt_nodes.push_back(move(var_def_node2));
  BreakNode *break_node_ptr = new BreakNode(
      TokenInfo(Token::kBreak, "break", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
  unique_ptr<StmtNode> break_node(break_node_ptr);
  loop_body_stmt_nodes.push_back(move(break_node));
  unique_ptr<DataTypeNode> var_data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(13))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(14), UINT32_C(14)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  loop_body_stmt_nodes.push_back(move(var_def_node3));
  ScopeNode *loop_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(loop_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> loop_body_node(loop_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(loop_body_node)));
  program_stmt_nodes.push_back(move(loop_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> loop_body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2, var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> loop_body_analysis(
      new ScopeAnalysis(loop_body_local_var_defs));
  node_analyzes.insert(make_pair(loop_body_node_ptr, move(loop_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> break_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes.insert(make_pair(break_node_ptr, move(break_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new BoolDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs3 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs3));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  vector<const VarDefNode*> flow_local_var_defs4 =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs4));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t loop_start_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t loop_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  uint32_t loop_end_offset_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset = static_cast<int32_t>(loop_start_address)
                              - (static_cast<int32_t>(cmds_code->GetPosition())
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset);

  uint32_t loop_end_address = cmds_code->GetPosition();
  int32_t loop_end_offset = static_cast<int32_t>(loop_end_address)
                            - (static_cast<int32_t>(loop_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder);
  cmds_code->Write<int32_t>(loop_end_offset);
  int32_t loop_end_offset2 = static_cast<int32_t>(loop_end_address)
                             - (static_cast<int32_t>(loop_end_offset_placeholder2)
                                + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder2);
  cmds_code->Write<int32_t>(loop_end_offset2);
  cmds_code->SetPosition(loop_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, ContinueWithinLoopWithoutVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > loop_body_stmt_nodes;
  ContinueNode *continue_node_ptr = new ContinueNode(
      TokenInfo(Token::kContinue, "continue", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)));
  unique_ptr<StmtNode> continue_node(continue_node_ptr);
  loop_body_stmt_nodes.push_back(move(continue_node));
  ScopeNode *loop_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(loop_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> loop_body_node(loop_body_node_ptr);
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(loop_body_node)));
  program_stmt_nodes.push_back(move(loop_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> loop_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> loop_body_analysis(
      new ScopeAnalysis(loop_body_local_var_defs));
  node_analyzes.insert(make_pair(loop_body_node_ptr, move(loop_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> continue_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes.insert(make_pair(continue_node_ptr, move(continue_analysis)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t loop_start_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t loop_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset = static_cast<int32_t>(loop_start_address)
                              - (static_cast<int32_t>(cmds_code->GetPosition())
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset2 = static_cast<int32_t>(loop_start_address)
                               - (static_cast<int32_t>(cmds_code->GetPosition())
                                  + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset2);

  uint32_t loop_end_address = cmds_code->GetPosition();
  int32_t loop_end_offset = static_cast<int32_t>(loop_end_address)
                            - (static_cast<int32_t>(loop_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder);
  cmds_code->Write<int32_t>(loop_end_offset);
  cmds_code->SetPosition(loop_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, ContinueWithinLoopWithLocalVarDefs) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > loop_body_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(5), UINT32_C(5))));
  auto *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  loop_body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(8), UINT32_C(8))));
  auto *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  loop_body_stmt_nodes.push_back(move(var_def_node2));
  auto *continue_node_ptr = new ContinueNode(
      TokenInfo(Token::kContinue, "continue", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
  unique_ptr<StmtNode> continue_node(continue_node_ptr);
  loop_body_stmt_nodes.push_back(move(continue_node));
  unique_ptr<DataTypeNode> var_data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(13))));
  auto *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(14), UINT32_C(14)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  loop_body_stmt_nodes.push_back(move(var_def_node3));
  auto *loop_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(loop_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> loop_body_node(loop_body_node_ptr);
  auto *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(loop_body_node)));
  program_stmt_nodes.push_back(move(loop_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> loop_body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2, var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> loop_body_analysis(
      new ScopeAnalysis(loop_body_local_var_defs));
  node_analyzes.insert(make_pair(loop_body_node_ptr, move(loop_body_analysis)));
  vector<const VarDefNode*> flow_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> continue_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes.insert(make_pair(continue_node_ptr, move(continue_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new BoolDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs3 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs3));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  vector<const VarDefNode*> flow_local_var_defs4 =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs4));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t loop_start_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kJumpIfNot);
  uint32_t loop_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset = static_cast<int32_t>(loop_start_address)
                              - (static_cast<int32_t>(cmds_code->GetPosition())
                                 + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kDirectJump);
  int32_t loop_start_offset2 = static_cast<int32_t>(loop_start_address)
                               - (static_cast<int32_t>(cmds_code->GetPosition())
                                  + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->Write<int32_t>(loop_start_offset2);

  uint32_t loop_end_address = cmds_code->GetPosition();
  int32_t loop_end_offset = static_cast<int32_t>(loop_end_address)
                            - (static_cast<int32_t>(loop_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(loop_end_offset_placeholder);
  cmds_code->Write<int32_t>(loop_end_offset);
  cmds_code->SetPosition(loop_end_address);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndLongArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(3), UINT32_C(3))));
  LongDataType arg_data_type;
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalLongVar);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalLongVar);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndIntArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  IntDataType arg_data_type;
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalIntVar);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalIntVar);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndDoubleArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(3), UINT32_C(3))));
  DoubleDataType arg_data_type;
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalDoubleVar);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalDoubleVar);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndCharArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(3), UINT32_C(3))));
  CharDataType arg_data_type;
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalCharVar);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalCharVar);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndBoolArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(3), UINT32_C(3))));
  BoolDataType arg_data_type;
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalBoolVar);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndStringArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(3), UINT32_C(3))));
  StringDataType arg_data_type;
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalStringVar);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalStringVar);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndLongArrayArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(3), UINT32_C(3))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
  unique_ptr<DataType> element_data_type(new LongDataType());
  unique_ptr<DataType> array_data_type(
      new ArrayDataType(move(element_data_type)));
  ArrayDataType arg_data_type(move(array_data_type));
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalLongArrayVar);
  uint8_t dimensions_count = UINT8_C(2);
  expected_create_arg_code.Write<uint8_t>(dimensions_count);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalLongArrayVar);
  expected_destroy_arg_code.Write<uint8_t>(dimensions_count);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndIntArrayArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
  unique_ptr<DataType> element_data_type(new IntDataType());
  unique_ptr<DataType> array_data_type(
      new ArrayDataType(move(element_data_type)));
  ArrayDataType arg_data_type(move(array_data_type));
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalIntArrayVar);
  uint8_t dimensions_count = UINT8_C(2);
  expected_create_arg_code.Write<uint8_t>(dimensions_count);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalIntArrayVar);
  expected_destroy_arg_code.Write<uint8_t>(dimensions_count);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndDoubleArrayArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(3), UINT32_C(3))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
  unique_ptr<DataType> element_data_type(new DoubleDataType());
  unique_ptr<DataType> array_data_type(
      new ArrayDataType(move(element_data_type)));
  ArrayDataType arg_data_type(move(array_data_type));
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalDoubleArrayVar);
  uint8_t dimensions_count = UINT8_C(2);
  expected_create_arg_code.Write<uint8_t>(dimensions_count);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalDoubleArrayVar);
  expected_destroy_arg_code.Write<uint8_t>(dimensions_count);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndCharArrayArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(3), UINT32_C(3))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
  unique_ptr<DataType> element_data_type(new CharDataType());
  unique_ptr<DataType> array_data_type(
      new ArrayDataType(move(element_data_type)));
  ArrayDataType arg_data_type(move(array_data_type));
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalCharArrayVar);
  uint8_t dimensions_count = UINT8_C(2);
  expected_create_arg_code.Write<uint8_t>(dimensions_count);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalCharArrayVar);
  expected_destroy_arg_code.Write<uint8_t>(dimensions_count);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndBoolArrayArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(3), UINT32_C(3))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
  unique_ptr<DataType> element_data_type(new BoolDataType());
  unique_ptr<DataType> array_data_type(
      new ArrayDataType(move(element_data_type)));
  ArrayDataType arg_data_type(move(array_data_type));
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalBoolArrayVar);
  uint8_t dimensions_count = UINT8_C(2);
  expected_create_arg_code.Write<uint8_t>(dimensions_count);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalBoolArrayVar);
  expected_destroy_arg_code.Write<uint8_t>(dimensions_count);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithBodyAndStringArrayArg) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(3), UINT32_C(3))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(7))));
  unique_ptr<DataType> element_data_type(new StringDataType());
  unique_ptr<DataType> array_data_type(
      new ArrayDataType(move(element_data_type)));
  ArrayDataType arg_data_type(move(array_data_type));
  Code expected_create_arg_code;
  expected_create_arg_code.Write<CmdId>(CmdId::kCreateAndInitLocalStringArrayVar);
  uint8_t dimensions_count = UINT8_C(2);
  expected_create_arg_code.Write<uint8_t>(dimensions_count);
  Code expected_destroy_arg_code;
  expected_destroy_arg_code.Write<CmdId>(CmdId::kDestroyLocalStringArrayVar);
  expected_destroy_arg_code.Write<uint8_t>(dimensions_count);
  TestFuncDefWithBodyAndArg(move(arg_data_type_node),
                            arg_data_type,
                            expected_create_arg_code,
                            expected_destroy_arg_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnWithoutValue) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(5), UINT32_C(5))));
  auto *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  body_stmt_nodes.push_back(move(var_def_node));
  unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(8), UINT32_C(8))));
  auto *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  body_stmt_nodes.push_back(move(var_def_node2));
  auto *return_node_ptr = new ReturnWithoutValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
  unique_ptr<StmtNode> return_node(return_node_ptr);
  body_stmt_nodes.push_back(move(return_node));
  unique_ptr<DataTypeNode> var_data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(13))));
  auto *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(14), UINT32_C(14)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  body_stmt_nodes.push_back(move(var_def_node3));
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  vector<TokenInfo> modifier_tokens;
  vector<TokenInfo> arg_separator_tokens;
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      modifier_tokens,
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  program_stmt_nodes.push_back(move(func_def_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new BoolDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {var_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  vector<const VarDefNode*> flow_local_var_defs3 =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs3));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  vector<const VarDefNode*> flow_local_var_defs4 =
      {var_def_node_ptr, var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> return_analysis(
      new ReturnAnalysis(func_def_node_ptr, flow_local_var_defs4));
  node_analyzes.insert(make_pair(return_node_ptr, move(return_analysis)));
  vector<const VarDefNode*> body_local_var_defs =
      {var_def_node_ptr, var_def_node_ptr2, var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = true;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      move(func_data_type), is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  uint32_t func_def_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  cmds_code->Write<CmdId>(CmdId::kReturn);
  cmds_code->Write<CmdId>(CmdId::kCreateLocalLongVar);

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs = {{"func", func_def_address}};
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, ReturnIntValue) {
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> return_data_type(new IntDataType());

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  expected_value_code.Write<CmdId>(CmdId::kCastCharToInt);

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  move(test_casts),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnLongValue) {
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> return_data_type(new LongDataType());

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));
  expected_value_code.Write<CmdId>(CmdId::kCastIntToLong);

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  move(test_casts),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnDoubleValue) {
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> return_data_type(new DoubleDataType());

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));
  expected_value_code.Write<CmdId>(CmdId::kCastIntToDouble);

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  move(test_casts),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnBoolValue) {
  BoolNode *value_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> return_data_type(new BoolDataType());

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadBoolValue);
  expected_value_code.Write<bool>(true);

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  vector<TestCast>(),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnCharValue) {
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> return_data_type(new CharDataType());

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  vector<TestCast>(),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnStringValue) {
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> return_data_type(new StringDataType());

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type(new StringDataType());
  unique_ptr<NodeSemanticAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  expected_value_code.Write<CmdId>(CmdId::kCastCharToString);

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  move(test_casts),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, ReturnArrayValue) {
  unique_ptr<PrimitiveDataTypeNode> bool_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(7), UINT32_C(7))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_sizes;
  unique_ptr<UnboundedArraySizeNode> array_size(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(9), UINT32_C(9))));
  array_sizes.push_back(move(array_size));
  vector< unique_ptr<ExprNode> > array_values;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(6), UINT32_C(6)),
      move(bool_data_type_node),
      move(array_sizes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      move(array_values),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  unique_ptr<DataTypeNode> bool_data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> return_data_type_node(new ArrayDataTypeNode(
      move(bool_data_type_node2),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  unique_ptr<DataType> return_data_type(
      new ArrayDataType(unique_ptr<DataType>(new BoolDataType())));

  SemanticAnalysis::NodeAnalyzes value_analyzes;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      return_data_type->Clone(),
      move(value_casted_data_type),
      ValueType::kRight));
  value_analyzes.insert(make_pair(value_node_ptr, move(value_analysis)));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitBoolArray);
  uint8_t dimensions_count = UINT8_C(1);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<uint8_t>(dimensions_count);
  expected_value_code.Write<int32_t>(values_count);

  TestReturnValue(move(value_node),
                  move(return_data_type_node),
                  move(return_data_type),
                  move(value_analyzes),
                  vector<TestCast>(),
                  expected_value_code);
}

TEST_F(SimpleCodeGeneratorTest, FuncDefWithoutBody) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(4), UINT32_C(4))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));

  unique_ptr<DataTypeNode> return_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector<TokenInfo> modifier_tokens =
      {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
  vector<TokenInfo> arg_separator_tokens;
  FuncDefWithoutBodyNode *func_def_node_ptr = new FuncDefWithoutBodyNode(
      modifier_tokens,
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  program_stmt_nodes.push_back(move(func_def_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));
  unique_ptr<DataType> return_data_type(new IntDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new LongDataType()));
  bool is_func_native = true;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      move(func_data_type), is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs = {{"func"}};
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalIntVar) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  IntDataType data_type;
  CmdId create_var_cmd_id = CmdId::kCreateGlobalIntVar;
  CmdId expected_load_cmd_id = CmdId::kLoadGlobalIntVarValue;
  Code expected_unload_code;
  expected_unload_code.Write<CmdId>(CmdId::kUnloadInt);
  TestIdAsNotAssigneeGlobalVar(move(data_type_node),
                               data_type,
                               DataTypeSize::kInt,
                               create_var_cmd_id,
                               expected_load_cmd_id,
                               expected_unload_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalLongVar) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  LongDataType data_type;
  CmdId create_var_cmd_id = CmdId::kCreateGlobalLongVar;
  CmdId expected_load_cmd_id = CmdId::kLoadGlobalLongVarValue;
  Code expected_unload_code;
  expected_unload_code.Write<CmdId>(CmdId::kUnloadLong);
  TestIdAsNotAssigneeGlobalVar(move(data_type_node),
                               data_type,
                               DataTypeSize::kLong,
                               create_var_cmd_id,
                               expected_load_cmd_id,
                               expected_unload_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalDoubleVar) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  DoubleDataType data_type;
  CmdId create_var_cmd_id = CmdId::kCreateGlobalDoubleVar;
  CmdId expected_load_cmd_id = CmdId::kLoadGlobalDoubleVarValue;
  Code expected_unload_code;
  expected_unload_code.Write<CmdId>(CmdId::kUnloadDouble);
  TestIdAsNotAssigneeGlobalVar(move(data_type_node),
                               data_type,
                               DataTypeSize::kDouble,
                               create_var_cmd_id,
                               expected_load_cmd_id,
                               expected_unload_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalBoolVar) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  BoolDataType data_type;
  CmdId create_var_cmd_id = CmdId::kCreateGlobalBoolVar;
  CmdId expected_load_cmd_id = CmdId::kLoadGlobalBoolVarValue;
  Code expected_unload_code;
  expected_unload_code.Write<CmdId>(CmdId::kUnloadBool);
  TestIdAsNotAssigneeGlobalVar(move(data_type_node),
                               data_type,
                               DataTypeSize::kBool,
                               create_var_cmd_id,
                               expected_load_cmd_id,
                               expected_unload_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalCharVar) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  CharDataType data_type;
  CmdId create_var_cmd_id = CmdId::kCreateGlobalCharVar;
  CmdId expected_load_cmd_id = CmdId::kLoadGlobalCharVarValue;
  Code expected_unload_code;
  expected_unload_code.Write<CmdId>(CmdId::kUnloadChar);
  TestIdAsNotAssigneeGlobalVar(move(data_type_node),
                               data_type,
                               DataTypeSize::kChar,
                               create_var_cmd_id,
                               expected_load_cmd_id,
                               expected_unload_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalStringVar) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  StringDataType data_type;
  CmdId create_var_cmd_id = CmdId::kCreateGlobalStringVar;
  CmdId expected_load_cmd_id = CmdId::kLoadGlobalStringVarValue;
  Code expected_unload_code;
  expected_unload_code.Write<CmdId>(CmdId::kUnloadString);
  TestIdAsNotAssigneeGlobalVar(move(data_type_node),
                               data_type,
                               DataTypeSize::kString,
                               create_var_cmd_id,
                               expected_load_cmd_id,
                               expected_unload_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalIntArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  TestIdAsNotAssigneeGlobalArrayVar(move(data_type_node),
                                    IntDataType(),
                                    CmdId::kLoadGlobalIntArrayVarValue,
                                    CmdId::kUnloadIntArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalLongArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  TestIdAsNotAssigneeGlobalArrayVar(move(data_type_node),
                                    LongDataType(),
                                    CmdId::kLoadGlobalLongArrayVarValue,
                                    CmdId::kUnloadLongArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalDoubleArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  TestIdAsNotAssigneeGlobalArrayVar(move(data_type_node),
                                    DoubleDataType(),
                                    CmdId::kLoadGlobalDoubleArrayVarValue,
                                    CmdId::kUnloadDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalCharArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  TestIdAsNotAssigneeGlobalArrayVar(move(data_type_node),
                                    CharDataType(),
                                    CmdId::kLoadGlobalCharArrayVarValue,
                                    CmdId::kUnloadCharArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalBoolArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "int", UINT32_C(0), UINT32_C(0))));
  TestIdAsNotAssigneeGlobalArrayVar(move(data_type_node),
                                    BoolDataType(),
                                    CmdId::kLoadGlobalBoolArrayVarValue,
                                    CmdId::kUnloadBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeGlobalStringArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  TestIdAsNotAssigneeGlobalArrayVar(move(data_type_node),
                                    StringDataType(),
                                    CmdId::kLoadGlobalStringArrayVarValue,
                                    CmdId::kUnloadStringArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalIntVar) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(4), UINT32_C(4))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  expected_code->Write<CmdId>(CmdId::kCreateLocalIntVar);
  expected_code->Write<CmdId>(CmdId::kLoadLocalIntVarValue);
  expected_code->Write<uint32_t>(UINT32_C(1));
  expected_code->Write<CmdId>(CmdId::kUnloadInt);
  TestIdAsNotAssigneeLocalVar(move(data_type_node),
                              move(data_type_node2),
                              IntDataType(),
                              move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalLongVar) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(4), UINT32_C(4))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  expected_code->Write<CmdId>(CmdId::kCreateLocalLongVar);
  expected_code->Write<CmdId>(CmdId::kLoadLocalLongVarValue);
  expected_code->Write<uint32_t>(UINT32_C(2));
  expected_code->Write<CmdId>(CmdId::kUnloadLong);
  TestIdAsNotAssigneeLocalVar(move(data_type_node),
                              move(data_type_node2),
                              LongDataType(),
                              move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalDoubleVar) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(4), UINT32_C(4))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
  expected_code->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
  expected_code->Write<CmdId>(CmdId::kLoadLocalDoubleVarValue);
  expected_code->Write<uint32_t>(UINT32_C(2));
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);
  TestIdAsNotAssigneeLocalVar(move(data_type_node),
                              move(data_type_node2),
                              DoubleDataType(),
                              move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalBoolVar) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(4), UINT32_C(4))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  expected_code->Write<CmdId>(CmdId::kCreateLocalBoolVar);
  expected_code->Write<CmdId>(CmdId::kLoadLocalBoolVarValue);
  expected_code->Write<uint32_t>(UINT32_C(1));
  expected_code->Write<CmdId>(CmdId::kUnloadBool);
  TestIdAsNotAssigneeLocalVar(move(data_type_node),
                              move(data_type_node2),
                              BoolDataType(),
                              move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalCharVar) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(4), UINT32_C(4))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kCreateLocalCharVar);
  expected_code->Write<CmdId>(CmdId::kCreateLocalCharVar);
  expected_code->Write<CmdId>(CmdId::kLoadLocalCharVarValue);
  expected_code->Write<uint32_t>(UINT32_C(1));
  expected_code->Write<CmdId>(CmdId::kUnloadChar);
  TestIdAsNotAssigneeLocalVar(move(data_type_node),
                              move(data_type_node2),
                              CharDataType(),
                              move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalStringVar) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(4), UINT32_C(4))));
  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kCreateLocalStringVar);
  expected_code->Write<CmdId>(CmdId::kCreateLocalStringVar);
  expected_code->Write<CmdId>(CmdId::kLoadLocalStringVarValue);
  expected_code->Write<uint32_t>(UINT32_C(1));
  expected_code->Write<CmdId>(CmdId::kUnloadString);
  TestIdAsNotAssigneeLocalVar(move(data_type_node),
                              move(data_type_node2),
                              StringDataType(),
                              move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalIntArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  TestIdAsNotAssigneeLocalArrayVar(move(data_type_node),
                                   move(data_type_node2),
                                   IntDataType(),
                                   CmdId::kUnloadIntArray,
                                   CmdId::kLoadLocalIntArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalLongArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(1))));
  TestIdAsNotAssigneeLocalArrayVar(move(data_type_node),
                                   move(data_type_node2),
                                   LongDataType(),
                                   CmdId::kUnloadLongArray,
                                   CmdId::kLoadLocalLongArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalDoubleArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(1))));
  TestIdAsNotAssigneeLocalArrayVar(move(data_type_node),
                                   move(data_type_node2),
                                   DoubleDataType(),
                                   CmdId::kUnloadDoubleArray,
                                   CmdId::kLoadLocalDoubleArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalCharArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(1))));
  TestIdAsNotAssigneeLocalArrayVar(move(data_type_node),
                                   move(data_type_node2),
                                   CharDataType(),
                                   CmdId::kUnloadCharArray,
                                   CmdId::kLoadLocalCharArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalBoolArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(1))));
  TestIdAsNotAssigneeLocalArrayVar(move(data_type_node),
                                   move(data_type_node2),
                                   BoolDataType(),
                                   CmdId::kUnloadBoolArray,
                                   CmdId::kLoadLocalBoolArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, IdAsNotAssigneeLocalStringArrayVar) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(1))));
  TestIdAsNotAssigneeLocalArrayVar(move(data_type_node),
                                   move(data_type_node2),
                                   StringDataType(),
                                   CmdId::kUnloadStringArray,
                                   CmdId::kLoadLocalStringArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalIntVar) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5))));
  IntDataType id_data_type;

  unique_ptr<DataType> value_data_type(new CharDataType());
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  expected_value_code.Write<CmdId>(CmdId::kCastCharToInt);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreInt);
  TestIdAsAssigneeGlobalVar(move(data_type_node),
                            move(value_node),
                            id_data_type,
                            move(value_analysis),
                            move(test_casts),
                            DataTypeSize::kInt,
                            CmdId::kCreateGlobalIntVar,
                            expected_value_code,
                            expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalLongVar) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5))));
  LongDataType id_data_type;

  unique_ptr<DataType> value_data_type(new IntDataType());
  unique_ptr<DataType> value_casted_data_type(new LongDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));
  expected_value_code.Write<CmdId>(CmdId::kCastIntToLong);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreLong);
  TestIdAsAssigneeGlobalVar(move(data_type_node),
                            move(value_node),
                            id_data_type,
                            move(value_analysis),
                            move(test_casts),
                            DataTypeSize::kLong,
                            CmdId::kCreateGlobalLongVar,
                            expected_value_code,
                            expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalDoubleVar) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5))));
  DoubleDataType data_type;

  unique_ptr<DataType> value_data_type(new IntDataType());
  unique_ptr<DataType> value_casted_data_type(new DoubleDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));
  expected_value_code.Write<CmdId>(CmdId::kCastIntToDouble);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreDouble);
  TestIdAsAssigneeGlobalVar(move(data_type_node),
                            move(value_node),
                            data_type,
                            move(value_analysis),
                            move(test_casts),
                            DataTypeSize::kDouble,
                            CmdId::kCreateGlobalDoubleVar,
                            expected_value_code,
                            expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalBoolVar) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(5))));
  BoolDataType data_type;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadBoolValue);
  expected_value_code.Write<bool>(true);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreBool);
  TestIdAsAssigneeGlobalVar(move(data_type_node),
                            move(value_node),
                            data_type,
                            move(value_analysis),
                            vector<TestCast>(),
                            DataTypeSize::kBool,
                            CmdId::kCreateGlobalBoolVar,
                            expected_value_code,
                            expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalCharVar) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5))));
  CharDataType data_type;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreChar);
  TestIdAsAssigneeGlobalVar(move(data_type_node),
                            move(value_node),
                            data_type,
                            move(value_analysis),
                            vector<TestCast>(),
                            DataTypeSize::kChar,
                            CmdId::kCreateGlobalCharVar,
                            expected_value_code,
                            expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalStringVar) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5))));
  StringDataType data_type;

  unique_ptr<DataType> value_data_type(new CharDataType());
  unique_ptr<DataType> value_casted_data_type(new StringDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  expected_value_code.Write<CmdId>(CmdId::kCastCharToString);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreString);
  TestIdAsAssigneeGlobalVar(move(data_type_node),
                            move(value_node),
                            data_type,
                            move(value_analysis),
                            move(test_casts),
                            DataTypeSize::kString,
                            CmdId::kCreateGlobalStringVar,
                            expected_value_code,
                            expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalIntArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestIdAsAssigneeGlobalArrayVar(move(def_data_type_node),
                                 move(value_data_type_node),
                                 move(data_type),
                                 CmdId::kCreateAndInitIntArray,
                                 CmdId::kStoreIntArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalLongArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestIdAsAssigneeGlobalArrayVar(move(def_data_type_node),
                                 move(value_data_type_node),
                                 move(data_type),
                                 CmdId::kCreateAndInitLongArray,
                                 CmdId::kStoreLongArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalDoubleArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestIdAsAssigneeGlobalArrayVar(move(def_data_type_node),
                                 move(value_data_type_node),
                                 move(data_type),
                                 CmdId::kCreateAndInitDoubleArray,
                                 CmdId::kStoreDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalCharArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestIdAsAssigneeGlobalArrayVar(move(def_data_type_node),
                                 move(value_data_type_node),
                                 move(data_type),
                                 CmdId::kCreateAndInitCharArray,
                                 CmdId::kStoreCharArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalBoolArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestIdAsAssigneeGlobalArrayVar(move(def_data_type_node),
                                 move(value_data_type_node),
                                 move(data_type),
                                 CmdId::kCreateAndInitBoolArray,
                                 CmdId::kStoreBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeGlobalStringArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestIdAsAssigneeGlobalArrayVar(move(def_data_type_node),
                                 move(value_data_type_node),
                                 move(data_type),
                                 CmdId::kCreateAndInitStringArray,
                                 CmdId::kStoreStringArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalIntVar) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5))));
  IntDataType id_data_type;

  unique_ptr<DataType> value_data_type(new CharDataType());
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  expected_value_code.Write<CmdId>(CmdId::kCastCharToInt);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreInt);
  TestIdAsAssigneeLocalVar(move(data_type_node),
                           move(value_node),
                           id_data_type,
                           move(value_analysis),
                           move(test_casts),
                           CmdId::kCreateLocalIntVar,
                           expected_value_code,
                           expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalLongVar) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5))));
  LongDataType id_data_type;

  unique_ptr<DataType> value_data_type(new IntDataType());
  unique_ptr<DataType> value_casted_data_type(new LongDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));
  expected_value_code.Write<CmdId>(CmdId::kCastIntToLong);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreLong);
  TestIdAsAssigneeLocalVar(move(data_type_node),
                           move(value_node),
                           id_data_type,
                           move(value_analysis),
                           move(test_casts),
                           CmdId::kCreateLocalLongVar,
                           expected_value_code,
                           expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalDoubleVar) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5))));
  DoubleDataType data_type;

  unique_ptr<DataType> value_data_type(new IntDataType());
  unique_ptr<DataType> value_casted_data_type(new DoubleDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadIntValue);
  expected_value_code.Write<int32_t>(INT32_C(7));
  expected_value_code.Write<CmdId>(CmdId::kCastIntToDouble);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreDouble);
  TestIdAsAssigneeLocalVar(move(data_type_node),
                           move(value_node),
                           data_type,
                           move(value_analysis),
                           move(test_casts),
                           CmdId::kCreateLocalDoubleVar,
                           expected_value_code,
                           expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalBoolVar) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(5))));
  BoolDataType data_type;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadBoolValue);
  expected_value_code.Write<bool>(true);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreBool);
  TestIdAsAssigneeLocalVar(move(data_type_node),
                           move(value_node),
                           data_type,
                           move(value_analysis),
                           vector<TestCast>(),
                           CmdId::kCreateLocalBoolVar,
                           expected_value_code,
                           expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalCharVar) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5))));
  CharDataType data_type;
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreChar);
  TestIdAsAssigneeLocalVar(move(data_type_node),
                           move(value_node),
                           data_type,
                           move(value_analysis),
                           vector<TestCast>(),
                           CmdId::kCreateLocalCharVar,
                           expected_value_code,
                           expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalStringVar) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5))));
  StringDataType data_type;

  unique_ptr<DataType> value_data_type(new CharDataType());
  unique_ptr<DataType> value_casted_data_type(new StringDataType());
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kLoadCharValue);
  expected_value_code.Write<char>('a');
  expected_value_code.Write<CmdId>(CmdId::kCastCharToString);
  Code expected_store_code;
  expected_store_code.Write<CmdId>(CmdId::kStoreString);
  TestIdAsAssigneeLocalVar(move(data_type_node),
                           move(value_node),
                           data_type,
                           move(value_analysis),
                           move(test_casts),
                           CmdId::kCreateLocalStringVar,
                           expected_value_code,
                           expected_store_code);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalIntArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestIdAsAssigneeLocalArrayVar(move(def_data_type_node),
                                move(value_data_type_node),
                                move(data_type),
                                CmdId::kCreateAndInitIntArray,
                                CmdId::kStoreIntArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalLongArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestIdAsAssigneeLocalArrayVar(move(def_data_type_node),
                                move(value_data_type_node),
                                move(data_type),
                                CmdId::kCreateAndInitLongArray,
                                CmdId::kStoreLongArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalDoubleArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestIdAsAssigneeLocalArrayVar(move(def_data_type_node),
                                move(value_data_type_node),
                                move(data_type),
                                CmdId::kCreateAndInitDoubleArray,
                                CmdId::kStoreDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalCharArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestIdAsAssigneeLocalArrayVar(move(def_data_type_node),
                                move(value_data_type_node),
                                move(data_type),
                                CmdId::kCreateAndInitCharArray,
                                CmdId::kStoreCharArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalBoolArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestIdAsAssigneeLocalArrayVar(move(def_data_type_node),
                                move(value_data_type_node),
                                move(data_type),
                                CmdId::kCreateAndInitBoolArray,
                                CmdId::kStoreBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, IdAsAssigneeLocalStringArrayVar) {
  unique_ptr<DataTypeNode> def_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(5), UINT32_C(5))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestIdAsAssigneeLocalArrayVar(move(def_data_type_node),
                                move(value_data_type_node),
                                move(data_type),
                                CmdId::kCreateAndInitStringArray,
                                CmdId::kStoreStringArray);
}

TEST_F(SimpleCodeGeneratorTest, NotNativeCall) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  vector<TokenInfo> arg_def_separator_tokens;
  unique_ptr<DataTypeNode> arg_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(3), UINT32_C(3))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  arg_def_separator_tokens.push_back(
      TokenInfo(Token::kSeparator, ",", UINT32_C(5), UINT32_C(5)));
  unique_ptr<DataTypeNode> arg_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(6), UINT32_C(6))));
  ArgDefNode *arg_def_node_ptr2 = new ArgDefNode(
      move(arg_data_type_node2),
      TokenInfo(Token::kName, "arg2", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ArgDefNode> arg_def_node2(arg_def_node_ptr2);
  arg_def_nodes.push_back(move(arg_def_node2));

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(9)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  vector<TokenInfo> modifier_tokens;
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      modifier_tokens,
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_def_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(8), UINT32_C(8)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  program_stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  vector< unique_ptr<ExprNode> > arg_expr_nodes;
  vector<TokenInfo> arg_expr_separator_tokens;
  IntNode *arg_expr_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(13), UINT32_C(13)));
  unique_ptr<ExprNode> arg_expr_node(arg_expr_node_ptr);
  arg_expr_nodes.push_back(move(arg_expr_node));
  arg_expr_separator_tokens.push_back(
      TokenInfo(Token::kSeparator, ",", UINT32_C(14), UINT32_C(14)));
  CharNode *arg_expr_node_ptr2 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(15), UINT32_C(15)));
  unique_ptr<ExprNode> arg_expr_node2(arg_expr_node_ptr2);
  arg_expr_nodes.push_back(move(arg_expr_node2));
  CallNode *call_expr_node_ptr = new CallNode(
      move(id_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(12), UINT32_C(12)),
      move(arg_expr_nodes),
      arg_expr_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ExprNode> call_expr_node(call_expr_node_ptr);
  unique_ptr<StmtNode> call_stmt_node(new ExprStmtNode(
      move(call_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(17))));
  program_stmt_nodes.push_back(move(call_stmt_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new LongDataType()));
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {arg_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(arg_def_node_ptr2, move(arg_def_analysis2)));
  vector<const VarDefNode*> body_local_var_defs =
      {arg_def_node_ptr, arg_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      func_data_type.Clone(),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
  unique_ptr<DataType> arg_expr_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> arg_expr_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(arg_expr_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(arg_expr_node_ptr, move(arg_expr_analysis)));
  unique_ptr<DataType> arg_expr_casted_data_type2(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> arg_expr_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(arg_expr_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(arg_expr_node_ptr2, move(arg_expr_analysis2)));
  unique_ptr<DataType> call_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> call_analysis(new CommonExprAnalysis(
      unique_ptr<DataType>(new VoidDataType()),
      move(call_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(call_expr_node_ptr, move(call_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));
  unique_ptr<DataType> dest_data_type2(new LongDataType());
  unique_ptr<DataType> src_data_type2(new IntDataType());
  TestCast test_cast2 =
      {move(dest_data_type2), move(src_data_type2), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast2));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadCharValue);
  cmds_code->Write<char>('a');
  cmds_code->Write<CmdId>(CmdId::kCastCharToInt);
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(1));
  cmds_code->Write<CmdId>(CmdId::kCastIntToLong);
  cmds_code->Write<CmdId>(CmdId::kLoadFuncValue);
  uint32_t func_ref_address = cmds_code->GetPosition();
  uint32_t func_address = numeric_limits<uint32_t>::max();
  cmds_code->Write<uint32_t>(func_address);
  cmds_code->Write<CmdId>(CmdId::kCall);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  uint32_t func_def_address = cmds_code->GetPosition();
  cmds_code->Write<CmdId>(CmdId::kCreateAndInitLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kCreateAndInitLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  cmds_code->Write<CmdId>(CmdId::kDestroyLocalLongVar);
  cmds_code->Write<CmdId>(CmdId::kReturn);

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs = {{"func", func_def_address}};
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs = {{"func", {func_ref_address}}};
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(move(test_casts),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, NativeCall) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  vector<TokenInfo> arg_def_separator_tokens;
  unique_ptr<DataTypeNode> arg_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(4), UINT32_C(4))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  arg_def_separator_tokens.push_back(
      TokenInfo(Token::kSeparator, ",", UINT32_C(6), UINT32_C(6)));
  unique_ptr<DataTypeNode> arg_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(7))));
  ArgDefNode *arg_def_node_ptr2 = new ArgDefNode(
      move(arg_data_type_node2),
      TokenInfo(Token::kName, "arg2", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ArgDefNode> arg_def_node2(arg_def_node_ptr2);
  arg_def_nodes.push_back(move(arg_def_node2));

  vector<TokenInfo> modifier_tokens =
      {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  FuncDefWithoutBodyNode *func_def_node_ptr = new FuncDefWithoutBodyNode(
      modifier_tokens,
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      move(arg_def_nodes),
      arg_def_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  program_stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  vector< unique_ptr<ExprNode> > arg_expr_nodes;
  vector<TokenInfo> arg_expr_separator_tokens;
  IntNode *arg_expr_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(13), UINT32_C(13)));
  unique_ptr<ExprNode> arg_expr_node(arg_expr_node_ptr);
  arg_expr_nodes.push_back(move(arg_expr_node));
  arg_expr_separator_tokens.push_back(
      TokenInfo(Token::kSeparator, ",", UINT32_C(14), UINT32_C(14)));
  CharNode *arg_expr_node_ptr2 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(15), UINT32_C(15)));
  unique_ptr<ExprNode> arg_expr_node2(arg_expr_node_ptr2);
  arg_expr_nodes.push_back(move(arg_expr_node2));
  CallNode *call_expr_node_ptr = new CallNode(
      move(id_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(12), UINT32_C(12)),
      move(arg_expr_nodes),
      arg_expr_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ExprNode> call_expr_node(call_expr_node_ptr);
  unique_ptr<StmtNode> call_stmt_node(new ExprStmtNode(
      move(call_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(17))));
  program_stmt_nodes.push_back(move(call_stmt_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new LongDataType()));
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  bool is_func_native = true;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), flow_local_var_defs));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));
  vector<const VarDefNode*> flow_local_var_defs2 = {arg_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis2(new LocalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), flow_local_var_defs2));
  node_analyzes.insert(make_pair(arg_def_node_ptr2, move(arg_def_analysis2)));
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      func_data_type.Clone(),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
  unique_ptr<DataType> arg_expr_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> arg_expr_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(arg_expr_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(arg_expr_node_ptr, move(arg_expr_analysis)));
  unique_ptr<DataType> arg_expr_casted_data_type2(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> arg_expr_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(arg_expr_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(arg_expr_node_ptr2, move(arg_expr_analysis2)));
  unique_ptr<DataType> call_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> call_analysis(new CommonExprAnalysis(
      unique_ptr<DataType>(new VoidDataType()),
      move(call_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(call_expr_node_ptr, move(call_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));
  unique_ptr<DataType> dest_data_type2(new LongDataType());
  unique_ptr<DataType> src_data_type2(new IntDataType());
  TestCast test_cast2 =
      {move(dest_data_type2), move(src_data_type2), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast2));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadCharValue);
  cmds_code->Write<char>('a');
  cmds_code->Write<CmdId>(CmdId::kCastCharToInt);
  cmds_code->Write<CmdId>(CmdId::kLoadIntValue);
  cmds_code->Write<int32_t>(INT32_C(1));
  cmds_code->Write<CmdId>(CmdId::kCastIntToLong);
  cmds_code->Write<CmdId>(CmdId::kLoadNativeFuncValue);
  uint32_t func_ref_address = cmds_code->GetPosition();
  uint32_t func_index = numeric_limits<uint32_t>::max();
  cmds_code->Write<uint32_t>(func_index);
  cmds_code->Write<CmdId>(CmdId::kCallNative);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs = {"func"};
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs =
      {{"func", {func_ref_address}}};
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(move(test_casts),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, NotAssigneeSubscriptWithIntArray) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  ArrayDataType element_data_type(unique_ptr<DataType>(new IntDataType()));
  Code expected_code;
  expected_code.Write<CmdId>(CmdId::kLoadIntArrayElementValue);
  uint8_t dimensions_count = UINT8_C(2);
  expected_code.Write<uint8_t>(dimensions_count);
  expected_code.Write<CmdId>(CmdId::kUnloadIntArray);
  uint8_t dimensions_count2 = UINT8_C(1);
  expected_code.Write<uint8_t>(dimensions_count2);
  TestNotAssigneeSubscriptWithArray(move(element_data_type_node),
                                    element_data_type,
                                    expected_code,
                                    CmdId::kLoadGlobalIntArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, NotAssigneeSubscriptWithLongArray) {
  unique_ptr<DataTypeNode> long_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(long_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  ArrayDataType element_data_type(unique_ptr<DataType>(new LongDataType()));
  Code expected_code;
  expected_code.Write<CmdId>(CmdId::kLoadLongArrayElementValue);
  uint8_t dimensions_count = UINT8_C(2);
  expected_code.Write<uint8_t>(dimensions_count);
  expected_code.Write<CmdId>(CmdId::kUnloadLongArray);
  uint8_t dimensions_count2 = UINT8_C(1);
  expected_code.Write<uint8_t>(dimensions_count2);
  TestNotAssigneeSubscriptWithArray(move(element_data_type_node),
                                    element_data_type,
                                    expected_code,
                                    CmdId::kLoadGlobalLongArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, NotAssigneeSubscriptWithDoubleArray) {
  unique_ptr<DataTypeNode> double_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(double_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  ArrayDataType element_data_type(unique_ptr<DataType>(new DoubleDataType()));
  Code expected_code;
  expected_code.Write<CmdId>(CmdId::kLoadDoubleArrayElementValue);
  uint8_t dimensions_count = UINT8_C(2);
  expected_code.Write<uint8_t>(dimensions_count);
  expected_code.Write<CmdId>(CmdId::kUnloadDoubleArray);
  uint8_t dimensions_count2 = UINT8_C(1);
  expected_code.Write<uint8_t>(dimensions_count2);
  TestNotAssigneeSubscriptWithArray(move(element_data_type_node),
                                    element_data_type,
                                    expected_code,
                                    CmdId::kLoadGlobalDoubleArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, NotAssigneeSubscriptWithBoolArray) {
  unique_ptr<DataTypeNode> bool_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(bool_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  ArrayDataType element_data_type(unique_ptr<DataType>(new BoolDataType()));
  Code expected_code;
  expected_code.Write<CmdId>(CmdId::kLoadBoolArrayElementValue);
  uint8_t dimensions_count = UINT8_C(2);
  expected_code.Write<uint8_t>(dimensions_count);
  expected_code.Write<CmdId>(CmdId::kUnloadBoolArray);
  uint8_t dimensions_count2 = UINT8_C(1);
  expected_code.Write<uint8_t>(dimensions_count2);
  TestNotAssigneeSubscriptWithArray(move(element_data_type_node),
                                    element_data_type,
                                    expected_code,
                                    CmdId::kLoadGlobalBoolArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, NotAssigneeSubscriptWithCharArray) {
  unique_ptr<DataTypeNode> char_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(char_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  ArrayDataType element_data_type(unique_ptr<DataType>(new CharDataType()));
  Code expected_code;
  expected_code.Write<CmdId>(CmdId::kLoadCharArrayElementValue);
  uint8_t dimensions_count = UINT8_C(2);
  expected_code.Write<uint8_t>(dimensions_count);
  expected_code.Write<CmdId>(CmdId::kUnloadCharArray);
  uint8_t dimensions_count2 = UINT8_C(1);
  expected_code.Write<uint8_t>(dimensions_count2);
  TestNotAssigneeSubscriptWithArray(move(element_data_type_node),
                                    element_data_type,
                                    expected_code,
                                    CmdId::kLoadGlobalCharArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, NotAssigneeSubscriptWithStringArray) {
  unique_ptr<DataTypeNode> string_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(string_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  ArrayDataType element_data_type(unique_ptr<DataType>(new StringDataType()));
  Code expected_code;
  expected_code.Write<CmdId>(CmdId::kLoadStringArrayElementValue);
  uint8_t dimensions_count = UINT8_C(2);
  expected_code.Write<uint8_t>(dimensions_count);
  expected_code.Write<CmdId>(CmdId::kUnloadStringArray);
  uint8_t dimensions_count2 = UINT8_C(1);
  expected_code.Write<uint8_t>(dimensions_count2);
  TestNotAssigneeSubscriptWithArray(move(element_data_type_node),
                                    element_data_type,
                                    expected_code,
                                    CmdId::kLoadGlobalStringArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, AssigneeSubscriptWithIntArray) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));

  unique_ptr<PrimitiveDataTypeNode> int_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
  unique_ptr<UnboundedArraySizeNode> array_size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  array_size_nodes.push_back(move(array_size_node));
  vector< unique_ptr<ExprNode> > array_value_nodes;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
      move(int_data_type_node2),
      move(array_size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      move(array_value_nodes),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  ArrayDataType element_data_type(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      element_data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitIntArray);
  uint8_t dimensions_count = UINT8_C(1);
  expected_value_code.Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<int32_t>(values_count);

  Code expected_assign_code;
  expected_assign_code.Write<CmdId>(CmdId::kLoadIntArrayElementAddress);
  uint8_t dimensions_count2 = UINT8_C(2);
  expected_assign_code.Write<uint8_t>(dimensions_count2);
  expected_assign_code.Write<CmdId>(CmdId::kStoreIntArray);
  expected_assign_code.Write<uint8_t>(dimensions_count);

  TestAssigneeSubscriptWithArray(
      move(element_data_type_node),
      move(value_node),
      element_data_type,
      move(value_analysis),
      expected_value_code,
      expected_assign_code,
      CmdId::kLoadGlobalIntArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, AssigneeSubscriptWithLongArray) {
  unique_ptr<DataTypeNode> long_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(long_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));

  unique_ptr<PrimitiveDataTypeNode> long_data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
  unique_ptr<UnboundedArraySizeNode> array_size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  array_size_nodes.push_back(move(array_size_node));
  vector< unique_ptr<ExprNode> > array_value_nodes;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
      move(long_data_type_node2),
      move(array_size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      move(array_value_nodes),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  ArrayDataType element_data_type(unique_ptr<DataType>(new LongDataType()));
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      element_data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitLongArray);
  uint8_t dimensions_count = UINT8_C(1);
  expected_value_code.Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<int32_t>(values_count);

  Code expected_assign_code;
  expected_assign_code.Write<CmdId>(CmdId::kLoadLongArrayElementAddress);
  uint8_t dimensions_count2 = UINT8_C(2);
  expected_assign_code.Write<uint8_t>(dimensions_count2);
  expected_assign_code.Write<CmdId>(CmdId::kStoreLongArray);
  expected_assign_code.Write<uint8_t>(dimensions_count);

  TestAssigneeSubscriptWithArray(
      move(element_data_type_node),
      move(value_node),
      element_data_type,
      move(value_analysis),
      expected_value_code,
      expected_assign_code,
      CmdId::kLoadGlobalLongArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, AssigneeSubscriptWithDoubleArray) {
  unique_ptr<DataTypeNode> double_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(double_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));

  unique_ptr<PrimitiveDataTypeNode> double_data_type_node2(
      new DoubleDataTypeNode(
          TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
  unique_ptr<UnboundedArraySizeNode> array_size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  array_size_nodes.push_back(move(array_size_node));
  vector< unique_ptr<ExprNode> > array_value_nodes;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
      move(double_data_type_node2),
      move(array_size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      move(array_value_nodes),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  ArrayDataType element_data_type(unique_ptr<DataType>(new DoubleDataType()));
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      element_data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitDoubleArray);
  uint8_t dimensions_count = UINT8_C(1);
  expected_value_code.Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<int32_t>(values_count);

  Code expected_assign_code;
  expected_assign_code.Write<CmdId>(CmdId::kLoadDoubleArrayElementAddress);
  uint8_t dimensions_count2 = UINT8_C(2);
  expected_assign_code.Write<uint8_t>(dimensions_count2);
  expected_assign_code.Write<CmdId>(CmdId::kStoreDoubleArray);
  expected_assign_code.Write<uint8_t>(dimensions_count);

  TestAssigneeSubscriptWithArray(
      move(element_data_type_node),
      move(value_node),
      element_data_type,
      move(value_analysis),
      expected_value_code,
      expected_assign_code,
      CmdId::kLoadGlobalDoubleArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, AssigneeSubscriptWithBoolArray) {
  unique_ptr<DataTypeNode> bool_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(bool_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));

  unique_ptr<PrimitiveDataTypeNode> bool_data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
  unique_ptr<UnboundedArraySizeNode> array_size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  array_size_nodes.push_back(move(array_size_node));
  vector< unique_ptr<ExprNode> > array_value_nodes;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
      move(bool_data_type_node2),
      move(array_size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      move(array_value_nodes),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  ArrayDataType element_data_type(unique_ptr<DataType>(new BoolDataType()));
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      element_data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitBoolArray);
  uint8_t dimensions_count = UINT8_C(1);
  expected_value_code.Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<int32_t>(values_count);

  Code expected_assign_code;
  expected_assign_code.Write<CmdId>(CmdId::kLoadBoolArrayElementAddress);
  uint8_t dimensions_count2 = UINT8_C(2);
  expected_assign_code.Write<uint8_t>(dimensions_count2);
  expected_assign_code.Write<CmdId>(CmdId::kStoreBoolArray);
  expected_assign_code.Write<uint8_t>(dimensions_count);

  TestAssigneeSubscriptWithArray(
      move(element_data_type_node),
      move(value_node),
      element_data_type,
      move(value_analysis),
      expected_value_code,
      expected_assign_code,
      CmdId::kLoadGlobalBoolArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, AssigneeSubscriptWithCharArray) {
  unique_ptr<DataTypeNode> char_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(char_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));

  unique_ptr<PrimitiveDataTypeNode> char_data_type_node2(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
  unique_ptr<UnboundedArraySizeNode> array_size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  array_size_nodes.push_back(move(array_size_node));
  vector< unique_ptr<ExprNode> > array_value_nodes;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
      move(char_data_type_node2),
      move(array_size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      move(array_value_nodes),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  ArrayDataType element_data_type(unique_ptr<DataType>(new CharDataType()));
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      element_data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitCharArray);
  uint8_t dimensions_count = UINT8_C(1);
  expected_value_code.Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<int32_t>(values_count);

  Code expected_assign_code;
  expected_assign_code.Write<CmdId>(CmdId::kLoadCharArrayElementAddress);
  uint8_t dimensions_count2 = UINT8_C(2);
  expected_assign_code.Write<uint8_t>(dimensions_count2);
  expected_assign_code.Write<CmdId>(CmdId::kStoreCharArray);
  expected_assign_code.Write<uint8_t>(dimensions_count);

  TestAssigneeSubscriptWithArray(
      move(element_data_type_node),
      move(value_node),
      element_data_type,
      move(value_analysis),
      expected_value_code,
      expected_assign_code,
      CmdId::kLoadGlobalCharArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, AssigneeSubscriptWithStringArray) {
  unique_ptr<DataTypeNode> string_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> element_data_type_node(new ArrayDataTypeNode(
      move(string_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));

  unique_ptr<PrimitiveDataTypeNode> string_data_type_node2(
      new StringDataTypeNode(
          TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  vector< unique_ptr<UnboundedArraySizeNode> > array_size_nodes;
  unique_ptr<UnboundedArraySizeNode> array_size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  array_size_nodes.push_back(move(array_size_node));
  vector< unique_ptr<ExprNode> > array_value_nodes;
  vector<TokenInfo> array_value_separator_tokens;
  ArrayAllocWithInitNode *value_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "new", UINT32_C(10), UINT32_C(10)),
      move(string_data_type_node2),
      move(array_size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      move(array_value_nodes),
      array_value_separator_tokens,
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> value_node(value_node_ptr);

  ArrayDataType element_data_type(unique_ptr<DataType>(new StringDataType()));
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_analysis(new CommonExprAnalysis(
      element_data_type.Clone(),
      move(value_casted_data_type),
      ValueType::kRight));

  Code expected_value_code;
  expected_value_code.Write<CmdId>(CmdId::kCreateAndInitStringArray);
  uint8_t dimensions_count = UINT8_C(1);
  expected_value_code.Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(0);
  expected_value_code.Write<int32_t>(values_count);

  Code expected_assign_code;
  expected_assign_code.Write<CmdId>(CmdId::kLoadStringArrayElementAddress);
  uint8_t dimensions_count2 = UINT8_C(2);
  expected_assign_code.Write<uint8_t>(dimensions_count2);
  expected_assign_code.Write<CmdId>(CmdId::kStoreStringArray);
  expected_assign_code.Write<uint8_t>(dimensions_count);

  TestAssigneeSubscriptWithArray(
      move(element_data_type_node),
      move(value_node),
      element_data_type,
      move(value_analysis),
      expected_value_code,
      expected_assign_code,
      CmdId::kLoadGlobalStringArrayVarValue);
}

TEST_F(SimpleCodeGeneratorTest, And) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  AndNode *and_expr_node_ptr = new AndNode(
      TokenInfo(Token::kAndOp, "&&", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      move(bool_node2));
  unique_ptr<ExprNode> and_expr_node(and_expr_node_ptr);
  unique_ptr<StmtNode> and_stmt_node(new ExprStmtNode(
      move(and_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  program_stmt_nodes.push_back(move(and_stmt_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> bool_data_type(new BoolDataType());
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      move(bool_data_type),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<DataType> bool_data_type2(new BoolDataType());
  unique_ptr<DataType> bool_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      move(bool_data_type2),
      move(bool_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));
  unique_ptr<DataType> and_data_type(new BoolDataType());
  unique_ptr<DataType> and_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> and_analysis(new CommonExprAnalysis(
      move(and_data_type),
      move(and_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(and_expr_node_ptr, move(and_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kImplicitJumpIfNot);
  uint32_t expr_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(false);
  cmds_code->Write<CmdId>(CmdId::kAnd);
  uint32_t expr_end_address = cmds_code->GetPosition();
  int32_t expr_end_offset = static_cast<int32_t>(expr_end_address)
                            - (static_cast<int32_t>(expr_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(expr_end_offset_placeholder);
  cmds_code->Write<int32_t>(expr_end_offset);
  cmds_code->SetPosition(expr_end_address);
  cmds_code->Write<CmdId>(CmdId::kUnloadBool);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, Or) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  OrNode *or_expr_node_ptr = new OrNode(
      TokenInfo(Token::kOrOp, "||", UINT32_C(1), UINT32_C(1)),
      move(bool_node),
      move(bool_node2));
  unique_ptr<ExprNode> or_expr_node(or_expr_node_ptr);
  unique_ptr<StmtNode> or_stmt_node(new ExprStmtNode(
      move(or_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  program_stmt_nodes.push_back(move(or_stmt_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> bool_data_type(new BoolDataType());
  unique_ptr<DataType> bool_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      move(bool_data_type),
      move(bool_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<DataType> bool_data_type2(new BoolDataType());
  unique_ptr<DataType> bool_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      move(bool_data_type2),
      move(bool_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));
  unique_ptr<DataType> or_data_type(new BoolDataType());
  unique_ptr<DataType> or_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> or_analysis(new CommonExprAnalysis(
      move(or_data_type),
      move(or_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(or_expr_node_ptr, move(or_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::ProgramProblems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(true);
  cmds_code->Write<CmdId>(CmdId::kImplicitJumpIf);
  uint32_t expr_end_offset_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(int32_t));
  cmds_code->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds_code->Write<bool>(false);
  cmds_code->Write<CmdId>(CmdId::kOr);
  uint32_t expr_end_address = cmds_code->GetPosition();
  int32_t expr_end_offset = static_cast<int32_t>(expr_end_address)
                            - (static_cast<int32_t>(expr_end_offset_placeholder)
                               + static_cast<int32_t>(sizeof(int32_t)));
  cmds_code->SetPosition(expr_end_offset_placeholder);
  cmds_code->Write<int32_t>(expr_end_offset);
  cmds_code->SetPosition(expr_end_address);
  cmds_code->Write<CmdId>(CmdId::kUnloadBool);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();

  vector<IdSize> global_var_defs;
  vector<IdAddress> func_defs;
  vector<string> native_func_defs;
  vector<IdAddresses> global_var_refs;
  vector<IdAddresses> func_refs;
  vector<IdAddresses> native_func_refs;
  uint32_t version = UINT32_C(1);
  Module expected_module(version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  TestGenerate(vector<TestCast>(),
               program_node,
               semantic_analysis,
               version,
               expected_module);
}

TEST_F(SimpleCodeGeneratorTest, MulChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new MulNode(
      TokenInfo(Token::kMulOp, "*", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new CharDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kMulChar);
  expected_code->Write<CmdId>(CmdId::kUnloadChar);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, MulInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new MulNode(
      TokenInfo(Token::kMulOp, "*", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kMulInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, MulLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new MulNode(
      TokenInfo(Token::kMulOp, "*", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kMulLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, MulDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new MulNode(
      TokenInfo(Token::kMulOp, "*", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kMulDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, DivChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new DivNode(
      TokenInfo(Token::kDivOp, "/", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new CharDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kDivChar);
  expected_code->Write<CmdId>(CmdId::kUnloadChar);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, DivInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new DivNode(
      TokenInfo(Token::kDivOp, "/", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kDivInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, DivLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new DivNode(
      TokenInfo(Token::kDivOp, "/", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kDivLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, DivDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new DivNode(
      TokenInfo(Token::kDivOp, "/", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kDivDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SumChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new CharDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kSumChar);
  expected_code->Write<CmdId>(CmdId::kUnloadChar);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SumInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kSumInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SumLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kSumLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SumDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kSumDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SumString) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  StringNode *right_operand_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"bc\"", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new StringDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new StringDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("bc"))));
  unique_ptr<DataType> expr_data_type(new StringDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToString);
  expected_code->Write<CmdId>(CmdId::kLoadStringValue);
  expected_code->Write<string>("bc");
  expected_code->Write<CmdId>(CmdId::kSumString);
  expected_code->Write<CmdId>(CmdId::kUnloadString);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SubChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SubNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new CharDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kSubChar);
  expected_code->Write<CmdId>(CmdId::kUnloadChar);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SubInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SubNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kSubInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SubLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SubNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kSubLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, SubDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new SubNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kSubDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kEqualChar);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kEqualInt);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kEqualLong);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kEqualDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualString) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  StringNode *right_operand_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"bc\"", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new StringDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new StringDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("bc"))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToString);
  expected_code->Write<CmdId>(CmdId::kLoadStringValue);
  expected_code->Write<string>("bc");
  expected_code->Write<CmdId>(CmdId::kEqualString);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualBool) {
  BoolNode *left_operand_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  BoolNode *right_operand_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new BoolDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  unique_ptr<DataType> right_operand_data_type(new BoolDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadBoolValue);
  expected_code->Write<bool>(false);
  expected_code->Write<CmdId>(CmdId::kLoadBoolValue);
  expected_code->Write<bool>(true);
  expected_code->Write<CmdId>(CmdId::kEqualBool);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, EqualIntArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestEqualArray(move(left_data_type_node),
                 move(right_data_type_node),
                 move(data_type),
                 CmdId::kCreateAndInitIntArray,
                 CmdId::kEqualIntArray);
}

TEST_F(SimpleCodeGeneratorTest, EqualLongArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestEqualArray(move(left_data_type_node),
                 move(right_data_type_node),
                 move(data_type),
                 CmdId::kCreateAndInitLongArray,
                 CmdId::kEqualLongArray);
}

TEST_F(SimpleCodeGeneratorTest, EqualDoubleArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestEqualArray(move(left_data_type_node),
                 move(right_data_type_node),
                 move(data_type),
                 CmdId::kCreateAndInitDoubleArray,
                 CmdId::kEqualDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, EqualCharArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestEqualArray(move(left_data_type_node),
                 move(right_data_type_node),
                 move(data_type),
                 CmdId::kCreateAndInitCharArray,
                 CmdId::kEqualCharArray);
}

TEST_F(SimpleCodeGeneratorTest, EqualBoolArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestEqualArray(move(left_data_type_node),
                 move(right_data_type_node),
                 move(data_type),
                 CmdId::kCreateAndInitBoolArray,
                 CmdId::kEqualBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, EqualStringArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestEqualArray(move(left_data_type_node),
                 move(right_data_type_node),
                 move(data_type),
                 CmdId::kCreateAndInitStringArray,
                 CmdId::kEqualStringArray);
}

TEST_F(SimpleCodeGeneratorTest, NotEqualChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kNotEqualChar);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, NotEqualInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kNotEqualInt);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, NotEqualLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kNotEqualLong);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, NotEqualDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kNotEqualDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, NotEqualString) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  StringNode *right_operand_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"bc\"", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new StringDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new StringDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("bc"))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new StringDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToString};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToString);
  expected_code->Write<CmdId>(CmdId::kLoadStringValue);
  expected_code->Write<string>("bc");
  expected_code->Write<CmdId>(CmdId::kNotEqualString);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, NotEqualBool) {
  BoolNode *left_operand_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  BoolNode *right_operand_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new BoolDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  unique_ptr<DataType> right_operand_data_type(new BoolDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadBoolValue);
  expected_code->Write<bool>(false);
  expected_code->Write<CmdId>(CmdId::kLoadBoolValue);
  expected_code->Write<bool>(true);
  expected_code->Write<CmdId>(CmdId::kNotEqualBool);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, NotEqualIntArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestNotEqualArray(move(left_data_type_node),
                    move(right_data_type_node),
                    move(data_type),
                    CmdId::kCreateAndInitIntArray,
                    CmdId::kNotEqualIntArray);
}

TEST_F(SimpleCodeGeneratorTest, NotEqualLongArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestNotEqualArray(move(left_data_type_node),
                    move(right_data_type_node),
                    move(data_type),
                    CmdId::kCreateAndInitLongArray,
                    CmdId::kNotEqualLongArray);
}

TEST_F(SimpleCodeGeneratorTest, NotEqualDoubleArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestNotEqualArray(move(left_data_type_node),
                    move(right_data_type_node),
                    move(data_type),
                    CmdId::kCreateAndInitDoubleArray,
                    CmdId::kNotEqualDoubleArray);
}

TEST_F(SimpleCodeGeneratorTest, NotEqualCharArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestNotEqualArray(move(left_data_type_node),
                    move(right_data_type_node),
                    move(data_type),
                    CmdId::kCreateAndInitCharArray,
                    CmdId::kNotEqualCharArray);
}

TEST_F(SimpleCodeGeneratorTest, NotEqualBoolArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestNotEqualArray(move(left_data_type_node),
                    move(right_data_type_node),
                    move(data_type),
                    CmdId::kCreateAndInitBoolArray,
                    CmdId::kNotEqualBoolArray);
}

TEST_F(SimpleCodeGeneratorTest, NotEqualStringArray) {
  unique_ptr<PrimitiveDataTypeNode> left_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(1))));
  unique_ptr<PrimitiveDataTypeNode> right_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(10), UINT32_C(10))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestNotEqualArray(move(left_data_type_node),
                    move(right_data_type_node),
                    move(data_type),
                    CmdId::kCreateAndInitStringArray,
                    CmdId::kNotEqualStringArray);
}

TEST_F(SimpleCodeGeneratorTest, GreaterChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterNode(
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kGreaterChar);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterNode(
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kGreaterInt);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterNode(
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kGreaterLong);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterNode(
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kGreaterDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterOrEqualChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterOrEqualNode(
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kGreaterOrEqualChar);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterOrEqualInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterOrEqualNode(
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kGreaterOrEqualInt);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterOrEqualLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterOrEqualNode(
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kGreaterOrEqualLong);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, GreaterOrEqualDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new GreaterOrEqualNode(
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kGreaterOrEqualDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessNode(
      TokenInfo(Token::kLessOp, "<", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kLessChar);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessNode(
      TokenInfo(Token::kLessOp, "<", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kLessInt);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessNode(
      TokenInfo(Token::kLessOp, "<", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kLessLong);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessNode(
      TokenInfo(Token::kLessOp, "<", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kLessDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessOrEqualChar) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  CharNode *right_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessOrEqualNode(
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new CharDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('b');
  expected_code->Write<CmdId>(CmdId::kLessOrEqualChar);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 vector<TestCast>(),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessOrEqualInt) {
  CharNode *left_operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  IntNode *right_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessOrEqualNode(
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new CharDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> right_operand_data_type(new IntDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new IntDataType());
  unique_ptr<DataType> src_data_type(new CharDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastCharToInt};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kCastCharToInt);
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(2));
  expected_code->Write<CmdId>(CmdId::kLessOrEqualInt);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessOrEqualLong) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  LongNode *right_operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "2L", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessOrEqualNode(
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new LongDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(2)))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new LongDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToLong};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToLong);
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(2));
  expected_code->Write<CmdId>(CmdId::kLessOrEqualLong);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LessOrEqualDouble) {
  IntNode *left_operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> left_operand_node(left_operand_node_ptr);
  DoubleNode *right_operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> right_operand_node(right_operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new LessOrEqualNode(
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(1), UINT32_C(1)),
      move(left_operand_node),
      move(right_operand_node)));

  unique_ptr<DataType> left_operand_data_type(new IntDataType());
  unique_ptr<DataType> left_operand_casted_data_type(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> left_operand_analysis(new LitAnalysis(
      move(left_operand_data_type),
      move(left_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  unique_ptr<DataType> right_operand_data_type(new DoubleDataType());
  unique_ptr<DataType> right_operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> right_operand_analysis(new LitAnalysis(
      move(right_operand_data_type),
      move(right_operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  vector<TestCast> test_casts;
  unique_ptr<DataType> dest_data_type(new DoubleDataType());
  unique_ptr<DataType> src_data_type(new IntDataType());
  TestCast test_cast =
      {move(dest_data_type), move(src_data_type), CmdId::kCastIntToDouble};
  test_casts.push_back(move(test_cast));

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(1));
  expected_code->Write<CmdId>(CmdId::kCastIntToDouble);
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(2.2);
  expected_code->Write<CmdId>(CmdId::kLessOrEqualDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestBinaryExpr(left_operand_node_ptr,
                 right_operand_node_ptr,
                 move(expr_node),
                 move(left_operand_analysis),
                 move(right_operand_analysis),
                 move(expr_data_type),
                 move(test_casts),
                 move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, LogicalNegateBool) {
  BoolNode *operand_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NotNode(
      TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new BoolDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  unique_ptr<DataType> expr_data_type(new BoolDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadBoolValue);
  expected_code->Write<bool>(false);
  expected_code->Write<CmdId>(CmdId::kLogicalNegateBool);
  expected_code->Write<CmdId>(CmdId::kUnloadBool);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreDecChar) {
  CharNode *operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreDecNode(
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new CharDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> expr_data_type(new CharDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kPreDecChar);
  expected_code->Write<CmdId>(CmdId::kUnloadChar);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreDecInt) {
  IntNode *operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreDecNode(
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new IntDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(7));
  expected_code->Write<CmdId>(CmdId::kPreDecInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreDecLong) {
  LongNode *operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreDecNode(
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new LongDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(7)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(7));
  expected_code->Write<CmdId>(CmdId::kPreDecLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreDecDouble) {
  DoubleNode *operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreDecNode(
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new DoubleDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.7))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(7.7);
  expected_code->Write<CmdId>(CmdId::kPreDecDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreIncChar) {
  CharNode *operand_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreIncNode(
      TokenInfo(Token::kPreIncOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new CharDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  unique_ptr<DataType> expr_data_type(new CharDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadCharValue);
  expected_code->Write<char>('a');
  expected_code->Write<CmdId>(CmdId::kPreIncChar);
  expected_code->Write<CmdId>(CmdId::kUnloadChar);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreIncInt) {
  IntNode *operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreIncNode(
      TokenInfo(Token::kPreIncOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new IntDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(7));
  expected_code->Write<CmdId>(CmdId::kPreIncInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreIncLong) {
  LongNode *operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreIncNode(
      TokenInfo(Token::kPreIncOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new LongDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(7)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(7));
  expected_code->Write<CmdId>(CmdId::kPreIncLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, PreIncDouble) {
  DoubleNode *operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new PreIncNode(
      TokenInfo(Token::kPreIncOp, "--", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new DoubleDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.7))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(7.7);
  expected_code->Write<CmdId>(CmdId::kPreIncDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, ArithmeticNegateInt) {
  IntNode *operand_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NegativeNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new IntDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  unique_ptr<DataType> expr_data_type(new IntDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadIntValue);
  expected_code->Write<int32_t>(INT32_C(7));
  expected_code->Write<CmdId>(CmdId::kArithmeticNegateInt);
  expected_code->Write<CmdId>(CmdId::kUnloadInt);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, ArithmeticNegateLong) {
  LongNode *operand_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NegativeNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new LongDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(7)))));
  unique_ptr<DataType> expr_data_type(new LongDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadLongValue);
  expected_code->Write<int64_t>(INT64_C(7));
  expected_code->Write<CmdId>(CmdId::kArithmeticNegateLong);
  expected_code->Write<CmdId>(CmdId::kUnloadLong);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}

TEST_F(SimpleCodeGeneratorTest, ArithmeticNegateDouble) {
  DoubleNode *operand_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> operand_node(operand_node_ptr);
  unique_ptr<ExprNode> expr_node(new NegativeNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(operand_node)));

  unique_ptr<DataType> operand_data_type(new DoubleDataType());
  unique_ptr<DataType> operand_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> operand_analysis(new LitAnalysis(
      move(operand_data_type),
      move(operand_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.7))));
  unique_ptr<DataType> expr_data_type(new DoubleDataType());

  unique_ptr<Code> expected_code(new Code());
  expected_code->Write<CmdId>(CmdId::kLoadDoubleValue);
  expected_code->Write<double>(7.7);
  expected_code->Write<CmdId>(CmdId::kArithmeticNegateDouble);
  expected_code->Write<CmdId>(CmdId::kUnloadDouble);

  TestUnaryExpr(operand_node_ptr,
                move(expr_node),
                move(operand_analysis),
                move(expr_data_type),
                vector<TestCast>(),
                move(expected_code));
}
}
}
