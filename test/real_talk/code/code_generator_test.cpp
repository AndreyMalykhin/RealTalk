
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <limits>
#include <iterator>
#include "real_talk/parser/if_else_if_else_node.h"
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
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/value_type.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/string_lit.h"
#include "real_talk/semantic/double_lit.h"
#include "real_talk/semantic/char_lit.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/module.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/code/test_utils.h"

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
using boost::filesystem::path;
using boost::numeric_cast;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;
using real_talk::parser::StmtNode;
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
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::IfNode;
using real_talk::parser::ElseIfNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::VarDefAnalysis;
using real_talk::semantic::LitAnalysis;
using real_talk::semantic::ExprAnalysis;
using real_talk::semantic::CommonExprAnalysis;
using real_talk::semantic::DataType;
using real_talk::semantic::IntDataType;
using real_talk::semantic::LongDataType;
using real_talk::semantic::ArrayDataType;
using real_talk::semantic::DoubleDataType;
using real_talk::semantic::CharDataType;
using real_talk::semantic::StringDataType;
using real_talk::semantic::BoolDataType;
using real_talk::semantic::DataStorage;
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

class CodeGeneratorTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

  string PrintCode(istream &code) {
    unique_ptr<Module> module = ModuleReader().Read(code);
    ostringstream result;
    result << *module;
    return result.str();
  }

  void TestGenerate(const ProgramNode &program_node,
                    const SemanticAnalysis &semantic_analysis,
                    uint32_t version,
                    const Code &expected_code) {
    stringstream actual_stream;
    CodeGenerator generator;
    generator.Generate(program_node, semantic_analysis, version, actual_stream);

    actual_stream.seekg(0, ios::end);
    streampos actual_code_size = actual_stream.tellg();
    actual_stream.seekg(0);
    stringstream expected_stream;
    expected_stream.exceptions(ios::failbit | ios::badbit);
    expected_stream.write(
        reinterpret_cast<const char*>(expected_code.GetData()),
        expected_code.GetSize());
    ASSERT_EQ(expected_code.GetSize(), actual_code_size)
        << "expected=\n" << PrintCode(expected_stream)
        << "\nactual=\n" << PrintCode(actual_stream);
    unique_ptr<char[]> actual_stream_content(new char[actual_code_size]);
    actual_stream.read(actual_stream_content.get(), actual_code_size);
    ASSERT_TRUE(memcmp(expected_code.GetData(),
                       actual_stream_content.get(),
                       expected_code.GetSize()) == 0)
        << "expected=\n" << PrintCode(expected_stream)
        << "\nactual=\n" << PrintCode(actual_stream);
  }

  void TestCreateGlobalVarCmd(unique_ptr<DataTypeNode> data_type_node,
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
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new VarDefAnalysis(move(data_type), DataStorage::kGlobal));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::Problems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(expected_cmd_id);
    const uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->WriteUint32(var_index);
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);

    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs = {"var"};
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module module(version,
                  move(cmds_code),
                  id_addresses_of_func_defs,
                  ids_of_global_var_defs,
                  ids_of_native_func_defs,
                  id_addresses_of_func_refs,
                  id_addresses_of_global_var_refs,
                  import_file_paths);
    Code module_code;
    WriteModule(module, module_code);
    TestGenerate(program_node, semantic_analysis, version, module_code);
  }

  void TestCreateAndInitGlobalVarCmd(
      unique_ptr<DataTypeNode> data_type_node,
      unique_ptr<DataType> data_type,
      unique_ptr<ExprNode> value_node,
      SemanticAnalysis::NodeAnalyzes value_node_analyzes,
      const Code &value_code,
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
        new VarDefAnalysis(move(data_type), DataStorage::kGlobal));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::Problems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteBytes(value_code.GetData(), value_code.GetSize());
    cmds_code->WriteCmdId(expected_cmd_id);
    const uint32_t var_index = numeric_limits<uint32_t>::max();
    cmds_code->WriteUint32(var_index);
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);

    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs = {"var"};
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module module(version,
                  move(cmds_code),
                  id_addresses_of_func_defs,
                  ids_of_global_var_defs,
                  ids_of_native_func_defs,
                  id_addresses_of_func_refs,
                  id_addresses_of_global_var_refs,
                  import_file_paths);
    Code module_code;
    WriteModule(module, module_code);
    TestGenerate(program_node, semantic_analysis, version, module_code);
  }

  void TestCreateAndInitLocalVarCmd(
      unique_ptr<DataTypeNode> data_type_node,
      unique_ptr<DataType> data_type,
      unique_ptr<ExprNode> value_node,
      SemanticAnalysis::NodeAnalyzes value_node_analyzes,
      const Code &value_code,
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
        new VarDefAnalysis(move(data_type), DataStorage::kLocal));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::Problems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteBytes(value_code.GetData(), value_code.GetSize());
    cmds_code->WriteCmdId(expected_cmd_id);
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);

    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs;
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module module(version,
                  move(cmds_code),
                  id_addresses_of_func_defs,
                  ids_of_global_var_defs,
                  ids_of_native_func_defs,
                  id_addresses_of_func_refs,
                  id_addresses_of_global_var_refs,
                  import_file_paths);
    Code module_code;
    WriteModule(module, module_code);
    TestGenerate(program_node, semantic_analysis, version, module_code);
  }

  void TestCreateLocalVarCmd(unique_ptr<DataTypeNode> data_type_node,
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
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new VarDefAnalysis(move(data_type), DataStorage::kLocal));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::Problems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(expected_cmd_id);
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);

    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs;
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module module(version,
                  move(cmds_code),
                  id_addresses_of_func_defs,
                  ids_of_global_var_defs,
                  ids_of_native_func_defs,
                  id_addresses_of_func_refs,
                  id_addresses_of_global_var_refs,
                  import_file_paths);
    Code module_code;
    WriteModule(module, module_code);
    TestGenerate(program_node, semantic_analysis, version, module_code);
  }

  void TestLoadValueCmd(unique_ptr<LitNode> lit_node,
                        unique_ptr<LitAnalysis> lit_analysis,
                        const Code &expected_code) {
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
        SemanticAnalysis::Problems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteBytes(expected_code.GetData(), expected_code.GetSize());
    cmds_code->WriteCmdId(CmdId::kUnload);
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);

    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs;
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module module(version,
                  move(cmds_code),
                  id_addresses_of_func_defs,
                  ids_of_global_var_defs,
                  ids_of_native_func_defs,
                  id_addresses_of_func_refs,
                  id_addresses_of_global_var_refs,
                  import_file_paths);
    Code module_code;
    WriteModule(module, module_code);
    TestGenerate(program_node, semantic_analysis, version, module_code);
  }

  void TestCreateArrayCmd(
      unique_ptr<PrimitiveDataTypeNode> primitive_data_type_node,
      unique_ptr<DataType> primitive_data_type,
      CmdId expected_cmd_id) {
    vector< unique_ptr<StmtNode> > program_stmt_nodes;
    vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
    IntNode *size_expr_node_ptr1 = new IntNode(
        TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)));
    unique_ptr<ExprNode> size_expr_node1(size_expr_node_ptr1);
    unique_ptr<BoundedArraySizeNode> size_node1(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
        move(size_expr_node1),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
    size_nodes.push_back(move(size_node1));
    IntNode *size_expr_node_ptr2 = new IntNode(
        TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(6)));
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
    unique_ptr<NodeSemanticAnalysis> size_lit_analysis1(new LitAnalysis(
        unique_ptr<DataType>(new IntDataType()),
        ValueType::kRight,
        unique_ptr<Lit>(new IntLit(INT32_C(1)))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr1, move(size_lit_analysis1)));

    unique_ptr<NodeSemanticAnalysis> size_lit_analysis2(new LitAnalysis(
        unique_ptr<DataType>(new IntDataType()),
        ValueType::kRight,
        unique_ptr<Lit>(new IntLit(INT32_C(2)))));
    node_analyzes.insert(
        make_pair(size_expr_node_ptr2, move(size_lit_analysis2)));

    unique_ptr<DataType> array_data_type1(
        new ArrayDataType(move(primitive_data_type)));
    unique_ptr<DataType> array_data_type2(
        new ArrayDataType(move(array_data_type1)));
    unique_ptr<NodeSemanticAnalysis> array_alloc_analysis(
        new CommonExprAnalysis(move(array_data_type2), ValueType::kRight));
    node_analyzes.insert(
        make_pair(array_alloc_node_ptr, move(array_alloc_analysis)));

    SemanticAnalysis semantic_analysis(
        SemanticAnalysis::Problems(), move(node_analyzes));

    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kLoadIntValue);
    cmds_code->WriteInt32(INT32_C(2));
    cmds_code->WriteCmdId(CmdId::kLoadIntValue);
    cmds_code->WriteInt32(INT32_C(1));
    cmds_code->WriteCmdId(expected_cmd_id);
    const uint8_t dimensions_count = UINT8_C(2);
    cmds_code->WriteUint8(dimensions_count);
    cmds_code->WriteCmdId(CmdId::kUnload);
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);

    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs;
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module module(version,
                  move(cmds_code),
                  id_addresses_of_func_defs,
                  ids_of_global_var_defs,
                  ids_of_native_func_defs,
                  id_addresses_of_func_refs,
                  id_addresses_of_global_var_refs,
                  import_file_paths);
    Code module_code;
    WriteModule(module, module_code);
    TestGenerate(program_node, semantic_analysis, version, module_code);
  }
};

