
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
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/parser.h"
#include "real_talk/parser/file_parser.h"
#include "real_talk/parser/program_node.h"
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
#include "real_talk/parser/func_def_node.h"
#include "real_talk/parser/arg_def_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/scope_node.h"
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/parser/if_else_if_else_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/less_node.h"
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
#include "real_talk/semantic/global_data_storage.h"
#include "real_talk/semantic/local_data_storage.h"
#include "real_talk/semantic/import_file_searcher.h"
#include "real_talk/semantic/lit_parser.h"
#include "real_talk/semantic/semantic_problems.h"
#include "real_talk/semantic/data_type_converter.h"

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
using boost::filesystem::path;
using real_talk::lexer::TokenInfo;
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
using real_talk::parser::FuncDefNode;
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
using real_talk::parser::ArraySizeNode;
using real_talk::parser::CallNode;
using real_talk::parser::LessNode;

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

struct TestDataTypeConversion {
  DataType *dest;
  DataType *src;
  bool is_convertable;
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
  unique_ptr<ProgramNode> node;
  SemanticAnalysis analysis;
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses;
  vector<TestDataTypeConversion> test_data_type_conversions;
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

class DataTypeConverterMock: public DataTypeConverter {
 public:
  MOCK_CONST_METHOD2(IsConvertable, bool(const DataType&,
                                         const DataType&));
};

class SimpleSemanticAnalyzerTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
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

    LitParserMock *lit_parser_mock = new LitParserMock();
    unique_ptr<LitParser> lit_parser(lit_parser_mock);

