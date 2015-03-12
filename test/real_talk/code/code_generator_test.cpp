
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
#include "real_talk/parser/program_node.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/int_data_type.h"
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
using real_talk::parser::ProgramNode;
using real_talk::semantic::SemanticAnalysis;
using real_talk::semantic::NodeSemanticAnalysis;
using real_talk::semantic::VarDefAnalysis;
using real_talk::semantic::DataType;
using real_talk::semantic::IntDataType;
using real_talk::semantic::DataStorage;

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
};

TEST_F(CodeGeneratorTest, VarDefWithoutInit) {
  vector< unique_ptr<StmtNode> > program_stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  program_stmt_nodes.push_back(move(var_def_node));
  ProgramNode program_node(move(program_stmt_nodes));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  SemanticAnalysis semantic_analysis(
      SemanticAnalysis::Problems(), move(node_analyzes));

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalInt);
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
}
}