TEST_F(CodeGeneratorTest, CreateGlobalIntVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestCreateGlobalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateGlobalIntVar);
}

TEST_F(CodeGeneratorTest, CreateGlobalArrayVarCmd) {
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

  TestCreateGlobalVarCmd(move(array_data_type_node2),
                         move(array_data_type2),
                         CmdId::kCreateGlobalArrayVar);
}

TEST_F(CodeGeneratorTest, CreateGlobalLongVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestCreateGlobalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateGlobalLongVar);
}

TEST_F(CodeGeneratorTest, CreateGlobalDoubleVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestCreateGlobalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateGlobalDoubleVar);
}

TEST_F(CodeGeneratorTest, CreateGlobalCharVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestCreateGlobalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateGlobalCharVar);
}

TEST_F(CodeGeneratorTest, CreateGlobalStringVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestCreateGlobalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateGlobalStringVar);
}

TEST_F(CodeGeneratorTest, CreateGlobalBoolVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestCreateGlobalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateGlobalBoolVar);
}

TEST_F(CodeGeneratorTest, CreateLocalIntVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestCreateLocalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLocalIntVar);
}

TEST_F(CodeGeneratorTest, CreateLocalArrayVarCmd) {
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

  TestCreateLocalVarCmd(move(array_data_type_node2),
                         move(array_data_type2),
                         CmdId::kCreateLocalArrayVar);
}

