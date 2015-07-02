
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <iterator>
#include <limits>
#include "real_talk/util/errors.h"
#include "real_talk/lexer/token_info.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/parser/parser.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/not_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/equal_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/long_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/double_data_type_node.h"
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/void_data_type_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/arg_def_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/scope_node.h"
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/parser/if_else_if_else_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/less_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/parser/not_equal_node.h"
#include "real_talk/parser/greater_node.h"
#include "real_talk/parser/less_or_equal_node.h"
#include "real_talk/parser/greater_or_equal_node.h"
#include "real_talk/parser/and_node.h"
#include "real_talk/parser/or_node.h"
#include "real_talk/parser/mul_node.h"
#include "real_talk/parser/div_node.h"
#include "real_talk/parser/sum_node.h"
#include "real_talk/parser/sub_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/pre_inc_node.h"
#include "real_talk/parser/pre_dec_node.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/bool_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/void_data_type.h"
#include "real_talk/semantic/array_data_type.h"
#include "real_talk/semantic/func_data_type.h"
#include "real_talk/semantic/int_lit.h"
#include "real_talk/semantic/long_lit.h"
#include "real_talk/semantic/double_lit.h"
#include "real_talk/semantic/bool_lit.h"
#include "real_talk/semantic/string_lit.h"
#include "real_talk/semantic/char_lit.h"
#include "real_talk/semantic/global_var_def_analysis.h"
#include "real_talk/semantic/local_var_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/common_expr_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/subscript_analysis.h"
#include "real_talk/semantic/scope_analysis.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"
#include "real_talk/semantic/return_analysis.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/semantic_problems.h"

using std::numeric_limits;
using std::vector;
using std::string;
using std::to_string;
using std::unordered_map;
using std::unique_ptr;
using std::move;
using std::make_pair;
using std::make_move_iterator;
using std::pair;
using std::ostream;
using testing::Test;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::InSequence;
using testing::Eq;
using testing::ByRef;
using testing::Throw;
using boost::filesystem::path;
using boost::format;
using real_talk::lexer::Lexer;
using real_talk::lexer::TokenInfo;
using real_talk::lexer::Token;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::parser::ExprNode;
using real_talk::parser::DefNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::VarDefNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::IdNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::ArrayDataTypeNode;
using real_talk::parser::StringNode;
using real_talk::parser::BoolNode;
using real_talk::parser::IntNode;
using real_talk::parser::LongNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::AssignNode;
using real_talk::parser::EqualNode;
using real_talk::parser::ExprStmtNode;
using real_talk::parser::ScopeNode;
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::FuncDefWithoutBodyNode;
using real_talk::parser::IfNode;
using real_talk::parser::ElseIfNode;
using real_talk::parser::IfElseIfNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::parser::ImportNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::Parser;
using real_talk::parser::PrimitiveDataTypeNode;
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::CallNode;
using real_talk::parser::LessNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::BreakNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::NotNode;
using real_talk::parser::CharNode;
using real_talk::parser::NotEqualNode;
using real_talk::parser::GreaterNode;
using real_talk::parser::LessOrEqualNode;
using real_talk::parser::GreaterOrEqualNode;
using real_talk::parser::AndNode;
using real_talk::parser::OrNode;
using real_talk::parser::MulNode;
using real_talk::parser::DivNode;
using real_talk::parser::SumNode;
using real_talk::parser::SubNode;
using real_talk::parser::NegativeNode;
using real_talk::parser::PreIncNode;
using real_talk::parser::PreDecNode;
using real_talk::util::FileNotFoundError;

namespace real_talk {
namespace semantic {

template<typename T> struct TestLitParse {
  string str;
  T value;
};

struct TestLitParses {
  vector< TestLitParse<int32_t> > ints;
  vector< TestLitParse<int64_t> > longs;
  vector< TestLitParse<double> > doubles;
  vector< TestLitParse<bool> > bools;
  vector< TestLitParse<string> > strings;
  vector< TestLitParse<char> > chars;
};

struct TestProgram {
  unique_ptr<ProgramNode> main_program_node;
  SemanticAnalyzer::ImportPrograms import_program_nodes;
  SemanticAnalysis analysis;
  TestLitParses test_lit_parses;
};

class LitParserMock: public LitParser {
 public:
  MOCK_CONST_METHOD1(ParseInt, int32_t(const string&));
  MOCK_CONST_METHOD1(ParseLong, int64_t(const string&));
  MOCK_CONST_METHOD1(ParseDouble, double(const string&));
  MOCK_CONST_METHOD1(ParseBool, bool(const string&));
  MOCK_CONST_METHOD1(ParseString, string(const string&));
  MOCK_CONST_METHOD1(ParseChar, char(const string&));
};

class SimpleSemanticAnalyzerTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  unique_ptr<LitParser> CreateLitParserMock(
      const TestLitParses &test_lit_parses) {
    LitParserMock *lit_parser_mock = new LitParserMock();
    unique_ptr<LitParser> lit_parser(lit_parser_mock);

    for (const TestLitParse<int32_t> &test_lit_parse: test_lit_parses.ints) {
      EXPECT_CALL(*lit_parser_mock, ParseInt(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<int64_t> &test_lit_parse: test_lit_parses.longs) {
      EXPECT_CALL(*lit_parser_mock, ParseLong(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<double> &test_lit_parse: test_lit_parses.doubles) {
      EXPECT_CALL(*lit_parser_mock, ParseDouble(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<string> &test_lit_parse: test_lit_parses.strings) {
      EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<char> &test_lit_parse: test_lit_parses.chars) {
      EXPECT_CALL(*lit_parser_mock, ParseChar(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<bool> &test_lit_parse: test_lit_parses.bools) {
      EXPECT_CALL(*lit_parser_mock, ParseBool(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    return lit_parser;
  }

  void TestAnalyze(const TestProgram &test_program) {
    unique_ptr<LitParser> lit_parser =
        CreateLitParserMock(test_program.test_lit_parses);
    SimpleSemanticAnalyzer analyzer(*lit_parser);
    unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
        *(test_program.main_program_node), test_program.import_program_nodes);
    ASSERT_EQ(test_program.analysis, *actual_analysis);
  }
};

TEST_F(SimpleSemanticAnalyzerTest, VarDefWithoutInit) {
  struct TestData {
    unique_ptr<DataTypeNode> data_type_node;
    unique_ptr<DataType> data_type;
    TestLitParses test_lit_parses;
  };

  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new IntDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
        TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new LongDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
        TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new DoubleDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
        TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new BoolDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
        TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new CharDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
        TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new StringDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
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
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}});
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

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<NodeSemanticAnalysis> def_analysis(new GlobalVarDefAnalysis(
        test_data.data_type->Clone()));
    node_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));

    SemanticAnalysis::ProgramProblems problems;
    problems.insert(
        make_pair(program_node.get(), SemanticAnalysis::Problems()));
    SemanticAnalysis analysis(move(problems), move(node_analyzes));
    TestProgram test_program = {move(program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_data.test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithoutInitUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
    SemanticAnalysis::NodeAnalyzes node_analyzes;
    SemanticAnalysis::ProgramProblems problems;
  };
  vector<TestData> test_data_suits;

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    vector< unique_ptr<StmtNode> > func_body_stmt_nodes;
    unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(7))));
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(var_data_type_node),
        TokenInfo(Token::kName, "name", UINT32_C(8), UINT32_C(8)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    func_body_stmt_nodes.push_back(move(var_def_node));
    ScopeNode *func_body_node_ptr = new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
        move(func_body_stmt_nodes),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10)));
    unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);

    vector< unique_ptr<ArgDefNode> > arg_def_nodes;
    unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
    ArgDefNode *arg_def_node_ptr = new ArgDefNode(
        move(arg_data_type_node),
        TokenInfo(Token::kName, "name", UINT32_C(4), UINT32_C(4)));
    unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
    arg_def_nodes.push_back(move(arg_def_node));

    unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
        vector<TokenInfo>(),
        move(func_data_type_node),
        TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
        move(arg_def_nodes),
        vector<TokenInfo>(),
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
        move(func_body_node));
    unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
    stmt_nodes.push_back(move(func_def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
    bool is_func_native = false;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types), is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type), is_func_has_return));
    node_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DataType> arg_data_type(new IntDataType());
    uint32_t var_index_within_func = UINT32_C(0);
    unique_ptr<NodeSemanticAnalysis> arg_def_analysis(new LocalVarDefAnalysis(
        move(arg_data_type), var_index_within_func));
    node_analyzes.insert(
        make_pair(arg_def_node_ptr, move(arg_def_analysis)));
    unique_ptr<DataType> var_data_type(new IntDataType());
    uint32_t var_index_within_func2 = UINT32_C(1);
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new LocalVarDefAnalysis(
        move(var_data_type), var_index_within_func2));
    node_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(
        new DuplicateDefError(*var_def_node_ptr));
    problems[program_node.get()].push_back(move(problem));

    TestData test_data =
        {move(program_node), move(node_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    ScopeNode *body_node_ptr(new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
        vector< unique_ptr<StmtNode> >(),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
    unique_ptr<ScopeNode> body_node(body_node_ptr);
    unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
        vector<TokenInfo>(),
        move(func_data_type_node),
        TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
        vector< unique_ptr<ArgDefNode> >(),
        vector<TokenInfo>(),
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
        move(body_node));
    unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
    stmt_nodes.push_back(move(func_def_node));

    unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(6), UINT32_C(6))));
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(var_data_type_node),
        TokenInfo(Token::kName, "name", UINT32_C(7), UINT32_C(7)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    stmt_nodes.push_back(move(var_def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    vector<const VarDefNode*> body_local_var_defs;
    unique_ptr<NodeSemanticAnalysis> body_analysis(
        new ScopeAnalysis(body_local_var_defs));
    node_analyzes.insert(
        make_pair(body_node_ptr, move(body_analysis)));
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    bool is_func_native = false;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types), is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type), is_func_has_return));
    node_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DataType> var_data_type(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        move(var_data_type)));
    node_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        *var_def_node_ptr));
    problems[program_node.get()].push_back(move(problem));

    TestData test_data =
        {move(program_node), move(node_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
        move(var_data_type_node1),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
    stmt_nodes.push_back(move(var_def_node1));

    unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
    VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
        move(var_data_type_node2),
        TokenInfo(Token::kName, "var", UINT32_C(4), UINT32_C(4)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
    unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
    stmt_nodes.push_back(move(var_def_node2));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> var_data_type1(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> var_def_analysis1(new GlobalVarDefAnalysis(
        move(var_data_type1)));
    node_analyzes.insert(
        make_pair(var_def_node_ptr1, move(var_def_analysis1)));
    unique_ptr<DataType> var_data_type2(new IntDataType());
    unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new GlobalVarDefAnalysis(
        move(var_data_type2)));
    node_analyzes.insert(
        make_pair(var_def_node_ptr2, move(var_def_analysis2)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        *var_def_node_ptr2));
    problems[program_node.get()].push_back(move(problem));

    TestData test_data =
        {move(program_node), move(node_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis analysis(move(test_data.problems),
                              move(test_data.node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(test_data.program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithoutInitUsingUnsupportedDataTypeIsInvalid) {
  struct TestData {
    unique_ptr<DataTypeNode> var_data_type_node;
    unique_ptr<DataType> var_data_type;
  };
  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> var_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> var_data_type(new VoidDataType());
    test_data_suits.push_back({move(var_data_type_node), move(var_data_type)});
  }

  {
    unique_ptr<DataTypeNode> element_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataType> element_data_type(new VoidDataType());
    unique_ptr<DataType> var_data_type(
        new ArrayDataType(move(element_data_type)));
    test_data_suits.push_back({move(var_data_type_node), move(var_data_type)});
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(test_data.var_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    stmt_nodes.push_back(move(var_def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        test_data.var_data_type->Clone()));
    node_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
        *var_def_node_ptr, test_data.var_data_type->Clone()));
    problems[program_node.get()].push_back(move(problem));

    SemanticAnalysis analysis(move(problems), move(node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, VarDefWithInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  CharNode *value_node = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3)));
  VarDefWithInitNode *def_node_ptr = new VarDefWithInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
      unique_ptr<ExprNode>(value_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> def_node(def_node_ptr);
  stmt_nodes.push_back(move(def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> def_analysis(new GlobalVarDefAnalysis(
      move(var_data_type)));
  node_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));

  unique_ptr<DataType> value_data_type(new CharDataType());
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(value_node, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingIncompatibleValueDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  StringNode *value_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"7\"", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
      move(data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
      move(value_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"7\"", "7"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> value_data_type(new StringDataType());
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      value_data_type->Clone(),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("7"))));
  node_analyzes.insert(make_pair(value_node_ptr, move(lit_analysis)));

  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new GlobalVarDefAnalysis(var_data_type->Clone()));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new VarDefWithIncompatibleValueTypeError(
      *var_def_node_ptr,
      var_data_type->Clone(),
      value_data_type->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingUnsupportedDataTypeIsInvalid) {
  struct TestData {
    unique_ptr<DataTypeNode> data_type_node;
    unique_ptr<DataType> data_type;
  };
  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new VoidDataType());
    TestData test_data = {move(data_type_node), move(data_type)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<DataTypeNode> element_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataType> element_data_type(new VoidDataType());
    unique_ptr<DataType> array_data_type(
        new ArrayDataType(move(element_data_type)));
    TestData test_data = {move(array_data_type_node), move(array_data_type)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    IntNode *var_value_node_ptr = new IntNode(
        TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5)));
    unique_ptr<ExprNode> value_node(var_value_node_ptr);
    VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
        move(test_data.data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(4), UINT32_C(4)),
        move(value_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    stmt_nodes.push_back(move(var_def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        test_data.data_type->Clone()));
    node_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
        *var_def_node_ptr, test_data.data_type->Clone()));
    problems[program_node.get()].push_back(move(problem));

    SemanticAnalysis analysis(move(problems), move(node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
    SemanticAnalysis::NodeAnalyzes node_analyzes;
    SemanticAnalysis::ProgramProblems problems;
    TestLitParses test_lit_parses;
  };
  vector<TestData> test_data_suits;

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    IntNode *var_value_node_ptr1 = new IntNode(
        TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)));
    unique_ptr<ExprNode> var_value_node1(var_value_node_ptr1);
    unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    VarDefWithInitNode *var_def_node_ptr1 = new VarDefWithInitNode(
        move(var_data_type_node1),
        TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
        move(var_value_node1),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
    stmt_nodes.push_back(move(var_def_node1));

    IntNode *var_value_node_ptr2 = new IntNode(
        TokenInfo(Token::kIntLit, "2", UINT32_C(8), UINT32_C(8)));
    unique_ptr<ExprNode> var_value_node2(var_value_node_ptr2);
    unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
    VarDefWithInitNode *var_def_node_ptr2 = new VarDefWithInitNode(
        move(var_data_type_node2),
        TokenInfo(Token::kName, "var", UINT32_C(6), UINT32_C(6)),
        TokenInfo(Token::kAssignOp, "=", UINT32_C(7), UINT32_C(7)),
        move(var_value_node2),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9)));
    unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
    stmt_nodes.push_back(move(var_def_node2));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    IntDataType *var_data_type_ptr1 = new IntDataType();
    unique_ptr<DataType> var_data_type1(var_data_type_ptr1);
    unique_ptr<NodeSemanticAnalysis> var_def_analysis1(new GlobalVarDefAnalysis(
        move(var_data_type1)));
    node_analyzes.insert(
        make_pair(var_def_node_ptr1, move(var_def_analysis1)));
    IntDataType *var_data_type_ptr2 = new IntDataType();
    unique_ptr<DataType> var_data_type2(var_data_type_ptr2);
    unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new GlobalVarDefAnalysis(
        move(var_data_type2)));
    node_analyzes.insert(
        make_pair(var_def_node_ptr2, move(var_def_analysis2)));

    IntDataType *var_value_data_type_ptr1 = new IntDataType();
    unique_ptr<DataType> var_value_data_type1(var_value_data_type_ptr1);
    unique_ptr<DataType> var_value_casted_data_type;
    unique_ptr<Lit> var_value_lit1(new IntLit(INT32_C(1)));
    unique_ptr<NodeSemanticAnalysis> var_value_lit_analysis1(new LitAnalysis(
        move(var_value_data_type1),
        move(var_value_casted_data_type),
        ValueType::kRight,
        move(var_value_lit1)));
    node_analyzes.insert(
        make_pair(var_value_node_ptr1, move(var_value_lit_analysis1)));
    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        *var_def_node_ptr2));
    problems[program_node.get()].push_back(move(problem));

    TestLitParses test_lit_parses = {};
    test_lit_parses.ints = {{"1", INT32_C(1)}};

    TestData test_data = {move(program_node),
                          move(node_analyzes),
                          move(problems),
                          test_lit_parses};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis analysis(
        move(test_data.problems), move(test_data.node_analyzes));
    TestProgram test_program = {move(test_data.program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_data.test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithBodyAndArgsAndReturnValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new CharDataTypeNode(
      TokenInfo(Token::kCharType, "char", UINT32_C(3), UINT32_C(3))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  vector<TokenInfo> arg_separator_tokens;

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  IdNode *return_expr_node_ptr =
      new IdNode(TokenInfo(Token::kName, "name", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> return_expr_node(return_expr_node_ptr);
  ReturnValueNode *return_stmt_node_ptr = new ReturnValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(7), UINT32_C(7)),
      move(return_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9)));
  unique_ptr<StmtNode> return_stmt_node(return_stmt_node_ptr);
  body_stmt_nodes.push_back(move(return_stmt_node));

  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> func_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> return_analysis(
      new ReturnAnalysis(func_def_node_ptr));
  node_analyzes.insert(make_pair(return_stmt_node_ptr, move(return_analysis)));

  vector<const VarDefNode*> body_local_var_defs = {arg_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new CharDataType()));
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_func_has_return = true;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> arg_data_type(new CharDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(
      new LocalVarDefAnalysis(move(arg_data_type), var_index_within_func));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  unique_ptr<DataType> return_expr_data_type(new CharDataType());
  unique_ptr<DataType> return_expr_casted_data_type(new IntDataType());
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(return_expr_data_type),
      move(return_expr_casted_data_type),
      ValueType::kLeft,
      arg_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(return_expr_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithBodyWithoutArgsAndReturnValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyUsingNativeModifierIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(6), UINT32_C(6))));

  vector<TokenInfo> modifiers =
      {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      modifiers,
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(4), UINT32_C(4)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = true;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new FuncDefWithBodyIsNativeError(
      *func_def_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
    SemanticAnalysis::NodeAnalyzes node_analyzes;
    SemanticAnalysis::ProgramProblems problems;
  };
  vector<TestData> test_data_suits;

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    ScopeNode *body_node_ptr1 = new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
        vector< unique_ptr<StmtNode> >(),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
    unique_ptr<ScopeNode> body_node1(body_node_ptr1);
    unique_ptr<DataTypeNode> func_data_type_node1(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    FuncDefWithBodyNode *func_def_node_ptr1 = new FuncDefWithBodyNode(
        vector<TokenInfo>(),
        move(func_data_type_node1),
        TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
        vector< unique_ptr<ArgDefNode> >(),
        vector<TokenInfo>(),
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
        move(body_node1));
    unique_ptr<StmtNode> func_def_node1(func_def_node_ptr1);
    stmt_nodes.push_back(move(func_def_node1));

    unique_ptr<ScopeNode> body_node2(new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
        vector< unique_ptr<StmtNode> >(),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));
    unique_ptr<DataTypeNode> func_data_type_node2(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(6), UINT32_C(6))));
    FuncDefWithBodyNode *func_def_node_ptr2 = new FuncDefWithBodyNode(
        vector<TokenInfo>(),
        move(func_data_type_node2),
        TokenInfo(Token::kName, "name", UINT32_C(7), UINT32_C(7)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
        vector< unique_ptr<ArgDefNode> >(),
        vector<TokenInfo>(),
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
        move(body_node2));
    unique_ptr<StmtNode> func_def_node2(func_def_node_ptr2);
    stmt_nodes.push_back(move(func_def_node2));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    vector<const VarDefNode*> body_local_var_defs1;
    unique_ptr<NodeSemanticAnalysis> body_analysis1(
        new ScopeAnalysis(body_local_var_defs1));
    node_analyzes.insert(make_pair(body_node_ptr1, move(body_analysis1)));
    unique_ptr<DataType> return_data_type1(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types1;
    bool is_func_native = false;
    unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
        move(return_data_type1), move(arg_data_types1), is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis1(new FuncDefAnalysis(
        move(func_data_type1), is_func_has_return));
    node_analyzes.insert(
        make_pair(func_def_node_ptr1, move(func_def_analysis1)));
    unique_ptr<DataType> return_data_type2(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types2;
    bool is_func_native2 = false;
    unique_ptr<FuncDataType> func_data_type2(new FuncDataType(
        move(return_data_type2), move(arg_data_types2), is_func_native2));
    bool is_func_has_return2 = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis2(new FuncDefAnalysis(
        move(func_data_type2), is_func_has_return2));
    node_analyzes.insert(
        make_pair(func_def_node_ptr2, move(func_def_analysis2)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        *func_def_node_ptr2));
    problems[program_node.get()].push_back(move(problem));

    TestData test_data =
        {move(program_node), move(node_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis analysis(
        move(test_data.problems), move(test_data.node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(test_data.program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyUsingUnsupportedReturnDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

  unique_ptr<DataTypeNode> array_element_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> func_data_type_node(new ArrayDataTypeNode(
      move(array_element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> array_element_data_type(new VoidDataType());
  ArrayDataType *return_data_type_ptr =
      new ArrayDataType(move(array_element_data_type));
  unique_ptr<DataType> return_data_type(return_data_type_ptr);

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
      *func_def_node_ptr, return_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyWithinNonGlobalScopeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes2;
  unique_ptr<ScopeNode> body_node2(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(9)),
      move(body_stmt_nodes2),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10))));

  vector< unique_ptr<StmtNode> > body_stmt_nodes1;
  unique_ptr<DataTypeNode> return_data_type_node2(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(5), UINT32_C(5))));
  FuncDefWithBodyNode *func_def_node_ptr2 = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node2),
      TokenInfo(Token::kName, "func2", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(7), UINT32_C(7)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(8), UINT32_C(8)),
      move(body_node2));
  unique_ptr<StmtNode> func_def_node2(func_def_node_ptr2);
  body_stmt_nodes1.push_back(move(func_def_node2));

  unique_ptr<ScopeNode> body_node1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));

  unique_ptr<DataTypeNode> return_data_type_node1(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr1 = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node1),
      TokenInfo(Token::kName, "func1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node1));
  unique_ptr<StmtNode> func_def_node1(func_def_node_ptr1);
  stmt_nodes.push_back(move(func_def_node1));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> return_data_type1(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types1;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
      move(return_data_type1), move(arg_data_types1), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis1(
      new FuncDefAnalysis(move(func_data_type1), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr1, move(func_def_analysis1)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new FuncDefWithinNonGlobalScopeError(
      *func_def_node_ptr2));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyUsingNonVoidDataTypeWithoutReturnValueIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > func_body_stmt_nodes;
  vector< unique_ptr<StmtNode> > if_body_stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  ReturnValueNode *return_node_ptr = new ReturnValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(10), UINT32_C(10)),
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
  unique_ptr<StmtNode> return_node(return_node_ptr);
  if_body_stmt_nodes.push_back(move(return_node));

  ScopeNode *if_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(9)),
      move(if_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(13), UINT32_C(13)));
  unique_ptr<ScopeNode> if_body_node(if_body_node_ptr);

  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> cond_node(bool_node_ptr);
  unique_ptr<IfNode> if_node(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(6), UINT32_C(6)),
      move(cond_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(8), UINT32_C(8)),
      move(if_body_node)));
  unique_ptr<StmtNode> if_else_if_node(new IfElseIfNode(
      move(if_node), vector< unique_ptr<ElseIfNode> >()));
  func_body_stmt_nodes.push_back(move(if_else_if_node));

  ScopeNode *func_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(func_body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(14), UINT32_C(14)));
  unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);

  unique_ptr<DataTypeNode> return_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(func_body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> return_analysis(
      new ReturnAnalysis(func_def_node_ptr));
  node_analyzes.insert(make_pair(return_node_ptr, move(return_analysis)));

  vector<const VarDefNode*> if_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> if_body_analysis(
      new ScopeAnalysis(if_body_local_var_defs));
  node_analyzes.insert(make_pair(if_body_node_ptr, move(if_body_analysis)));

  vector<const VarDefNode*> func_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> func_body_analysis(
      new ScopeAnalysis(func_body_local_var_defs));
  node_analyzes.insert(make_pair(func_body_node_ptr, move(func_body_analysis)));

  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      unique_ptr<DataType>(new IntDataType()),
      vector< unique_ptr<DataType> >(),
      is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> bool_lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> bool_lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(bool_lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(bool_lit_analysis)));

  unique_ptr<DataType> int_lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> int_lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(int_lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(int_lit_analysis)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};
  test_lit_parses.bools = {{"yeah", true}};

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new FuncDefWithoutReturnValueError(
      *func_def_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithoutBodyWithArgsAndReturnValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(4), UINT32_C(4))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  vector<TokenInfo> arg_separator_tokens;

  vector<TokenInfo> modifier_tokens =
      {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
  unique_ptr<DataTypeNode> func_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  FuncDefWithoutBodyNode *func_def_node_ptr = new FuncDefWithoutBodyNode(
      modifier_tokens,
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  bool is_func_native = true;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new IntDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(
      new LocalVarDefAnalysis(move(arg_data_type), var_index_within_func));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithoutBodyAndArgsAndReturnValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  vector<TokenInfo> arg_separator_tokens;
  vector<TokenInfo> modifier_tokens =
      {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  FuncDefWithoutBodyNode *func_def_node_ptr = new FuncDefWithoutBodyNode(
      modifier_tokens,
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  VoidDataType *func_return_data_type_ptr = new VoidDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  bool is_func_native = true;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithoutBodyNotUsingNativeModifierIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  FuncDefWithoutBodyNode *func_def_node_ptr = new FuncDefWithoutBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new FuncDefWithoutBodyNotNativeError(
      *func_def_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithoutBodyUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
    SemanticAnalysis::NodeAnalyzes node_analyzes;
    SemanticAnalysis::ProgramProblems problems;
  };
  vector<TestData> test_data_suits;

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    vector<TokenInfo> modifier_tokens1 =
        {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
    unique_ptr<DataTypeNode> func_data_type_node1(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
    FuncDefWithoutBodyNode *func_def_node_ptr1 = new FuncDefWithoutBodyNode(
        modifier_tokens1,
        move(func_data_type_node1),
        TokenInfo(Token::kName, "name", UINT32_C(2), UINT32_C(2)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
        vector< unique_ptr<ArgDefNode> >(),
        vector<TokenInfo>(),
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(4), UINT32_C(4)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
    unique_ptr<StmtNode> func_def_node1(func_def_node_ptr1);
    stmt_nodes.push_back(move(func_def_node1));

    vector<TokenInfo> modifier_tokens2 =
        {TokenInfo(Token::kNative, "native", UINT32_C(6), UINT32_C(6))};
    unique_ptr<DataTypeNode> func_data_type_node2(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(7), UINT32_C(7))));
    FuncDefWithoutBodyNode *func_def_node_ptr2 = new FuncDefWithoutBodyNode(
        modifier_tokens2,
        move(func_data_type_node2),
        TokenInfo(Token::kName, "name", UINT32_C(8), UINT32_C(8)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(9), UINT32_C(9)),
        vector< unique_ptr<ArgDefNode> >(),
        vector<TokenInfo>(),
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(10), UINT32_C(10)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(11), UINT32_C(11)));
    unique_ptr<StmtNode> func_def_node2(func_def_node_ptr2);
    stmt_nodes.push_back(move(func_def_node2));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<DataType> return_data_type1(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types1;
    bool is_func_native = true;
    unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
        move(return_data_type1), move(arg_data_types1), is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis1(new FuncDefAnalysis(
        move(func_data_type1), is_func_has_return));
    node_analyzes.insert(
        make_pair(func_def_node_ptr1, move(func_def_analysis1)));
    unique_ptr<DataType> return_data_type2(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types2;
    bool is_func_native2 = true;
    unique_ptr<FuncDataType> func_data_type2(new FuncDataType(
        move(return_data_type2), move(arg_data_types2), is_func_native2));
    bool is_func_has_return2 = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis2(new FuncDefAnalysis(
        move(func_data_type2), is_func_has_return2));
    node_analyzes.insert(
        make_pair(func_def_node_ptr2, move(func_def_analysis2)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        *func_def_node_ptr2));
    problems[program_node.get()].push_back(move(problem));

    TestData test_data =
        {move(program_node), move(node_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis analysis(
        move(test_data.problems), move(test_data.node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(test_data.program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithoutBodyUsingUnsupportedReturnDataTypeInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector<TokenInfo> modifier_tokens =
      {TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0))};
  unique_ptr<DataTypeNode> array_element_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  unique_ptr<DataTypeNode> func_data_type_node(new ArrayDataTypeNode(
      move(array_element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3))));
  FuncDefWithoutBodyNode *func_def_node_ptr = new FuncDefWithoutBodyNode(
      modifier_tokens,
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(5), UINT32_C(5)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> array_element_data_type(new VoidDataType());
  ArrayDataType *return_data_type_ptr =
      new ArrayDataType(move(array_element_data_type));
  unique_ptr<DataType> return_data_type(return_data_type_ptr);

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
      *func_def_node_ptr, return_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithoutBodyWithinNonGlobalScopeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector<TokenInfo> modifier_tokens =
      {TokenInfo(Token::kNative, "native", UINT32_C(5), UINT32_C(5))};
  unique_ptr<DataTypeNode> return_data_type_node2(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(6), UINT32_C(6))));
  FuncDefWithoutBodyNode *func_def_node_ptr2 = new FuncDefWithoutBodyNode(
      modifier_tokens,
      move(return_data_type_node2),
      TokenInfo(Token::kName, "func2", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> func_def_node2(func_def_node_ptr2);
  vector< unique_ptr<StmtNode> > body_stmt_nodes1;
  body_stmt_nodes1.push_back(move(func_def_node2));

  unique_ptr<ScopeNode> body_node1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));
  unique_ptr<DataTypeNode> return_data_type_node1(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr1 = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node1),
      TokenInfo(Token::kName, "func1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node1));
  unique_ptr<StmtNode> func_def_node1(func_def_node_ptr1);
  stmt_nodes.push_back(move(func_def_node1));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> return_data_type1(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types1;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
      move(return_data_type1), move(arg_data_types1), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis1(
      new FuncDefAnalysis(move(func_data_type1), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr1, move(func_def_analysis1)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new FuncDefWithinNonGlobalScopeError(
      *func_def_node_ptr2));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ArgDefWithUnsupportedDataTypeIsInvalid) {
  struct TestData {
    unique_ptr<DataTypeNode> arg_data_type_node;
    unique_ptr<DataType> arg_data_type;
  };
  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> arg_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(3), UINT32_C(3))));
    unique_ptr<DataType> arg_data_type(new VoidDataType());
    TestData test_data = {move(arg_data_type_node), move(arg_data_type)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<DataTypeNode> element_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(3), UINT32_C(3))));
    unique_ptr<DataTypeNode> arg_data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(5), UINT32_C(5))));
    unique_ptr<DataType> element_data_type(new VoidDataType());
    unique_ptr<DataType> arg_data_type(
        new ArrayDataType(move(element_data_type)));
    TestData test_data = {move(arg_data_type_node), move(arg_data_type)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    vector< unique_ptr<ArgDefNode> > arg_def_nodes;
    ArgDefNode *arg_def_node_ptr = new ArgDefNode(
        move(test_data.arg_data_type_node),
        TokenInfo(Token::kName, "arg", UINT32_C(6), UINT32_C(6)));
    unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
    arg_def_nodes.push_back(move(arg_def_node));
    vector<TokenInfo> arg_separator_tokens;

    vector< unique_ptr<StmtNode> > body_stmt_nodes;
    unique_ptr<ScopeNode> body_node(new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(8)),
        move(body_stmt_nodes),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(9), UINT32_C(9))));

    unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
        TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
    FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
        vector<TokenInfo>(),
        move(func_data_type_node),
        TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
        move(arg_def_nodes),
        arg_separator_tokens,
        TokenInfo(Token::kGroupEnd, ")", UINT32_C(7), UINT32_C(7)),
        move(body_node));
    unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
    stmt_nodes.push_back(move(func_def_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    VoidDataType *func_return_data_type_ptr = new VoidDataType();
    unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
    vector< unique_ptr<DataType> > func_arg_data_types;
    func_arg_data_types.push_back(test_data.arg_data_type->Clone());
    bool is_func_native = false;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type),
        move(func_arg_data_types),
        is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), is_func_has_return));
    node_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    uint32_t var_index_within_func = UINT32_C(0);
    unique_ptr<NodeSemanticAnalysis> arg_def_analysis(new LocalVarDefAnalysis(
        test_data.arg_data_type->Clone(), var_index_within_func));
    node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
        *arg_def_node_ptr,
        test_data.arg_data_type->Clone()));
    problems[program_node.get()].push_back(move(problem));

    SemanticAnalysis analysis(move(problems), move(node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       ReturnValueWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new StringDataTypeNode(
      TokenInfo(Token::kStringType, "string", UINT32_C(3), UINT32_C(3))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  vector<TokenInfo> arg_separator_tokens;

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  IdNode *return_expr_node_ptr =
      new IdNode(TokenInfo(Token::kName, "arg", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> return_expr_node(return_expr_node_ptr);
  ReturnValueNode *return_stmt_node_ptr = new ReturnValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(7), UINT32_C(7)),
      move(return_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9)));
  unique_ptr<StmtNode> return_stmt_node(return_stmt_node_ptr);
  body_stmt_nodes.push_back(move(return_stmt_node));

  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10))));

  unique_ptr<DataTypeNode> func_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new StringDataType()));
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> arg_data_type(new StringDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(
      new LocalVarDefAnalysis(move(arg_data_type), var_index_within_func));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  StringDataType *return_expr_data_type_ptr = new StringDataType();
  unique_ptr<DataType> return_expr_data_type(return_expr_data_type_ptr);
  bool is_id_assignee = false;
  unique_ptr<DataType> return_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(return_expr_data_type),
      move(return_expr_casted_data_type),
      ValueType::kLeft,
      arg_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(return_expr_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ReturnWithIncompatibleTypeError(
      *return_stmt_node_ptr,
      func_return_data_type_ptr->Clone(),
      return_expr_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ReturnValueNotWithinFuncIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *return_expr_node_ptr =
      new IntNode(TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> return_expr_node(return_expr_node_ptr);
  ReturnValueNode *return_stmt_node_ptr = new ReturnValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(0)),
      move(return_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> return_stmt_node(return_stmt_node_ptr);
  stmt_nodes.push_back(move(return_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ReturnNotWithinFuncError(
      *return_stmt_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ReturnWithoutValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ReturnWithoutValueNode *return_stmt_node_ptr = new ReturnWithoutValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)));
  unique_ptr<StmtNode> return_stmt_node(return_stmt_node_ptr);
  body_stmt_nodes.push_back(move(return_stmt_node));

  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> return_analysis(
      new ReturnAnalysis(func_def_node_ptr));
  node_analyzes.insert(make_pair(return_stmt_node_ptr, move(return_analysis)));

  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  VoidDataType *func_return_data_type_ptr = new VoidDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_func_has_return = true;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ReturnWithoutValueNotWithinFuncIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ReturnWithoutValueNode *return_stmt_node_ptr = new ReturnWithoutValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StmtNode> return_stmt_node(return_stmt_node_ptr);
  stmt_nodes.push_back(move(return_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ReturnNotWithinFuncError(
      *return_stmt_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ReturnWithoutValueWithinFuncWithNonVoidDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ReturnWithoutValueNode *return_stmt_node_ptr = new ReturnWithoutValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)));
  unique_ptr<StmtNode> return_stmt_node(return_stmt_node_ptr);
  body_stmt_nodes.push_back(move(return_stmt_node));

  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

  unique_ptr<DataTypeNode> func_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ReturnWithoutValueError(
      *return_stmt_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Call) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  vector<TokenInfo> arg_separator_tokens;

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  vector< unique_ptr<ExprNode> > call_arg_nodes;
  CharNode *call_arg_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(10), UINT32_C(10)));
  unique_ptr<ExprNode> call_arg_node(call_arg_node_ptr);
  call_arg_nodes.push_back(move(call_arg_node));
  CallNode *call_expr_node_ptr = new CallNode(
      move(id_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(9), UINT32_C(9)),
      move(call_arg_nodes),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> call_expr_node(call_expr_node_ptr);
  unique_ptr<StmtNode> call_stmt_node(new ExprStmtNode(
      move(call_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12))));
  stmt_nodes.push_back(move(call_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs = {arg_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> arg_data_type(new IntDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(
      new LocalVarDefAnalysis(move(arg_data_type), var_index_within_func));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<DataType> func_arg_data_type(new IntDataType());
  func_arg_data_types.push_back(move(func_arg_data_type));
  unique_ptr<DataType> id_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  bool is_id_assignee = false;
  unique_ptr<DataType> id_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  unique_ptr<DataType> call_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> call_expr_analysis(new CommonExprAnalysis(
      move(call_expr_data_type),
      move(call_expr_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));

  unique_ptr<DataType> call_arg_data_type(new CharDataType());
  unique_ptr<DataType> call_arg_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> call_arg_lit_analysis(new LitAnalysis(
      move(call_arg_data_type),
      move(call_arg_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, CallWithIncompatibleArgDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  vector<TokenInfo> arg_separator_tokens;

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  vector< unique_ptr<ExprNode> > call_arg_nodes;
  LongNode *call_arg_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(10), UINT32_C(10)));
  unique_ptr<ExprNode> call_arg_node(call_arg_node_ptr);
  call_arg_nodes.push_back(move(call_arg_node));
  vector<TokenInfo> call_arg_separator_tokens;
  CallNode *call_expr_node_ptr = new CallNode(
      move(id_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(9), UINT32_C(9)),
      move(call_arg_nodes),
      call_arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> call_expr_node(call_expr_node_ptr);
  unique_ptr<StmtNode> call_stmt_node(new ExprStmtNode(
      move(call_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12))));
  stmt_nodes.push_back(move(call_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.longs = {{"7L", INT64_C(7)}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs = {arg_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  IntDataType *arg_def_data_type_ptr = new IntDataType();
  unique_ptr<DataType> arg_def_data_type(arg_def_data_type_ptr);
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(
      new LocalVarDefAnalysis(move(arg_def_data_type), var_index_within_func));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<DataType> func_arg_data_type(new IntDataType());
  func_arg_data_types.push_back(move(func_arg_data_type));
  unique_ptr<DataType> id_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  unique_ptr<DataType> call_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> call_expr_analysis(new CommonExprAnalysis(
      move(call_expr_data_type),
      move(call_expr_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));

  LongDataType *call_arg_data_type_ptr = new LongDataType();
  unique_ptr<DataType> call_arg_data_type(call_arg_data_type_ptr);
  unique_ptr<DataType> call_arg_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> call_arg_lit_analysis(new LitAnalysis(
      move(call_arg_data_type),
      move(call_arg_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(7)))));
  node_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  size_t bad_arg_index = 0;
  unique_ptr<SemanticProblem> problem(new CallWithIncompatibleArgTypeError(
      *call_expr_node_ptr,
      bad_arg_index,
      arg_def_data_type_ptr->Clone(),
      call_arg_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, CallWithNotMatchingArgsCountIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  ArgDefNode *arg_def_node_ptr = new ArgDefNode(
      move(arg_data_type_node),
      TokenInfo(Token::kName, "arg", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ArgDefNode> arg_def_node(arg_def_node_ptr);
  arg_def_nodes.push_back(move(arg_def_node));
  vector<TokenInfo> arg_separator_tokens;

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(func_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  vector< unique_ptr<ExprNode> > call_arg_nodes;
  IntNode *call_arg_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(10), UINT32_C(10)));
  unique_ptr<ExprNode> call_arg_node1(call_arg_node_ptr1);
  call_arg_nodes.push_back(move(call_arg_node1));
  IntNode *call_arg_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(12), UINT32_C(12)));
  unique_ptr<ExprNode> call_arg_node2(call_arg_node_ptr2);
  call_arg_nodes.push_back(move(call_arg_node2));
  vector<TokenInfo> call_arg_separator_tokens =
      {TokenInfo(Token::kSeparator, ",", UINT32_C(11), UINT32_C(11))};
  CallNode *call_expr_node_ptr = new CallNode(
      move(id_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(9), UINT32_C(9)),
      move(call_arg_nodes),
      call_arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(13), UINT32_C(13)));
  unique_ptr<ExprNode> call_expr_node(call_expr_node_ptr);
  unique_ptr<StmtNode> call_stmt_node(new ExprStmtNode(
      move(call_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(14), UINT32_C(14))));
  stmt_nodes.push_back(move(call_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs = {arg_def_node_ptr};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  bool is_func_native = false;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types), is_func_native));
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), is_func_has_return));
  node_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> arg_data_type(new IntDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> arg_def_analysis(
      new LocalVarDefAnalysis(move(arg_data_type), var_index_within_func));
  node_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<DataType> func_arg_data_type(new IntDataType());
  func_arg_data_types.push_back(move(func_arg_data_type));
  unique_ptr<DataType> id_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native));
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  unique_ptr<DataType> call_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> call_expr_analysis(new CommonExprAnalysis(
      move(call_expr_data_type),
      move(call_expr_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  size_t expected_args_count = 1;
  size_t actual_args_count = 2;
  unique_ptr<SemanticProblem> problem(new CallWithInvalidArgsCountError(
      *call_expr_node_ptr,
      expected_args_count,
      actual_args_count));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       CallUsingOperandWithNonFuncDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  CallNode *call_expr_node_ptr = new CallNode(
      move(id_node),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<ExprNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> call_expr_node(call_expr_node_ptr);
  unique_ptr<StmtNode> call_stmt_node(new ExprStmtNode(
      move(call_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  stmt_nodes.push_back(move(call_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new GlobalVarDefAnalysis(move(var_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  IntDataType id_data_type;
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis(new IdAnalysis(
      id_data_type.Clone(),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new CallWithUnsupportedTypeError(
      *call_expr_node_ptr, id_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, IfElseIf) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node1(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  stmt_nodes.push_back(move(var_def_node1));

  vector< unique_ptr<StmtNode> > body_stmt_nodes1;
  unique_ptr<DataTypeNode> var_data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "name", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  body_stmt_nodes1.push_back(move(var_def_node2));
  ScopeNode *if_body_ptr1 = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> if_body1(if_body_ptr1);

  IdNode *if_cond_ptr1 = new IdNode(
      TokenInfo(Token::kName, "name", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> if_cond1(if_cond_ptr1);
  unique_ptr<IfNode> if_node1(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      move(if_cond1),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      move(if_body1)));

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes2;
  unique_ptr<DataTypeNode> var_data_type_node3(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(18), UINT32_C(18))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "name", UINT32_C(19), UINT32_C(19)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(20), UINT32_C(20)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  body_stmt_nodes2.push_back(move(var_def_node3));
  ScopeNode *if_body_ptr2 = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(17), UINT32_C(17)),
      move(body_stmt_nodes2),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21)));
  unique_ptr<ScopeNode> if_body2(if_body_ptr2);

  IdNode *if_cond_ptr2 = new IdNode(
      TokenInfo(Token::kName, "name", UINT32_C(15), UINT32_C(15)));
  unique_ptr<ExprNode> if_cond2(if_cond_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(13), UINT32_C(13)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(14), UINT32_C(14)),
      move(if_cond2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(16), UINT32_C(16)),
      move(if_body2)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(12), UINT32_C(12)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  unique_ptr<StmtNode> if_else_if_node(
      new IfElseIfNode(move(if_node1), move(else_if_nodes)));
  stmt_nodes.push_back(move(if_else_if_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs1 = {var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis1(
      new ScopeAnalysis(if_body_local_var_defs1));
  node_analyzes.insert(make_pair(if_body_ptr1, move(if_body_analysis1)));

  vector<const VarDefNode*> if_body_local_var_defs2 = {var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis2(
      new ScopeAnalysis(if_body_local_var_defs2));
  node_analyzes.insert(make_pair(if_body_ptr2, move(if_body_analysis2)));

  unique_ptr<DataType> var_data_type1(new BoolDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(
      new GlobalVarDefAnalysis(move(var_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<DataType> var_data_type2(new BoolDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(
      new LocalVarDefAnalysis(move(var_data_type2), var_index_within_func));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  unique_ptr<DataType> var_data_type3(new BoolDataType());
  uint32_t var_index_within_func2 = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(
      new LocalVarDefAnalysis(move(var_data_type3), var_index_within_func2));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  unique_ptr<DataType> if_cond_data_type1(new BoolDataType());
  unique_ptr<DataType> if_cond_casted_data_type1;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> if_cond_expr_analysis1(new IdAnalysis(
      move(if_cond_data_type1),
      move(if_cond_casted_data_type1),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee));
  node_analyzes.insert(make_pair(if_cond_ptr1, move(if_cond_expr_analysis1)));

  unique_ptr<DataType> if_cond_data_type2(new BoolDataType());
  unique_ptr<DataType> if_cond_casted_data_type2;
  bool is_id_assignee2 = false;
  unique_ptr<NodeSemanticAnalysis> if_cond_expr_analysis2(new IdAnalysis(
      move(if_cond_data_type2),
      move(if_cond_casted_data_type2),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee2));
  node_analyzes.insert(make_pair(if_cond_ptr2, move(if_cond_expr_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfUsingIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *func_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(func_body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  unique_ptr<ScopeNode> if_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  IfNode *if_node_ptr = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(7), UINT32_C(7)),
      move(id_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      move(if_body_node));
  unique_ptr<IfNode> if_node(if_node_ptr);

  IfElseIfNode *if_else_if_node_ptr =
      new IfElseIfNode(move(if_node), vector< unique_ptr<ElseIfNode> >());
  unique_ptr<StmtNode> if_else_if_node(if_else_if_node_ptr);
  stmt_nodes.push_back(move(if_else_if_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> func_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> func_body_analysis(
      new ScopeAnalysis(func_body_local_var_defs));
  node_analyzes.insert(make_pair(func_body_node_ptr, move(func_body_analysis)));
  bool is_func_native = false;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type),
        move(func_arg_data_types),
        is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native);
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  unique_ptr<FuncDataType> func_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis(new IdAnalysis(
      move(func_data_type),
      move(func_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      *if_else_if_node_ptr,
      *if_node_ptr,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfUsingElseIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *func_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(func_body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(6), UINT32_C(6))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  ScopeNode *if_body_node_ptr1 = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(14), UINT32_C(14)));
  unique_ptr<ScopeNode> if_body_node1(if_body_node_ptr1);
  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IfNode *if_node_ptr1 = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(10), UINT32_C(10)),
      move(id_node1),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(12), UINT32_C(12)),
      move(if_body_node1));
  unique_ptr<IfNode> if_node1(if_node_ptr1);

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  unique_ptr<ScopeNode> if_body_node2(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(20), UINT32_C(20)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21))));
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(18), UINT32_C(18)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  IfNode *if_node_ptr2 = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(16), UINT32_C(16)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(17), UINT32_C(17)),
      move(id_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(19), UINT32_C(19)),
      move(if_body_node2));
  unique_ptr<IfNode> if_node2(if_node_ptr2);
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(15), UINT32_C(15)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  IfElseIfNode *if_else_if_node_ptr =
      new IfElseIfNode(move(if_node1), move(else_if_nodes));
  unique_ptr<StmtNode> if_else_if_node(if_else_if_node_ptr);
  stmt_nodes.push_back(move(if_else_if_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> func_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> func_body_analysis(
      new ScopeAnalysis(func_body_local_var_defs));
  node_analyzes.insert(make_pair(func_body_node_ptr, move(func_body_analysis)));

  vector<const VarDefNode*> if_body_local_var_defs1;
  unique_ptr<NodeSemanticAnalysis> if_body_analysis1(
      new ScopeAnalysis(if_body_local_var_defs1));
  node_analyzes.insert(make_pair(if_body_node_ptr1, move(if_body_analysis1)));
  bool is_func_native = false;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type),
        move(func_arg_data_types),
        is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  {
    unique_ptr<DataType> var_data_type(new BoolDataType());
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new GlobalVarDefAnalysis(move(var_data_type)));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  }

  unique_ptr<DataType> var_data_type(new BoolDataType());
  unique_ptr<DataType> var_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis1(new IdAnalysis(
      move(var_data_type),
      move(var_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native);
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  unique_ptr<FuncDataType> func_casted_data_type;
  bool is_id_assignee2 = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis2(new IdAnalysis(
      move(func_data_type),
      move(func_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      *if_else_if_node_ptr,
      *if_node_ptr2,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, IfElseIfElse) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node1(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  stmt_nodes.push_back(move(var_def_node1));

  vector< unique_ptr<StmtNode> > body_stmt_nodes1;
  unique_ptr<DataTypeNode> var_data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "name", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  body_stmt_nodes1.push_back(move(var_def_node2));
  ScopeNode *if_body_ptr1 = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> if_body1(if_body_ptr1);

  IdNode *if_cond_ptr1 = new IdNode(
      TokenInfo(Token::kName, "name", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> if_cond1(if_cond_ptr1);
  unique_ptr<IfNode> if_node1(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      move(if_cond1),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      move(if_body1)));

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes2;
  unique_ptr<DataTypeNode> var_data_type_node3(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(18), UINT32_C(18))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "name", UINT32_C(19), UINT32_C(19)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(20), UINT32_C(20)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  body_stmt_nodes2.push_back(move(var_def_node3));
  ScopeNode *if_body_ptr2 = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(17), UINT32_C(17)),
      move(body_stmt_nodes2),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21)));
  unique_ptr<ScopeNode> if_body2(if_body_ptr2);

  IdNode *if_cond_ptr2 = new IdNode(
      TokenInfo(Token::kName, "name", UINT32_C(15), UINT32_C(15)));
  unique_ptr<ExprNode> if_cond2(if_cond_ptr2);
  unique_ptr<IfNode> if_node2(new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(13), UINT32_C(13)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(14), UINT32_C(14)),
      move(if_cond2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(16), UINT32_C(16)),
      move(if_body2)));
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(12), UINT32_C(12)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  vector< unique_ptr<StmtNode> > body_stmt_nodes3;
  unique_ptr<DataTypeNode> var_data_type_node4(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(24), UINT32_C(24))));
  VarDefWithoutInitNode *var_def_node_ptr4 = new VarDefWithoutInitNode(
      move(var_data_type_node4),
      TokenInfo(Token::kName, "name", UINT32_C(25), UINT32_C(25)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(26), UINT32_C(26)));
  unique_ptr<StmtNode> var_def_node4(var_def_node_ptr4);
  body_stmt_nodes3.push_back(move(var_def_node4));
  ScopeNode *else_body_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(23), UINT32_C(23)),
      move(body_stmt_nodes3),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(27), UINT32_C(27)));
  unique_ptr<ScopeNode> else_body(else_body_ptr);

  unique_ptr<StmtNode> if_else_if_node(new IfElseIfElseNode(
      move(if_node1),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(22), UINT32_C(22)),
      move(else_body)));
  stmt_nodes.push_back(move(if_else_if_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> if_body_local_var_defs1 = {var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis1(
      new ScopeAnalysis(if_body_local_var_defs1));
  node_analyzes.insert(make_pair(if_body_ptr1, move(if_body_analysis1)));

  vector<const VarDefNode*> if_body_local_var_defs2 = {var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> if_body_analysis2(
      new ScopeAnalysis(if_body_local_var_defs2));
  node_analyzes.insert(make_pair(if_body_ptr2, move(if_body_analysis2)));

  vector<const VarDefNode*> else_body_local_var_defs = {var_def_node_ptr4};
  unique_ptr<NodeSemanticAnalysis> else_body_analysis(
      new ScopeAnalysis(else_body_local_var_defs));
  node_analyzes.insert(make_pair(else_body_ptr, move(else_body_analysis)));

  unique_ptr<DataType> var_data_type1(new BoolDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(
      new GlobalVarDefAnalysis(move(var_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<DataType> var_data_type2(new BoolDataType());
  uint32_t var_index_within_func2 = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(
      new LocalVarDefAnalysis(move(var_data_type2), var_index_within_func2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  unique_ptr<DataType> var_data_type3(new BoolDataType());
  uint32_t var_index_within_func3 = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(
      new LocalVarDefAnalysis(move(var_data_type3), var_index_within_func3));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  unique_ptr<DataType> var_data_type4(new BoolDataType());
  uint32_t var_index_within_func4 = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis4(
      new LocalVarDefAnalysis(move(var_data_type4), var_index_within_func4));
  node_analyzes.insert(make_pair(var_def_node_ptr4, move(var_def_analysis4)));

  unique_ptr<DataType> if_cond_data_type1(new BoolDataType());
  unique_ptr<DataType> if_cond_casted_data_type1;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> if_cond_expr_analysis1(new IdAnalysis(
      move(if_cond_data_type1),
      move(if_cond_casted_data_type1),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee));
  node_analyzes.insert(make_pair(if_cond_ptr1, move(if_cond_expr_analysis1)));

  unique_ptr<DataType> if_cond_data_type2(new BoolDataType());
  unique_ptr<DataType> if_cond_casted_data_type2;
  bool is_id_assignee2 = false;
  unique_ptr<NodeSemanticAnalysis> if_cond_expr_analysis2(new IdAnalysis(
      move(if_cond_data_type2),
      move(if_cond_casted_data_type2),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee2));
  node_analyzes.insert(make_pair(if_cond_ptr2, move(if_cond_expr_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfElseUsingIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *func_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(func_body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  unique_ptr<ScopeNode> if_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  IfNode *if_node_ptr = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(7), UINT32_C(7)),
      move(id_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      move(if_body_node));
  unique_ptr<IfNode> if_node(if_node_ptr);

  unique_ptr<ScopeNode> else_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(14), UINT32_C(14))));
  IfElseIfElseNode *if_else_if_else_node_ptr = new IfElseIfElseNode(
      move(if_node),
      vector< unique_ptr<ElseIfNode> >(),
      TokenInfo(Token::kElse, "else", UINT32_C(12), UINT32_C(12)),
      move(else_body_node));
  unique_ptr<StmtNode> if_else_if_else_node(if_else_if_else_node_ptr);
  stmt_nodes.push_back(move(if_else_if_else_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> func_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> func_body_analysis(
      new ScopeAnalysis(func_body_local_var_defs));
  node_analyzes.insert(make_pair(func_body_node_ptr, move(func_body_analysis)));
  bool is_func_native = false;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type),
        move(func_arg_data_types),
        is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native);
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  unique_ptr<FuncDataType> func_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis(new IdAnalysis(
      move(func_data_type),
      move(func_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      *if_else_if_else_node_ptr,
      *if_node_ptr,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfElseUsingElseIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *func_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(func_body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(6), UINT32_C(6))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  ScopeNode *if_body_node_ptr1 = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(14), UINT32_C(14)));
  unique_ptr<ScopeNode> if_body_node1(if_body_node_ptr1);
  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IfNode *if_node_ptr1 = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(10), UINT32_C(10)),
      move(id_node1),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(12), UINT32_C(12)),
      move(if_body_node1));
  unique_ptr<IfNode> if_node1(if_node_ptr1);

  vector< unique_ptr<ElseIfNode> > else_if_nodes;
  unique_ptr<ScopeNode> if_body_node2(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(20), UINT32_C(20)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21))));
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(18), UINT32_C(18)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  IfNode *if_node_ptr2 = new IfNode(
      TokenInfo(Token::kIf, "if", UINT32_C(16), UINT32_C(16)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(17), UINT32_C(17)),
      move(id_node2),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(19), UINT32_C(19)),
      move(if_body_node2));
  unique_ptr<IfNode> if_node2(if_node_ptr2);
  unique_ptr<ElseIfNode> else_if_node(new ElseIfNode(
      TokenInfo(Token::kElse, "else", UINT32_C(15), UINT32_C(15)),
      move(if_node2)));
  else_if_nodes.push_back(move(else_if_node));

  unique_ptr<ScopeNode> else_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(23), UINT32_C(23)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(24), UINT32_C(24))));
  IfElseIfElseNode *if_else_if_else_node_ptr = new IfElseIfElseNode(
      move(if_node1),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(22), UINT32_C(22)),
      move(else_body_node));
  unique_ptr<StmtNode> if_else_if_else_node(if_else_if_else_node_ptr);
  stmt_nodes.push_back(move(if_else_if_else_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> func_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> func_body_analysis(
      new ScopeAnalysis(func_body_local_var_defs));
  node_analyzes.insert(make_pair(func_body_node_ptr, move(func_body_analysis)));

  vector<const VarDefNode*> if_body_local_var_defs1;
  unique_ptr<NodeSemanticAnalysis> if_body_analysis1(
      new ScopeAnalysis(if_body_local_var_defs1));
  node_analyzes.insert(make_pair(if_body_node_ptr1, move(if_body_analysis1)));
  bool is_func_native = false;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type),
        move(func_arg_data_types),
        is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  {
    unique_ptr<DataType> var_data_type(new BoolDataType());
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(
        new GlobalVarDefAnalysis(move(var_data_type)));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  }

  unique_ptr<DataType> var_data_type(new BoolDataType());
  unique_ptr<DataType> var_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis1(new IdAnalysis(
      move(var_data_type),
      move(var_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native);
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  unique_ptr<FuncDataType> func_casted_data_type;
  bool is_id_assignee2 = false;
  unique_ptr<NodeSemanticAnalysis> id_expr_analysis2(new IdAnalysis(
      move(func_data_type),
      move(func_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      *if_else_if_else_node_ptr,
      *if_node_ptr2,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Import) {
  SemanticAnalysis::ProgramProblems problems;
  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr1 = new StringNode(TokenInfo(
      Token::kStringLit, "\"a.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node1(file_path_node_ptr1);
  ImportNode *import_node_ptr1 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node1),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node1(import_node_ptr1);
  main_program_stmt_nodes.push_back(move(import_node1));

  IdNode *id_expr_node_ptr = new IdNode(
      TokenInfo(Token::kName, "a", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_expr_node(id_expr_node_ptr);
  unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
      move(id_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4))));
  main_program_stmt_nodes.push_back(move(id_stmt_node));
  unique_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));
  problems.insert(
      make_pair(main_program_node.get(), SemanticAnalysis::Problems()));

  SemanticAnalyzer::ImportPrograms import_program_nodes;
  vector< unique_ptr<StmtNode> > import_program_stmt_nodes1;
  StringNode *file_path_node_ptr2 = new StringNode(TokenInfo(
      Token::kStringLit, "\"b.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node2(file_path_node_ptr2);
  ImportNode *import_node_ptr2 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node2(import_node_ptr2);
  import_program_stmt_nodes1.push_back(move(import_node2));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "a", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  import_program_stmt_nodes1.push_back(move(var_def_node1));

  IdNode *id_expr_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "b", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_expr_node2(id_expr_node_ptr2);
  unique_ptr<StmtNode> id_stmt_node2(new ExprStmtNode(
      move(id_expr_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7))));
  import_program_stmt_nodes1.push_back(move(id_stmt_node2));
  unique_ptr<ProgramNode> import_program_node1(
      new ProgramNode(move(import_program_stmt_nodes1)));
  problems.insert(
      make_pair(import_program_node1.get(), SemanticAnalysis::Problems()));
  import_program_nodes.push_back(move(import_program_node1));

  vector< unique_ptr<StmtNode> > import_program_stmt_nodes2;
  unique_ptr<DataTypeNode> var_data_type_node2(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "b", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  import_program_stmt_nodes2.push_back(move(var_def_node2));
  unique_ptr<ProgramNode> import_program_node2(
      new ProgramNode(move(import_program_stmt_nodes2)));
  problems.insert(
      make_pair(import_program_node2.get(), SemanticAnalysis::Problems()));
  import_program_nodes.push_back(move(import_program_node2));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"a.rt\"", "a.rt"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> id_data_type(new IntDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_expr_node_ptr, move(id_analysis)));

  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(
      new GlobalVarDefAnalysis(move(var_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<DataType> var_data_type2(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(
      new GlobalVarDefAnalysis(move(var_data_type2)));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  unique_ptr<DataType> file_path_expr_data_type1(new StringDataType());
  unique_ptr<DataType> file_path_expr_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> file_path_lit_analysis1(new LitAnalysis(
      move(file_path_expr_data_type1),
      move(file_path_expr_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("a.rt"))));
  node_analyzes.insert(
      make_pair(file_path_node_ptr1, move(file_path_lit_analysis1)));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(main_program_node),
                              move(import_program_nodes),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ImportAfterAnyOtherStmtIsInvalid) {
  SemanticAnalysis::ProgramProblems problems;
  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  main_program_stmt_nodes.push_back(move(var_def_node1));

  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"import.rt\"", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));
  unique_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  SemanticAnalyzer::ImportPrograms import_program_nodes;
  vector< unique_ptr<StmtNode> > import_program_stmt_nodes;
  StringNode *file_path_node_ptr2 = new StringNode(
      TokenInfo(Token::kStringLit, "\"import2.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node2(file_path_node_ptr2);
  ImportNode *import_node_ptr2 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node2(import_node_ptr2);
  import_program_stmt_nodes.push_back(move(import_node2));
  unique_ptr<ProgramNode> import_program_node(
      new ProgramNode(move(import_program_stmt_nodes)));
  problems.insert(
      make_pair(import_program_node.get(), SemanticAnalysis::Problems()));
  import_program_nodes.push_back(move(import_program_node));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(
      new GlobalVarDefAnalysis(move(var_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<SemanticProblem> problem(
      new ImportIsNotFirstStmtError(*import_node_ptr));
  problems[main_program_node.get()].push_back(move(problem));

  TestLitParses test_lit_parses = {};
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(main_program_node),
                              move(import_program_nodes),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, PreTestLoop) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node1(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  stmt_nodes.push_back(move(var_def_node1));

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node2(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "name", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  body_stmt_nodes.push_back(move(var_def_node2));
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "name", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      move(id_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      move(body_node)));
  stmt_nodes.push_back(move(loop_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs = {var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> var_def_data_type1(new BoolDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(new GlobalVarDefAnalysis(
      move(var_def_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<DataType> var_def_data_type2(new BoolDataType());
  uint32_t var_index_within_func = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      move(var_def_data_type2), var_index_within_func));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  unique_ptr<DataType> id_data_type(new BoolDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       PreTestLoopWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *func_body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> func_body_node(func_body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(func_body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  unique_ptr<ScopeNode> loop_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  PreTestLoopNode *loop_node_ptr = new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(7), UINT32_C(7)),
      move(id_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      move(loop_body_node));
  unique_ptr<StmtNode> loop_node(loop_node_ptr);
  stmt_nodes.push_back(move(loop_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> func_body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> func_body_analysis(
      new ScopeAnalysis(func_body_local_var_defs));
  node_analyzes.insert(make_pair(func_body_node_ptr, move(func_body_analysis)));
  bool is_func_native = false;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_def_data_type(new FuncDataType(
        move(func_return_data_type),
        move(func_arg_data_types),
        is_func_native));
    bool is_func_has_return = false;
    unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_def_data_type), is_func_has_return));
    node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *id_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types), is_func_native);
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new PreTestLoopWithIncompatibleTypeError(
      *loop_node_ptr,
      unique_ptr<DataType>(new BoolDataType()),
      id_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Continue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node1(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  stmt_nodes.push_back(move(var_def_node1));

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
    unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  body_stmt_nodes.push_back(move(var_def_node2));

  ContinueNode *continue_node_ptr = new ContinueNode(
      TokenInfo(Token::kContinue, "continue", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
  unique_ptr<StmtNode> continue_node(continue_node_ptr);
  body_stmt_nodes.push_back(move(continue_node));

  unique_ptr<DataTypeNode> var_data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(13))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(14), UINT32_C(14)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  body_stmt_nodes.push_back(move(var_def_node3));

  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      move(id_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      move(body_node)));
  stmt_nodes.push_back(move(loop_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs =
      {var_def_node_ptr2, var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  vector<const VarDefNode*> flow_local_var_defs = {var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> continue_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes.insert(make_pair(continue_node_ptr, move(continue_analysis)));

  unique_ptr<DataType> var_def_data_type1(new BoolDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(new GlobalVarDefAnalysis(
      move(var_def_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<DataType> var_def_data_type2(new IntDataType());
  uint32_t var_index_within_func2 = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      move(var_def_data_type2), var_index_within_func2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  unique_ptr<DataType> var_def_data_type3(new LongDataType());
  uint32_t var_index_within_func3 = UINT32_C(1);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      move(var_def_data_type3), var_index_within_func3));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  unique_ptr<DataType> id_data_type(new BoolDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ContinueNotWithinLoopIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ContinueNode *continue_node_ptr = new ContinueNode(
      TokenInfo(Token::kContinue, "continue", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StmtNode> continue_node(continue_node_ptr);
  stmt_nodes.push_back(move(continue_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ContinueNotWithinLoopError(
      *continue_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Break) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node1(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  stmt_nodes.push_back(move(var_def_node1));

  vector< unique_ptr<StmtNode> > body_stmt_nodes;
    unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(8), UINT32_C(8))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  body_stmt_nodes.push_back(move(var_def_node2));

  BreakNode *break_node_ptr = new BreakNode(
      TokenInfo(Token::kBreak, "break", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)));
  unique_ptr<StmtNode> break_node(break_node_ptr);
  body_stmt_nodes.push_back(move(break_node));

  unique_ptr<DataTypeNode> var_data_type_node3(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(13))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(14), UINT32_C(14)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  body_stmt_nodes.push_back(move(var_def_node3));

  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  unique_ptr<StmtNode> loop_node(new PreTestLoopNode(
      TokenInfo(Token::kWhile, "while", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(4), UINT32_C(4)),
      move(id_node),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(6), UINT32_C(6)),
      move(body_node)));
  stmt_nodes.push_back(move(loop_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs =
      {var_def_node_ptr2, var_def_node_ptr3};
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  vector<const VarDefNode*> flow_local_var_defs = {var_def_node_ptr2};
  unique_ptr<NodeSemanticAnalysis> break_analysis(
      new ControlFlowTransferAnalysis(flow_local_var_defs));
  node_analyzes.insert(make_pair(break_node_ptr, move(break_analysis)));

  unique_ptr<DataType> var_def_data_type1(new BoolDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis1(new GlobalVarDefAnalysis(
      move(var_def_data_type1)));
  node_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  unique_ptr<DataType> var_def_data_type2(new IntDataType());
  uint32_t var_index_within_func2 = UINT32_C(0);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new LocalVarDefAnalysis(
      move(var_def_data_type2), var_index_within_func2));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  unique_ptr<DataType> var_def_data_type3(new LongDataType());
  uint32_t var_index_within_func3 = UINT32_C(1);
  unique_ptr<NodeSemanticAnalysis> var_def_analysis3(new LocalVarDefAnalysis(
      move(var_def_data_type3), var_index_within_func3));
  node_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  unique_ptr<DataType> id_data_type(new BoolDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr1,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, BreakNotWithinLoopIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  BreakNode *break_node_ptr = new BreakNode(
      TokenInfo(Token::kBreak, "break", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StmtNode> break_node(break_node_ptr);
  stmt_nodes.push_back(move(break_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BreakNotWithinLoopError(
      *break_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, AssignWithId) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  CharNode *lit_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> lit_node(lit_node_ptr);
  AssignNode *assign_node_ptr = new AssignNode(
      TokenInfo(Token::kAssignOp, "=", UINT32_C(4), UINT32_C(4)),
      move(id_node),
      move(lit_node));
  unique_ptr<ExprNode> assign_node(assign_node_ptr);
  unique_ptr<StmtNode> assign_stmt_node(new ExprStmtNode(
      move(assign_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  stmt_nodes.push_back(move(assign_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_def_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
      move(var_def_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  unique_ptr<DataType> assign_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> assign_expr_analysis(new CommonExprAnalysis(
      unique_ptr<DataType>(new VoidDataType()),
      move(assign_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(assign_node_ptr, move(assign_expr_analysis)));

  unique_ptr<DataType> id_data_type(new IntDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = true;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  unique_ptr<DataType> lit_data_type(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(lit_data_type),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, AssignWithSubscript) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> data_type_node2(new ArrayDataTypeNode(
      move(data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(data_type_node2),
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  unique_ptr<DataTypeNode> data_type_node3(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(5), UINT32_C(5))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(data_type_node3),
      TokenInfo(Token::kName, "index", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  stmt_nodes.push_back(move(var_def_node2));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "index", UINT32_C(10), UINT32_C(10)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  SubscriptNode *subscript_node_ptr = new SubscriptNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(11), UINT32_C(11)),
      move(id_node),
      move(id_node2));
  unique_ptr<ExprNode> subscript_node(subscript_node_ptr);
  CharNode *lit_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(13), UINT32_C(13)));
  unique_ptr<ExprNode> lit_node(lit_node_ptr);
  AssignNode *assign_node_ptr = new AssignNode(
      TokenInfo(Token::kAssignOp, "=", UINT32_C(12), UINT32_C(12)),
      move(subscript_node),
      move(lit_node));
  unique_ptr<ExprNode> assign_node(assign_node_ptr);
  unique_ptr<StmtNode> assign_stmt_node(new ExprStmtNode(
      move(assign_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(14), UINT32_C(14))));
  stmt_nodes.push_back(move(assign_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> array_data_type(new ArrayDataType(
      unique_ptr<DataType>(new IntDataType())));
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
      array_data_type->Clone()));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  unique_ptr<NodeSemanticAnalysis> var_def_analysis2(new GlobalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType())));
  node_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> assign_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> assign_expr_analysis(new CommonExprAnalysis(
      unique_ptr<DataType>(new VoidDataType()),
      move(assign_casted_data_type),
      ValueType::kRight));
  node_analyzes.insert(make_pair(assign_node_ptr, move(assign_expr_analysis)));
  bool is_id_assignee = false;
  unique_ptr<DataType> id_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      array_data_type->Clone(),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
  bool is_id_assignee2 = false;
  unique_ptr<DataType> id_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(id_casted_data_type2),
      ValueType::kLeft,
      var_def_node_ptr2,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));
  bool is_subscript_assignee = true;
  unique_ptr<DataType> subscript_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> subscript_analysis(new SubscriptAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(subscript_casted_data_type),
      ValueType::kLeft,
      is_subscript_assignee));
  node_analyzes.insert(make_pair(subscript_node_ptr, move(subscript_analysis)));
  unique_ptr<DataType> lit_data_type(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(lit_data_type),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, AssignWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  StringNode *lit_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"7\"", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> lit_node(lit_node_ptr);
  AssignNode *assign_node_ptr = new AssignNode(
      TokenInfo(Token::kAssignOp, "=", UINT32_C(4), UINT32_C(4)),
      move(id_node),
      move(lit_node));
  unique_ptr<ExprNode> assign_node(assign_node_ptr);
  unique_ptr<StmtNode> assign_stmt_node(new ExprStmtNode(
      move(assign_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  stmt_nodes.push_back(move(assign_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"7\"", "7"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_def_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
      move(var_def_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  IntDataType *id_data_type_ptr = new IntDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = true;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  StringDataType *lit_data_type_ptr = new StringDataType();
  unique_ptr<DataType> lit_data_type(lit_data_type_ptr);
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(lit_data_type),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("7"))));
  node_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *assign_node_ptr,
      id_data_type_ptr->Clone(),
      lit_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       AssignUsingLeftOperandWithRightValueTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  StringNode *lit_node_ptr1 = new StringNode(
      TokenInfo(Token::kStringLit, "\"1\"", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  StringNode *lit_node_ptr2 = new StringNode(
      TokenInfo(Token::kStringLit, "\"2\"", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  AssignNode *assign_node_ptr = new AssignNode(
      TokenInfo(Token::kAssignOp, "=", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> assign_node(assign_node_ptr);
  unique_ptr<StmtNode> assign_stmt_node(new ExprStmtNode(
      move(assign_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(assign_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"1\"", "1"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new StringDataType());
  unique_ptr<DataType> lit_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("1"))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new AssignWithRightValueAssigneeError(
      *assign_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Less) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  CharNode *lit_node_ptr2 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  LessNode *less_node_ptr = new LessNode(
      TokenInfo(Token::kLessOp, "<", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> less_node(less_node_ptr);
  unique_ptr<StmtNode> less_stmt_node(new ExprStmtNode(
      move(less_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(less_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type2(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> less_data_type(new BoolDataType());
  unique_ptr<DataType> less_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> less_expr_analysis(new CommonExprAnalysis(
      move(less_data_type), move(less_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(less_node_ptr, move(less_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, LessWithUnsupportedDataTypesIsInvalid) {
  struct TestData {
    unique_ptr<DataTypeNode> var_data_type_node;
    unique_ptr<DataType> var_data_type;
  };
  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
        TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> var_data_type(new BoolDataType());
    TestData test_data = {move(var_data_type_node), move(var_data_type)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<DataTypeNode> var_data_type_node(new StringDataTypeNode(
        TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> var_data_type(new StringDataType());
    TestData test_data = {move(var_data_type_node), move(var_data_type)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<DataTypeNode> element_data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
        move(element_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
        TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
    unique_ptr<DataType> element_data_type(new IntDataType());
    unique_ptr<DataType> var_data_type(
        new ArrayDataType(move(element_data_type)));
    TestData test_data = {move(var_data_type_node), move(var_data_type)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
        move(test_data.var_data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
    unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
    stmt_nodes.push_back(move(var_def_node));

    IdNode *id_node_ptr1 = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
    unique_ptr<ExprNode> id_node1(id_node_ptr1);
    IdNode *id_node_ptr2 = new IdNode(
        TokenInfo(Token::kName, "var", UINT32_C(7), UINT32_C(7)));
    unique_ptr<ExprNode> id_node2(id_node_ptr2);
    LessNode *less_node_ptr = new LessNode(
        TokenInfo(Token::kLessOp, "<", UINT32_C(6), UINT32_C(6)),
        move(id_node1),
        move(id_node2));
    unique_ptr<ExprNode> less_node(less_node_ptr);
    unique_ptr<StmtNode> less_stmt_node(new ExprStmtNode(
        move(less_node),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8))));
    stmt_nodes.push_back(move(less_stmt_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::NodeAnalyzes node_analyzes;
    unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
        test_data.var_data_type->Clone()));
    node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

    bool is_id_assignee = false;
    unique_ptr<DataType> id_casted_data_type1;
    unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
        test_data.var_data_type->Clone(),
        move(id_casted_data_type1),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee));
    node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

    bool is_id_assignee2 = false;
    unique_ptr<DataType> id_casted_data_type2;
    unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
        test_data.var_data_type->Clone(),
        move(id_casted_data_type2),
        ValueType::kLeft,
        var_def_node_ptr,
        is_id_assignee2));
    node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

    SemanticAnalysis::ProgramProblems problems;
    unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
        *less_node_ptr,
        test_data.var_data_type->Clone(),
        test_data.var_data_type->Clone()));
    problems[program_node.get()].push_back(move(problem));

    SemanticAnalysis analysis(move(problems), move(node_analyzes));
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {move(program_node),
                                SemanticAnalyzer::ImportPrograms(),
                                move(analysis),
                                test_lit_parses};
    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, And) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  BoolNode *lit_node_ptr1 = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "true", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  BoolNode *lit_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "true", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  AndNode *and_node_ptr = new AndNode(
      TokenInfo(Token::kAndOp, "&&", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> and_node(and_node_ptr);
  unique_ptr<StmtNode> and_stmt_node(new ExprStmtNode(
      move(and_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(and_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.bools = {{"true", true},
                           {"true", true}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new BoolDataType());
  unique_ptr<DataType> lit_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new BoolDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> and_data_type(new BoolDataType());
  unique_ptr<DataType> and_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> and_expr_analysis(new CommonExprAnalysis(
      move(and_data_type), move(and_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(and_node_ptr, move(and_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, AndWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  AndNode *and_node_ptr = new AndNode(
      TokenInfo(Token::kAndOp, "&&", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> and_node(and_node_ptr);
  unique_ptr<StmtNode> and_stmt_node(new ExprStmtNode(
      move(and_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(and_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *and_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Or) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  BoolNode *lit_node_ptr1 = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "true", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  BoolNode *lit_node_ptr2 = new BoolNode(
      TokenInfo(Token::kBoolTrueLit, "true", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  OrNode *or_node_ptr = new OrNode(
      TokenInfo(Token::kOrOp, "||", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> or_node(or_node_ptr);
  unique_ptr<StmtNode> or_stmt_node(new ExprStmtNode(
      move(or_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(or_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.bools = {{"true", true},
                           {"true", true}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new BoolDataType());
  unique_ptr<DataType> lit_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new BoolDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(true))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> or_data_type(new BoolDataType());
  unique_ptr<DataType> or_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> or_expr_analysis(new CommonExprAnalysis(
      move(or_data_type), move(or_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(or_node_ptr, move(or_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, OrWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  OrNode *or_node_ptr = new OrNode(
      TokenInfo(Token::kOrOp, "||", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> or_node(or_node_ptr);
  unique_ptr<StmtNode> or_stmt_node(new ExprStmtNode(
      move(or_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(or_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *or_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Mul) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  DoubleNode *lit_node_ptr2 = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  MulNode *mul_node_ptr = new MulNode(
      TokenInfo(Token::kMulOp, "*", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> mul_node(mul_node_ptr);
  unique_ptr<StmtNode> mul_stmt_node(new ExprStmtNode(
      move(mul_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(mul_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};
  test_lit_parses.doubles = {{"2.2", 2.2}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type1(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new DoubleDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> mul_data_type(new DoubleDataType());
  unique_ptr<DataType> mul_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> mul_expr_analysis(new CommonExprAnalysis(
      move(mul_data_type), move(mul_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(mul_node_ptr, move(mul_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, MulWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  MulNode *mul_node_ptr = new MulNode(
      TokenInfo(Token::kMulOp, "*", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> mul_node(mul_node_ptr);
  unique_ptr<StmtNode> mul_stmt_node(new ExprStmtNode(
      move(mul_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(mul_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *mul_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Div) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  DoubleNode *lit_node_ptr2 = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  DivNode *div_node_ptr = new DivNode(
      TokenInfo(Token::kDivOp, "/", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> div_node(div_node_ptr);
  unique_ptr<StmtNode> div_stmt_node(new ExprStmtNode(
      move(div_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(div_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};
  test_lit_parses.doubles = {{"2.2", 2.2}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type1(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new DoubleDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> div_data_type(new DoubleDataType());
  unique_ptr<DataType> div_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> div_expr_analysis(new CommonExprAnalysis(
      move(div_data_type), move(div_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(div_node_ptr, move(div_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, DivWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  DivNode *div_node_ptr = new DivNode(
      TokenInfo(Token::kDivOp, "/", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> div_node(div_node_ptr);
  unique_ptr<StmtNode> div_stmt_node(new ExprStmtNode(
      move(div_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(div_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *div_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Sum) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  DoubleNode *lit_node_ptr2 = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  SumNode *sum_node_ptr = new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> sum_node(sum_node_ptr);
  unique_ptr<StmtNode> sum_stmt_node(new ExprStmtNode(
      move(sum_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(sum_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};
  test_lit_parses.doubles = {{"2.2", 2.2}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type1(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new DoubleDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> sum_data_type(new DoubleDataType());
  unique_ptr<DataType> sum_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> sum_expr_analysis(new CommonExprAnalysis(
      move(sum_data_type), move(sum_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(sum_node_ptr, move(sum_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, SumWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  SumNode *sum_node_ptr = new SumNode(
      TokenInfo(Token::kSumOp, "+", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> sum_node(sum_node_ptr);
  unique_ptr<StmtNode> sum_stmt_node(new ExprStmtNode(
      move(sum_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(sum_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *sum_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Sub) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  DoubleNode *lit_node_ptr2 = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "2.2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  SubNode *sub_node_ptr = new SubNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> sub_node(sub_node_ptr);
  unique_ptr<StmtNode> sub_stmt_node(new ExprStmtNode(
      move(sub_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(sub_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};
  test_lit_parses.doubles = {{"2.2", 2.2}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type1(new DoubleDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(1)))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new DoubleDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(2.2))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> sub_data_type(new DoubleDataType());
  unique_ptr<DataType> sub_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> sub_expr_analysis(new CommonExprAnalysis(
      move(sub_data_type), move(sub_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(sub_node_ptr, move(sub_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, SubWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  SubNode *sub_node_ptr = new SubNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> sub_node(sub_node_ptr);
  unique_ptr<StmtNode> sub_stmt_node(new ExprStmtNode(
      move(sub_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(sub_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *sub_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Equal) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *lit_node_ptr1 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  IntNode *lit_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  EqualNode *equal_node_ptr = new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> equal_node(equal_node_ptr);
  unique_ptr<StmtNode> equal_stmt_node(new ExprStmtNode(
      move(equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"2", INT32_C(2)}};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> equal_data_type(new BoolDataType());
  unique_ptr<DataType> equal_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> equal_expr_analysis(new CommonExprAnalysis(
      move(equal_data_type), move(equal_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(equal_node_ptr, move(equal_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, EqualWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  EqualNode *equal_node_ptr = new EqualNode(
      TokenInfo(Token::kEqualOp, "==", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> equal_node(equal_node_ptr);
  unique_ptr<StmtNode> equal_stmt_node(new ExprStmtNode(
      move(equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *equal_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, LessOrEqual) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *lit_node_ptr1 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  IntNode *lit_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  LessOrEqualNode *less_or_equal_node_ptr = new LessOrEqualNode(
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> less_or_equal_node(less_or_equal_node_ptr);
  unique_ptr<StmtNode> less_or_equal_stmt_node(new ExprStmtNode(
      move(less_or_equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(less_or_equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"2", INT32_C(2)}};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> less_or_equal_data_type(new BoolDataType());
  unique_ptr<DataType> less_or_equal_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> less_or_equal_expr_analysis(
      new CommonExprAnalysis(move(less_or_equal_data_type),
                             move(less_or_equal_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(
      make_pair(less_or_equal_node_ptr, move(less_or_equal_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       LessOrEqualWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  LessOrEqualNode *less_or_equal_node_ptr = new LessOrEqualNode(
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> less_or_equal_node(less_or_equal_node_ptr);
  unique_ptr<StmtNode> less_or_equal_stmt_node(new ExprStmtNode(
      move(less_or_equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(less_or_equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *less_or_equal_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, GreaterOrEqual) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *lit_node_ptr1 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  IntNode *lit_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  GreaterOrEqualNode *greater_or_equal_node_ptr = new GreaterOrEqualNode(
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> greater_or_equal_node(greater_or_equal_node_ptr);
  unique_ptr<StmtNode> greater_or_equal_stmt_node(new ExprStmtNode(
      move(greater_or_equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(greater_or_equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"2", INT32_C(2)}};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> greater_or_equal_data_type(new BoolDataType());
  unique_ptr<DataType> greater_or_equal_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> greater_or_equal_expr_analysis(
      new CommonExprAnalysis(move(greater_or_equal_data_type),
                             move(greater_or_equal_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(make_pair(
      greater_or_equal_node_ptr, move(greater_or_equal_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       GreaterOrEqualWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  GreaterOrEqualNode *greater_or_equal_node_ptr = new GreaterOrEqualNode(
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> greater_or_equal_node(greater_or_equal_node_ptr);
  unique_ptr<StmtNode> greater_or_equal_stmt_node(new ExprStmtNode(
      move(greater_or_equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(greater_or_equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *greater_or_equal_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, NotEqual) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *lit_node_ptr1 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  IntNode *lit_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  NotEqualNode *not_equal_node_ptr = new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> not_equal_node(not_equal_node_ptr);
  unique_ptr<StmtNode> not_equal_stmt_node(new ExprStmtNode(
      move(not_equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(not_equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"2", INT32_C(2)}};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> not_equal_data_type(new BoolDataType());
  unique_ptr<DataType> not_equal_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> not_equal_expr_analysis(
      new CommonExprAnalysis(move(not_equal_data_type),
                             move(not_equal_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(
      make_pair(not_equal_node_ptr, move(not_equal_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, NotEqualWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  NotEqualNode *not_equal_node_ptr = new NotEqualNode(
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> not_equal_node(not_equal_node_ptr);
  unique_ptr<StmtNode> not_equal_stmt_node(new ExprStmtNode(
      move(not_equal_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(not_equal_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *not_equal_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Greater) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *lit_node_ptr1 = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  IntNode *lit_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
  unique_ptr<ExprNode> lit_node2(lit_node_ptr2);
  GreaterNode *greater_node_ptr = new GreaterNode(
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(1), UINT32_C(1)),
      move(lit_node1),
      move(lit_node2));
  unique_ptr<ExprNode> greater_node(greater_node_ptr);
  unique_ptr<StmtNode> greater_stmt_node(new ExprStmtNode(
      move(greater_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3))));
  stmt_nodes.push_back(move(greater_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"2", INT32_C(2)}};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_data_type1(new CharDataType());
  unique_ptr<DataType> lit_casted_data_type1(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> lit_analysis1(new LitAnalysis(
      move(lit_data_type1),
      move(lit_casted_data_type1),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  unique_ptr<DataType> lit_data_type2(new IntDataType());
  unique_ptr<DataType> lit_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> lit_analysis2(new LitAnalysis(
      move(lit_data_type2),
      move(lit_casted_data_type2),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(2)))));
  node_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  unique_ptr<DataType> greater_data_type(new BoolDataType());
  unique_ptr<DataType> greater_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> greater_expr_analysis(
      new CommonExprAnalysis(move(greater_data_type),
                             move(greater_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(
      make_pair(greater_node_ptr, move(greater_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, GreaterWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr1 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> id_node1(id_node_ptr1);
  IdNode *id_node_ptr2 = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(8), UINT32_C(8)));
  unique_ptr<ExprNode> id_node2(id_node_ptr2);
  GreaterNode *greater_node_ptr = new GreaterNode(
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(7), UINT32_C(7)),
      move(id_node1),
      move(id_node2));
  unique_ptr<ExprNode> greater_node(greater_node_ptr);
  unique_ptr<StmtNode> greater_stmt_node(new ExprStmtNode(
      move(greater_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(greater_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  bool is_func_native = false;
  FuncDataType func_data_type(
      move(return_data_type), move(arg_data_types), is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  bool is_id_assignee = false;
  unique_ptr<DataType> func_casted_data_type1;
  unique_ptr<NodeSemanticAnalysis> id_analysis1(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type1),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));

  bool is_id_assignee2 = false;
  unique_ptr<DataType> func_casted_data_type2;
  unique_ptr<NodeSemanticAnalysis> id_analysis2(new IdAnalysis(
      func_data_type.Clone(),
      move(func_casted_data_type2),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee2));
  node_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      *greater_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ArrayAllocWithoutInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
  CharNode *size_value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> size_value_node(size_value_node_ptr);
  unique_ptr<BoundedArraySizeNode> size_node(new BoundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      move(size_value_node),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  ArrayAllocWithoutInitNode *alloc_node_ptr = new ArrayAllocWithoutInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes));
  unique_ptr<ExprNode> alloc_expr_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> element_data_type(new IntDataType());
  unique_ptr<DataType> alloc_data_type(
      new ArrayDataType(move(element_data_type)));
  unique_ptr<DataType> alloc_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> alloc_expr_analysis(new CommonExprAnalysis(
      move(alloc_data_type), move(alloc_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(alloc_node_ptr, move(alloc_expr_analysis)));

  unique_ptr<DataType> size_data_type(new CharDataType());
  unique_ptr<DataType> size_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> size_lit_analysis(new LitAnalysis(
      move(size_data_type),
      move(size_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(size_value_node_ptr, move(size_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayAllocWithoutInitUsingTooManyDimensionsIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
  const uint8_t max_dimensions_count = numeric_limits<uint8_t>::max();

  for (uint32_t i = 0; i != max_dimensions_count + 1; ++i) {
    unique_ptr<ExprNode> size_value_node(new IntNode(
        TokenInfo(Token::kIntLit, "1", i + 3, i + 3)));
    unique_ptr<BoundedArraySizeNode> size_node(new BoundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", i + 2, i + 2),
        move(size_value_node),
        TokenInfo(Token::kSubscriptEnd, "]", i + 4, i + 4)));
    size_nodes.push_back(move(size_node));
  }

  ArrayAllocWithoutInitNode *alloc_node_ptr = new ArrayAllocWithoutInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes));
  unique_ptr<ExprNode> alloc_expr_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1000), UINT32_C(1000))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ArrayAllocWithTooManyDimensionsError(
      *alloc_node_ptr, max_dimensions_count));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayAllocWithoutInitUsingUnsupportedSizeDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
  StringNode *size_value_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"1\"", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> size_value_node(size_value_node_ptr);
  BoundedArraySizeNode *size_node_ptr = new BoundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      move(size_value_node),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4)));
  unique_ptr<BoundedArraySizeNode> size_node(size_node_ptr);
  size_nodes.push_back(move(size_node));
  ArrayAllocWithoutInitNode *alloc_node_ptr = new ArrayAllocWithoutInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes));
  unique_ptr<ExprNode> alloc_expr_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"1\"", "1"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  StringDataType *size_data_type_ptr = new StringDataType();
  unique_ptr<DataType> size_data_type(size_data_type_ptr);
  unique_ptr<DataType> size_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> size_lit_analysis(new LitAnalysis(
      move(size_data_type),
      move(size_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("1"))));
  node_analyzes.insert(make_pair(size_value_node_ptr, move(size_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithUnsupportedSizeTypeError(
          *alloc_node_ptr,
          *size_node_ptr,
          size_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayAllocWithoutInitUsingUnsupportedElementDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<BoundedArraySizeNode> > size_nodes;
  IntNode *size_value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> size_value_node(size_value_node_ptr);
  unique_ptr<BoundedArraySizeNode> size_node(new BoundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      move(size_value_node),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  ArrayAllocWithoutInitNode *alloc_node_ptr = new ArrayAllocWithoutInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes));
  unique_ptr<ExprNode> alloc_expr_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<DataType> element_data_type(new VoidDataType());
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithUnsupportedElementTypeError(
          *alloc_node_ptr, move(element_data_type)));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ArrayAllocWithInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  CharNode *value_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  ArrayAllocWithInitNode *alloc_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      move(value_nodes),
      vector<TokenInfo>(),
      TokenInfo(Token::kScopeStart, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> alloc_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  IntDataType *element_data_type_ptr = new IntDataType();
  unique_ptr<DataType> element_data_type(element_data_type_ptr);
  unique_ptr<DataType> alloc_data_type(
      new ArrayDataType(move(element_data_type)));
  unique_ptr<DataType> alloc_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> alloc_expr_analysis(new CommonExprAnalysis(
      move(alloc_data_type), move(alloc_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(alloc_node_ptr, move(alloc_expr_analysis)));

  unique_ptr<DataType> value_data_type(new CharDataType());
  unique_ptr<DataType> value_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> value_lit_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(value_node_ptr, move(value_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayAllocWithInitUsingTooManyDimensionsIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  const uint8_t max_dimensions_count = numeric_limits<uint8_t>::max();

  for (uint32_t i = 0; i != max_dimensions_count + 1; ++i) {
    unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
        TokenInfo(Token::kSubscriptStart, "[", i + 2, i + 2),
        TokenInfo(Token::kSubscriptEnd, "]", i + 3, i + 3)));
    size_nodes.push_back(move(size_node));
  }

  ArrayAllocWithInitNode *alloc_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1000), UINT32_C(1000)),
      vector< unique_ptr<ExprNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(1001), UINT32_C(1001)));
  unique_ptr<ExprNode> alloc_expr_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1002), UINT32_C(1002))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ArrayAllocWithTooManyDimensionsError(
      *alloc_node_ptr, max_dimensions_count));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayAllocWithInitUsingUnsupportedElementDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(1), UINT32_C(1))));
  ArrayAllocWithInitNode *alloc_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      move(value_nodes),
      vector<TokenInfo>(),
      TokenInfo(Token::kScopeStart, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> alloc_expr_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<DataType> element_data_type(new VoidDataType());
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithUnsupportedElementTypeError(
          *alloc_node_ptr, move(element_data_type)));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayAllocWithInitUsingIncompatibleValueTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<UnboundedArraySizeNode> > size_nodes;
  unique_ptr<UnboundedArraySizeNode> size_node(new UnboundedArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  StringNode *value_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"2\"", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  ArrayAllocWithInitNode *alloc_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      move(value_nodes),
      vector<TokenInfo>(),
      TokenInfo(Token::kScopeStart, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> alloc_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"2\"", "2"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  StringDataType *value_data_type_ptr = new StringDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  unique_ptr<DataType> value_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> value_lit_analysis(new LitAnalysis(
      move(value_data_type),
      move(value_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("2"))));
  node_analyzes.insert(make_pair(value_node_ptr, move(value_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  size_t bad_value_index = 0;
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithIncompatibleValueTypeError(
          *alloc_node_ptr,
          bad_value_index,
          unique_ptr<DataType>(new IntDataType()),
          value_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Subscript) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  CharNode *index_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> index_node(index_node_ptr);
  SubscriptNode *subscript_node_ptr = new SubscriptNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(8), UINT32_C(8)),
      move(id_node),
      move(index_node));
  unique_ptr<ExprNode> subscript_node(subscript_node_ptr);
  unique_ptr<StmtNode> subscript_stmt_node(new ExprStmtNode(
      move(subscript_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(subscript_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> element_data_type(new LongDataType());
  unique_ptr<DataType> var_data_type(
      new ArrayDataType(move(element_data_type)));
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new GlobalVarDefAnalysis(move(var_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  unique_ptr<DataType> id_data_type(
      new ArrayDataType(unique_ptr<DataType>(new LongDataType())));
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  unique_ptr<DataType> index_data_type(new CharDataType());
  unique_ptr<DataType> index_casted_data_type(new IntDataType());
  unique_ptr<NodeSemanticAnalysis> index_lit_analysis(new LitAnalysis(
      move(index_data_type),
      move(index_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(index_node_ptr, move(index_lit_analysis)));

  unique_ptr<DataType> subscript_data_type(new LongDataType());
  unique_ptr<DataType> subscript_casted_data_type;
  bool is_subscript_assignee = false;
  unique_ptr<NodeSemanticAnalysis> subscript_expr_analysis(
      new SubscriptAnalysis(move(subscript_data_type),
                            move(subscript_casted_data_type),
                            ValueType::kLeft,
                            is_subscript_assignee));
  node_analyzes.insert(
      make_pair(subscript_node_ptr, move(subscript_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       SubscriptUsingOperandWithNonArrayDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  IntNode *index_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> index_node(index_node_ptr);
  SubscriptNode *subscript_node_ptr = new SubscriptNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(6), UINT32_C(6)),
      move(id_node),
      move(index_node));
  unique_ptr<ExprNode> subscript_node(subscript_node_ptr);
  unique_ptr<StmtNode> subscript_stmt_node(new ExprStmtNode(
      move(subscript_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(7), UINT32_C(7))));
  stmt_nodes.push_back(move(subscript_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new GlobalVarDefAnalysis(move(var_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  unique_ptr<DataType> id_data_type(new LongDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(
      new SubscriptWithUnsupportedOperandTypeError(
          *subscript_node_ptr, unique_ptr<DataType>(new LongDataType())));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       SubscriptWithUnsupportedIndexDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(5), UINT32_C(5)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  StringNode *index_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"1\"", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> index_node(index_node_ptr);
  SubscriptNode *subscript_node_ptr = new SubscriptNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(8), UINT32_C(8)),
      move(id_node),
      move(index_node));
  unique_ptr<ExprNode> subscript_node(subscript_node_ptr);
  unique_ptr<StmtNode> subscript_stmt_node(new ExprStmtNode(
      move(subscript_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  stmt_nodes.push_back(move(subscript_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"1\"", "1"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  StringDataType *index_data_type_ptr = new StringDataType();
  unique_ptr<DataType> index_data_type(index_data_type_ptr);
  unique_ptr<DataType> index_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> index_lit_analysis(new LitAnalysis(
      move(index_data_type),
      move(index_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("1"))));
  node_analyzes.insert(make_pair(index_node_ptr, move(index_lit_analysis)));

  unique_ptr<DataType> var_data_type(
      new ArrayDataType(unique_ptr<DataType>(new LongDataType())));
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new GlobalVarDefAnalysis(move(var_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  unique_ptr<DataType> id_data_type(
      new ArrayDataType(unique_ptr<DataType>(new LongDataType())));
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(
      new SubscriptWithUnsupportedIndexTypeError(
          *subscript_node_ptr,
          index_data_type_ptr->Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Id) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
      move(id_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4))));
  stmt_nodes.push_back(move(id_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> var_data_type(new LongDataType());
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(
      new GlobalVarDefAnalysis(move(var_data_type)));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  unique_ptr<DataType> id_data_type(new LongDataType());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, IdWithoutDefIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
      move(id_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4))));
  stmt_nodes.push_back(move(id_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new IdWithoutDefError(*id_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Not) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  BoolNode *lit_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> lit_node(lit_node_ptr);
  NotNode *not_node_ptr = new NotNode(
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(0)),
      move(lit_node));
  unique_ptr<ExprNode> not_node(not_node_ptr);
  unique_ptr<StmtNode> not_stmt_node(new ExprStmtNode(
      move(not_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
  stmt_nodes.push_back(move(not_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.bools = {{"false", false}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> not_data_type(new BoolDataType());
  unique_ptr<DataType> not_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> not_expr_analysis(new CommonExprAnalysis(
      move(not_data_type), move(not_casted_data_type), ValueType::kRight));
  node_analyzes.insert(make_pair(not_node_ptr, move(not_expr_analysis)));

  unique_ptr<DataType> lit_data_type(new BoolDataType());
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      move(lit_data_type),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, NotWithUnsupportedDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(0)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  NotNode *not_node_ptr = new NotNode(
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(0)),
      move(id_node));
  unique_ptr<ExprNode> not_node(not_node_ptr);
  unique_ptr<StmtNode> not_stmt_node(new ExprStmtNode(
      move(not_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
  stmt_nodes.push_back(move(not_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  bool is_func_native = false;
  FuncDataType func_data_type(
      unique_ptr<DataType>(new VoidDataType()),
      vector< unique_ptr<DataType> >(),
      is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> id_data_type(func_data_type.Clone());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new UnaryExprWithUnsupportedTypeError(
      *not_node_ptr, func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Negative) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  NegativeNode *negative_node_ptr = new NegativeNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(3), UINT32_C(3)),
      move(id_node));
  unique_ptr<ExprNode> negative_node(negative_node_ptr);
  unique_ptr<StmtNode> negative_stmt_node(new ExprStmtNode(
      move(negative_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(negative_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType())));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  bool is_id_assignee = false;
  unique_ptr<DataType> id_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
  unique_ptr<DataType> negative_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> negative_expr_analysis(
      new CommonExprAnalysis(unique_ptr<DataType>(new IntDataType()),
                             move(negative_expr_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(
      make_pair(negative_node_ptr, move(negative_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, NegativeWithUnsupportedDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(0)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  NegativeNode *negative_node_ptr = new NegativeNode(
      TokenInfo(Token::kSubOp, "-", UINT32_C(0), UINT32_C(0)),
      move(id_node));
  unique_ptr<ExprNode> negative_node(negative_node_ptr);
  unique_ptr<StmtNode> negative_stmt_node(new ExprStmtNode(
      move(negative_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
  stmt_nodes.push_back(move(negative_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  bool is_func_native = false;
  FuncDataType func_data_type(
      unique_ptr<DataType>(new VoidDataType()),
      vector< unique_ptr<DataType> >(),
      is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> id_data_type(func_data_type.Clone());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new UnaryExprWithUnsupportedTypeError(
      *negative_node_ptr, func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, PreInc) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  PreIncNode *pre_inc_node_ptr = new PreIncNode(
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(3), UINT32_C(3)),
      move(id_node));
  unique_ptr<ExprNode> pre_inc_node(pre_inc_node_ptr);
  unique_ptr<StmtNode> pre_inc_stmt_node(new ExprStmtNode(
      move(pre_inc_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(pre_inc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType())));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  bool is_id_assignee = false;
  unique_ptr<DataType> id_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
  unique_ptr<DataType> pre_inc_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> pre_inc_expr_analysis(
      new CommonExprAnalysis(unique_ptr<DataType>(new IntDataType()),
                             move(pre_inc_expr_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(
      make_pair(pre_inc_node_ptr, move(pre_inc_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, PreIncWithUnsupportedDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(0)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  PreIncNode *pre_inc_node_ptr = new PreIncNode(
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(0), UINT32_C(0)),
      move(id_node));
  unique_ptr<ExprNode> pre_inc_node(pre_inc_node_ptr);
  unique_ptr<StmtNode> pre_inc_stmt_node(new ExprStmtNode(
      move(pre_inc_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
  stmt_nodes.push_back(move(pre_inc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  bool is_func_native = false;
  FuncDataType func_data_type(
      unique_ptr<DataType>(new VoidDataType()),
      vector< unique_ptr<DataType> >(),
      is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> id_data_type(func_data_type.Clone());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new UnaryExprWithUnsupportedTypeError(
      *pre_inc_node_ptr, func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, PreDec) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));
  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(4), UINT32_C(4)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  PreDecNode *pre_dec_node_ptr = new PreDecNode(
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(3), UINT32_C(3)),
      move(id_node));
  unique_ptr<ExprNode> pre_dec_node(pre_dec_node_ptr);
  unique_ptr<StmtNode> pre_dec_stmt_node(new ExprStmtNode(
      move(pre_dec_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(pre_dec_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<NodeSemanticAnalysis> var_def_analysis(new GlobalVarDefAnalysis(
      unique_ptr<DataType>(new IntDataType())));
  node_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  bool is_id_assignee = false;
  unique_ptr<DataType> id_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(id_casted_data_type),
      ValueType::kLeft,
      var_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));
  unique_ptr<DataType> pre_dec_expr_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> pre_dec_expr_analysis(
      new CommonExprAnalysis(unique_ptr<DataType>(new IntDataType()),
                             move(pre_dec_expr_casted_data_type),
                             ValueType::kRight));
  node_analyzes.insert(
      make_pair(pre_dec_node_ptr, move(pre_dec_expr_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, PreDecWithUnsupportedDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  ScopeNode *body_node_ptr = new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ScopeNode> body_node(body_node_ptr);
  unique_ptr<DataTypeNode> return_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefWithBodyNode *func_def_node_ptr = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(0)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  PreDecNode *pre_dec_node_ptr = new PreDecNode(
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(0), UINT32_C(0)),
      move(id_node));
  unique_ptr<ExprNode> pre_dec_node(pre_dec_node_ptr);
  unique_ptr<StmtNode> pre_dec_stmt_node(new ExprStmtNode(
      move(pre_dec_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))));
  stmt_nodes.push_back(move(pre_dec_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  vector<const VarDefNode*> body_local_var_defs;
  unique_ptr<NodeSemanticAnalysis> body_analysis(
      new ScopeAnalysis(body_local_var_defs));
  node_analyzes.insert(make_pair(body_node_ptr, move(body_analysis)));

  bool is_func_native = false;
  FuncDataType func_data_type(
      unique_ptr<DataType>(new VoidDataType()),
      vector< unique_ptr<DataType> >(),
      is_func_native);
  bool is_func_has_return = false;
  unique_ptr<NodeSemanticAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      is_func_has_return));
  node_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  unique_ptr<DataType> id_data_type(func_data_type.Clone());
  unique_ptr<DataType> id_casted_data_type;
  bool is_id_assignee = false;
  unique_ptr<NodeSemanticAnalysis> id_analysis(new IdAnalysis(
      move(id_data_type),
      move(id_casted_data_type),
      ValueType::kRight,
      func_def_node_ptr,
      is_id_assignee));
  node_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new UnaryExprWithUnsupportedTypeError(
      *pre_dec_node_ptr, func_data_type.Clone()));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, String) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  StringNode *string_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"swag\"", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> string_node(string_node_ptr);
  unique_ptr<StmtNode> string_stmt_node(new ExprStmtNode(
      move(string_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(string_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"swag\"", "swag"}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> string_lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> string_lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new StringDataType()),
      move(string_lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new StringLit("swag"))));
  node_analyzes.insert(make_pair(string_node_ptr, move(string_lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, StringWithEmptyHexValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::EmptyHexValueError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  StringNode *string_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"\\x\"", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> string_node(string_node_ptr);
  unique_ptr<StmtNode> string_stmt_node(new ExprStmtNode(
      move(string_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(string_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(
      new StringWithEmptyHexValueError(*string_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"\"\\x\"", LitParser::EmptyHexValueError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, StringWithOutOfRangeHexValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::OutOfRangeError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  StringNode *string_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"\\xFFF\"", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> string_node(string_node_ptr);
  unique_ptr<StmtNode> string_stmt_node(new ExprStmtNode(
      move(string_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(string_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new StringWithOutOfRangeHexValueError(
      *string_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"\"\\xFFF\"", LitParser::OutOfRangeError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, Char) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *char_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> char_node(char_node_ptr);
  unique_ptr<StmtNode> char_stmt_node(new ExprStmtNode(
      move(char_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(char_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.chars = {{"'a'", 'a'}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new CharDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new CharLit('a'))));
  node_analyzes.insert(make_pair(char_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, CharWithEmptyHexValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::EmptyHexValueError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *char_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'\\x'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> char_node(char_node_ptr);
  unique_ptr<StmtNode> char_stmt_node(new ExprStmtNode(
      move(char_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(char_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(
      new CharWithEmptyHexValueError(*char_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"'\\x'", LitParser::EmptyHexValueError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseChar(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, CharWithOutOfRangeHexValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::OutOfRangeError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *char_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'\\xFFF'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> char_node(char_node_ptr);
  unique_ptr<StmtNode> char_stmt_node(new ExprStmtNode(
      move(char_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(char_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new CharWithOutOfRangeHexValueError(
      *char_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"'\\xFFF'", LitParser::OutOfRangeError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseChar(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, CharWithMultipleCharsIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::MultipleCharsError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  CharNode *char_node_ptr = new CharNode(
      TokenInfo(Token::kCharLit, "'ab'", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> char_node(char_node_ptr);
  unique_ptr<StmtNode> char_stmt_node(new ExprStmtNode(
      move(char_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(char_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new CharWithMultipleCharsError(
      *char_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"'ab'", LitParser::MultipleCharsError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseChar(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, Int) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *int_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(int_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new IntDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new IntLit(INT32_C(7)))));
  node_analyzes.insert(make_pair(int_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, IntWithOutOfRangeValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::OutOfRangeError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *int_node_ptr = new IntNode(TokenInfo(
      Token::kIntLit, "18446744073709551616", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> int_node(int_node_ptr);
  unique_ptr<StmtNode> int_stmt_node(new ExprStmtNode(
      move(int_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(int_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new IntWithOutOfRangeValueError(
      *int_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"18446744073709551616", LitParser::OutOfRangeError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseInt(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, Long) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  LongNode *long_node_ptr = new LongNode(
      TokenInfo(Token::kLongLit, "7L", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> long_node(long_node_ptr);
  unique_ptr<StmtNode> long_stmt_node(new ExprStmtNode(
      move(long_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(long_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.longs = {{"7L", INT64_C(7)}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new LongDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new LongLit(INT64_C(7)))));
  node_analyzes.insert(make_pair(long_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, LongWithOutOfRangeValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::OutOfRangeError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  LongNode *long_node_ptr = new LongNode(TokenInfo(
      Token::kLongLit, "918446744073709551616L", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> long_node(long_node_ptr);
  unique_ptr<StmtNode> long_stmt_node(new ExprStmtNode(
      move(long_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(long_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new LongWithOutOfRangeValueError(
      *long_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{"918446744073709551616L", LitParser::OutOfRangeError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseLong(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, Double) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  DoubleNode *double_node_ptr = new DoubleNode(
      TokenInfo(Token::kDoubleLit, "7.7", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> double_node(double_node_ptr);
  unique_ptr<StmtNode> double_stmt_node(new ExprStmtNode(
      move(double_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(double_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.doubles = {{"7.7", 7.7}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new DoubleDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new DoubleLit(7.7))));
  node_analyzes.insert(make_pair(double_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, DoubleWithOutOfRangeValueIsInvalid) {
  struct FailingTestLitParse {
    string str;
    LitParser::OutOfRangeError error;
  };

  vector< unique_ptr<StmtNode> > stmt_nodes;
  const string &double_str = "918446744073709551616" + to_string(1.79769e+308);
  DoubleNode *double_node_ptr = new DoubleNode(TokenInfo(
      Token::kDoubleLit, double_str, UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> double_node(double_node_ptr);
  unique_ptr<StmtNode> double_stmt_node(new ExprStmtNode(
      move(double_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(double_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new DoubleWithOutOfRangeValueError(
      *double_node_ptr));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis expected_analysis(move(problems), move(node_analyzes));

  vector<FailingTestLitParse> test_lit_parses =
      {{double_str, LitParser::OutOfRangeError("test")}};
  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const FailingTestLitParse &test_lit_parse: test_lit_parses) {
    EXPECT_CALL(*lit_parser_mock, ParseDouble(test_lit_parse.str))
        .Times(1)
        .WillOnce(Throw(test_lit_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(*lit_parser);
  unique_ptr<SemanticAnalysis> actual_analysis = analyzer.Analyze(
      *program_node, SemanticAnalyzer::ImportPrograms());
  ASSERT_EQ(expected_analysis, *actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, Bool) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  BoolNode *bool_node_ptr = new BoolNode(
      TokenInfo(Token::kBoolFalseLit, "false", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> bool_node(bool_node_ptr);
  unique_ptr<StmtNode> bool_stmt_node(new ExprStmtNode(
      move(bool_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))));
  stmt_nodes.push_back(move(bool_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.bools = {{"false", false}};

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  unique_ptr<DataType> lit_casted_data_type;
  unique_ptr<NodeSemanticAnalysis> lit_analysis(new LitAnalysis(
      unique_ptr<DataType>(new BoolDataType()),
      move(lit_casted_data_type),
      ValueType::kRight,
      unique_ptr<Lit>(new BoolLit(false))));
  node_analyzes.insert(make_pair(bool_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ProgramProblems problems;
  problems.insert(
      make_pair(program_node.get(), SemanticAnalysis::Problems()));
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ArrayDataTypeWithTooManyDimensionsIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> element_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> array_data_type_node(new ArrayDataTypeNode(
      move(element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  size_t max_dimensions_count = numeric_limits<uint8_t>::max();

  for (uint32_t i = UINT32_C(0); i != max_dimensions_count; ++i) {
    array_data_type_node.reset(new ArrayDataTypeNode(
        move(array_data_type_node),
        TokenInfo(Token::kSubscriptStart, "[", i + 3, i + 3),
        TokenInfo(Token::kSubscriptEnd, "]", i + 4, i + 4)));
  }

  ArrayDataTypeNode *array_data_type_node_ptr =
      static_cast<ArrayDataTypeNode*>(array_data_type_node.get());
  VarDefWithoutInitNode *def_node_ptr = new VarDefWithoutInitNode(
      move(array_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1000), UINT32_C(1000)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1001), UINT32_C(1001)));
  unique_ptr<StmtNode> def_node(def_node_ptr);
  stmt_nodes.push_back(move(def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ProgramProblems problems;
  unique_ptr<SemanticProblem> problem(new ArrayTypeWithTooManyDimensionsError(
      *array_data_type_node_ptr, max_dimensions_count));
  problems[program_node.get()].push_back(move(problem));

  SemanticAnalysis::NodeAnalyzes node_analyzes;
  SemanticAnalysis analysis(move(problems), move(node_analyzes));
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              SemanticAnalyzer::ImportPrograms(),
                              move(analysis),
                              test_lit_parses};
  TestAnalyze(test_program);
}
}
}
