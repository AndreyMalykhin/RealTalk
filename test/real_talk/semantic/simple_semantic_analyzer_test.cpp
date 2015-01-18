
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <iterator>
#include "real_talk/util/errors.h"
#include "real_talk/lexer/token_info.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/parser/parser.h"
#include "real_talk/parser/file_parser.h"
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
#include "real_talk/semantic/var_def_analysis.h"
#include "real_talk/semantic/func_def_analysis.h"
#include "real_talk/semantic/arg_def_analysis.h"
#include "real_talk/semantic/semantic_analysis.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"
#include "real_talk/semantic/import_file_searcher.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/semantic_problems.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::shared_ptr;
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
using real_talk::lexer::TokenInfo;
using real_talk::lexer::UnexpectedCharError;
using real_talk::lexer::Token;
using real_talk::parser::ProgramNode;
using real_talk::parser::StmtNode;
using real_talk::parser::ExprNode;
using real_talk::parser::DefNode;
using real_talk::parser::ArgDefNode;
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
using real_talk::parser::FileParser;
using real_talk::parser::Parser;
using real_talk::parser::PrimitiveDataTypeNode;
using real_talk::parser::BoundedArraySizeNode;
using real_talk::parser::UnboundedArraySizeNode;
using real_talk::parser::CallNode;
using real_talk::parser::LessNode;
using real_talk::parser::UnexpectedTokenError;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::BreakNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::NotNode;
using real_talk::util::FileNotFoundError;
using real_talk::util::IOError;

namespace boost {
namespace filesystem {

void PrintTo(const path &p, ostream *stream);
void PrintTo(const path &p, ostream *stream) {
  *stream << p;
}
}
}

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

struct TestImportFileSearch {
  path relative_file_path;
  path absolute_file_path;
};

struct TestFileParse {
  path absolute_file_path;
  shared_ptr<ProgramNode> node;
};

struct TestProgram {
  shared_ptr<ProgramNode> node;
  path file_path;
  SemanticAnalysis analysis;
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
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

class FileParserMock: public FileParser {
 public:
  MOCK_CONST_METHOD1(Parse, shared_ptr<ProgramNode>(const path&));
};

class ImportFileSearcherMock: public ImportFileSearcher {
 public:
  MOCK_CONST_METHOD1(Search, path(const path&));
};

class SimpleSemanticAnalyzerTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

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
    ImportFileSearcherMock *import_file_searcher_mock =
        new ImportFileSearcherMock();
    unique_ptr<ImportFileSearcher> import_file_searcher(
        import_file_searcher_mock);

    for (const TestImportFileSearch &test_import_file_search:
             test_program.test_import_file_searches) {
      EXPECT_CALL(*import_file_searcher_mock,
                  Search(test_import_file_search.relative_file_path))
          .Times(1)
          .WillOnce(Return(test_import_file_search.absolute_file_path))
          .RetiresOnSaturation();
    }

    FileParserMock *file_parser_mock = new FileParserMock();
    unique_ptr<FileParser> file_parser(file_parser_mock);

    for (const TestFileParse &test_file_parse:
             test_program.test_file_parses) {
      EXPECT_CALL(*file_parser_mock,
                  Parse(test_file_parse.absolute_file_path))
          .Times(1)
          .WillOnce(Return(test_file_parse.node))
          .RetiresOnSaturation();
    }

    unique_ptr<LitParser> lit_parser =
        CreateLitParserMock(test_program.test_lit_parses);
    SimpleSemanticAnalyzer analyzer(test_program.node,
                                    test_program.file_path,
                                    *file_parser,
                                    *import_file_searcher,
                                    *lit_parser);
    SemanticAnalysis actual_analysis = analyzer.Analyze();
    ASSERT_EQ(test_program.analysis, actual_analysis);
  }
};

