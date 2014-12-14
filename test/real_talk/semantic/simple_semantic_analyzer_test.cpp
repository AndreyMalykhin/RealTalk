
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <iterator>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/var_load_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/long_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/double_data_type_node.h"
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/void_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/bounded_array_data_type_node.h"
#include "real_talk/parser/def_node.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/bounded_array_data_type.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/double_lit.h"
#include "real_talk/semantic/semantic_analysis_result.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::move;
using std::make_pair;
using std::make_move_iterator;
using std::pair;
using testing::Test;
using testing::Return;
using testing::_;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Token;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::parser::ExprNode;
using real_talk::parser::DefNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::VarLoadNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::BoundedArrayDataTypeNode;
using real_talk::parser::StringNode;
using real_talk::parser::IntNode;
using real_talk::parser::LongNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::AssignNode;
using real_talk::parser::ExprStmtNode;

namespace real_talk {
namespace semantic {

struct TestProgram {
  unique_ptr<ProgramNode> program;
  SemanticAnalysisResult result;
};

class SimpleSemanticAnalyzerTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

  void TestAnalyze(const TestProgram &test_program) {
    SimpleSemanticAnalyzer analyzer(*(test_program.program));
    SemanticAnalysisResult actual_analysis_result = analyzer.Analyze();
    ASSERT_EQ(test_program.result, actual_analysis_result);
  }
};

TEST_F(SimpleSemanticAnalyzerTest, VarDefWithoutInit) {
  struct TestData {
    unique_ptr<DataTypeNode> data_type_node;
    unique_ptr<DataType> data_type;
    SemanticAnalysisResult::ExprAnalyzes expr_analyzes;
    SemanticAnalysisResult::LitAnalyzes lit_analyzes;
  };

  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new IntDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
        TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new LongDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
        TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new DoubleDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
        TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new BoolDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
        TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new CharDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
        TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new StringDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new VoidDataType());
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> element_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataTypeNode> data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataType> data_type(new ArrayDataType(
        unique_ptr<DataType>(new IntDataType())));
    test_data_suits.push_back({move(data_type_node), move(data_type), {}, {}});
  }

  {
    unique_ptr<DataTypeNode> element_data_type_node(new LongDataTypeNode(
        TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
    IntNode *size_node_ptr =
        new IntNode(TokenInfo(Token::kIntLit, "7", UINT32_C(2), UINT32_C(2)));
    unique_ptr<IntNode> size_node(size_node_ptr);
    unique_ptr<DataTypeNode> array_data_type_node(
        new BoundedArrayDataTypeNode(
            move(element_data_type_node),
            TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
            move(size_node),
            TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3))));
    unique_ptr<DataType> element_data_type(new LongDataType());
    unique_ptr<IntLit> size(new IntLit(INT32_C(7)));
    unique_ptr<DataType> array_data_type(new BoundedArrayDataType(
        move(element_data_type), size->Clone()));
    SemanticAnalysisResult::ExprAnalyzes expr_analyzes;
    unique_ptr<DataType> size_data_type(new IntDataType());
    ExprAnalysis expr_analysis(move(size_data_type));
    expr_analyzes.insert(make_pair(size_node_ptr, move(expr_analysis)));
    SemanticAnalysisResult::LitAnalyzes lit_analyzes;
    LitAnalysis lit_analysis(size->Clone());
    lit_analyzes.insert(make_pair(size_node_ptr, move(lit_analysis)));
    TestData test_data = {move(array_data_type_node),
                          move(array_data_type),
                          move(expr_analyzes),
                          move(lit_analyzes)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    VarDefWithoutInitNode *def_node_ptr = new VarDefWithoutInitNode(
        move(test_data.data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(4), UINT32_C(4)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
    unique_ptr<StmtNode> def_node(def_node_ptr);
    stmt_nodes.push_back(move(def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    vector< unique_ptr<SemanticError> > errors;
    SemanticAnalysisResult::DefAnalyzes def_analyzes;
    DefAnalysis def_analysis(move(test_data.data_type));
    def_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));
    SemanticAnalysisResult::IdAnalyzes id_analyzes;
    SemanticAnalysisResult result(move(errors),
                                  move(def_analyzes),
                                  move(test_data.expr_analyzes),
                                  move(test_data.lit_analyzes),
                                  id_analyzes);

    TestAnalyze({move(program_node), move(result)});
  }
}

TEST_F(SimpleSemanticAnalyzerTest, VarDefWithInit) {
  struct TestData {
    unique_ptr<DataTypeNode> var_data_type_node;
    unique_ptr<DataType> var_data_type;
    ExprNode *value_node;
    SemanticAnalysisResult::ExprAnalyzes expr_analyzes;
    SemanticAnalysisResult::LitAnalyzes lit_analyzes;
  };

  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> var_data_type(new IntDataType());
    IntNode *value_node = new IntNode(
        TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3)));
    SemanticAnalysisResult::ExprAnalyzes expr_analyzes;
    unique_ptr<DataType> value_data_type(new IntDataType());
    ExprAnalysis expr_analysis(move(value_data_type));
    expr_analyzes.insert(make_pair(value_node, move(expr_analysis)));
    SemanticAnalysisResult::LitAnalyzes lit_analyzes;
    LitAnalysis lit_analysis(unique_ptr<Lit>(new IntLit(INT32_C(7))));
    lit_analyzes.insert(make_pair(value_node, move(lit_analysis)));
    TestData test_data = {move(var_data_type_node),
                          move(var_data_type),
                          value_node,
                          move(expr_analyzes),
                          move(lit_analyzes)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<DataTypeNode> var_data_type_node(new LongDataTypeNode(
        TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> var_data_type(new LongDataType());
    LongNode *value_node = new LongNode(
        TokenInfo(Token::kLongLit, "7L", UINT32_C(3), UINT32_C(3)));
    SemanticAnalysisResult::ExprAnalyzes expr_analyzes;
    unique_ptr<DataType> value_data_type(new LongDataType());
    ExprAnalysis expr_analysis(move(value_data_type));
    expr_analyzes.insert(make_pair(value_node, move(expr_analysis)));
    SemanticAnalysisResult::LitAnalyzes lit_analyzes;
    LitAnalysis lit_analysis(unique_ptr<Lit>(new LongLit(INT64_C(7))));
    lit_analyzes.insert(make_pair(value_node, move(lit_analysis)));
    TestData test_data = {move(var_data_type_node),
                          move(var_data_type),
                          value_node,
                          move(expr_analyzes),
                          move(lit_analyzes)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<DataTypeNode> var_data_type_node(new DoubleDataTypeNode(
        TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> var_data_type(new DoubleDataType());
    DoubleNode *value_node = new DoubleNode(
        TokenInfo(Token::kDoubleLit, "7.777777777", UINT32_C(3), UINT32_C(3)));
    SemanticAnalysisResult::ExprAnalyzes expr_analyzes;
    unique_ptr<DataType> value_data_type(new DoubleDataType());
    ExprAnalysis expr_analysis(move(value_data_type));
    expr_analyzes.insert(make_pair(value_node, move(expr_analysis)));
    SemanticAnalysisResult::LitAnalyzes lit_analyzes;
    LitAnalysis lit_analysis(unique_ptr<Lit>(new DoubleLit(7.777777777)));
    lit_analyzes.insert(make_pair(value_node, move(lit_analysis)));
    TestData test_data = {move(var_data_type_node),
                          move(var_data_type),
                          value_node,
                          move(expr_analyzes),
                          move(lit_analyzes)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    VarDefWithInitNode *def_node_ptr = new VarDefWithInitNode(
        move(test_data.var_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
        unique_ptr<ExprNode>(test_data.value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> def_node(def_node_ptr);
    stmt_nodes.push_back(move(def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    vector< unique_ptr<SemanticError> > errors;
    SemanticAnalysisResult::DefAnalyzes def_analyzes;
    DefAnalysis def_analysis(move(test_data.var_data_type));
    def_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));
    SemanticAnalysisResult::IdAnalyzes id_analyzes;
    SemanticAnalysisResult result(move(errors),
                                  move(def_analyzes),
                                  move(test_data.expr_analyzes),
                                  move(test_data.lit_analyzes),
                                  id_analyzes);

    TestAnalyze({move(program_node), move(result)});
  }
}

// TEST_F(SimpleSemanticAnalyzerTest, AssignWithIncompatibleDataTypesIsInvalid) {
//   struct TestData {
//     unique_ptr<DataTypeNode> var_data_type;
//     unique_ptr<ExprNode> var_value;
//     string error_message;
//   };
//   vector<TestData> test_data_suits;

//   {
//     unique_ptr<DataTypeNode> var_data_type(new SimpleDataTypeNode(
//         TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
//     unique_ptr<ExprNode> var_value(new StringNode(
//         TokenInfo(Token::kStringLit, "\"a\"", UINT32_C(4), UINT32_C(4))));
//     string error_message = "Assigning 'string' to 'int'";
//     TestData test_data = {move(var_data_type), move(var_value), error_message};
//     test_data_suits.push_back(move(test_data));
//   }

//   {
//     unique_ptr<DataTypeNode> var_data_type(new SimpleDataTypeNode(
//         TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
//     unique_ptr<ExprNode> var_value(new LongNode(
//         TokenInfo(Token::kLongLit, "1L", UINT32_C(4), UINT32_C(4))));
//     string error_message = "Assigning 'long' to 'int'";
//     TestData test_data = {move(var_data_type), move(var_value), error_message};
//     test_data_suits.push_back(move(test_data));
//   }

//   vector<TestProgram> test_programs;

//   for (TestData &test_data: test_data_suits) {
//     vector< unique_ptr<StmtNode> > stmts;
//     unique_ptr<StmtNode> var_def(new VarDefWithoutInitNode(
//         move(test_data.var_data_type),
//         TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
//         TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
//     stmts.push_back(move(var_def));

//     unique_ptr<ExprNode> var(new VarLoadNode(
//         TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3))));
//     unique_ptr<ExprNode> assign_expr(new AssignNode(
//         TokenInfo(Token::kAssignOp, "=", UINT32_C(5), UINT32_C(5)),
//         move(var),
//         move(test_data.var_value)));
//     unique_ptr<StmtNode> assign_stmt(new ExprStmtNode(
//         move(assign_expr),
//         TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
//     stmts.push_back(move(assign_stmt));
//     unique_ptr<ProgramNode> program(new ProgramNode(move(stmts)));

//     vector< unique_ptr<SemanticError> > errors;
//     vector< unique_ptr<SemanticHint> > hints;
//     uint32_t error_line_number = UINT32_C(5);
//     uint32_t error_column_number = UINT32_C(5);
//     unique_ptr<SemanticError> error(new SemanticError(
//         test_data.error_message,
//         error_line_number,
//         error_column_number,
//         move(hints)));
//     errors.push_back(move(error));

//     SemanticAnalysisResult result(move(errors));
//     TestProgram test_program = {move(program), move(result)};
//     test_programs.push_back(move(test_program));
//   }

//   for (const TestProgram &test_program: test_programs) {
//     SimpleSemanticAnalyzer analyzer(*(test_program.program));

//     SemanticAnalysisResult actual_analysis_result = analyzer.Analyze();

//     ASSERT_EQ(test_program.result, actual_analysis_result);
//   }
// }
}
}
