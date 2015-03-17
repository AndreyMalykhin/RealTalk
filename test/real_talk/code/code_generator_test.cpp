
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <limits>
#include "real_talk/parser/var_def_without_init_node.h"
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
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/var_def_analysis.h"
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
#include "real_talk/code/cmd.h"
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/module.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/code_generator.h"
#include "real_talk/code/code.h"
#include "real_talk/code/test_utils.h"

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
using real_talk::parser::ExprStmtNode;
using real_talk::parser::LitNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::VarDefAnalysis;
using real_talk::semantic::LitAnalysis;
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
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
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
}
}