TEST_F(SimpleSemanticAnalyzerTest, VarDefWithoutInit) {
  struct TestData {
    unique_ptr<DataTypeNode> data_type_node;
    unique_ptr<DataType> data_type;
    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    TestLitParses test_lit_parses;
  };

  vector<TestData> test_data_suits;

  {
    unique_ptr<DataTypeNode> data_type_node(new IntDataTypeNode(
        TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new IntDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}, {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new LongDataTypeNode(
        TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new LongDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}, {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new DoubleDataTypeNode(
        TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new DoubleDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}, {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new BoolDataTypeNode(
        TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new BoolDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}, {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new CharDataTypeNode(
        TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new CharDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}, {}, {}});
  }

  {
    unique_ptr<DataTypeNode> data_type_node(new StringDataTypeNode(
        TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))));
    unique_ptr<DataType> data_type(new StringDataType());
    test_data_suits.push_back(
        {move(data_type_node), move(data_type), {}, {}, {}});
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
        {move(data_type_node), move(data_type), {}, {}, {}});
  }

  for (TestData &test_data: test_data_suits) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    VarDefWithoutInitNode *def_node_ptr = new VarDefWithoutInitNode(
        move(test_data.data_type_node),
        TokenInfo(Token::kName, "var", UINT32_C(4), UINT32_C(4)),
        TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
    unique_ptr<StmtNode> def_node(def_node_ptr);
    stmt_nodes.push_back(move(def_node));
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DefAnalysis> def_analysis(new VarDefAnalysis(
        test_data.data_type->Clone(), DataStorage::kGlobal));
    def_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(problems),
                              move(def_analyzes),
                              move(test_data.expr_analyzes),
                              move(test_data.lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    path program_file_path;
    TestProgram test_program = {program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_data.test_lit_parses};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithoutInitUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    shared_ptr<ProgramNode> program_node;
    SemanticAnalysis::DefAnalyzes def_analyzes;
    vector< unique_ptr<SemanticProblem> > problems;
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
    unique_ptr<ScopeNode> func_body_node(new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
        move(func_body_stmt_nodes),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10))));

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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type), false));
    def_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DataType> arg_data_type(new IntDataType());
    unique_ptr<DefAnalysis> arg_def_analysis(new ArgDefAnalysis(
        move(arg_data_type)));
    def_analyzes.insert(
        make_pair(arg_def_node_ptr, move(arg_def_analysis)));
    unique_ptr<DataType> var_data_type(new IntDataType());
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        move(var_data_type), DataStorage::kLocal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr));
    problems.push_back(move(problem));

    TestData test_data =
        {program_node, move(def_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    unique_ptr<ScopeNode> body_node(new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
        vector< unique_ptr<StmtNode> >(),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type), false));
    def_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DataType> var_data_type(new IntDataType());
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        move(var_data_type), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr));
    problems.push_back(move(problem));

    TestData test_data =
        {program_node, move(def_analyzes), move(problems)};
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> var_data_type1(new IntDataType());
    unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
        move(var_data_type1), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr1, move(var_def_analysis1)));
    unique_ptr<DataType> var_data_type2(new IntDataType());
    unique_ptr<DefAnalysis> var_def_analysis2(new VarDefAnalysis(
        move(var_data_type2), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr2, move(var_def_analysis2)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr2));
    problems.push_back(move(problem));

    TestData test_data =
        {program_node, move(def_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(test_data.problems),
                              move(test_data.def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    TestLitParses test_lit_parses = {};
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    path program_file_path;
    TestProgram test_program = {test_data.program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        test_data.var_data_type->Clone(), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
        file_path, *var_def_node_ptr, test_data.var_data_type->Clone()));
    problems.push_back(move(problem));

    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(problems),
                              move(def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    TestLitParses test_lit_parses = {};
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    path program_file_path;
    TestProgram test_program = {program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_lit_parses};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, VarDefWithInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  IntNode *value_node = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3)));
  VarDefWithInitNode *def_node_ptr = new VarDefWithInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
      unique_ptr<ExprNode>(value_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> def_node(def_node_ptr);
  stmt_nodes.push_back(move(def_node));
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *var_data_type_ptr = new IntDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  unique_ptr<DefAnalysis> def_analysis(new VarDefAnalysis(
      move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *value_data_type_ptr = new IntDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis expr_analysis(move(value_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(value_node, move(expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  LitAnalysis lit_analysis(unique_ptr<Lit>(new IntLit(INT32_C(7))));
  lit_analyzes.insert(make_pair(value_node, move(lit_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"7\"", "7"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  StringDataType *value_data_type_ptr = new StringDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis value_expr_analysis(move(value_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(value_node_ptr, move(value_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> value_lit(new StringLit("7"));
  LitAnalysis lit_analysis(move(move(value_lit)));
  lit_analyzes.insert(make_pair(value_node_ptr, move(lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *var_data_type_ptr = new IntDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new VarDefWithIncompatibleValueTypeError(
      program_file_path,
      *var_def_node_ptr,
      var_data_type_ptr->Clone(),
      value_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        test_data.data_type->Clone(), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path program_file_path;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
        program_file_path, *var_def_node_ptr, test_data.data_type->Clone()));
    problems.push_back(move(problem));

    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(problems),
                              move(def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_lit_parses};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    shared_ptr<ProgramNode> program_node;
    SemanticAnalysis::DefAnalyzes def_analyzes;
    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    vector< unique_ptr<SemanticProblem> > problems;
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    IntDataType *var_data_type_ptr1 = new IntDataType();
    unique_ptr<DataType> var_data_type1(var_data_type_ptr1);
    unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
        move(var_data_type1), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr1, move(var_def_analysis1)));
    IntDataType *var_data_type_ptr2 = new IntDataType();
    unique_ptr<DataType> var_data_type2(var_data_type_ptr2);
    unique_ptr<DefAnalysis> var_def_analysis2(new VarDefAnalysis(
        move(var_data_type2), DataStorage::kGlobal));
    def_analyzes.insert(
        make_pair(var_def_node_ptr2, move(var_def_analysis2)));

    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    IntDataType *var_value_data_type_ptr1 = new IntDataType();
    unique_ptr<DataType> var_value_data_type1(var_value_data_type_ptr1);
    ExprAnalysis var_value_expr_analysis1(
        move(var_value_data_type1), ValueType::kRight);
    expr_analyzes.insert(
        make_pair(var_value_node_ptr1, move(var_value_expr_analysis1)));

    SemanticAnalysis::LitAnalyzes lit_analyzes;
    unique_ptr<Lit> var_value_lit1(new IntLit(INT32_C(1)));
    LitAnalysis var_value_lit_analysis1(move(var_value_lit1));
    lit_analyzes.insert(
        make_pair(var_value_node_ptr1, move(var_value_lit_analysis1)));
    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr2));
    problems.push_back(move(problem));

    TestLitParses test_lit_parses = {};
    test_lit_parses.ints = {{"1", INT32_C(1)}};

    TestData test_data = {program_node,
                          move(def_analyzes),
                          move(expr_analyzes),
                          move(lit_analyzes),
                          move(problems),
                          test_lit_parses};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(test_data.problems),
                              move(test_data.def_analyzes),
                              move(test_data.expr_analyzes),
                              move(test_data.lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    path program_file_path;
    TestProgram test_program = {test_data.program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_data.test_lit_parses};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithBodyAndArgsAndReturnValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<ArgDefNode> > arg_def_nodes;
  unique_ptr<DataTypeNode> arg_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
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
  unique_ptr<StmtNode> return_stmt_node(new ReturnValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(7), UINT32_C(7)),
      move(return_expr_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
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
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      move(arg_def_nodes),
      arg_separator_tokens,
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(5)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new IntDataType());
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *return_expr_data_type_ptr = new IntDataType();
  unique_ptr<DataType> return_expr_data_type(return_expr_data_type_ptr);
  ExprAnalysis return_expr_analysis(
      move(return_expr_data_type), ValueType::kLeft);
  expr_analyzes.insert(
      make_pair(return_expr_node_ptr, move(return_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(arg_def_node_ptr);
  id_analyzes.insert(make_pair(return_expr_node_ptr, id_analysis));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithBodyWithoutArgsAndReturnValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), true));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new FuncDefWithBodyIsNativeError(
      program_file_path, *func_def_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    shared_ptr<ProgramNode> program_node;
    SemanticAnalysis::DefAnalyzes def_analyzes;
    vector< unique_ptr<SemanticProblem> > problems;
  };
  vector<TestData> test_data_suits;

  {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    unique_ptr<ScopeNode> body_node1(new ScopeNode(
        TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
        vector< unique_ptr<StmtNode> >(),
        TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type1(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types1;
    unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
        move(return_data_type1), move(arg_data_types1)));
    unique_ptr<DefAnalysis> func_def_analysis1(new FuncDefAnalysis(
        move(func_data_type1), false));
    def_analyzes.insert(
        make_pair(func_def_node_ptr1, move(func_def_analysis1)));
    unique_ptr<DataType> return_data_type2(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types2;
    unique_ptr<FuncDataType> func_data_type2(new FuncDataType(
        move(return_data_type2), move(arg_data_types2)));
    unique_ptr<DefAnalysis> func_def_analysis2(new FuncDefAnalysis(
        move(func_data_type2), false));
    def_analyzes.insert(
        make_pair(func_def_node_ptr2, move(func_def_analysis2)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *func_def_node_ptr2));
    problems.push_back(move(problem));

    TestData test_data =
        {program_node, move(def_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(test_data.problems),
                              move(test_data.def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    TestLitParses test_lit_parses = {};
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    path program_file_path;
    TestProgram test_program = {test_data.program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> array_element_data_type(new VoidDataType());
  ArrayDataType *return_data_type_ptr =
      new ArrayDataType(move(array_element_data_type));
  unique_ptr<DataType> return_data_type(return_data_type_ptr);
  vector< unique_ptr<DataType> > arg_data_types;
  FuncDataType *func_data_type_ptr =
      new FuncDataType(move(return_data_type), move(arg_data_types));
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
      program_file_path, *func_def_node_ptr, return_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type1(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types1;
  unique_ptr<FuncDataType> func_data_type1(
      new FuncDataType(move(return_data_type1), move(arg_data_types1)));
  unique_ptr<DefAnalysis> func_def_analysis1(
      new FuncDefAnalysis(move(func_data_type1), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr1, move(func_def_analysis1)));
  unique_ptr<DataType> return_data_type2(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types2;
  unique_ptr<FuncDataType> func_data_type2(
      new FuncDataType(move(return_data_type2), move(arg_data_types2)));
  unique_ptr<DefAnalysis> func_def_analysis2(
      new FuncDefAnalysis(move(func_data_type2), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr2, move(func_def_analysis2)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new FuncDefWithinNonGlobalScope(
      file_path, *func_def_node_ptr2));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithBodyUsingNonVoidDataTypeWithoutReturnValueIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));

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
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new IntDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new FuncDefWithoutReturnValueError(
      file_path, *func_def_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), true));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new IntDataType());
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  VoidDataType *func_return_data_type_ptr = new VoidDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), true));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new FuncDefWithoutBodyNotNativeError(
      program_file_path, *func_def_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithoutBodyUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    shared_ptr<ProgramNode> program_node;
    SemanticAnalysis::DefAnalyzes def_analyzes;
    vector< unique_ptr<SemanticProblem> > problems;
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type1(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types1;
    unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
        move(return_data_type1), move(arg_data_types1)));
    unique_ptr<DefAnalysis> func_def_analysis1(new FuncDefAnalysis(
        move(func_data_type1), true));
    def_analyzes.insert(
        make_pair(func_def_node_ptr1, move(func_def_analysis1)));
    unique_ptr<DataType> return_data_type2(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types2;
    unique_ptr<FuncDataType> func_data_type2(new FuncDataType(
        move(return_data_type2), move(arg_data_types2)));
    unique_ptr<DefAnalysis> func_def_analysis2(new FuncDefAnalysis(
        move(func_data_type2), true));
    def_analyzes.insert(
        make_pair(func_def_node_ptr2, move(func_def_analysis2)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *func_def_node_ptr2));
    problems.push_back(move(problem));

    TestData test_data =
        {program_node, move(def_analyzes), move(problems)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(test_data.problems),
                              move(test_data.def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    TestLitParses test_lit_parses = {};
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    path program_file_path;
    TestProgram test_program = {test_data.program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> array_element_data_type(new VoidDataType());
  ArrayDataType *return_data_type_ptr =
      new ArrayDataType(move(array_element_data_type));
  unique_ptr<DataType> return_data_type(return_data_type_ptr);
  vector< unique_ptr<DataType> > arg_data_types;
  FuncDataType *func_data_type_ptr =
      new FuncDataType(move(return_data_type), move(arg_data_types));
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), true));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
      program_file_path, *func_def_node_ptr, return_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type1(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types1;
  unique_ptr<FuncDataType> func_data_type1(
      new FuncDataType(move(return_data_type1), move(arg_data_types1)));
  unique_ptr<DefAnalysis> func_def_analysis1(
      new FuncDefAnalysis(move(func_data_type1), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr1, move(func_def_analysis1)));
  unique_ptr<DataType> return_data_type2(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types2;
  unique_ptr<FuncDataType> func_data_type2(
      new FuncDataType(move(return_data_type2), move(arg_data_types2)));
  unique_ptr<DefAnalysis> func_def_analysis2(
      new FuncDefAnalysis(move(func_data_type2), true));
  def_analyzes.insert(
      make_pair(func_def_node_ptr2, move(func_def_analysis2)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new FuncDefWithinNonGlobalScope(
      file_path, *func_def_node_ptr2));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    VoidDataType *func_return_data_type_ptr = new VoidDataType();
    unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
    vector< unique_ptr<DataType> > func_arg_data_types;
    func_arg_data_types.push_back(test_data.arg_data_type->Clone());
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type), move(func_arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), false));
    def_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DefAnalysis> arg_def_analysis(
        new ArgDefAnalysis(test_data.arg_data_type->Clone()));
    def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path program_file_path;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedTypeError(
        program_file_path,
        *arg_def_node_ptr,
        test_data.arg_data_type->Clone()));
    problems.push_back(move(problem));

    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::IdAnalyzes id_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(problems),
                              move(def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    TestLitParses test_lit_parses = {};
    TestProgram test_program = {program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new StringDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new StringDataType());
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  StringDataType *return_expr_data_type_ptr = new StringDataType();
  unique_ptr<DataType> return_expr_data_type(return_expr_data_type_ptr);
  ExprAnalysis return_expr_analysis(
      move(return_expr_data_type), ValueType::kLeft);
  expr_analyzes.insert(
      make_pair(return_expr_node_ptr, move(return_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(arg_def_node_ptr);
  id_analyzes.insert(make_pair(return_expr_node_ptr, id_analysis));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new ReturnWithIncompatibleTypeError(
      program_file_path,
      *return_stmt_node_ptr,
      func_return_data_type_ptr->Clone(),
      return_expr_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new ReturnNotWithinFuncError(
      file_path, *return_stmt_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ReturnWithoutValue) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<StmtNode> return_stmt_node(new ReturnWithoutValueNode(
      TokenInfo(Token::kReturn, "return", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))));
  body_stmt_nodes.push_back(move(return_stmt_node));

  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  VoidDataType *func_return_data_type_ptr = new VoidDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new ReturnNotWithinFuncError(
      file_path, *return_stmt_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new ReturnWithoutValueError(
      file_path, *return_stmt_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

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
  IntNode *call_arg_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(10), UINT32_C(10)));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  IntDataType *arg_data_type_ptr = new IntDataType();
  unique_ptr<DataType> arg_data_type(arg_data_type_ptr);
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<DataType> func_arg_data_type(new IntDataType());
  func_arg_data_types.push_back(move(func_arg_data_type));
  unique_ptr<DataType> id_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  ExprAnalysis call_expr_analysis(move(call_expr_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));
  IntDataType *call_arg_data_type_ptr = new IntDataType();
  unique_ptr<DataType> call_arg_data_type(call_arg_data_type_ptr);
  ExprAnalysis call_arg_expr_analysis(
      move(call_arg_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> call_arg_lit(new IntLit(INT32_C(7)));
  LitAnalysis call_arg_lit_analysis(move(call_arg_lit));
  lit_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_lit_analysis)));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.longs = {{"7L", INT64_C(7)}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  IntDataType *arg_def_data_type_ptr = new IntDataType();
  unique_ptr<DataType> arg_def_data_type(arg_def_data_type_ptr);
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_def_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<DataType> func_arg_data_type(new IntDataType());
  func_arg_data_types.push_back(move(func_arg_data_type));
  unique_ptr<DataType> id_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  ExprAnalysis call_expr_analysis(move(call_expr_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));
  LongDataType *call_arg_data_type_ptr = new LongDataType();
  unique_ptr<DataType> call_arg_data_type(call_arg_data_type_ptr);
  ExprAnalysis call_arg_expr_analysis(
      move(call_arg_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> call_arg_lit(new LongLit(INT64_C(7)));
  LitAnalysis call_arg_lit_analysis(move(call_arg_lit));
  lit_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  size_t bad_arg_index = 0;
  unique_ptr<SemanticProblem> problem(new CallWithIncompatibleArgTypeError(
      program_file_path,
      *call_expr_node_ptr,
      bad_arg_index,
      arg_def_data_type_ptr->Clone(),
      call_arg_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type), false));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new IntDataType());
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  unique_ptr<DataType> func_arg_data_type(new IntDataType());
  func_arg_data_types.push_back(move(func_arg_data_type));
  unique_ptr<DataType> id_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  ExprAnalysis call_expr_analysis(
      move(call_expr_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  size_t expected_args_count = 1;
  size_t actual_args_count = 2;
  unique_ptr<SemanticProblem> problem(new CallWithInvalidArgsCount(
      program_file_path,
      *call_expr_node_ptr,
      expected_args_count,
      actual_args_count));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> id_data_type(new IntDataType());
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(
      new CallWithNonFuncError(program_file_path, *call_expr_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<ScopeNode> if_body1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));

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
  unique_ptr<ScopeNode> if_body2(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(17), UINT32_C(17)),
      move(body_stmt_nodes2),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_data_type2(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis2(
      new VarDefAnalysis(move(var_data_type2), DataStorage::kLocal));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> var_data_type3(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis3(
      new VarDefAnalysis(move(var_data_type3), DataStorage::kLocal));
  def_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *if_cond_data_type_ptr1 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type1(if_cond_data_type_ptr1);
  ExprAnalysis if_cond_expr_analysis1(
      move(if_cond_data_type1), ValueType::kLeft);
  expr_analyzes.insert(make_pair(if_cond_ptr1, move(if_cond_expr_analysis1)));
  BoolDataType *if_cond_data_type_ptr2 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type2(if_cond_data_type_ptr2);
  ExprAnalysis if_cond_expr_analysis2(
      move(if_cond_data_type2), ValueType::kLeft);
  expr_analyzes.insert(make_pair(if_cond_ptr2, move(if_cond_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr1, id_analysis1));
  IdAnalysis id_analysis2(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr2, id_analysis2));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfUsingIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> func_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type), move(func_arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), false));
    def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types));
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(func_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      program_file_path,
      *if_else_if_node_ptr,
      *if_node_ptr,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfUsingElseIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> func_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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

  unique_ptr<ScopeNode> if_body_node1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(14), UINT32_C(14))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type), move(func_arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), false));
    def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  {
    unique_ptr<DataType> var_data_type(new BoolDataType());
    unique_ptr<DefAnalysis> var_def_analysis(
        new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
    def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  }

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *var_data_type_ptr = new BoolDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  ExprAnalysis id_expr_analysis1(move(var_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types));
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  ExprAnalysis id_expr_analysis2(move(func_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr1, id_analysis1));
  IdAnalysis id_analysis2(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr2, id_analysis2));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      program_file_path,
      *if_else_if_node_ptr,
      *if_node_ptr2,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<ScopeNode> if_body1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));

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
  unique_ptr<ScopeNode> if_body2(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(17), UINT32_C(17)),
      move(body_stmt_nodes2),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(21), UINT32_C(21))));

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
  unique_ptr<ScopeNode> else_body(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(23), UINT32_C(23)),
      move(body_stmt_nodes3),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(27), UINT32_C(27))));

  unique_ptr<StmtNode> if_else_if_node(new IfElseIfElseNode(
      move(if_node1),
      move(else_if_nodes),
      TokenInfo(Token::kElse, "else", UINT32_C(22), UINT32_C(22)),
      move(else_body)));
  stmt_nodes.push_back(move(if_else_if_node));
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_data_type2(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis2(
      new VarDefAnalysis(move(var_data_type2), DataStorage::kLocal));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> var_data_type3(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis3(
      new VarDefAnalysis(move(var_data_type3), DataStorage::kLocal));
  def_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  unique_ptr<DataType> var_data_type4(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis4(
      new VarDefAnalysis(move(var_data_type4), DataStorage::kLocal));
  def_analyzes.insert(make_pair(var_def_node_ptr4, move(var_def_analysis4)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *if_cond_data_type_ptr1 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type1(if_cond_data_type_ptr1);
  ExprAnalysis if_cond_expr_analysis1(
      move(if_cond_data_type1), ValueType::kLeft);
  expr_analyzes.insert(make_pair(if_cond_ptr1, move(if_cond_expr_analysis1)));
  BoolDataType *if_cond_data_type_ptr2 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type2(if_cond_data_type_ptr2);
  ExprAnalysis if_cond_expr_analysis2(
      move(if_cond_data_type2), ValueType::kLeft);
  expr_analyzes.insert(make_pair(if_cond_ptr2, move(if_cond_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr1, id_analysis1));
  IdAnalysis id_analysis2(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr2, id_analysis2));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfElseUsingIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> func_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type), move(func_arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), false));
    def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types));
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(func_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      program_file_path,
      *if_else_if_else_node_ptr,
      *if_node_ptr,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       IfElseIfElseUsingElseIfWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> func_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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

  unique_ptr<ScopeNode> if_body_node1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(13), UINT32_C(13)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(14), UINT32_C(14))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(func_return_data_type), move(func_arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(
        new FuncDefAnalysis(move(func_data_type), false));
    def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  {
    unique_ptr<DataType> var_data_type(new BoolDataType());
    unique_ptr<DefAnalysis> var_def_analysis(
        new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
    def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));
  }

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *var_data_type_ptr = new BoolDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  ExprAnalysis id_expr_analysis1(move(var_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *func_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types));
  unique_ptr<FuncDataType> func_data_type(func_data_type_ptr);
  ExprAnalysis id_expr_analysis2(move(func_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr1, id_analysis1));
  IdAnalysis id_analysis2(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr2, id_analysis2));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new IfWithIncompatibleTypeError(
      program_file_path,
      *if_else_if_else_node_ptr,
      *if_node_ptr2,
      unique_ptr<DataType>(new BoolDataType()),
      func_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Import) {
  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr1 = new StringNode(TokenInfo(
      Token::kStringLit, "\"import.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node1(file_path_node_ptr1);
  ImportNode *import_node_ptr1 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node1),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node1(import_node_ptr1);
  main_program_stmt_nodes.push_back(move(import_node1));

  StringNode *file_path_node_ptr2 = new StringNode(TokenInfo(
      Token::kStringLit, "\"./import.rt\"", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StringNode> file_path_node2(file_path_node_ptr2);
  ImportNode *import_node_ptr2 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_node2(import_node_ptr2);
  main_program_stmt_nodes.push_back(move(import_node2));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  vector< unique_ptr<StmtNode> > import_program_stmt_nodes1;
  StringNode *file_path_node_ptr3 = new StringNode(TokenInfo(
      Token::kStringLit, "\"main.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node3(file_path_node_ptr3);
  ImportNode *import_node_ptr3 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node3),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node3(import_node_ptr3);
  import_program_stmt_nodes1.push_back(move(import_node3));

  StringNode *file_path_node_ptr4 = new StringNode(TokenInfo(
      Token::kStringLit, "\"import2.rt\"", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StringNode> file_path_node4(file_path_node_ptr4);
  ImportNode *import_node_ptr4 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_node4),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_node4(import_node_ptr4);
  import_program_stmt_nodes1.push_back(move(import_node4));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(6), UINT32_C(6))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  import_program_stmt_nodes1.push_back(move(var_def_node1));

  unique_ptr<ExprNode> var_value_node2(new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(12), UINT32_C(12))));
  unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(9), UINT32_C(9))));
  VarDefWithInitNode *var_def_node_ptr2 = new VarDefWithInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(11), UINT32_C(11)),
      move(var_value_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(13), UINT32_C(13)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  import_program_stmt_nodes1.push_back(move(var_def_node2));

  vector< unique_ptr<ArgDefNode> > arg_def_nodes1;
  unique_ptr<DataTypeNode> arg_data_type_node1(new LongDataTypeNode(
      TokenInfo(Token::kLongType, "long", UINT32_C(17), UINT32_C(17))));
  ArgDefNode *arg_def_node_ptr1 = new ArgDefNode(
      move(arg_data_type_node1),
      TokenInfo(Token::kName, "arg1", UINT32_C(18), UINT32_C(18)));
  unique_ptr<ArgDefNode> arg_def_node1(arg_def_node_ptr1);
  arg_def_nodes1.push_back(move(arg_def_node1));

  vector< unique_ptr<StmtNode> > body_stmt_nodes1;
  unique_ptr<ExprNode> lit_expr_node1(new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(21), UINT32_C(21))));
  unique_ptr<StmtNode> lit_stmt_node1(new ExprStmtNode(
      move(lit_expr_node1),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(22), UINT32_C(22))));
  body_stmt_nodes1.push_back(move(lit_stmt_node1));
  unique_ptr<ScopeNode> body_node1(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(20), UINT32_C(20)),
      move(body_stmt_nodes1),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(23), UINT32_C(23))));

  unique_ptr<DataTypeNode> return_data_type_node1(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(14), UINT32_C(14))));
  FuncDefWithBodyNode *func_def_node_ptr1 = new FuncDefWithBodyNode(
      vector<TokenInfo>(),
      move(return_data_type_node1),
      TokenInfo(Token::kName, "func1", UINT32_C(15), UINT32_C(15)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(16), UINT32_C(16)),
      move(arg_def_nodes1),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(19), UINT32_C(19)),
      move(body_node1));
  unique_ptr<StmtNode> func_def_node1(func_def_node_ptr1);
  import_program_stmt_nodes1.push_back(move(func_def_node1));

  unique_ptr<ExprNode> lit_expr_node2(new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(24), UINT32_C(24))));
  unique_ptr<StmtNode> lit_stmt_node2(new ExprStmtNode(
      move(lit_expr_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(25), UINT32_C(25))));
  import_program_stmt_nodes1.push_back(move(lit_stmt_node2));
  shared_ptr<ProgramNode> import_program_node1(
      new ProgramNode(move(import_program_stmt_nodes1)));

  vector< unique_ptr<StmtNode> > import_program_stmt_nodes2;
  StringNode *file_path_node_ptr5 = new StringNode(TokenInfo(
      Token::kStringLit, "\"import.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node5(file_path_node_ptr5);
  ImportNode *import_node_ptr5 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node5),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node5(import_node_ptr5);
  import_program_stmt_nodes2.push_back(move(import_node5));

  unique_ptr<DataTypeNode> var_data_type_node3(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3))));
  VarDefWithoutInitNode *var_def_node_ptr3 = new VarDefWithoutInitNode(
      move(var_data_type_node3),
      TokenInfo(Token::kName, "var3", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> var_def_node3(var_def_node_ptr3);
  import_program_stmt_nodes2.push_back(move(var_def_node3));
  shared_ptr<ProgramNode> import_program_node2(
      new ProgramNode(move(import_program_stmt_nodes2)));

  vector<TestFileParse> test_file_parses;
  path import_program_absolute_file_path1("/myproject/import.rt");
  TestFileParse test_file_parse1 =
      {import_program_absolute_file_path1, import_program_node1};
  test_file_parses.push_back(test_file_parse1);
  path import_program_absolute_file_path2("/myproject/import2.rt");
  TestFileParse test_file_parse2 =
      {import_program_absolute_file_path2, import_program_node2};
  test_file_parses.push_back(test_file_parse2);

  vector<TestImportFileSearch> test_import_file_searches;
  path import_relative_file_path1("import.rt");
  path import_absolute_file_path1 = import_program_absolute_file_path1;
  TestImportFileSearch test_import_file_search1 =
      {import_relative_file_path1, import_absolute_file_path1};
  test_import_file_searches.push_back(test_import_file_search1);
  path import_relative_file_path2("./import.rt");
  path import_absolute_file_path2 = import_program_absolute_file_path1;
  TestImportFileSearch test_import_file_search2 =
      {import_relative_file_path2, import_absolute_file_path2};
  test_import_file_searches.push_back(test_import_file_search2);
  path import_relative_file_path3("main.rt");
  path main_program_absolute_file_path("/myproject/main.rt");
  path import_absolute_file_path3 = main_program_absolute_file_path;
  TestImportFileSearch test_import_file_search3 =
      {import_relative_file_path3, import_absolute_file_path3};
  test_import_file_searches.push_back(test_import_file_search3);
  path import_relative_file_path4("import2.rt");
  path import_absolute_file_path4 = import_program_absolute_file_path2;
  TestImportFileSearch test_import_file_search4 =
      {import_relative_file_path4, import_absolute_file_path4};
  test_import_file_searches.push_back(test_import_file_search4);
  path import_relative_file_path5("import.rt");
  path import_absolute_file_path5 = import_program_absolute_file_path1;
  TestImportFileSearch test_import_file_search5 =
      {import_relative_file_path5, import_absolute_file_path5};
  test_import_file_searches.push_back(test_import_file_search5);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"import.rt\"", "import.rt"},
                             {"\"./import.rt\"", "./import.rt"},
                             {"\"main.rt\"", "main.rt"},
                             {"\"import2.rt\"", "import2.rt"},
                             {"\"import.rt\"", "import.rt"}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_data_type2(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis2(
      new VarDefAnalysis(move(var_data_type2), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> func_return_data_type1(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types1;
  func_arg_data_types1.push_back(unique_ptr<DataType>(new LongDataType()));
  unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
      move(func_return_data_type1), move(func_arg_data_types1)));
  unique_ptr<DefAnalysis> func_def_analysis1(
      new FuncDefAnalysis(move(func_data_type1), false));
  def_analyzes.insert(make_pair(func_def_node_ptr1, move(func_def_analysis1)));
  unique_ptr<DataType> var_data_type3(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis3(
      new VarDefAnalysis(move(var_data_type3), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit1(new StringLit("import.rt"));
  LitAnalysis file_path_lit_analysis1(move(file_path_lit1));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr1, move(file_path_lit_analysis1)));
  unique_ptr<Lit> file_path_lit2(new StringLit("./import.rt"));
  LitAnalysis file_path_lit_analysis2(move(file_path_lit2));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr2, move(file_path_lit_analysis2)));
  unique_ptr<Lit> file_path_lit3(new StringLit("main.rt"));
  LitAnalysis file_path_lit_analysis3(move(file_path_lit3));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr3, move(file_path_lit_analysis3)));
  unique_ptr<Lit> file_path_lit4(new StringLit("import2.rt"));
  LitAnalysis file_path_lit_analysis4(move(file_path_lit4));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr4, move(file_path_lit_analysis4)));
  unique_ptr<Lit> file_path_lit5(new StringLit("import.rt"));
  LitAnalysis file_path_lit_analysis5(move(file_path_lit5));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr5, move(file_path_lit_analysis5)));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  ImportAnalysis import_analysis1(import_program_node1);
  import_analyzes.insert(make_pair(import_node_ptr1, import_analysis1));
  ImportAnalysis import_analysis2(import_program_node1);
  import_analyzes.insert(make_pair(import_node_ptr2, import_analysis2));
  ImportAnalysis import_analysis3(main_program_node);
  import_analyzes.insert(make_pair(import_node_ptr3, import_analysis3));
  ImportAnalysis import_analysis4(import_program_node2);
  import_analyzes.insert(make_pair(import_node_ptr4, import_analysis4));
  ImportAnalysis import_analysis5(import_program_node1);
  import_analyzes.insert(make_pair(import_node_ptr5, import_analysis5));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_expr_data_type1(new StringDataType());
  ExprAnalysis file_path_expr_analysis1(
      move(file_path_expr_data_type1), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr1, move(file_path_expr_analysis1)));
  unique_ptr<DataType> file_path_expr_data_type2(new StringDataType());
  ExprAnalysis file_path_expr_analysis2(
      move(file_path_expr_data_type2), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr2, move(file_path_expr_analysis2)));
  unique_ptr<DataType> file_path_expr_data_type3(new StringDataType());
  ExprAnalysis file_path_expr_analysis3(
      move(file_path_expr_data_type3), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr3, move(file_path_expr_analysis3)));
  unique_ptr<DataType> file_path_expr_data_type4(new StringDataType());
  ExprAnalysis file_path_expr_analysis4(
      move(file_path_expr_data_type4), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr4, move(file_path_expr_analysis4)));
  unique_ptr<DataType> file_path_expr_data_type5(new StringDataType());
  ExprAnalysis file_path_expr_analysis5(
      move(file_path_expr_data_type5), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr5, move(file_path_expr_analysis5)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);
  TestProgram test_program = {move(main_program_node),
                              main_program_absolute_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ImportAfterAnyOtherStmtIsInvalid) {
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
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  vector< unique_ptr<StmtNode> > import_program_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node2(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr2 = new VarDefWithoutInitNode(
      move(var_data_type_node2),
      TokenInfo(Token::kName, "var2", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node2(var_def_node_ptr2);
  import_program_stmt_nodes.push_back(move(var_def_node2));
  shared_ptr<ProgramNode> import_program_node(
      new ProgramNode(move(import_program_stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_data_type2(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis2(
      new VarDefAnalysis(move(var_data_type2), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_data_type(new StringDataType());
  ExprAnalysis file_path_expr_analysis(
      move(file_path_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis(move(file_path_lit));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  ImportAnalysis import_analysis(import_program_node);
  import_analyzes.insert(make_pair(import_node_ptr, import_analysis));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(
      new ImportIsNotFirstStmtError(file_path, *import_node_ptr));
  problems.push_back(move(problem));

  vector<TestFileParse> test_file_parses;
  path import_program_absolute_file_path("/myproject/file.rt");
  TestFileParse test_file_parse =
      {import_program_absolute_file_path, import_program_node};
  test_file_parses.push_back(test_file_parse);

  vector<TestImportFileSearch> test_import_file_searches;
  path import_relative_file_path("file.rt");
  path import_absolute_file_path = import_program_absolute_file_path;
  TestImportFileSearch test_import_file_search =
      {import_relative_file_path, import_absolute_file_path};
  test_import_file_searches.push_back(test_import_file_search);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"}};

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);
  path main_program_file_path;
  TestProgram test_program = {main_program_node,
                              main_program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ImportWithUnexpectedCharErrorDuringFileParseIsInvalid) {
  struct FailingTestFileParse {
    path absolute_file_path;
    UnexpectedCharError error;
  };

  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  main_program_stmt_nodes.push_back(move(var_def_node1));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  vector<TestImportFileSearch> test_import_file_searches;
  path import_relative_file_path("file.rt");
  path import_absolute_file_path("/myproject/file.rt");
  TestImportFileSearch test_import_file_search1 =
      {import_relative_file_path, import_absolute_file_path};
  test_import_file_searches.push_back(test_import_file_search1);

  vector<FailingTestFileParse> test_file_parses;
  char unexpected_char = '$';
  uint32_t unexpected_char_line = UINT32_C(7);
  uint32_t unexpected_char_column = UINT32_C(7);
  UnexpectedCharError file_parse_error(
      unexpected_char, unexpected_char_line, unexpected_char_column, "test");
  FailingTestFileParse test_file_parse1 =
      {import_absolute_file_path, file_parse_error};
  test_file_parses.push_back(test_file_parse1);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_data_type(new StringDataType());
  ExprAnalysis file_path_expr_analysis(
      move(file_path_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis(move(file_path_lit));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path main_program_file_path;
  unique_ptr<SemanticProblem> problem(new ImportWithUnexpectedCharError(
      main_program_file_path,
      *import_node_ptr,
      import_absolute_file_path,
      unexpected_char,
      unexpected_char_line,
      unexpected_char_column));
  problems.push_back(move(problem));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);

  ImportFileSearcherMock *import_file_searcher_mock =
      new ImportFileSearcherMock();
  unique_ptr<ImportFileSearcher> import_file_searcher(
      import_file_searcher_mock);

  for (const TestImportFileSearch &test_import_file_search:
           test_import_file_searches) {
    EXPECT_CALL(*import_file_searcher_mock,
                Search(test_import_file_search.relative_file_path))
        .Times(1)
        .WillOnce(Return(test_import_file_search.absolute_file_path))
        .RetiresOnSaturation();
  }

  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const TestLitParse<string> &test_lit_parse:
           test_lit_parses.strings) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Return(test_lit_parse.value))
        .RetiresOnSaturation();
  }

  FileParserMock *file_parser_mock = new FileParserMock();
  unique_ptr<FileParser> file_parser(file_parser_mock);

  for (const FailingTestFileParse &test_file_parse:
           test_file_parses) {
    EXPECT_CALL(*file_parser_mock,
                Parse(test_file_parse.absolute_file_path))
        .Times(1)
        .WillOnce(Throw(test_file_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(main_program_node,
                                  main_program_file_path,
                                  *file_parser,
                                  *import_file_searcher,
                                  *lit_parser);
  SemanticAnalysis actual_analysis = analyzer.Analyze();
  ASSERT_EQ(analysis, actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest,
       ImportWithUnexpectedTokenErrorDuringFileParseIsInvalid) {
  struct FailingTestFileParse {
    path absolute_file_path;
    UnexpectedTokenError error;
  };

  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  main_program_stmt_nodes.push_back(move(var_def_node1));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  vector<TestImportFileSearch> test_import_file_searches;
  path import_relative_file_path("file.rt");
  path import_absolute_file_path("/myproject/file.rt");
  TestImportFileSearch test_import_file_search1 =
      {import_relative_file_path, import_absolute_file_path};
  test_import_file_searches.push_back(test_import_file_search1);

  vector<FailingTestFileParse> test_file_parses;
  TokenInfo unexpected_token(Token::kFileEnd, "", UINT32_C(0), UINT32_C(0));
  UnexpectedTokenError file_parse_error(unexpected_token, "test");
  FailingTestFileParse test_file_parse1 =
      {import_absolute_file_path, file_parse_error};
  test_file_parses.push_back(test_file_parse1);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_data_type(new StringDataType());
  ExprAnalysis file_path_expr_analysis(
      move(file_path_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis(move(file_path_lit));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path main_program_file_path;
  unique_ptr<SemanticProblem> problem(new ImportWithUnexpectedTokenError(
      main_program_file_path,
      *import_node_ptr,
      import_absolute_file_path,
      unexpected_token));
  problems.push_back(move(problem));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);

  ImportFileSearcherMock *import_file_searcher_mock =
      new ImportFileSearcherMock();
  unique_ptr<ImportFileSearcher> import_file_searcher(
      import_file_searcher_mock);

  for (const TestImportFileSearch &test_import_file_search:
           test_import_file_searches) {
    EXPECT_CALL(*import_file_searcher_mock,
                Search(test_import_file_search.relative_file_path))
        .Times(1)
        .WillOnce(Return(test_import_file_search.absolute_file_path))
        .RetiresOnSaturation();
  }

  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const TestLitParse<string> &test_lit_parse:
           test_lit_parses.strings) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Return(test_lit_parse.value))
        .RetiresOnSaturation();
  }

  FileParserMock *file_parser_mock = new FileParserMock();
  unique_ptr<FileParser> file_parser(file_parser_mock);

  for (const FailingTestFileParse &test_file_parse:
           test_file_parses) {
    EXPECT_CALL(*file_parser_mock,
                Parse(test_file_parse.absolute_file_path))
        .Times(1)
        .WillOnce(Throw(test_file_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(main_program_node,
                                  main_program_file_path,
                                  *file_parser,
                                  *import_file_searcher,
                                  *lit_parser);
  SemanticAnalysis actual_analysis = analyzer.Analyze();
  ASSERT_EQ(analysis, actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, ImportWithIOErrorDuringFileParseIsInvalid) {
  struct FailingTestFileParse {
    path absolute_file_path;
    IOError error;
  };

  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  main_program_stmt_nodes.push_back(move(var_def_node1));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  vector<TestImportFileSearch> test_import_file_searches;
  path import_relative_file_path("file.rt");
  path import_absolute_file_path("/myproject/file.rt");
  TestImportFileSearch test_import_file_search1 =
      {import_relative_file_path, import_absolute_file_path};
  test_import_file_searches.push_back(test_import_file_search1);

  vector<FailingTestFileParse> test_file_parses;
  IOError file_parse_error("test");
  FailingTestFileParse test_file_parse1 =
      {import_absolute_file_path, file_parse_error};
  test_file_parses.push_back(test_file_parse1);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_data_type(new StringDataType());
  ExprAnalysis file_path_expr_analysis(
      move(file_path_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis(move(file_path_lit));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path main_program_file_path;
  unique_ptr<SemanticProblem> problem(new ImportWithIOError(
      main_program_file_path, *import_node_ptr, import_absolute_file_path));
  problems.push_back(move(problem));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);

  ImportFileSearcherMock *import_file_searcher_mock =
      new ImportFileSearcherMock();
  unique_ptr<ImportFileSearcher> import_file_searcher(
      import_file_searcher_mock);

  for (const TestImportFileSearch &test_import_file_search:
           test_import_file_searches) {
    EXPECT_CALL(*import_file_searcher_mock,
                Search(test_import_file_search.relative_file_path))
        .Times(1)
        .WillOnce(Return(test_import_file_search.absolute_file_path))
        .RetiresOnSaturation();
  }

  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const TestLitParse<string> &test_lit_parse:
           test_lit_parses.strings) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Return(test_lit_parse.value))
        .RetiresOnSaturation();
  }

  FileParserMock *file_parser_mock = new FileParserMock();
  unique_ptr<FileParser> file_parser(file_parser_mock);

  for (const FailingTestFileParse &test_file_parse:
           test_file_parses) {
    EXPECT_CALL(*file_parser_mock,
                Parse(test_file_parse.absolute_file_path))
        .Times(1)
        .WillOnce(Throw(test_file_parse.error))
        .RetiresOnSaturation();
  }

  SimpleSemanticAnalyzer analyzer(main_program_node,
                                  main_program_file_path,
                                  *file_parser,
                                  *import_file_searcher,
                                  *lit_parser);
  SemanticAnalysis actual_analysis = analyzer.Analyze();
  ASSERT_EQ(analysis, actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, ImportWithIOErrorDuringFileSearchIsInvalid) {
  struct FailingTestImportFileSearch {
    path relative_file_path;
    IOError error;
  };

  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  main_program_stmt_nodes.push_back(move(var_def_node1));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_data_type(new StringDataType());
  ExprAnalysis file_path_expr_analysis(
      move(file_path_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis(move(file_path_lit));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path main_program_file_path;
  path import_relative_file_path("file.rt");
  unique_ptr<SemanticProblem> problem(new ImportWithIOError(
      main_program_file_path, *import_node_ptr, import_relative_file_path));
  problems.push_back(move(problem));

  vector<FailingTestImportFileSearch> test_import_file_searches;
  IOError import_file_search_error("test");
  FailingTestImportFileSearch test_import_file_search1 =
      {import_relative_file_path, import_file_search_error};
  test_import_file_searches.push_back(test_import_file_search1);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"}};

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);

  ImportFileSearcherMock *import_file_searcher_mock =
      new ImportFileSearcherMock();
  unique_ptr<ImportFileSearcher> import_file_searcher(
      import_file_searcher_mock);

  for (const FailingTestImportFileSearch &test_import_file_search:
           test_import_file_searches) {
    EXPECT_CALL(*import_file_searcher_mock,
                Search(test_import_file_search.relative_file_path))
        .Times(1)
        .WillOnce(Throw(test_import_file_search.error))
        .RetiresOnSaturation();
  }

  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const TestLitParse<string> &test_lit_parse:
           test_lit_parses.strings) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Return(test_lit_parse.value))
        .RetiresOnSaturation();
  }

  unique_ptr<FileParser> file_parser(new FileParserMock());
  SimpleSemanticAnalyzer analyzer(main_program_node,
                                  main_program_file_path,
                                  *file_parser,
                                  *import_file_searcher,
                                  *lit_parser);
  SemanticAnalysis actual_analysis = analyzer.Analyze();
  ASSERT_EQ(analysis, actual_analysis);
}

TEST_F(SimpleSemanticAnalyzerTest, ImportWithNotExistingFileIsInvalid) {
  struct FailingTestImportFileSearch {
    path relative_file_path;
    FileNotFoundError error;
  };

  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node(file_path_node_ptr);
  ImportNode *import_node_ptr = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node(import_node_ptr);
  main_program_stmt_nodes.push_back(move(import_node));

  unique_ptr<DataTypeNode> var_data_type_node1(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr1 = new VarDefWithoutInitNode(
      move(var_data_type_node1),
      TokenInfo(Token::kName, "var1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node1(var_def_node_ptr1);
  main_program_stmt_nodes.push_back(move(var_def_node1));
  shared_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_data_type(new StringDataType());
  ExprAnalysis file_path_expr_analysis(
      move(file_path_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis(move(file_path_lit));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr, move(file_path_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path main_program_file_path;
  path import_relative_file_path("file.rt");
  unique_ptr<SemanticProblem> problem(new ImportWithNotExistingFileError(
      main_program_file_path, *import_node_ptr, import_relative_file_path));
  problems.push_back(move(problem));

  vector<FailingTestImportFileSearch> test_import_file_searches;
  FileNotFoundError import_file_search_error("");
  FailingTestImportFileSearch test_import_file_search1 =
      {import_relative_file_path, import_file_search_error};
  test_import_file_searches.push_back(test_import_file_search1);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"}};

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);

  ImportFileSearcherMock *import_file_searcher_mock =
      new ImportFileSearcherMock();
  unique_ptr<ImportFileSearcher> import_file_searcher(
      import_file_searcher_mock);

  for (const FailingTestImportFileSearch &test_import_file_search:
           test_import_file_searches) {
    EXPECT_CALL(*import_file_searcher_mock,
                Search(test_import_file_search.relative_file_path))
        .Times(1)
        .WillOnce(Throw(test_import_file_search.error))
        .RetiresOnSaturation();
  }

  LitParserMock *lit_parser_mock = new LitParserMock();
  unique_ptr<LitParser> lit_parser(lit_parser_mock);

  for (const TestLitParse<string> &test_lit_parse:
           test_lit_parses.strings) {
    EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
        .Times(1)
        .WillOnce(Return(test_lit_parse.value))
        .RetiresOnSaturation();
  }

  unique_ptr<FileParser> file_parser(new FileParserMock());
  SimpleSemanticAnalyzer analyzer(main_program_node,
                                  main_program_file_path,
                                  *file_parser,
                                  *import_file_searcher,
                                  *lit_parser);
  SemanticAnalysis actual_analysis = analyzer.Analyze();
  ASSERT_EQ(analysis, actual_analysis);
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
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_def_data_type1(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
      move(var_def_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_def_data_type2(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis2(new VarDefAnalysis(
      move(var_def_data_type2), DataStorage::kLocal));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *id_data_type_ptr = new BoolDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr1);
  id_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       PreTestLoopWithIncompatibleDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> func_body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;

  {
    unique_ptr<DataType> func_return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > func_arg_data_types;
    unique_ptr<FuncDataType> func_def_data_type(new FuncDataType(
        move(func_return_data_type), move(func_arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_def_data_type), false));
    def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));
  }

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> func_return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > func_arg_data_types;
  FuncDataType *id_data_type_ptr = new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types));
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new PreTestLoopWithIncompatibleTypeError(
      program_file_path,
      *loop_node_ptr,
      unique_ptr<DataType>(new BoolDataType()),
      id_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<StmtNode> continue_node(new ContinueNode(
      TokenInfo(Token::kContinue, "continue", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  body_stmt_nodes.push_back(move(continue_node));
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_def_data_type1(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
      move(var_def_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *id_data_type_ptr = new BoolDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr1);
  id_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new ContinueNotWithinLoopError(
      program_file_path, *continue_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  unique_ptr<StmtNode> break_node(new BreakNode(
      TokenInfo(Token::kBreak, "break", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(9))));
  body_stmt_nodes.push_back(move(break_node));
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10))));

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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_def_data_type1(new BoolDataType());
  unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
      move(var_def_data_type1), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *id_data_type_ptr = new BoolDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr1);
  id_analyzes.insert(make_pair(id_node_ptr, move(id_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new BreakNotWithinLoopError(
      program_file_path, *break_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Assign) {
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
  IntNode *lit_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(5)));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_def_data_type(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
      move(var_def_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *id_data_type_ptr = new IntDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  IntDataType *lit_data_type_ptr = new IntDataType();
  unique_ptr<DataType> lit_data_type(lit_data_type_ptr);
  ExprAnalysis lit_expr_analysis(move(lit_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr, move(lit_expr_analysis)));
  unique_ptr<DataType> assign_data_type(new IntDataType());
  ExprAnalysis assign_expr_analysis(move(assign_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(assign_node_ptr, move(assign_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit(new IntLit(INT32_C(7)));
  LitAnalysis lit_analysis(move(lit));
  lit_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"7\"", "7"}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_def_data_type(new IntDataType());
  unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
      move(var_def_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *id_data_type_ptr = new IntDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  StringDataType *lit_data_type_ptr = new StringDataType();
  unique_ptr<DataType> lit_data_type(lit_data_type_ptr);
  ExprAnalysis lit_expr_analysis(move(lit_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr, move(lit_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit(new StringLit("7"));
  LitAnalysis lit_analysis(move(lit));
  lit_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      program_file_path,
      *assign_node_ptr,
      id_data_type_ptr->Clone(),
      lit_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"1\"", "1"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  StringDataType *lit_data_type_ptr1 = new StringDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new StringLit("1"));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new AssignWithRightValueAssigneeError(
      program_file_path, *assign_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Less) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  IntNode *lit_node_ptr2 = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(2), UINT32_C(2)));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)},
                          {"2", INT32_C(2)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *lit_data_type_ptr1 = new IntDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));
  IntDataType *lit_data_type_ptr2 = new IntDataType();
  unique_ptr<DataType> lit_data_type2(lit_data_type_ptr2);
  ExprAnalysis lit_expr_analysis2(move(lit_data_type2), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr2, move(lit_expr_analysis2)));
  unique_ptr<DataType> less_data_type(new BoolDataType());
  ExprAnalysis less_expr_analysis(move(less_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(less_node_ptr, move(less_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new IntLit(INT32_C(1)));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));
  unique_ptr<Lit> lit2(new IntLit(INT32_C(2)));
  LitAnalysis lit_analysis2(move(lit2));
  lit_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
    shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        test_data.var_data_type->Clone(), DataStorage::kGlobal));
    def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    ExprAnalysis id_expr_analysis1(
        test_data.var_data_type->Clone(), ValueType::kLeft);
    expr_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));
    ExprAnalysis id_expr_analysis2(
        test_data.var_data_type->Clone(), ValueType::kLeft);
    expr_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));

    SemanticAnalysis::IdAnalyzes id_analyzes;
    IdAnalysis id_analysis1(var_def_node_ptr);
    id_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));
    IdAnalysis id_analysis2(var_def_node_ptr);
    id_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

    vector< unique_ptr<SemanticProblem> > problems;
    path program_file_path;
    unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
        program_file_path,
        *less_node_ptr,
        test_data.var_data_type->Clone(),
        test_data.var_data_type->Clone()));
    problems.push_back(move(problem));

    SemanticAnalysis::LitAnalyzes lit_analyzes;
    SemanticAnalysis::ImportAnalyzes import_analyzes;
    SemanticAnalysis analysis(move(problems),
                              move(def_analyzes),
                              move(expr_analyzes),
                              move(lit_analyzes),
                              import_analyzes,
                              id_analyzes);
    TestLitParses test_lit_parses = {};
    vector<TestFileParse> test_file_parses;
    vector<TestImportFileSearch> test_import_file_searches;
    TestProgram test_program = {program_node,
                                program_file_path,
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_lit_parses};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, Equal) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)},
                          {"2", INT32_C(2)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *lit_data_type_ptr1 = new IntDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));
  IntDataType *lit_data_type_ptr2 = new IntDataType();
  unique_ptr<DataType> lit_data_type2(lit_data_type_ptr2);
  ExprAnalysis lit_expr_analysis2(move(lit_data_type2), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr2, move(lit_expr_analysis2)));
  unique_ptr<DataType> equal_data_type(new BoolDataType());
  ExprAnalysis equal_expr_analysis(move(equal_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(equal_node_ptr, move(equal_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new IntLit(INT32_C(1)));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));
  unique_ptr<Lit> lit2(new IntLit(INT32_C(2)));
  LitAnalysis lit_analysis2(move(lit2));
  lit_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, EqualWithUnsupportedDataTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  FuncDataType func_data_type(move(return_data_type), move(arg_data_types));
  unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      false));
  def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  ExprAnalysis id_expr_analysis1(func_data_type.Clone(), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));
  ExprAnalysis id_expr_analysis2(func_data_type.Clone(), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr1, id_analysis1));
  IdAnalysis id_analysis2(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr2, id_analysis2));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      program_file_path,
      *equal_node_ptr,
      func_data_type.Clone(),
      func_data_type.Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ArrayAllocWithoutInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *size_data_type_ptr = new IntDataType();
  unique_ptr<DataType> size_data_type(size_data_type_ptr);
  ExprAnalysis size_expr_analysis(move(size_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(size_value_node_ptr, move(size_expr_analysis)));
  unique_ptr<DataType> element_data_type(new IntDataType());
  unique_ptr<DataType> alloc_data_type(
      new ArrayDataType(move(element_data_type)));
  ExprAnalysis alloc_expr_analysis(move(alloc_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(alloc_node_ptr, move(alloc_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> size_lit(new IntLit(INT32_C(1)));
  LitAnalysis size_lit_analysis(move(size_lit));
  lit_analyzes.insert(make_pair(size_value_node_ptr, move(size_lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"1\"", "1"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  StringDataType *size_data_type_ptr = new StringDataType();
  unique_ptr<DataType> size_data_type(size_data_type_ptr);
  ExprAnalysis size_expr_analysis(move(size_data_type), ValueType::kRight);
  expr_analyzes.insert(
      make_pair(size_value_node_ptr, move(size_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> size_lit(new StringLit("1"));
  LitAnalysis size_lit_analysis(move(size_lit));
  lit_analyzes.insert(make_pair(size_value_node_ptr, move(size_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithUnsupportedSizeTypeError(
          program_file_path,
          *alloc_node_ptr,
          *size_node_ptr,
          size_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<DataType> element_data_type(new VoidDataType());
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithUnsupportedElementTypeError(
          program_file_path, *alloc_node_ptr, move(element_data_type)));
  problems.push_back(move(problem));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(6)));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"2", INT32_C(2)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *element_data_type_ptr = new IntDataType();
  unique_ptr<DataType> element_data_type(element_data_type_ptr);
  unique_ptr<DataType> alloc_data_type(
      new ArrayDataType(move(element_data_type)));
  ExprAnalysis alloc_expr_analysis(move(alloc_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(alloc_node_ptr, move(alloc_expr_analysis)));
  IntDataType *value_data_type_ptr = new IntDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis value_expr_analysis(move(value_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(value_node_ptr, move(value_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> value_lit(new IntLit(INT32_C(2)));
  LitAnalysis value_lit_analysis(move(value_lit));
  lit_analyzes.insert(make_pair(value_node_ptr, move(value_lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<DataType> element_data_type(new VoidDataType());
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithUnsupportedElementTypeError(
          program_file_path, *alloc_node_ptr, move(element_data_type)));
  problems.push_back(move(problem));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"2\"", "2"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  StringDataType *value_data_type_ptr = new StringDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis value_expr_analysis(move(value_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(value_node_ptr, move(value_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> value_lit(new StringLit("2"));
  LitAnalysis value_lit_analysis(move(value_lit));
  lit_analyzes.insert(make_pair(value_node_ptr, move(value_lit_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  size_t bad_value_index = 0;
  unique_ptr<SemanticProblem> problem(
      new ArrayAllocWithIncompatibleValueTypeError(
          program_file_path,
          *alloc_node_ptr,
          bad_value_index,
          unique_ptr<DataType>(new IntDataType()),
          value_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  IntNode *index_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(7), UINT32_C(7)));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;

  {
    unique_ptr<DataType> element_data_type(new LongDataType());
    unique_ptr<DataType> id_data_type(
        new ArrayDataType(move(element_data_type)));
    ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
    expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  }

  IntDataType *index_data_type_ptr = new IntDataType();
  unique_ptr<DataType> index_data_type(index_data_type_ptr);
  ExprAnalysis index_expr_analysis(move(index_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(index_node_ptr, move(index_expr_analysis)));
  unique_ptr<DataType> subscript_data_type(new LongDataType());
  ExprAnalysis subscript_expr_analysis(
      move(subscript_data_type), ValueType::kLeft);
  expr_analyzes.insert(
      make_pair(subscript_node_ptr, move(subscript_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> index_lit(new IntLit(INT32_C(1)));
  LitAnalysis index_lit_analysis(move(index_lit));
  lit_analyzes.insert(make_pair(index_node_ptr, move(index_lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> element_data_type(new LongDataType());
  unique_ptr<DataType> var_data_type(
      new ArrayDataType(move(element_data_type)));
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> id_data_type(new LongDataType());
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type(new LongDataType());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(
      new SubscriptWithNonArrayError(program_file_path, *subscript_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  TestLitParses test_lit_parses = {};
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"1\"", "1"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;

  {
    unique_ptr<DataType> element_data_type(new LongDataType());
    unique_ptr<DataType> id_data_type(
        new ArrayDataType(move(element_data_type)));
    ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
    expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  }

  StringDataType *index_data_type_ptr = new StringDataType();
  unique_ptr<DataType> index_data_type(index_data_type_ptr);
  ExprAnalysis index_expr_analysis(move(index_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(index_node_ptr, move(index_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> index_lit(new StringLit("1"));
  LitAnalysis index_lit_analysis(move(index_lit));
  lit_analyzes.insert(make_pair(index_node_ptr, move(index_lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> element_data_type(new LongDataType());
  unique_ptr<DataType> var_data_type(
      new ArrayDataType(move(element_data_type)));
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(
      new SubscriptWithUnsupportedIndexTypeError(
          program_file_path,
          *subscript_node_ptr,
          index_data_type_ptr->Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> id_data_type(new LongDataType());
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kLeft);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type(new LongDataType());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), DataStorage::kGlobal));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(
      new IdWithoutDefError(program_file_path, *id_node_ptr));
  problems.push_back(move(problem));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  TestLitParses test_lit_parses = {};
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.bools = {{"false", false}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> lit_data_type(new BoolDataType());
  ExprAnalysis lit_expr_analysis(move(lit_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(lit_node_ptr, move(lit_expr_analysis)));
  unique_ptr<DataType> not_data_type(new BoolDataType());
  ExprAnalysis not_expr_analysis(move(not_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(not_node_ptr, move(not_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit(new BoolLit(false));
  LitAnalysis lit_analysis(move(lit));
  lit_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  SemanticAnalysis::IdAnalyzes id_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  path program_file_path;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, NotWithUnsupportedDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0)),
      vector< unique_ptr<StmtNode> >(),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0))));
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
  shared_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  FuncDataType func_data_type(
      unique_ptr<DataType>(new VoidDataType()),
      vector< unique_ptr<DataType> >());
  unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
      unique_ptr<FuncDataType>(
          static_cast<FuncDataType*>(func_data_type.Clone().release())),
      false));
  def_analyzes.insert(make_pair(func_def_node_ptr, move(func_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(func_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> id_data_type(func_data_type.Clone());
  ExprAnalysis id_expr_analysis(move(id_data_type), ValueType::kRight);
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path program_file_path;
  unique_ptr<SemanticProblem> problem(new UnaryExprWithUnsupportedTypeError(
      program_file_path, *not_node_ptr, func_data_type.Clone()));
  problems.push_back(move(problem));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  SemanticAnalysis::ImportAnalyzes import_analyzes;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            import_analyzes,
                            id_analyzes);
  vector<TestFileParse> test_file_parses;
  TestLitParses test_lit_parses = {};
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {program_node,
                              program_file_path,
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses};

  TestAnalyze(test_program);
}
}
}
