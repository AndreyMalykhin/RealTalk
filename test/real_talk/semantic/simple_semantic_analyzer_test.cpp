
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/simple_data_type_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/var_load_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/semantic_analysis_result.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::move;
using testing::Test;
using testing::Return;
using testing::_;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Token;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::parser::ExprNode;
using real_talk::parser::SimpleDataTypeNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::VarLoadNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::StringNode;
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
};

TEST_F(SimpleSemanticAnalyzerTest,
       AssignWithIncompatibleOperandDataTypesIsInvalid) {
  vector<TestProgram> test_programs;

  {
    vector< unique_ptr<StmtNode> > stmts;
    unique_ptr<DataTypeNode> var_data_type(new SimpleDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<StmtNode> var_def(new VarDefWithoutInitNode(
        move(var_data_type),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
    stmts.push_back(move(var_def));

    unique_ptr<ExprNode> var(new VarLoadNode(
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3))));
    unique_ptr<ExprNode> var_value(new StringNode(
        TokenInfo(Token::kStringLit, "\"a\"", UINT32_C(4), UINT32_C(4))));
    unique_ptr<ExprNode> assign_expr(new AssignNode(
        TokenInfo(Token::kAssignOp, "=", UINT32_C(5), UINT32_C(5)),
        move(var),
        move(var_value)));
    unique_ptr<StmtNode> assign_stmt(new ExprStmtNode(
        move(assign_expr),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
    stmts.push_back(move(assign_stmt));
    unique_ptr<ProgramNode> program(new ProgramNode(move(stmts)));

    vector< unique_ptr<SemanticError> > errors;
    vector< unique_ptr<SemanticHint> > hints;
    string error_message = "Assigning 'string' to 'int'";
    uint32_t error_line_number = UINT32_C(5);
    uint32_t error_column_number = UINT32_C(5);
    unique_ptr<SemanticError> error(new SemanticError(
        error_message, error_line_number, error_column_number, move(hints)));
    errors.push_back(move(error));

    SemanticAnalysisResult result(move(errors));
    TestProgram test_program = {move(program), move(result)};
    test_programs.push_back(move(test_program));
  }

  for (const TestProgram &test_program: test_programs) {
    SimpleSemanticAnalyzer analyzer(*(test_program.program));

    SemanticAnalysisResult actual_analysis_result = analyzer.Analyze();

    ASSERT_EQ(test_program.result, actual_analysis_result);
  }
}
}
}