TEST_F(CodeGeneratorTest, CreateLocalLongVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestCreateLocalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLocalLongVar);
}

TEST_F(CodeGeneratorTest, CreateLocalDoubleVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestCreateLocalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLocalDoubleVar);
}

TEST_F(CodeGeneratorTest, CreateLocalCharVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestCreateLocalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLocalCharVar);
}

TEST_F(CodeGeneratorTest, CreateLocalStringVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestCreateLocalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLocalStringVar);
}

TEST_F(CodeGeneratorTest, CreateLocalBoolVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestCreateLocalVarCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLocalBoolVar);
}

TEST_F(CodeGeneratorTest, UnloadCmd) {
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
  unique_ptr<NodeSemanticAnalysis> int_lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_lit_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::Problems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kLoadIntValue);
  cmds_code->WriteInt32(INT32_C(7));
  cmds_code->WriteCmdId(CmdId::kUnload);
  cmds_code->WriteCmdId(CmdId::kEndMain);
  cmds_code->WriteCmdId(CmdId::kEndFuncs);

  vector<path> import_file_paths;
  vector<string> ids_of_global_var_defs;
  vector<IdAddress> id_addresses_of_func_defs;
  vector<string> ids_of_native_func_defs;
  vector<IdAddress> id_addresses_of_global_var_refs;
  vector<IdAddress> id_addresses_of_func_refs;
  uint32_t version = UINT32_C(1);
  Module module(version,
                move(cmds_code),
                id_addresses_of_func_defs,
                ids_of_global_var_defs,
                ids_of_native_func_defs,
                id_addresses_of_func_refs,
                id_addresses_of_global_var_refs,
                import_file_paths);
  Code module_code;
  WriteModule(module, module_code);
  TestGenerate(program_node, semantic_analysis, version, module_code);
}