    for (const TestLitParse<int32_t> &test_lit_parse:
             test_program.test_lit_parses.ints) {
      EXPECT_CALL(*lit_parser_mock, ParseInt(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<int64_t> &test_lit_parse:
             test_program.test_lit_parses.longs) {
      EXPECT_CALL(*lit_parser_mock, ParseLong(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<double> &test_lit_parse:
             test_program.test_lit_parses.doubles) {
      EXPECT_CALL(*lit_parser_mock, ParseDouble(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<string> &test_lit_parse:
             test_program.test_lit_parses.strings) {
      EXPECT_CALL(*lit_parser_mock, ParseString(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<char> &test_lit_parse:
             test_program.test_lit_parses.chars) {
      EXPECT_CALL(*lit_parser_mock, ParseChar(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    for (const TestLitParse<bool> &test_lit_parse:
             test_program.test_lit_parses.bools) {
      EXPECT_CALL(*lit_parser_mock, ParseBool(test_lit_parse.str))
          .Times(1)
          .WillOnce(Return(test_lit_parse.value))
          .RetiresOnSaturation();
    }

    DataTypeConverterMock *data_type_converter_mock =
        new DataTypeConverterMock();
    unique_ptr<DataTypeConverter> data_type_converter(data_type_converter_mock);

    for (const TestDataTypeConversion &test_data_type_conversion:
             test_program.test_data_type_conversions) {
      EXPECT_CALL(*data_type_converter_mock,
                  IsConvertable(Eq(ByRef(*(test_data_type_conversion.dest))),
                                Eq(ByRef(*(test_data_type_conversion.src)))))
          .Times(1)
          .WillOnce(Return(test_data_type_conversion.is_convertable))
          .RetiresOnSaturation();
    }

    SimpleSemanticAnalyzer analyzer(*(test_program.node),
                                    *file_parser,
                                    *import_file_searcher,
                                    *lit_parser,
                                    *data_type_converter);
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
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataStorage> data_storage(new GlobalDataStorage());
    unique_ptr<DefAnalysis> def_analysis(
        new VarDefAnalysis(move(test_data.data_type), move(data_storage)));
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
    vector<TestDataTypeConversion> test_data_type_conversions;
    TestProgram test_program = {move(program_node),
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_data.test_lit_parses,
                                test_data_type_conversions};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithoutInitUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
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
    FuncDefNode *func_def_node_ptr = new FuncDefNode(
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

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type)));
    def_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DataType> arg_data_type(new IntDataType());
    unique_ptr<DefAnalysis> arg_def_analysis(new ArgDefAnalysis(
        move(arg_data_type)));
    def_analyzes.insert(
        make_pair(arg_def_node_ptr, move(arg_def_analysis)));
    unique_ptr<DataType> var_data_type(new IntDataType());
    unique_ptr<DataStorage> var_data_storage(new LocalDataStorage());
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        move(var_data_type), move(var_data_storage)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr));
    problems.push_back(move(problem));

    TestData test_data =
        {move(program_node), move(def_analyzes), move(problems)};
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
    FuncDefNode *func_def_node_ptr = new FuncDefNode(
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

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types;
    unique_ptr<FuncDataType> func_data_type(new FuncDataType(
        move(return_data_type), move(arg_data_types)));
    unique_ptr<DefAnalysis> func_def_analysis(new FuncDefAnalysis(
        move(func_data_type)));
    def_analyzes.insert(
        make_pair(func_def_node_ptr, move(func_def_analysis)));
    unique_ptr<DataType> var_data_type(new IntDataType());
    unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        move(var_data_type), move(var_data_storage)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr));
    problems.push_back(move(problem));

    TestData test_data =
        {move(program_node), move(def_analyzes), move(problems)};
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

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> var_data_type1(new IntDataType());
    unique_ptr<DataStorage> var_data_storage1(new GlobalDataStorage());
    unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
        move(var_data_type1), move(var_data_storage1)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr1, move(var_def_analysis1)));
    unique_ptr<DataType> var_data_type2(new IntDataType());
    unique_ptr<DataStorage> var_data_storage2(new GlobalDataStorage());
    unique_ptr<DefAnalysis> var_def_analysis2(new VarDefAnalysis(
        move(var_data_type2), move(var_data_storage2)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr2, move(var_def_analysis2)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *var_def_node_ptr2));
    problems.push_back(move(problem));

    TestData test_data =
        {move(program_node), move(def_analyzes), move(problems)};
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
    vector<TestDataTypeConversion> test_data_type_conversions;
    TestProgram test_program = {move(test_data.program_node),
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_lit_parses,
                                test_data_type_conversions};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithoutInitUsingVoidDataTypeIsInvalid) {
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

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
    DataType *var_data_type_ptr = test_data.var_data_type.get();
    unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
        move(test_data.var_data_type), move(var_data_storage)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr, move(var_def_analysis)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DefWithUnsupportedDataTypeError(
        file_path, *var_def_node_ptr, *var_data_type_ptr));
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
    vector<TestDataTypeConversion> test_data_type_conversions;
    TestProgram test_program = {move(program_node),
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_lit_parses,
                                test_data_type_conversions};

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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *var_data_type_ptr = new IntDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> def_analysis(new VarDefAnalysis(
      move(var_data_type), move(var_data_storage)));
  def_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *value_data_type_ptr = new IntDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis expr_analysis(move(value_data_type));
  expr_analyzes.insert(make_pair(value_node, move(expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  LitAnalysis lit_analysis(unique_ptr<Lit>(new IntLit(INT32_C(7))));
  lit_analyzes.insert(make_pair(value_node, move(lit_analysis)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = var_data_type_ptr;
  DataType *src_data_type = value_data_type_ptr;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingIncompatibleDataTypeIsInvalid) {
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

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  StringDataType *value_data_type_ptr = new StringDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis value_expr_analysis(move(value_data_type));
  expr_analyzes.insert(make_pair(value_node_ptr, move(value_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> value_lit(new StringLit("7"));
  LitAnalysis lit_analysis(move(move(value_lit)));
  lit_analyzes.insert(make_pair(value_node_ptr, move(lit_analysis)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *var_data_type_ptr = new IntDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), move(var_data_storage)));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new InitWithIncompatibleTypeError(
      file_path, *var_def_node_ptr, *var_data_type_ptr, *value_data_type_ptr));
  problems.push_back(move(problem));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = var_data_type_ptr;
  DataType *src_data_type = value_data_type_ptr;
  bool is_data_type_convertable = false;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingVoidDataTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *var_value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> value_node(var_value_node_ptr);
  unique_ptr<DataTypeNode> var_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  VarDefWithInitNode *var_def_node_ptr = new VarDefWithInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
      move(value_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  stmt_nodes.push_back(move(var_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  VoidDataType *var_data_type_ptr = new VoidDataType();
  unique_ptr<DataType> var_data_type(var_data_type_ptr);
  unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
      move(var_data_type), move(var_data_storage)));
  def_analyzes.insert(
      make_pair(var_def_node_ptr, move(var_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new DefWithUnsupportedDataTypeError(
      file_path, *var_def_node_ptr, *var_data_type_ptr));
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
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestLitParses test_lit_parses = {};
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       VarDefWithInitUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
    SemanticAnalysis::DefAnalyzes def_analyzes;
    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    SemanticAnalysis::LitAnalyzes lit_analyzes;
    vector< unique_ptr<SemanticProblem> > problems;
    TestLitParses test_lit_parses;
    vector<TestDataTypeConversion> test_data_type_conversions;
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

    SemanticAnalysis::DefAnalyzes def_analyzes;
    IntDataType *var_data_type_ptr1 = new IntDataType();
    unique_ptr<DataType> var_data_type1(var_data_type_ptr1);
    unique_ptr<DataStorage> var_data_storage1(new GlobalDataStorage());
    unique_ptr<DefAnalysis> var_def_analysis1(new VarDefAnalysis(
        move(var_data_type1), move(var_data_storage1)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr1, move(var_def_analysis1)));
    IntDataType *var_data_type_ptr2 = new IntDataType();
    unique_ptr<DataType> var_data_type2(var_data_type_ptr2);
    unique_ptr<DataStorage> var_data_storage2(new GlobalDataStorage());
    unique_ptr<DefAnalysis> var_def_analysis2(new VarDefAnalysis(
        move(var_data_type2), move(var_data_storage2)));
    def_analyzes.insert(
        make_pair(var_def_node_ptr2, move(var_def_analysis2)));

    SemanticAnalysis::ExprAnalyzes expr_analyzes;
    IntDataType *var_value_data_type_ptr1 = new IntDataType();
    unique_ptr<DataType> var_value_data_type1(var_value_data_type_ptr1);
    ExprAnalysis var_value_expr_analysis1(move(var_value_data_type1));
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

    vector<TestDataTypeConversion> test_data_type_conversions;
    DataType *dest_data_type = var_data_type_ptr1;
    DataType *src_data_type = var_value_data_type_ptr1;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);

    TestData test_data = {move(program_node),
                          move(def_analyzes),
                          move(expr_analyzes),
                          move(lit_analyzes),
                          move(problems),
                          test_lit_parses,
                          test_data_type_conversions};
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
    TestProgram test_program = {move(test_data.program_node),
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_data.test_lit_parses,
                                test_data.test_data_type_conversions};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithReturnValue) {
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
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  IntDataType *func_return_data_type_ptr = new IntDataType();
  unique_ptr<DataType> func_return_data_type(func_return_data_type_ptr);
  vector< unique_ptr<DataType> > func_arg_data_types;
  func_arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(func_return_data_type), move(func_arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type)));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new IntDataType());
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *return_expr_data_type_ptr = new IntDataType();
  unique_ptr<DataType> return_expr_data_type(return_expr_data_type_ptr);
  ExprAnalysis return_expr_analysis(move(return_expr_data_type));
  expr_analyzes.insert(
      make_pair(return_expr_node_ptr, move(return_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(arg_def_node_ptr);
  id_analyzes.insert(make_pair(return_expr_node_ptr, id_analysis));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = func_return_data_type_ptr;
  DataType *src_data_type = return_expr_data_type_ptr;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithoutReturnValue) {
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
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(6), UINT32_C(6)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(7), UINT32_C(7))));

  unique_ptr<DataTypeNode> func_data_type_node(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))));
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type)));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));
  unique_ptr<DataType> arg_data_type(new IntDataType());
  unique_ptr<DefAnalysis> arg_def_analysis(
      new ArgDefAnalysis(move(arg_data_type)));
  def_analyzes.insert(make_pair(arg_def_node_ptr, move(arg_def_analysis)));

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
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefUsingAlreadyExistingIdIsInvalid) {
  struct TestData {
    unique_ptr<ProgramNode> program_node;
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
    FuncDefNode *func_def_node_ptr1 = new FuncDefNode(
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
    FuncDefNode *func_def_node_ptr2 = new FuncDefNode(
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

    SemanticAnalysis::DefAnalyzes def_analyzes;
    unique_ptr<DataType> return_data_type1(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types1;
    unique_ptr<FuncDataType> func_data_type1(new FuncDataType(
        move(return_data_type1), move(arg_data_types1)));
    unique_ptr<DefAnalysis> func_def_analysis1(new FuncDefAnalysis(
        move(func_data_type1)));
    def_analyzes.insert(
        make_pair(func_def_node_ptr1, move(func_def_analysis1)));
    unique_ptr<DataType> return_data_type2(new VoidDataType());
    vector< unique_ptr<DataType> > arg_data_types2;
    unique_ptr<FuncDataType> func_data_type2(new FuncDataType(
        move(return_data_type2), move(arg_data_types2)));
    unique_ptr<DefAnalysis> func_def_analysis2(new FuncDefAnalysis(
        move(func_data_type2)));
    def_analyzes.insert(
        make_pair(func_def_node_ptr2, move(func_def_analysis2)));

    vector< unique_ptr<SemanticProblem> > problems;
    path file_path;
    unique_ptr<SemanticProblem> problem(new DuplicateDefError(
        file_path, *func_def_node_ptr2));
    problems.push_back(move(problem));

    TestData test_data =
        {move(program_node), move(def_analyzes), move(problems)};
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
    vector<TestDataTypeConversion> test_data_type_conversions;
    TestProgram test_program = {move(test_data.program_node),
                                move(analysis),
                                test_file_parses,
                                test_import_file_searches,
                                test_lit_parses,
                                test_data_type_conversions};

    TestAnalyze(test_program);
  }
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefUsingVoidArrayDataTypeIsInvalid) {
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
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
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
      new FuncDefAnalysis(move(func_data_type)));
  def_analyzes.insert(
      make_pair(func_def_node_ptr, move(func_def_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new DefWithUnsupportedDataTypeError(
      file_path, *func_def_node_ptr, *return_data_type_ptr));
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
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, FuncDefWithinNotGlobalScopeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes2;
  unique_ptr<ScopeNode> body_node2(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(9)),
      move(body_stmt_nodes2),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10))));

  vector< unique_ptr<StmtNode> > body_stmt_nodes1;
  unique_ptr<DataTypeNode> return_data_type_node2(new VoidDataTypeNode(
      TokenInfo(Token::kVoidType, "void", UINT32_C(5), UINT32_C(5))));
  FuncDefNode *func_def_node_ptr2 = new FuncDefNode(
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
  FuncDefNode *func_def_node_ptr1 = new FuncDefNode(
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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type1(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types1;
  unique_ptr<FuncDataType> func_data_type1(
      new FuncDataType(move(return_data_type1), move(arg_data_types1)));
  unique_ptr<DefAnalysis> func_def_analysis1(
      new FuncDefAnalysis(move(func_data_type1)));
  def_analyzes.insert(
      make_pair(func_def_node_ptr1, move(func_def_analysis1)));
  unique_ptr<DataType> return_data_type2(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types2;
  unique_ptr<FuncDataType> func_data_type2(
      new FuncDataType(move(return_data_type2), move(arg_data_types2)));
  unique_ptr<DefAnalysis> func_def_analysis2(
      new FuncDefAnalysis(move(func_data_type2)));
  def_analyzes.insert(
      make_pair(func_def_node_ptr2, move(func_def_analysis2)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new NestedFuncDefError(
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
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest,
       FuncDefWithNonVoidDataTypeButWithoutReturnValueIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  vector< unique_ptr<StmtNode> > body_stmt_nodes;
  unique_ptr<ScopeNode> body_node(new ScopeNode(
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      move(body_stmt_nodes),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11))));

  unique_ptr<DataTypeNode> return_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
      move(return_data_type_node),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      vector< unique_ptr<ArgDefNode> >(),
      vector<TokenInfo>(),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      move(body_node));
  unique_ptr<StmtNode> func_def_node(func_def_node_ptr);
  stmt_nodes.push_back(move(func_def_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new IntDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  unique_ptr<FuncDataType> func_data_type(
      new FuncDataType(move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type)));
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
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Id) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> def_node(def_node_ptr);
  stmt_nodes.push_back(move(def_node));

  IdNode *id_node_ptr = new IdNode(
      TokenInfo(Token::kName, "var", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> id_node(id_node_ptr);
  unique_ptr<StmtNode> id_stmt_node(new ExprStmtNode(
      move(id_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4))));
  stmt_nodes.push_back(move(id_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> def_analysis(new VarDefAnalysis(
      move(var_data_type), move(var_data_storage)));
  def_analyzes.insert(make_pair(def_node_ptr, move(def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> expr_data_type(new IntDataType());
  ExprAnalysis expr_analysis(move(expr_data_type));
  expr_analyzes.insert(make_pair(id_node_ptr, move(expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(def_node_ptr);
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
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestLitParses test_lit_parses = {};
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"7", INT32_C(7)}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type)));
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
  ExprAnalysis id_expr_analysis(move(id_data_type));
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  ExprAnalysis call_expr_analysis(move(call_expr_data_type));
  expr_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));
  IntDataType *call_arg_data_type_ptr = new IntDataType();
  unique_ptr<DataType> call_arg_data_type(call_arg_data_type_ptr);
  ExprAnalysis call_arg_expr_analysis(move(call_arg_data_type));
  expr_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> call_arg_lit(new IntLit(INT32_C(7)));
  LitAnalysis call_arg_lit_analysis(move(call_arg_lit));
  lit_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_lit_analysis)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = arg_data_type_ptr;
  DataType *src_data_type = call_arg_data_type_ptr;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, CallWithIncompatibleDataTypeIsInvalid) {
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
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type)));
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
  ExprAnalysis id_expr_analysis(move(id_data_type));
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  ExprAnalysis call_expr_analysis(move(call_expr_data_type));
  expr_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));
  LongDataType *call_arg_data_type_ptr = new LongDataType();
  unique_ptr<DataType> call_arg_data_type(call_arg_data_type_ptr);
  ExprAnalysis call_arg_expr_analysis(move(call_arg_data_type));
  expr_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> call_arg_lit(new LongLit(INT64_C(7)));
  LitAnalysis call_arg_lit_analysis(move(call_arg_lit));
  lit_analyzes.insert(
      make_pair(call_arg_node_ptr, move(call_arg_lit_analysis)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = arg_def_data_type_ptr;
  DataType *src_data_type = call_arg_data_type_ptr;
  bool is_data_type_convertable = false;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  size_t bad_arg_index = 0;
  unique_ptr<SemanticProblem> problem(new CallWithIncompatibleTypeError(
      file_path,
      *call_expr_node_ptr,
      bad_arg_index,
      *arg_def_data_type_ptr,
      *call_arg_data_type_ptr));
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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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
  FuncDefNode *func_def_node_ptr = new FuncDefNode(
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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> return_data_type(new VoidDataType());
  vector< unique_ptr<DataType> > arg_data_types;
  arg_data_types.push_back(unique_ptr<DataType>(new IntDataType()));
  unique_ptr<FuncDataType> func_data_type(new FuncDataType(
      move(return_data_type), move(arg_data_types)));
  unique_ptr<DefAnalysis> func_def_analysis(
      new FuncDefAnalysis(move(func_data_type)));
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
  ExprAnalysis id_expr_analysis(move(id_data_type));
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  unique_ptr<DataType> call_expr_data_type(new VoidDataType());
  ExprAnalysis call_expr_analysis(move(call_expr_data_type));
  expr_analyzes.insert(make_pair(call_expr_node_ptr, move(call_expr_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  size_t expected_args_count = 1;
  size_t actual_args_count = 2;
  unique_ptr<SemanticProblem> problem(new InvalidArgsCountError(
      file_path, *call_expr_node_ptr, expected_args_count, actual_args_count));
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
  vector<TestDataTypeConversion> test_data_type_conversions;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type(new IntDataType());
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), move(var_data_storage)));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> id_data_type(new IntDataType());
  ExprAnalysis id_expr_analysis(move(id_data_type));
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(
      new NonFuncCallError(file_path, *call_expr_node_ptr));
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
  vector<TestDataTypeConversion> test_data_type_conversions;
  vector<TestFileParse> test_file_parses;
  vector<TestImportFileSearch> test_import_file_searches;
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new BoolDataType());
  unique_ptr<DataStorage> var_storage1(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), move(var_storage1)));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_data_type2(new BoolDataType());
  unique_ptr<DataStorage> var_storage2(new LocalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis2(
      new VarDefAnalysis(move(var_data_type2), move(var_storage2)));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> var_data_type3(new BoolDataType());
  unique_ptr<DataStorage> var_storage3(new LocalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis3(
      new VarDefAnalysis(move(var_data_type3), move(var_storage3)));
  def_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *if_cond_data_type_ptr1 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type1(if_cond_data_type_ptr1);
  ExprAnalysis if_cond_expr_analysis1(move(if_cond_data_type1));
  expr_analyzes.insert(make_pair(if_cond_ptr1, move(if_cond_expr_analysis1)));
  BoolDataType *if_cond_data_type_ptr2 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type2(if_cond_data_type_ptr2);
  ExprAnalysis if_cond_expr_analysis2(move(if_cond_data_type2));
  expr_analyzes.insert(make_pair(if_cond_ptr2, move(if_cond_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr1, id_analysis1));
  IdAnalysis id_analysis2(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr2, id_analysis2));

  vector<TestDataTypeConversion> test_data_type_conversions;
  unique_ptr<BoolDataType> bool_data_type(new BoolDataType());

  {
    DataType *dest_data_type = bool_data_type.get();
    DataType *src_data_type = if_cond_data_type_ptr1;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);
  }

  {
    DataType *dest_data_type = bool_data_type.get();
    DataType *src_data_type = if_cond_data_type_ptr2;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);
  }

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type1(new BoolDataType());
  unique_ptr<DataStorage> var_storage1(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis1(
      new VarDefAnalysis(move(var_data_type1), move(var_storage1)));
  def_analyzes.insert(make_pair(var_def_node_ptr1, move(var_def_analysis1)));
  unique_ptr<DataType> var_data_type2(new BoolDataType());
  unique_ptr<DataStorage> var_storage2(new LocalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis2(
      new VarDefAnalysis(move(var_data_type2), move(var_storage2)));
  def_analyzes.insert(make_pair(var_def_node_ptr2, move(var_def_analysis2)));
  unique_ptr<DataType> var_data_type3(new BoolDataType());
  unique_ptr<DataStorage> var_storage3(new LocalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis3(
      new VarDefAnalysis(move(var_data_type3), move(var_storage3)));
  def_analyzes.insert(make_pair(var_def_node_ptr3, move(var_def_analysis3)));
  unique_ptr<DataType> var_data_type4(new BoolDataType());
  unique_ptr<DataStorage> var_storage4(new LocalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis4(
      new VarDefAnalysis(move(var_data_type4), move(var_storage4)));
  def_analyzes.insert(make_pair(var_def_node_ptr4, move(var_def_analysis4)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  BoolDataType *if_cond_data_type_ptr1 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type1(if_cond_data_type_ptr1);
  ExprAnalysis if_cond_expr_analysis1(move(if_cond_data_type1));
  expr_analyzes.insert(make_pair(if_cond_ptr1, move(if_cond_expr_analysis1)));
  BoolDataType *if_cond_data_type_ptr2 = new BoolDataType();
  unique_ptr<DataType> if_cond_data_type2(if_cond_data_type_ptr2);
  ExprAnalysis if_cond_expr_analysis2(move(if_cond_data_type2));
  expr_analyzes.insert(make_pair(if_cond_ptr2, move(if_cond_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr1, id_analysis1));
  IdAnalysis id_analysis2(var_def_node_ptr1);
  id_analyzes.insert(make_pair(if_cond_ptr2, id_analysis2));

  vector<TestDataTypeConversion> test_data_type_conversions;
  unique_ptr<BoolDataType> bool_data_type(new BoolDataType());

  {
    DataType *dest_data_type = bool_data_type.get();
    DataType *src_data_type = if_cond_data_type_ptr1;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);
  }

  {
    DataType *dest_data_type = bool_data_type.get();
    DataType *src_data_type = if_cond_data_type_ptr2;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);
  }

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, Import) {
  vector< unique_ptr<StmtNode> > main_program_stmt_nodes;
  StringNode *file_path_node_ptr1 = new StringNode(
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)));
  unique_ptr<StringNode> file_path_node1(file_path_node_ptr1);
  ImportNode *import_node_ptr1 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      move(file_path_node1),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> import_node1(import_node_ptr1);
  main_program_stmt_nodes.push_back(move(import_node1));

  StringNode *file_path_node_ptr2 = new StringNode(
      TokenInfo(Token::kStringLit, "\"./file.rt\"", UINT32_C(4), UINT32_C(4)));
  unique_ptr<StringNode> file_path_node2(file_path_node_ptr2);
  ImportNode *import_node_ptr2 = new ImportNode(
      TokenInfo(Token::kImport, "import", UINT32_C(3), UINT32_C(3)),
      move(file_path_node2),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)));
  unique_ptr<StmtNode> import_node2(import_node_ptr2);
  main_program_stmt_nodes.push_back(move(import_node2));
  unique_ptr<ProgramNode> main_program_node(
      new ProgramNode(move(main_program_stmt_nodes)));

  vector<TestFileParse> test_file_parses;
  vector< unique_ptr<StmtNode> > import_program_stmt_nodes;
  unique_ptr<DataTypeNode> var_data_type_node(new BoolDataTypeNode(
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
      TokenInfo(Token::kName, "name", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)));
  unique_ptr<StmtNode> var_def_node(var_def_node_ptr);
  import_program_stmt_nodes.push_back(move(var_def_node));
  shared_ptr<ProgramNode> import_program_node(
      new ProgramNode(move(import_program_stmt_nodes)));
  path import_program_absolute_file_path("/myproject/file.rt");
  TestFileParse test_file_parse =
      {import_program_absolute_file_path, import_program_node};
  test_file_parses.push_back(test_file_parse);

  vector<TestImportFileSearch> test_import_file_searches;
  path import_relative_file_path1("file.rt");
  path import_absolute_file_path1 = import_program_absolute_file_path;
  TestImportFileSearch test_import_file_search1 =
      {import_relative_file_path1, import_absolute_file_path1};
  test_import_file_searches.push_back(test_import_file_search1);
  path import_relative_file_path2("./file.rt");
  path import_absolute_file_path2 = import_program_absolute_file_path;
  TestImportFileSearch test_import_file_search2 =
      {import_relative_file_path2, import_absolute_file_path2};
  test_import_file_searches.push_back(test_import_file_search2);

  TestLitParses test_lit_parses = {};
  test_lit_parses.strings = {{"\"file.rt\"", "file.rt"},
                       {"\"./file.rt\"", "./file.rt"}};

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_data_type(new BoolDataType());
  unique_ptr<DataStorage> var_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), move(var_storage)));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> file_path_lit1(new StringLit("file.rt"));
  LitAnalysis file_path_lit_analysis1(move(file_path_lit1));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr1, move(file_path_lit_analysis1)));
  unique_ptr<Lit> file_path_lit2(new StringLit("./file.rt"));
  LitAnalysis file_path_lit_analysis2(move(file_path_lit2));
  lit_analyzes.insert(
      make_pair(file_path_node_ptr2, move(file_path_lit_analysis2)));

  SemanticAnalysis::ImportAnalyzes import_analyzes;
  ImportAnalysis import_analysis1(import_program_node);
  import_analyzes.insert(make_pair(import_node_ptr1, import_analysis1));
  ImportAnalysis import_analysis2(import_program_node);
  import_analyzes.insert(make_pair(import_node_ptr2, import_analysis2));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> file_path_expr_data_type1(new StringDataType());
  ExprAnalysis file_path_expr_analysis1(move(file_path_expr_data_type1));
  expr_analyzes.insert(
      make_pair(file_path_node_ptr1, move(file_path_expr_analysis1)));
  unique_ptr<DataType> file_path_expr_data_type2(new StringDataType());
  ExprAnalysis file_path_expr_analysis2(move(file_path_expr_data_type2));
  expr_analyzes.insert(
      make_pair(file_path_node_ptr2, move(file_path_expr_analysis2)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  vector< unique_ptr<SemanticProblem> > problems;
  vector<TestDataTypeConversion> test_data_type_conversions;
  SemanticAnalysis analysis(move(problems),
                            move(def_analyzes),
                            move(expr_analyzes),
                            move(lit_analyzes),
                            move(import_analyzes),
                            id_analyzes);
  TestProgram test_program = {move(main_program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints.push_back({"7", INT32_C(7)});

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_def_data_type(new IntDataType());
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis(new VarDefAnalysis(
      move(var_def_data_type), move(var_data_storage)));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *id_data_type_ptr = new IntDataType();
  unique_ptr<DataType> id_data_type(id_data_type_ptr);
  ExprAnalysis id_expr_analysis(move(id_data_type));
  expr_analyzes.insert(make_pair(id_node_ptr, move(id_expr_analysis)));
  IntDataType *lit_data_type_ptr = new IntDataType();
  unique_ptr<DataType> lit_data_type(lit_data_type_ptr);
  ExprAnalysis lit_expr_analysis(move(lit_data_type));
  expr_analyzes.insert(make_pair(lit_node_ptr, move(lit_expr_analysis)));
  unique_ptr<DataType> assign_data_type(new IntDataType());
  ExprAnalysis assign_expr_analysis(move(assign_data_type));
  expr_analyzes.insert(make_pair(assign_node_ptr, move(assign_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr, id_analysis));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit(new IntLit(INT32_C(7)));
  LitAnalysis lit_analysis(move(lit));
  lit_analyzes.insert(make_pair(lit_node_ptr, move(lit_analysis)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = id_data_type_ptr;
  DataType *src_data_type = lit_data_type_ptr;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)},
                    {"2", INT32_C(2)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *lit_data_type_ptr1 = new IntDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1));
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));
  IntDataType *lit_data_type_ptr2 = new IntDataType();
  unique_ptr<DataType> lit_data_type2(lit_data_type_ptr2);
  ExprAnalysis lit_expr_analysis2(move(lit_data_type2));
  expr_analyzes.insert(make_pair(lit_node_ptr2, move(lit_expr_analysis2)));
  unique_ptr<DataType> less_data_type(new BoolDataType());
  ExprAnalysis less_expr_analysis(move(less_data_type));
  expr_analyzes.insert(make_pair(less_node_ptr, move(less_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new IntLit(INT32_C(1)));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));
  unique_ptr<Lit> lit2(new IntLit(INT32_C(2)));
  LitAnalysis lit_analysis2(move(lit2));
  lit_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = lit_data_type_ptr1;
  DataType *src_data_type = lit_data_type_ptr2;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, LessWithIncompatibleTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  StringNode *lit_node_ptr2 = new StringNode(
      TokenInfo(Token::kStringLit, "\"2\"", UINT32_C(2), UINT32_C(2)));
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
  test_lit_parses.strings = {{"\"2\"", "2"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *lit_data_type_ptr1 = new IntDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1));
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));
  StringDataType *lit_data_type_ptr2 = new StringDataType();
  unique_ptr<DataType> lit_data_type2(lit_data_type_ptr2);
  ExprAnalysis lit_expr_analysis2(move(lit_data_type2));
  expr_analyzes.insert(make_pair(lit_node_ptr2, move(lit_expr_analysis2)));
  unique_ptr<DataType> less_data_type(new BoolDataType());
  ExprAnalysis less_expr_analysis(move(less_data_type));
  expr_analyzes.insert(make_pair(less_node_ptr, move(less_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new IntLit(INT32_C(1)));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));
  unique_ptr<Lit> lit2(new StringLit("2"));
  LitAnalysis lit_analysis2(move(lit2));
  lit_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithIncompatibleTypeError(
      file_path, *less_node_ptr, *lit_data_type_ptr1, *lit_data_type_ptr2));
  problems.push_back(move(problem));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = lit_data_type_ptr1;
  DataType *src_data_type = lit_data_type_ptr2;
  bool is_data_type_convertable = false;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, LessWithUnsupportedTypesIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<DataTypeNode> array_element_data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))));
  unique_ptr<DataTypeNode> var_data_type_node(new ArrayDataTypeNode(
      move(array_element_data_type_node),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2))));
  VarDefWithoutInitNode *var_def_node_ptr = new VarDefWithoutInitNode(
      move(var_data_type_node),
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

  SemanticAnalysis::DefAnalyzes def_analyzes;
  unique_ptr<DataType> var_array_element_data_type(new IntDataType());
  unique_ptr<DataType> var_data_type(
      new ArrayDataType(move(var_array_element_data_type)));
  unique_ptr<DataStorage> var_data_storage(new GlobalDataStorage());
  unique_ptr<DefAnalysis> var_def_analysis(
      new VarDefAnalysis(move(var_data_type), move(var_data_storage)));
  def_analyzes.insert(make_pair(var_def_node_ptr, move(var_def_analysis)));

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  unique_ptr<DataType> id_array_element_data_type1(new IntDataType());
  ArrayDataType *id_data_type_ptr1 =
      new ArrayDataType(move(id_array_element_data_type1));
  unique_ptr<DataType> id_data_type1(id_data_type_ptr1);
  ExprAnalysis id_expr_analysis1(move(id_data_type1));
  expr_analyzes.insert(make_pair(id_node_ptr1, move(id_expr_analysis1)));
  unique_ptr<DataType> id_array_element_data_type2(new IntDataType());
  ArrayDataType *id_data_type_ptr2 =
      new ArrayDataType(move(id_array_element_data_type2));
  unique_ptr<DataType> id_data_type2(id_data_type_ptr2);
  ExprAnalysis id_expr_analysis2(move(id_data_type2));
  expr_analyzes.insert(make_pair(id_node_ptr2, move(id_expr_analysis2)));
  unique_ptr<DataType> less_data_type(new BoolDataType());
  ExprAnalysis less_expr_analysis(move(less_data_type));
  expr_analyzes.insert(make_pair(less_node_ptr, move(less_expr_analysis)));

  SemanticAnalysis::IdAnalyzes id_analyzes;
  IdAnalysis id_analysis1(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr1, move(id_analysis1)));
  IdAnalysis id_analysis2(var_def_node_ptr);
  id_analyzes.insert(make_pair(id_node_ptr2, move(id_analysis2)));

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithUnsupportedTypesError(
      file_path, *less_node_ptr, *id_data_type_ptr1, *id_data_type_ptr2));
  problems.push_back(move(problem));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = id_data_type_ptr1;
  DataType *src_data_type = id_data_type_ptr2;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
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
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)},
                    {"2", INT32_C(2)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *lit_data_type_ptr1 = new IntDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1));
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));
  IntDataType *lit_data_type_ptr2 = new IntDataType();
  unique_ptr<DataType> lit_data_type2(lit_data_type_ptr2);
  ExprAnalysis lit_expr_analysis2(move(lit_data_type2));
  expr_analyzes.insert(make_pair(lit_node_ptr2, move(lit_expr_analysis2)));
  unique_ptr<DataType> equal_data_type(new BoolDataType());
  ExprAnalysis equal_expr_analysis(move(equal_data_type));
  expr_analyzes.insert(make_pair(equal_node_ptr, move(equal_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new IntLit(INT32_C(1)));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));
  unique_ptr<Lit> lit2(new IntLit(INT32_C(2)));
  LitAnalysis lit_analysis2(move(lit2));
  lit_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = lit_data_type_ptr1;
  DataType *src_data_type = lit_data_type_ptr2;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, EqualWithIncompatibleTypeIsInvalid) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  IntNode *lit_node_ptr1 = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)));
  unique_ptr<ExprNode> lit_node1(lit_node_ptr1);
  StringNode *lit_node_ptr2 = new StringNode(
      TokenInfo(Token::kStringLit, "\"2\"", UINT32_C(2), UINT32_C(2)));
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
  test_lit_parses.ints = {{"1", INT32_C(1)}};
  test_lit_parses.strings = {{"\"2\"", "2"}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *lit_data_type_ptr1 = new IntDataType();
  unique_ptr<DataType> lit_data_type1(lit_data_type_ptr1);
  ExprAnalysis lit_expr_analysis1(move(lit_data_type1));
  expr_analyzes.insert(make_pair(lit_node_ptr1, move(lit_expr_analysis1)));
  StringDataType *lit_data_type_ptr2 = new StringDataType();
  unique_ptr<DataType> lit_data_type2(lit_data_type_ptr2);
  ExprAnalysis lit_expr_analysis2(move(lit_data_type2));
  expr_analyzes.insert(make_pair(lit_node_ptr2, move(lit_expr_analysis2)));
  unique_ptr<DataType> equal_data_type(new BoolDataType());
  ExprAnalysis equal_expr_analysis(move(equal_data_type));
  expr_analyzes.insert(make_pair(equal_node_ptr, move(equal_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> lit1(new IntLit(INT32_C(1)));
  LitAnalysis lit_analysis1(move(lit1));
  lit_analyzes.insert(make_pair(lit_node_ptr1, move(lit_analysis1)));
  unique_ptr<Lit> lit2(new StringLit("2"));
  LitAnalysis lit_analysis2(move(lit2));
  lit_analyzes.insert(make_pair(lit_node_ptr2, move(lit_analysis2)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  DataType *dest_data_type = lit_data_type_ptr1;
  DataType *src_data_type = lit_data_type_ptr2;
  bool is_data_type_convertable = false;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

  vector< unique_ptr<SemanticProblem> > problems;
  path file_path;
  unique_ptr<SemanticProblem> problem(new BinaryExprWithIncompatibleTypeError(
      file_path, *equal_node_ptr, *lit_data_type_ptr1, *lit_data_type_ptr2));
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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ArrayAllocWithoutInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<ArraySizeNode> > size_nodes;
  IntNode *size_value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> size_value_node(size_value_node_ptr);
  unique_ptr<ArraySizeNode> size_node(new ArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      move(size_value_node),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  ArrayAllocWithoutInitNode *alloc_node_ptr = new ArrayAllocWithoutInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes));
  unique_ptr<ExprNode> alloc_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *size_data_type_ptr = new IntDataType();
  unique_ptr<DataType> size_data_type(size_data_type_ptr);
  ExprAnalysis size_expr_analysis(move(size_data_type));
  expr_analyzes.insert(
      make_pair(size_value_node_ptr, move(size_expr_analysis)));
  unique_ptr<DataType> element_data_type(new IntDataType());
  unique_ptr<DataType> alloc_data_type(
      new ArrayDataType(move(element_data_type)));
  ExprAnalysis alloc_expr_analysis(move(alloc_data_type));
  expr_analyzes.insert(make_pair(alloc_node_ptr, move(alloc_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> size_lit(new IntLit(INT32_C(1)));
  LitAnalysis size_lit_analysis(move(size_lit));
  lit_analyzes.insert(make_pair(size_value_node_ptr, move(size_lit_analysis)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  unique_ptr<IntDataType> int_data_type(new IntDataType());
  DataType *dest_data_type = int_data_type.get();
  DataType *src_data_type = size_data_type_ptr;
  bool is_data_type_convertable = true;
  TestDataTypeConversion test_data_type_conversion =
      {dest_data_type, src_data_type, is_data_type_convertable};
  test_data_type_conversions.push_back(test_data_type_conversion);

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}

TEST_F(SimpleSemanticAnalyzerTest, ArrayAllocWithInit) {
  vector< unique_ptr<StmtNode> > stmt_nodes;
  unique_ptr<PrimitiveDataTypeNode> data_type_node(new IntDataTypeNode(
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1))));
  vector< unique_ptr<ArraySizeNode> > size_nodes;
  IntNode *size_value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)));
  unique_ptr<ExprNode> size_value_node(size_value_node_ptr);
  unique_ptr<ArraySizeNode> size_node(new ArraySizeNode(
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      move(size_value_node),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))));
  size_nodes.push_back(move(size_node));
  vector< unique_ptr<ExprNode> > value_nodes;
  IntNode *value_node_ptr = new IntNode(
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(6)));
  unique_ptr<ExprNode> value_node(value_node_ptr);
  value_nodes.push_back(move(value_node));
  vector<TokenInfo> value_separator_tokens;
  ArrayAllocWithInitNode *alloc_node_ptr = new ArrayAllocWithInitNode(
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      move(data_type_node),
      move(size_nodes),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      move(value_nodes),
      value_separator_tokens,
      TokenInfo(Token::kScopeStart, "}", UINT32_C(7), UINT32_C(7)));
  unique_ptr<ExprNode> alloc_node(alloc_node_ptr);
  unique_ptr<StmtNode> alloc_stmt_node(new ExprStmtNode(
      move(alloc_node),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8))));
  stmt_nodes.push_back(move(alloc_stmt_node));
  unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));

  TestLitParses test_lit_parses = {};
  test_lit_parses.ints = {{"1", INT32_C(1)},
                    {"2", INT32_C(2)}};

  SemanticAnalysis::ExprAnalyzes expr_analyzes;
  IntDataType *size_data_type_ptr = new IntDataType();
  unique_ptr<DataType> size_data_type(size_data_type_ptr);
  ExprAnalysis size_expr_analysis(move(size_data_type));
  expr_analyzes.insert(
      make_pair(size_value_node_ptr, move(size_expr_analysis)));
  IntDataType *element_data_type_ptr = new IntDataType();
  unique_ptr<DataType> element_data_type(element_data_type_ptr);
  unique_ptr<DataType> alloc_data_type(
      new ArrayDataType(move(element_data_type)));
  ExprAnalysis alloc_expr_analysis(move(alloc_data_type));
  expr_analyzes.insert(make_pair(alloc_node_ptr, move(alloc_expr_analysis)));
  IntDataType *value_data_type_ptr = new IntDataType();
  unique_ptr<DataType> value_data_type(value_data_type_ptr);
  ExprAnalysis value_expr_analysis(move(value_data_type));
  expr_analyzes.insert(make_pair(value_node_ptr, move(value_expr_analysis)));

  SemanticAnalysis::LitAnalyzes lit_analyzes;
  unique_ptr<Lit> size_lit(new IntLit(INT32_C(1)));
  LitAnalysis size_lit_analysis(move(size_lit));
  lit_analyzes.insert(make_pair(size_value_node_ptr, move(size_lit_analysis)));
  unique_ptr<Lit> value_lit(new IntLit(INT32_C(2)));
  LitAnalysis value_lit_analysis(move(value_lit));
  lit_analyzes.insert(make_pair(value_node_ptr, move(value_lit_analysis)));

  vector<TestDataTypeConversion> test_data_type_conversions;
  unique_ptr<IntDataType> int_data_type(new IntDataType());

  {
    DataType *dest_data_type = int_data_type.get();
    DataType *src_data_type = size_data_type_ptr;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);
  }

  {
    DataType *dest_data_type = element_data_type_ptr;
    DataType *src_data_type = value_data_type_ptr;
    bool is_data_type_convertable = true;
    TestDataTypeConversion test_data_type_conversion =
        {dest_data_type, src_data_type, is_data_type_convertable};
    test_data_type_conversions.push_back(test_data_type_conversion);
  }

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
  TestProgram test_program = {move(program_node),
                              move(analysis),
                              test_file_parses,
                              test_import_file_searches,
                              test_lit_parses,
                              test_data_type_conversions};

  TestAnalyze(test_program);
}
}
}