TEST_F(CodeGeneratorTest, LoadIntValueCmd) {
  unique_ptr<LitNode> lit_node(new IntNode(
      TokenInfo(Token::kIntLit, "-7", UINT32_C(0), UINT32_C(0))));
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(-7)))));
  Code expected_code;
  expected_code.WriteCmdId(CmdId::kLoadIntValue);
  expected_code.WriteInt32(INT32_C(-7));
  TestLoadValueCmd(move(lit_node), move(lit_analysis), expected_code);
}

TEST_F(CodeGeneratorTest, LoadLongValueCmd) {
  unique_ptr<LitNode> lit_node(new LongNode(
      TokenInfo(Token::kLongLit, "-77L", UINT32_C(0), UINT32_C(0))));
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new LongDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(-77)))));
  Code expected_code;
  expected_code.WriteCmdId(CmdId::kLoadLongValue);
  expected_code.WriteInt64(INT64_C(-77));
  TestLoadValueCmd(move(lit_node), move(lit_analysis), expected_code);
}

TEST_F(CodeGeneratorTest, LoadBoolValueCmd) {
  unique_ptr<LitNode> lit_node(new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0))));
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  Code expected_code;
  expected_code.WriteCmdId(CmdId::kLoadBoolValue);
  expected_code.WriteBool(false);
  TestLoadValueCmd(move(lit_node), move(lit_analysis), expected_code);
}

TEST_F(CodeGeneratorTest, LoadStringValueCmd) {
  unique_ptr<LitNode> lit_node(new StringNode(
      TokenInfo(Token::kStringLit, "\"swagger\"", UINT32_C(0), UINT32_C(0))));
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("swagger"))));
  Code expected_code;
  expected_code.WriteCmdId(CmdId::kLoadStringValue);
  expected_code.WriteString("swagger");
  TestLoadValueCmd(move(lit_node), move(lit_analysis), expected_code);
}

TEST_F(CodeGeneratorTest, LoadDoubleValueCmd) {
  unique_ptr<LitNode> lit_node(new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.77777777777", UINT32_C(0), UINT32_C(0))));
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new DoubleDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.77777777777))));
  Code expected_code;
  expected_code.WriteCmdId(CmdId::kLoadDoubleValue);
  expected_code.WriteDouble(7.77777777777);
  TestLoadValueCmd(move(lit_node), move(lit_analysis), expected_code);
}

TEST_F(CodeGeneratorTest, LoadCharValueCmd) {
  unique_ptr<LitNode> lit_node(new CharNode(
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(0), UINT32_C(0))));
  unique_ptr<LitAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('b'))));
  Code expected_code;
  expected_code.WriteCmdId(CmdId::kLoadCharValue);
  expected_code.WriteChar('b');
  TestLoadValueCmd(move(lit_node), move(lit_analysis), expected_code);
}

TEST_F(CodeGeneratorTest, Import) {
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
  unique_ptr<NodeSemanticAnalysis> file_path_lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("file.rt"))));
  node_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::Problems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kEndMain);
  cmds_code->WriteCmdId(CmdId::kEndFuncs);

  vector<path> import_file_paths = {"file.rt"};
  vector<string> ids_of_global_var_defs;
  vector<IdAddress> id_addresses_of_func_defs;
  vector<string> ids_of_native_func_defs;
  vector<IdAddress> id_addresses_of_global_var_refs;
  vector<IdAddress> id_addresses_of_func_refs;
  uint32_t version = UINT32_C(1);
  Module module(version,
                move(cmds_code),
                id_addresses_of_func_defs,
                ids_of_global_var_defs,
                ids_of_native_func_defs,
                id_addresses_of_func_refs,
                id_addresses_of_global_var_refs,
                import_file_paths);
  Code module_code;
  WriteModule(module, module_code);
  TestGenerate(program_node, semantic_analysis, version, module_code);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalIntVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadIntValue);
  value_code.WriteInt32(INT32_C(7));
  TestCreateAndInitGlobalVarCmd(move(data_type_node),
                                move(data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalIntVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalArrayVarCmd) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));

  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(7))));
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

  unique_ptr<DataType> int_data_type(new IntDataType());
  unique_ptr<DataType> array_data_type1(
      new ArrayDataType(move(int_data_type)));
  unique_ptr<DataType> var_data_type(
      new ArrayDataType(move(array_data_type1)));

  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<NodeSemanticAnalysis> size_lit_analysis1(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  value_node_analyzes.insert(
      make_pair(size_expr_node_ptr1, move(size_lit_analysis1)));

  unique_ptr<NodeSemanticAnalysis> size_lit_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  value_node_analyzes.insert(
      make_pair(size_expr_node_ptr2, move(size_lit_analysis2)));

  unique_ptr<NodeSemanticAnalysis> value_analysis(
      new CommonExprAnalysis(var_data_type->Clone(), ValueType::kRight));
  value_node_analyzes.insert(
      make_pair(value_node_ptr, move(value_analysis)));

  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadIntValue);
  value_code.WriteInt32(INT32_C(2));
  value_code.WriteCmdId(CmdId::kLoadIntValue);
  value_code.WriteInt32(INT32_C(1));
  value_code.WriteCmdId(CmdId::kCreateIntArray);
  const uint8_t dimensions_count = UINT8_C(2);
  value_code.WriteUint8(dimensions_count);

  TestCreateAndInitGlobalVarCmd(move(var_data_type_node),
                                move(var_data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalArrayVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalLongVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<ExprNode> value_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new LongDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadLongValue);
  value_code.WriteInt64(INT64_C(7));
  TestCreateAndInitGlobalVarCmd(move(data_type_node),
                                move(data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalLongVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalDoubleVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<ExprNode> value_node(new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.77", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new DoubleDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.77))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadDoubleValue);
  value_code.WriteDouble(7.77);
  TestCreateAndInitGlobalVarCmd(move(data_type_node),
                                move(data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalDoubleVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalCharVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadCharValue);
  value_code.WriteChar('a');
  TestCreateAndInitGlobalVarCmd(move(data_type_node),
                                move(data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalCharVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalStringVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<ExprNode> value_node(new StringNode(
      TokenInfo(Token::kStringLit, "\"ab\"", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("ab"))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadStringValue);
  value_code.WriteString("ab");
  TestCreateAndInitGlobalVarCmd(move(data_type_node),
                                move(data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalStringVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitGlobalBoolVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<ExprNode> value_node(new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadBoolValue);
  value_code.WriteBool(true);
  TestCreateAndInitGlobalVarCmd(move(data_type_node),
                                move(data_type),
                                move(value_node),
                                move(value_node_analyzes),
                                move(value_code),
                                CmdId::kCreateAndInitGlobalBoolVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalIntVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new IntDataType());
  unique_ptr<ExprNode> value_node(new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadIntValue);
  value_code.WriteInt32(INT32_C(7));
  TestCreateAndInitLocalVarCmd(move(data_type_node),
                               move(data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalIntVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalArrayVarCmd) {
  unique_ptr<DataTypeNode> int_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(int_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
      move(array_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));

  unique_ptr<PrimitiveDataTypeNode> value_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(7))));
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

  unique_ptr<DataType> int_data_type(new IntDataType());
  unique_ptr<DataType> array_data_type1(
      new ArrayDataType(move(int_data_type)));
  unique_ptr<DataType> var_data_type(
      new ArrayDataType(move(array_data_type1)));

  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<NodeSemanticAnalysis> size_lit_analysis1(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  value_node_analyzes.insert(
      make_pair(size_expr_node_ptr1, move(size_lit_analysis1)));

  unique_ptr<NodeSemanticAnalysis> size_lit_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  value_node_analyzes.insert(
      make_pair(size_expr_node_ptr2, move(size_lit_analysis2)));

  unique_ptr<NodeSemanticAnalysis> value_analysis(
      new CommonExprAnalysis(var_data_type->Clone(), ValueType::kRight));
  value_node_analyzes.insert(
      make_pair(value_node_ptr, move(value_analysis)));

  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadIntValue);
  value_code.WriteInt32(INT32_C(2));
  value_code.WriteCmdId(CmdId::kLoadIntValue);
  value_code.WriteInt32(INT32_C(1));
  value_code.WriteCmdId(CmdId::kCreateIntArray);
  const uint8_t dimensions_count = UINT8_C(2);
  value_code.WriteUint8(dimensions_count);

  TestCreateAndInitLocalVarCmd(move(var_data_type_node),
                               move(var_data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalArrayVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalLongVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new LongDataType());
  unique_ptr<ExprNode> value_node(new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new LongDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT32_C(7)))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadLongValue);
  value_code.WriteInt64(INT64_C(7));
  TestCreateAndInitLocalVarCmd(move(data_type_node),
                               move(data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalLongVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalDoubleVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  unique_ptr<ExprNode> value_node(new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.77", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new DoubleDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.77))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadDoubleValue);
  value_code.WriteDouble(7.77);
  TestCreateAndInitLocalVarCmd(move(data_type_node),
                               move(data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalDoubleVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalCharVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new CharDataType());
  unique_ptr<ExprNode> value_node(new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadCharValue);
  value_code.WriteChar('a');
  TestCreateAndInitLocalVarCmd(move(data_type_node),
                               move(data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalCharVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalStringVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new StringDataType());
  unique_ptr<ExprNode> value_node(new StringNode(
      TokenInfo(Token::kStringLit, "\"ab\"", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("ab"))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadStringValue);
  value_code.WriteString("ab");
  TestCreateAndInitLocalVarCmd(move(data_type_node),
                               move(data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalStringVar);
}

TEST_F(CodeGeneratorTest, CreateAndInitLocalBoolVarCmd) {
  unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataType> data_type(new BoolDataType());
  unique_ptr<ExprNode> value_node(new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(3), UINT32_C(3))));
  SemanticAnalysis::NodeAnalyzes value_node_analyzes;
  unique_ptr<ExprAnalysis> value_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  value_node_analyzes.insert(make_pair(value_node.get(), move(value_analysis)));
  Code value_code;
  value_code.WriteCmdId(CmdId::kLoadBoolValue);
  value_code.WriteBool(true);
  TestCreateAndInitLocalVarCmd(move(data_type_node),
                               move(data_type),
                               move(value_node),
                               move(value_node_analyzes),
                               move(value_code),
                               CmdId::kCreateAndInitLocalBoolVar);
}

TEST_F(CodeGeneratorTest, CreateIntArrayCmd) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new IntDataType());
  TestCreateArrayCmd(
      move(data_type_node), move(data_type), CmdId::kCreateIntArray);
}

TEST_F(CodeGeneratorTest, CreateLongArrayCmd) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new LongDataType());
  TestCreateArrayCmd(
      move(data_type_node), move(data_type), CmdId::kCreateLongArray);
}

TEST_F(CodeGeneratorTest, CreateDoubleArrayCmd) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new DoubleDataTypeNode(
      TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new DoubleDataType());
  TestCreateArrayCmd(
      move(data_type_node), move(data_type), CmdId::kCreateDoubleArray);
}

TEST_F(CodeGeneratorTest, CreateBoolArrayCmd) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new BoolDataType());
  TestCreateArrayCmd(
      move(data_type_node), move(data_type), CmdId::kCreateBoolArray);
}

TEST_F(CodeGeneratorTest, CreateCharArrayCmd) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new CharDataType());
  TestCreateArrayCmd(
      move(data_type_node), move(data_type), CmdId::kCreateCharArray);
}

TEST_F(CodeGeneratorTest, CreateStringArrayCmd) {
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataType> data_type(new StringDataType());
  TestCreateArrayCmd(
      move(data_type_node), move(data_type), CmdId::kCreateStringArray);
}

TEST_F(CodeGeneratorTest, JumpIfNotCmd) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  if_body_stmt_nodes.push_back(move(int_stmt_node));
  unique_ptr<ScopeNode> if_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));
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
  unique_ptr<ScopeNode> else_if_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      move(else_if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16))));
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
  unique_ptr<ScopeNode> else_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(18), UINT32_C(18)),
      move(else_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21))));

  unique_ptr<StmtNode> if_else_if_else_node(new IfElseIfElseNode(
      move(if_node),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(17), UINT32_C(17)),
      move(else_body_node)));
  program_stmt_nodes.push_back(move(if_else_if_else_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> int_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_analysis)));
  unique_ptr<NodeSemanticAnalysis> int_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(int_node_ptr2, move(int_analysis2)));
  unique_ptr<NodeSemanticAnalysis> int_analysis3(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(3)))));
  node_analyzes.insert(make_pair(int_node_ptr3, move(int_analysis3)));
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));

  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::Problems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kLoadBoolValue);
  cmds_code->WriteBool(true);
  cmds_code->WriteCmdId(CmdId::kJumpIfNot);
  uint32_t else_if_address_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kLoadIntValue);
  cmds_code->WriteInt32(INT32_C(1));
  cmds_code->WriteCmdId(CmdId::kUnload);
  cmds_code->WriteCmdId(CmdId::kDirectJump);
  uint32_t branch_end_address_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));

  uint32_t else_if_address = cmds_code->GetPosition();
  cmds_code->SetPosition(else_if_address_placeholder);
  cmds_code->WriteUint32(else_if_address);
  cmds_code->SetPosition(else_if_address);
  cmds_code->WriteCmdId(CmdId::kLoadBoolValue);
  cmds_code->WriteBool(false);
  cmds_code->WriteCmdId(CmdId::kJumpIfNot);
  uint32_t else_address_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kLoadIntValue);
  cmds_code->WriteInt32(INT32_C(2));
  cmds_code->WriteCmdId(CmdId::kUnload);
  cmds_code->WriteCmdId(CmdId::kDirectJump);
  uint32_t branch_end_address_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));

  uint32_t else_address = cmds_code->GetPosition();
  cmds_code->SetPosition(else_address_placeholder);
  cmds_code->WriteUint32(else_address);
  cmds_code->SetPosition(else_address);
  cmds_code->WriteCmdId(CmdId::kLoadIntValue);
  cmds_code->WriteInt32(INT32_C(3));
  cmds_code->WriteCmdId(CmdId::kUnload);

  uint32_t branch_end_address = cmds_code->GetPosition();
  cmds_code->SetPosition(branch_end_address_placeholder);
  cmds_code->WriteUint32(branch_end_address);
  cmds_code->SetPosition(branch_end_address_placeholder2);
  cmds_code->WriteUint32(branch_end_address);
  cmds_code->SetPosition(branch_end_address);
  cmds_code->WriteCmdId(CmdId::kEndMain);
  cmds_code->WriteCmdId(CmdId::kEndFuncs);

  vector<path> import_file_paths;
  vector<string> ids_of_global_var_defs;
  vector<IdAddress> id_addresses_of_func_defs;
  vector<string> ids_of_native_func_defs;
  vector<IdAddress> id_addresses_of_global_var_refs;
  vector<IdAddress> id_addresses_of_func_refs;
  uint32_t version = UINT32_C(1);
  Module module(version,
                move(cmds_code),
                id_addresses_of_func_defs,
                ids_of_global_var_defs,
                ids_of_native_func_defs,
                id_addresses_of_func_refs,
                id_addresses_of_global_var_refs,
                import_file_paths);
  Code module_code;
  WriteModule(module, module_code);
  TestGenerate(program_node, semantic_analysis, version, module_code);
}

TEST_F(CodeGeneratorTest, DestroyLocalVarsAndJumpCmd) {
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
  unique_ptr<ScopeNode> if_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(8), UINT32_C(8))));
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
  unique_ptr<DataTypeNode> data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(15), UINT32_C(15))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(16), UINT32_C(16)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(17)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  else_if_body_stmt_nodes.push_back(move(var_def_node2));
  unique_ptr<ScopeNode> else_if_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(14), UINT32_C(14)),
      move(else_if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(18), UINT32_C(18))));
  BoolNode *bool_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(12), UINT32_C(12)));
  unique_ptr<ExprNode> bool_node2(bool_node_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(11), UINT32_C(11)),
      move(bool_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(13), UINT32_C(13)),
      move(else_if_body_node)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(9), UINT32_C(9)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  vector< unique_ptr<StmtNode> > else_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "3", UINT32_C(21), UINT32_C(21)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(22), UINT32_C(22))));
  else_body_stmt_nodes.push_back(move(int_stmt_node));
  unique_ptr<ScopeNode> else_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(20), UINT32_C(20)),
      move(else_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(23), UINT32_C(23))));

  unique_ptr<StmtNode> if_else_if_else_node(new IfElseIfElseNode(
      move(if_node),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(19), UINT32_C(19)),
      move(else_body_node)));
  program_stmt_nodes.push_back(move(if_else_if_else_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new VarDefAnalysis(
      unique_ptr<DataType>(new IntDataType()), DataStorage::kLocal));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new VarDefAnalysis(
      unique_ptr<DataType>(new LongDataType()), DataStorage::kLocal));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<NodeSemanticAnalysis> int_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(3)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_analysis)));
  unique_ptr<NodeSemanticAnalysis> bool_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_analysis)));
  unique_ptr<NodeSemanticAnalysis> bool_analysis2(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr2, move(bool_analysis2)));

  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::Problems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kLoadBoolValue);
  cmds_code->WriteBool(true);
  cmds_code->WriteCmdId(CmdId::kJumpIfNot);
  uint32_t else_if_address_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kCreateLocalIntVar);
  cmds_code->WriteCmdId(CmdId::kDestroyLocalVarsAndJumpCmd);
  cmds_code->WriteUint32(UINT32_C(1));
  uint32_t branch_end_address_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));

  uint32_t else_if_address = cmds_code->GetPosition();
  cmds_code->SetPosition(else_if_address_placeholder);
  cmds_code->WriteUint32(else_if_address);
  cmds_code->SetPosition(else_if_address);
  cmds_code->WriteCmdId(CmdId::kLoadBoolValue);
  cmds_code->WriteBool(false);
  cmds_code->WriteCmdId(CmdId::kJumpIfNot);
  uint32_t else_address_placeholder = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kCreateLocalLongVar);
  cmds_code->WriteCmdId(CmdId::kDestroyLocalVarsAndJumpCmd);
  cmds_code->WriteUint32(UINT32_C(1));
  uint32_t branch_end_address_placeholder2 = cmds_code->GetPosition();
  cmds_code->Skip(sizeof(uint32_t));

  uint32_t else_address = cmds_code->GetPosition();
  cmds_code->SetPosition(else_address_placeholder);
  cmds_code->WriteUint32(else_address);
  cmds_code->SetPosition(else_address);
  cmds_code->WriteCmdId(CmdId::kLoadIntValue);
  cmds_code->WriteInt32(INT32_C(3));
  cmds_code->WriteCmdId(CmdId::kUnload);

  uint32_t branch_end_address = cmds_code->GetPosition();
  cmds_code->SetPosition(branch_end_address_placeholder);
  cmds_code->WriteUint32(branch_end_address);
  cmds_code->SetPosition(branch_end_address_placeholder2);
  cmds_code->WriteUint32(branch_end_address);
  cmds_code->SetPosition(branch_end_address);
  cmds_code->WriteCmdId(CmdId::kEndMain);
  cmds_code->WriteCmdId(CmdId::kEndFuncs);

  vector<path> import_file_paths;
  vector<string> ids_of_global_var_defs;
  vector<IdAddress> id_addresses_of_func_defs;
  vector<string> ids_of_native_func_defs;
  vector<IdAddress> id_addresses_of_global_var_refs;
  vector<IdAddress> id_addresses_of_func_refs;
  uint32_t version = UINT32_C(1);
  Module module(version,
                move(cmds_code),
                id_addresses_of_func_defs,
                ids_of_global_var_defs,
                ids_of_native_func_defs,
                id_addresses_of_func_refs,
                id_addresses_of_global_var_refs,
                import_file_paths);
  Code module_code;
  WriteModule(module, module_code);
  TestGenerate(program_node, semantic_analysis, version, module_code);
}
}
}
