
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <utility>
#include <limits>
#include "real_talk/lexer/lexer.h"
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/string_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/bool_node.h"
#include "real_talk/parser/mul_node.h"
#include "real_talk/parser/div_node.h"
#include "real_talk/parser/sum_node.h"
#include "real_talk/parser/sub_node.h"
#include "real_talk/parser/not_node.h"
#include "real_talk/parser/and_node.h"
#include "real_talk/parser/or_node.h"
#include "real_talk/parser/pre_inc_node.h"
#include "real_talk/parser/pre_dec_node.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/expr_stmt_node.h"
#include "real_talk/parser/program_node.h"
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/var_def_without_init_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/arg_def_node.h"
#include "real_talk/parser/array_alloc_with_init_node.h"
#include "real_talk/parser/array_alloc_without_init_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/greater_or_equal_node.h"
#include "real_talk/parser/less_or_equal_node.h"
#include "real_talk/parser/less_node.h"
#include "real_talk/parser/greater_node.h"
#include "real_talk/parser/equal_node.h"
#include "real_talk/parser/not_equal_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/if_else_if_node.h"
#include "real_talk/parser/if_else_if_else_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/parser/int_data_type_node.h"
#include "real_talk/parser/long_data_type_node.h"
#include "real_talk/parser/double_data_type_node.h"
#include "real_talk/parser/char_data_type_node.h"
#include "real_talk/parser/bool_data_type_node.h"
#include "real_talk/parser/string_data_type_node.h"
#include "real_talk/parser/void_data_type_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/negative_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/simple_parser.h"

using std::string;
using std::istringstream;
using std::unique_ptr;
using std::vector;
using std::move;
using std::make_move_iterator;
using std::numeric_limits;
using boost::lexical_cast;
using boost::format;
using testing::Test;
using testing::Return;
using testing::Between;
using testing::Invoke;
using testing::InSequence;
using testing::NiceMock;
using real_talk::lexer::Lexer;
using real_talk::lexer::Token;
using real_talk::lexer::TokenInfo;
using real_talk::parser::StmtNode;
using real_talk::parser::IntNode;
using real_talk::parser::LongNode;
using real_talk::parser::CharNode;
using real_talk::parser::StringNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::BoolNode;
using real_talk::parser::ExprNode;
using real_talk::parser::ExprStmtNode;
using real_talk::parser::NotNode;
using real_talk::parser::AndNode;
using real_talk::parser::OrNode;
using real_talk::parser::PreIncNode;
using real_talk::parser::PreDecNode;
using real_talk::parser::SumNode;
using real_talk::parser::SubNode;
using real_talk::parser::MulNode;
using real_talk::parser::DivNode;
using real_talk::parser::ProgramNode;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::VarDefWithoutInitNode;
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::FuncDefWithoutBodyNode;
using real_talk::parser::ArgDefNode;
using real_talk::parser::ArrayAllocWithInitNode;
using real_talk::parser::ArrayAllocWithoutInitNode;
using real_talk::parser::IdNode;
using real_talk::parser::SubscriptNode;
using real_talk::parser::AssignNode;
using real_talk::parser::GreaterOrEqualNode;
using real_talk::parser::LessOrEqualNode;
using real_talk::parser::LessNode;
using real_talk::parser::GreaterNode;
using real_talk::parser::EqualNode;
using real_talk::parser::NotEqualNode;
using real_talk::parser::CallNode;
using real_talk::parser::DataTypeNode;
using real_talk::parser::IntDataTypeNode;
using real_talk::parser::LongDataTypeNode;
using real_talk::parser::DoubleDataTypeNode;
using real_talk::parser::CharDataTypeNode;
using real_talk::parser::BoolDataTypeNode;
using real_talk::parser::StringDataTypeNode;
using real_talk::parser::VoidDataTypeNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::IfElseIfNode;
using real_talk::parser::IfElseIfElseNode;
using real_talk::parser::ImportNode;
using real_talk::parser::BreakNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::NegativeNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::SimpleParser;

namespace real_talk {
namespace parser {

template<typename TNode> struct TestNode {
  vector<TokenInfo> tokens;
  unique_ptr<TNode> node;
};

struct TestDataTypeNode {
  TokenInfo token;
  unique_ptr<PrimitiveDataTypeNode> node;
};

struct MailformedTestTokens {
  vector<TokenInfo> tokens;
  TokenInfo unexpected_token;
};

class LexerMock: public Lexer {
 public:
  MOCK_METHOD0(GetNextToken, TokenInfo());
};

class SimpleParserTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

  void TestParse(const TestNode<ProgramNode> &test_program_node) {
    LexerMock *lexer_ptr = new NiceMock<LexerMock>();
    unique_ptr<Lexer> lexer(lexer_ptr);

    {
      InSequence sequence;

      for (const TokenInfo &token: test_program_node.tokens) {
        EXPECT_CALL(*lexer_ptr, GetNextToken())
            .WillOnce(Return(token))
            .RetiresOnSaturation();
      }
    }

    SimpleParser parser;
    unique_ptr<ProgramNode> actual_program_node = parser.Parse(lexer_ptr);
    ASSERT_EQ(*(test_program_node.node), *(actual_program_node))
        << "expected:\n" << *(test_program_node.node)
        << "actual:\n" << *(actual_program_node);
  }

  void TestFailingParse(const MailformedTestTokens &mailformed_tokens) {
    LexerMock *lexer_ptr = new NiceMock<LexerMock>();
    unique_ptr<Lexer> lexer(lexer_ptr);
    vector<TokenInfo>::const_iterator next_token_it =
        mailformed_tokens.tokens.begin();
    auto next_token_generator = [&next_token_it]() {
      return *(next_token_it++);
    };
    ON_CALL(*lexer_ptr, GetNextToken())
        .WillByDefault(Invoke(next_token_generator));
    SimpleParser parser;

    try {
      unique_ptr<ProgramNode> program_node = parser.Parse(lexer_ptr);
      FAIL() << "expected_bad_token=" << mailformed_tokens.unexpected_token
             << "\nprogram:\n" << *program_node;
    } catch (const Parser::UnexpectedTokenError &error) {
      ASSERT_EQ(mailformed_tokens.unexpected_token, error.GetToken());
    }
  }

  TestNode<ProgramNode> ExprToProgram(TestNode<ExprNode> &test_expr_node) {
    TokenInfo stmt_end_token(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(2));
    unique_ptr<StmtNode> stmt_node(
        new ExprStmtNode(move(test_expr_node.node), stmt_end_token));
    vector< unique_ptr<StmtNode> > stmt_nodes;
    stmt_nodes.push_back(move(stmt_node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));
    TestNode<ProgramNode> test_program_node =
        {test_expr_node.tokens, move(program_node)};
    test_program_node.tokens.push_back(stmt_end_token);
    test_program_node.tokens.push_back(
        TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(4)));
    return test_program_node;
  }

  TestNode<ProgramNode> StmtToProgram(TestNode<StmtNode> &test_stmt_node) {
    vector< unique_ptr<StmtNode> > stmt_nodes;
    stmt_nodes.push_back(move(test_stmt_node.node));
    unique_ptr<ProgramNode> program_node(new ProgramNode(move(stmt_nodes)));
    test_stmt_node.tokens.push_back(
        TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(2)));
    TestNode<ProgramNode> test_program_node =
        {test_stmt_node.tokens, move(program_node)};
    return test_program_node;
  }

  vector<TestDataTypeNode> GetTestDataTypes() {
    vector<TestDataTypeNode> test_data_types;

    {
      TokenInfo token(Token::kIntType, "int", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(new IntDataTypeNode(token))});
    }

    {
      TokenInfo token(Token::kLongType, "long", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(new LongDataTypeNode(token))});
    }

    {
      TokenInfo token(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(
                  new DoubleDataTypeNode(token))});
    }

    {
      TokenInfo token(Token::kCharType, "char", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(new CharDataTypeNode(token))});
    }

    {
      TokenInfo token(Token::kStringType, "string", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(
                  new StringDataTypeNode(token))});
    }

    {
      TokenInfo token(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(new BoolDataTypeNode(token))});
    }

    {
      TokenInfo token(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0));
      test_data_types.push_back({token,
              unique_ptr<PrimitiveDataTypeNode>(new VoidDataTypeNode(token))});
    }

    return test_data_types;
  }
};

TEST_F(SimpleParserTest, Bool) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens =
        {TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(1), UINT32_C(2))};
    unique_ptr<ExprNode> bool_node(new BoolNode(tokens.front()));
    test_expr_nodes.push_back({tokens, move(bool_node)});
  }

  {
    vector<TokenInfo> tokens =
        {TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(1), UINT32_C(2))};
    unique_ptr<ExprNode> bool_node(new BoolNode(tokens.front()));
    test_expr_nodes.push_back({tokens, move(bool_node)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Int) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "7", UINT32_C(1), UINT32_C(2))
  };
  unique_ptr<ExprNode> int_node(new IntNode(tokens.front()));
  test_expr_nodes.push_back({tokens, move(int_node)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Long) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kLongLit, "7L", UINT32_C(1), UINT32_C(2))
  };
  unique_ptr<ExprNode> long1(new LongNode(tokens.front()));
  test_expr_nodes.push_back({tokens, move(long1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Char) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kCharLit, "'a'", UINT32_C(1), UINT32_C(2))
  };
  unique_ptr<ExprNode> char1(new CharNode(tokens.front()));
  test_expr_nodes.push_back({tokens, move(char1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, String) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kStringLit, "\"test\"", UINT32_C(1), UINT32_C(2))
  };
  unique_ptr<ExprNode> string1(new StringNode(tokens.front()));
  test_expr_nodes.push_back({tokens, move(string1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Double) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kDoubleLit, "7.7", UINT32_C(1), UINT32_C(2))
  };
  unique_ptr<ExprNode> double1(new DoubleNode(tokens.front()));
  test_expr_nodes.push_back({tokens, move(double1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Sub) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kSubOp, "-", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> sub1(new SubNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(sub1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, SubIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kSubOp, "-", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kSubOp, "-", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> sub1(new SubNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> sub2(new SubNode(tokens[3], move(sub1), move(int3)));
  test_expr_nodes.push_back({tokens, move(sub2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, SubPrioSameAsSum) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kSubOp, "-", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> sub1(new SubNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> sum1(new SumNode(tokens[3], move(sub1), move(int3)));
    test_expr_nodes.push_back({tokens, move(sum1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kSubOp, "-", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> sum1(new SumNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> sub1(new SubNode(tokens[3], move(sum1), move(int3)));
    test_expr_nodes.push_back({tokens, move(sub1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Sum) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kSumOp, "+", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> sum(new SumNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(sum)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, SumIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kSumOp, "+", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kSumOp, "+", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> sum1(new SumNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> sum2(new SumNode(tokens[3], move(sum1), move(int3)));
  test_expr_nodes.push_back({tokens, move(sum2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, SumPrecedesGreaterOrEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(9), UINT32_C(10)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(11), UINT32_C(12)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(13), UINT32_C(14))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> sum1(new SumNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[3], move(sum1), move(int3)));
    test_expr_nodes.push_back({tokens, move(greater_or_equal1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(9), UINT32_C(10)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(11), UINT32_C(12)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(13), UINT32_C(14))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> sum1(new SumNode(tokens[3], move(int2), move(int3)));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[1], move(int1), move(sum1)));
    test_expr_nodes.push_back({tokens, move(greater_or_equal1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Mul) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kMulOp, "*", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> mul(new MulNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(mul)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, MulIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kMulOp, "*", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kMulOp, "*", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> mul1(new MulNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> mul2(new MulNode(tokens[3], move(mul1), move(int3)));
  test_expr_nodes.push_back({tokens, move(mul2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, MulPrecedesSum) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(6), UINT32_C(7)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(8), UINT32_C(9))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> mul(new MulNode(tokens[3], move(int2), move(int3)));
    unique_ptr<ExprNode> sum(new SumNode(tokens[1], move(int1), move(mul)));
    test_expr_nodes.push_back({tokens, move(sum)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(6), UINT32_C(7)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(8), UINT32_C(9))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> mul(new MulNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> sum(new SumNode(tokens[3], move(mul), move(int3)));
    test_expr_nodes.push_back({tokens, move(sum)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Div) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kDivOp, "/", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> div1(new DivNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(div1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, DivPrioSameAsMul) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kDivOp, "/", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> div1(new DivNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> mul1(new MulNode(tokens[3], move(div1), move(int3)));
    test_expr_nodes.push_back({tokens, move(mul1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kDivOp, "/", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> mul1(new MulNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> div1(new DivNode(tokens[3], move(mul1), move(int3)));
    test_expr_nodes.push_back({tokens, move(div1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, DivIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kDivOp, "/", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kDivOp, "/", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> div1(new DivNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> div2(new DivNode(tokens[3], move(div1), move(int3)));
  test_expr_nodes.push_back({tokens, move(div2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Not) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(3), UINT32_C(4)),
  };
  unique_ptr<ExprNode> bool_node(new BoolNode(tokens[1]));
  unique_ptr<ExprNode> not_node(new NotNode(tokens[0], move(bool_node)));
  test_expr_nodes.push_back({tokens, move(not_node)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NotIsRightAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kNotOp, "!", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(6)),
  };
  unique_ptr<ExprNode> bool_node(new BoolNode(tokens[2]));
  unique_ptr<ExprNode> not1(new NotNode(tokens[1], move(bool_node)));
  unique_ptr<ExprNode> not2(new NotNode(tokens[0], move(not1)));
  test_expr_nodes.push_back({tokens, move(not2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NotPrecedesMul) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kNotOp, "!", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(7))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[3]));
    unique_ptr<ExprNode> not1(new NotNode(tokens[2], move(int2)));
    unique_ptr<ExprNode> mul1(new MulNode(tokens[1], move(int1), move(not1)));
    test_expr_nodes.push_back({tokens, move(mul1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(3)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(4), UINT32_C(5)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(6), UINT32_C(7))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[1]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[3]));
    unique_ptr<ExprNode> not1(new NotNode(tokens[0], move(int1)));
    unique_ptr<ExprNode> mul1(new MulNode(tokens[2], move(not1), move(int2)));
    test_expr_nodes.push_back({tokens, move(mul1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, And) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kAndOp, "&&", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> and1(new AndNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(and1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, AndIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kAndOp, "&&", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kAndOp, "&&", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> and1(new AndNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> and2(new AndNode(tokens[3], move(and1), move(int3)));
  test_expr_nodes.push_back({tokens, move(and2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, AndPrecedesOr) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kAndOp, "&&", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kOrOp, "||", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> and1(new AndNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> or1(new OrNode(tokens[3], move(and1), move(int3)));
    test_expr_nodes.push_back({tokens, move(or1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kOrOp, "||", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kAndOp, "&&", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> and1(new AndNode(tokens[3], move(int2), move(int3)));
    unique_ptr<ExprNode> or1(new OrNode(tokens[1], move(int1), move(and1)));
    test_expr_nodes.push_back({tokens, move(or1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Or) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kOrOp, "||", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> or1(new OrNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(or1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, OrIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kOrOp, "||", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kOrOp, "||", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> or1(new OrNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> or2(new OrNode(tokens[3], move(or1), move(int3)));
  test_expr_nodes.push_back({tokens, move(or2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, OrPrecedesAssign) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kOrOp, "||", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[4]));
    unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
    unique_ptr<ExprNode> or1(new OrNode(tokens[3], move(int1), move(int2)));
    unique_ptr<ExprNode> assign1(
        new AssignNode(tokens[1], move(id1), move(or1)));
    test_expr_nodes.push_back({tokens, move(assign1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kOrOp, "||", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kName, "myVar2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[4]));
    unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
    unique_ptr<ExprNode> id2(new IdNode(tokens[2]));
    unique_ptr<ExprNode> or1(
        new OrNode(tokens[1], move(id1), move(id2)));
    unique_ptr<ExprNode> assign1(
        new AssignNode(tokens[3], move(or1), move(int1)));
    test_expr_nodes.push_back({tokens, move(assign1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, PreDec) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(4)),
  };
  unique_ptr<ExprNode> int_node(new IntNode(tokens[1]));
  unique_ptr<ExprNode> pre_dec_node(new PreDecNode(tokens[0], move(int_node)));
  test_expr_nodes.push_back({tokens, move(pre_dec_node)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, PreDecIsRightAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kPreDecOp, "--", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
  unique_ptr<ExprNode> pre_dec1(new PreDecNode(tokens[1], move(int1)));
  unique_ptr<ExprNode> pre_dec2(new PreDecNode(tokens[0], move(pre_dec1)));
  test_expr_nodes.push_back({tokens, move(pre_dec2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, PreDecPrioSameAsNot) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kNotOp, "!", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> not1(new NotNode(tokens[1], move(int1)));
    unique_ptr<ExprNode> pre_dec1(new PreDecNode(tokens[0], move(not1)));
    test_expr_nodes.push_back({tokens, move(pre_dec1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> pre_dec1(new PreDecNode(tokens[1], move(int1)));
    unique_ptr<ExprNode> not1(new NotNode(tokens[0], move(pre_dec1)));
    test_expr_nodes.push_back({tokens, move(not1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Negative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kSubOp, "-", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(1), UINT32_C(1)),
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[1]));
  unique_ptr<ExprNode> negative1(new NegativeNode(tokens[0], move(int1)));
  test_expr_nodes.push_back({tokens, move(negative1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NegativeIsRightAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kSubOp, "-", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(2), UINT32_C(2)),
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
  unique_ptr<ExprNode> negative1(new NegativeNode(tokens[1], move(int1)));
  unique_ptr<ExprNode> negative2(new NegativeNode(tokens[0], move(negative1)));
  test_expr_nodes.push_back({tokens, move(negative2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NegativePrioSameAsNot) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubOp, "-", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kNotOp, "!", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> not1(new NotNode(tokens[1], move(int1)));
    unique_ptr<ExprNode> negative1(new NegativeNode(tokens[0], move(not1)));
    test_expr_nodes.push_back({tokens, move(negative1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kSubOp, "-", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> negative1(new NegativeNode(tokens[1], move(int1)));
    unique_ptr<ExprNode> not1(new NotNode(tokens[0], move(negative1)));
    test_expr_nodes.push_back({tokens, move(not1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, PreInc) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kPreIncOp, "++", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(3), UINT32_C(4)),
  };
  unique_ptr<ExprNode> int_node(new IntNode(tokens[1]));
  unique_ptr<ExprNode> pre_inc_node(new PreIncNode(tokens[0], move(int_node)));
  test_expr_nodes.push_back({tokens, move(pre_inc_node)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, PreIncIsRightAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kPreIncOp, "++", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kPreIncOp, "++", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
  unique_ptr<ExprNode> pre_inc1(new PreIncNode(tokens[1], move(int1)));
  unique_ptr<ExprNode> pre_inc2(new PreIncNode(tokens[0], move(pre_inc1)));
  test_expr_nodes.push_back({tokens, move(pre_inc2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, PreIncPrioSameAsNot) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kNotOp, "!", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> not1(new NotNode(tokens[1], move(int1)));
    unique_ptr<ExprNode> pre_inc1(new PreIncNode(tokens[0], move(not1)));
    test_expr_nodes.push_back({tokens, move(pre_inc1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> pre_inc1(new PreIncNode(tokens[1], move(int1)));
    unique_ptr<ExprNode> not1(new NotNode(tokens[0], move(pre_inc1)));
    test_expr_nodes.push_back({tokens, move(not1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Subscript) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(8))
  };
  unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
  unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
  unique_ptr<ExprNode> subscript1(
      new SubscriptNode(tokens[1], tokens[3], move(id1), move(int1)));
  test_expr_nodes.push_back({tokens, move(subscript1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, SubscriptIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kSubscriptStart, "[", UINT32_C(9), UINT32_C(10)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(11), UINT32_C(12)),
    TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(13), UINT32_C(14))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[5]));
  unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
  unique_ptr<ExprNode> subscript1(
      new SubscriptNode(tokens[1], tokens[3], move(id1), move(int1)));
  unique_ptr<ExprNode> subscript2(
      new SubscriptNode(tokens[4], tokens[6], move(subscript1), move(int2)));
  test_expr_nodes.push_back({tokens, move(subscript2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, SubscriptPrecedesNot) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kName, "myVar", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kSubscriptStart, "[", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> id1(new IdNode(tokens[1]));
  unique_ptr<ExprNode> int1(new IntNode(tokens[3]));
  unique_ptr<ExprNode> subscript1(
      new SubscriptNode(tokens[2], tokens[4], move(id1), move(int1)));
  unique_ptr<ExprNode> not1(
      new NotNode(tokens[0], move(subscript1)));
  test_expr_nodes.push_back({tokens, move(not1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Assign) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kAssignOp, "=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
  unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
  unique_ptr<ExprNode> assign1(
      new AssignNode(tokens[1], move(id1), move(int1)));
  test_expr_nodes.push_back({tokens, move(assign1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, AssignIsRightAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kAssignOp, "=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kName, "myVar2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kAssignOp, "=", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[4]));
  unique_ptr<ExprNode> id1(new IdNode(tokens[2]));
  unique_ptr<ExprNode> id2(new IdNode(tokens[0]));
  unique_ptr<ExprNode> assign1(
      new AssignNode(tokens[3], move(id1), move(int1)));
  unique_ptr<ExprNode> assign2(
      new AssignNode(tokens[1], move(id2), move(assign1)));
  test_expr_nodes.push_back({tokens, move(assign2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NotEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> not_equal1(
      new NotEqualNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(not_equal1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NotEqualIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> not_equal1(
      new NotEqualNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> not_equal2(
      new NotEqualNode(tokens[3], move(not_equal1), move(int3)));
  test_expr_nodes.push_back({tokens, move(not_equal2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, NotEqualPrioSameAsEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> not_equal1(
        new NotEqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> equal1(
        new EqualNode(tokens[3], move(not_equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(equal1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> equal1(
        new EqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> not_equal1(
        new NotEqualNode(tokens[3], move(equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(not_equal1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Equal) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kEqualOp, "==", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> equal1(
      new EqualNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(equal1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, EqualIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kEqualOp, "==", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kEqualOp, "==", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> equal1(
      new EqualNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> equal2(
      new EqualNode(tokens[3], move(equal1), move(int3)));
  test_expr_nodes.push_back({tokens, move(equal2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, EqualPrecedesAnd) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kAndOp, "&&", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> equal1(
        new EqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> and1(
        new AndNode(tokens[3], move(equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(and1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kAndOp, "&&", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> equal1(
        new EqualNode(tokens[3], move(int2), move(int3)));
    unique_ptr<ExprNode> and1(
        new AndNode(tokens[1], move(int1), move(equal1)));
    test_expr_nodes.push_back({tokens, move(and1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Greater) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGreaterOp, ">", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> greater(
      new GreaterNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(greater)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, GreaterIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGreaterOp, ">", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGreaterOp, ">", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> greater1(
      new GreaterNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> greater2(
      new GreaterNode(tokens[3], move(greater1), move(int3)));
  test_expr_nodes.push_back({tokens, move(greater2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, GreaterPrioSameAsGreaterOrEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> greater1(
        new GreaterNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[3], move(greater1), move(int3)));
    test_expr_nodes.push_back({tokens, move(greater_or_equal1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> greater1(
        new GreaterNode(tokens[3], move(greater_or_equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(greater1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Less) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kLessOp, "<", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> less(
      new LessNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(less)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, LessIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kLessOp, "<", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kLessOp, "<", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> less1(
      new LessNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> less2(
      new LessNode(tokens[3], move(less1), move(int3)));
  test_expr_nodes.push_back({tokens, move(less2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, LessPrioSameAsGreaterOrEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kLessOp, "<", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> less1(
        new LessNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[3], move(less1), move(int3)));
    test_expr_nodes.push_back({tokens, move(greater_or_equal1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kLessOp, "<", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> less1(
        new LessNode(tokens[3], move(greater_or_equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(less1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, LessOrEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> less_or_equal(
      new LessOrEqualNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(less_or_equal)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, LessOrEqualIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> less_or_equal1(
      new LessOrEqualNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> less_or_equal2(
      new LessOrEqualNode(tokens[3], move(less_or_equal1), move(int3)));
  test_expr_nodes.push_back({tokens, move(less_or_equal2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, LessOrEqualPrioSameAsGreaterOrEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> less_or_equal1(
        new LessOrEqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[3], move(less_or_equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(greater_or_equal1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> less_or_equal1(
        new LessOrEqualNode(tokens[3], move(greater_or_equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(less_or_equal1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, GreaterOrEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> greater_or_equal(
      new GreaterOrEqualNode(tokens[1], move(int1), move(int2)));
  test_expr_nodes.push_back({tokens, move(greater_or_equal)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, GreaterOrEqualIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
  unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
  unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
  unique_ptr<ExprNode> greater_or_equal1(
      new GreaterOrEqualNode(tokens[1], move(int1), move(int2)));
  unique_ptr<ExprNode> greater_or_equal2(
      new GreaterOrEqualNode(tokens[3], move(greater_or_equal1), move(int3)));
  test_expr_nodes.push_back({tokens, move(greater_or_equal2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, GreaterOrEqualPrecedesEqual) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[1], move(int1), move(int2)));
    unique_ptr<ExprNode> equal1(
        new EqualNode(tokens[3], move(greater_or_equal1), move(int3)));
    test_expr_nodes.push_back({tokens, move(equal1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(9), UINT32_C(10))
    };
    unique_ptr<ExprNode> int1(new IntNode(tokens[0]));
    unique_ptr<ExprNode> int2(new IntNode(tokens[2]));
    unique_ptr<ExprNode> int3(new IntNode(tokens[4]));
    unique_ptr<ExprNode> greater_or_equal1(
        new GreaterOrEqualNode(tokens[3], move(int2), move(int3)));
    unique_ptr<ExprNode> equal1(
        new EqualNode(tokens[1], move(int1), move(greater_or_equal1)));
    test_expr_nodes.push_back({tokens, move(equal1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Id) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myVar", UINT32_C(1), UINT32_C(2))
  };
  unique_ptr<ExprNode> id(new IdNode(tokens[0]));
  test_expr_nodes.push_back({tokens, move(id)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, Group) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(6))
  };
  unique_ptr<ExprNode> int1(new IntNode(tokens[1]));
  test_expr_nodes.push_back({tokens, move(int1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, CallWithArgs) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "myFunc", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kSeparator, ",", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(9), UINT32_C(10)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(11), UINT32_C(12))
    };
    unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
    const TokenInfo op_start_token = tokens[1];
    vector< unique_ptr<ExprNode> > args1;
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    args1.push_back(move(int1));
    unique_ptr<ExprNode> int2(new IntNode(tokens[4]));
    args1.push_back(move(int2));
    const vector<TokenInfo> arg_separator_tokens = {tokens[3]};
    const TokenInfo op_end_token = tokens[5];
    unique_ptr<ExprNode> call1(new CallNode(
        move(id1),
        op_start_token,
        move(args1),
        arg_separator_tokens,
        op_end_token));
    test_expr_nodes.push_back({tokens, move(call1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "myFunc", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(11), UINT32_C(12))
    };
    unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
    const TokenInfo op_start_token = tokens[1];
    vector< unique_ptr<ExprNode> > args1;
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    args1.push_back(move(int1));
    const vector<TokenInfo> arg_separator_tokens;
    const TokenInfo op_end_token = tokens[3];
    unique_ptr<ExprNode> call1(new CallNode(
        move(id1),
        op_start_token,
        move(args1),
        arg_separator_tokens,
        op_end_token));
    test_expr_nodes.push_back({tokens, move(call1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, CallWithoutArgs) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myFunc", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(11), UINT32_C(12))
  };
  unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
  const TokenInfo op_start_token = tokens[1];
  vector< unique_ptr<ExprNode> > args1;
  const vector<TokenInfo> arg_separator_tokens;
  const TokenInfo op_end_token = tokens[2];
  unique_ptr<ExprNode> call1(new CallNode(
      move(id1),
      op_start_token,
      move(args1),
      arg_separator_tokens,
      op_end_token));
  test_expr_nodes.push_back({tokens, move(call1)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, CallIsLeftAssociative) {
  vector< TestNode<ExprNode> > test_expr_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kName, "myFunc", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(10))
  };
  unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
  const TokenInfo op_start_token1 = tokens[1];
  vector< unique_ptr<ExprNode> > args1;
  const vector<TokenInfo> arg_separator_tokens1;
  const TokenInfo op_end_token1 = tokens[2];
  unique_ptr<ExprNode> call1(new CallNode(
      move(id1),
      op_start_token1,
      move(args1),
      arg_separator_tokens1,
      op_end_token1));
  const TokenInfo op_start_token2 = tokens[3];
  vector< unique_ptr<ExprNode> > args2;
  const vector<TokenInfo> arg_separator_tokens2;
  const TokenInfo op_end_token2 = tokens[4];
  unique_ptr<ExprNode> call2(new CallNode(
      move(call1),
      op_start_token2,
      move(args2),
      arg_separator_tokens2,
      op_end_token2));
  test_expr_nodes.push_back({tokens, move(call2)});

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, CallPrioSameAsSubscript) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "myFunc", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(10)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(11), UINT32_C(12))
    };
    unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
    const TokenInfo call_op_start_token = tokens[1];
    vector< unique_ptr<ExprNode> > call_args;
    const vector<TokenInfo> call_arg_separator_tokens;
    const TokenInfo call_op_end_token = tokens[2];
    unique_ptr<ExprNode> call1(new CallNode(
        move(id1),
        call_op_start_token,
        move(call_args),
        call_arg_separator_tokens,
        call_op_end_token));
    unique_ptr<ExprNode> int1(new IntNode(tokens[4]));
    unique_ptr<ExprNode> subscript1(
        new SubscriptNode(tokens[3], tokens[5], move(call1), move(int1)));
    test_expr_nodes.push_back({tokens, move(subscript1)});
  }

  {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "myFunc", UINT32_C(1), UINT32_C(2)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(9), UINT32_C(10)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(11), UINT32_C(12))
    };
    unique_ptr<ExprNode> id1(new IdNode(tokens[0]));
    unique_ptr<ExprNode> int1(new IntNode(tokens[2]));
    unique_ptr<ExprNode> subscript1(
        new SubscriptNode(tokens[1], tokens[3], move(id1), move(int1)));
    const TokenInfo call_op_start_token = tokens[4];
    vector< unique_ptr<ExprNode> > call_args;
    const vector<TokenInfo> call_arg_separator_tokens;
    const TokenInfo call_op_end_token = tokens[5];
    unique_ptr<ExprNode> call1(new CallNode(
        move(subscript1),
        call_op_start_token,
        move(call_args),
        call_arg_separator_tokens,
        call_op_end_token));
    test_expr_nodes.push_back({tokens, move(call1)});
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes) {
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, ArrayAllocWithoutInit) {
  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      test_data_type.token,
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &op_token = tokens[0];
    vector< unique_ptr<BoundedArraySizeNode> > sizes;
    unique_ptr<ExprNode> size_value(new IntNode(tokens[3]));
    unique_ptr<BoundedArraySizeNode> size(new BoundedArraySizeNode(
        tokens[2], move(size_value), tokens[4]));
    sizes.push_back(move(size));
    unique_ptr<ExprNode> array_alloc(new ArrayAllocWithoutInitNode(
        op_token, move(test_data_type.node), move(sizes)));
    TestNode<ExprNode> test_expr_node = {tokens, move(array_alloc)};

    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, ArrayAllocWithInit) {
  vector< TestNode<ExprNode> > test_expr_nodes;

  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      test_data_type.token,
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kIntLit, "3", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kSeparator, ",", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIntLit, "4", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(8), UINT32_C(8))
    };
    const TokenInfo op_token = tokens[0];
    vector< unique_ptr<UnboundedArraySizeNode> > sizes;
    unique_ptr<UnboundedArraySizeNode> size(new UnboundedArraySizeNode(
        tokens[2], tokens[3]));
    sizes.push_back(move(size));
    const TokenInfo values_start_token = tokens[4];
    vector< unique_ptr<ExprNode> > values;
    unique_ptr<ExprNode> value1(new IntNode(tokens[5]));
    values.push_back(move(value1));
    unique_ptr<ExprNode> value2(new IntNode(tokens[7]));
    values.push_back(move(value2));
    vector<TokenInfo> value_separator_tokens;
    value_separator_tokens.push_back(tokens[6]);
    const TokenInfo values_end_token = tokens[8];
    unique_ptr<ExprNode> array_alloc(new ArrayAllocWithInitNode(
        op_token,
        move(test_data_type.node),
        move(sizes),
        values_start_token,
        move(values),
        value_separator_tokens,
        values_end_token));
    TestNode<ExprNode> test_expr_node = {tokens, move(array_alloc)};
    test_expr_nodes.push_back(move(test_expr_node));
  }

  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      test_data_type.token,
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo op_token = tokens[0];
    vector< unique_ptr<UnboundedArraySizeNode> > sizes;
    unique_ptr<UnboundedArraySizeNode> size(new UnboundedArraySizeNode(
        tokens[2], tokens[3]));
    sizes.push_back(move(size));
    const TokenInfo values_start_token = tokens[4];
    vector< unique_ptr<ExprNode> > values;
    vector<TokenInfo> value_separator_tokens;
    const TokenInfo values_end_token = tokens[5];
    unique_ptr<ExprNode> array_alloc(new ArrayAllocWithInitNode(
        op_token,
        move(test_data_type.node),
        move(sizes),
        values_start_token,
        move(values),
        value_separator_tokens,
        values_end_token));
    TestNode<ExprNode> test_expr_node = {tokens, move(array_alloc)};
    test_expr_nodes.push_back(move(test_expr_node));
  }

  for (TestNode<ExprNode> &test_expr_node: test_expr_nodes){
    TestParse(ExprToProgram(test_expr_node));
  }
}

TEST_F(SimpleParserTest, FuncDefWithBodyAndModifiersAndArgs) {
  struct TestData {
    TestDataTypeNode return_data_type;
    TestDataTypeNode arg_data_type1;
    TestDataTypeNode arg_data_type2;
  };
  vector<TestData> test_data_suits;

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new IntDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new IntDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kIntType, "int", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new IntDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new LongDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kLongType, "long", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new LongDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new LongDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new DoubleDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kDoubleType, "double", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new DoubleDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kDoubleType, "double", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new DoubleDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new BoolDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kBoolType, "bool", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new BoolDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kBoolType, "bool", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new BoolDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new CharDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kCharType, "char", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new CharDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kCharType, "char", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new CharDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new StringDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kStringType, "string", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new StringDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kStringType, "string", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new StringDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0)),
      test_data.return_data_type.token,
      TokenInfo(Token::kName, "myFunc", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      test_data.arg_data_type1.token,
      TokenInfo(Token::kName, "arg1", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kSeparator, ",", UINT32_C(6), UINT32_C(6)),
      test_data.arg_data_type2.token,
      TokenInfo(Token::kName, "arg2", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kName, "arg1", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(12), UINT32_C(12)),
      TokenInfo(Token::kName, "arg2", UINT32_C(13), UINT32_C(13)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(14), UINT32_C(14)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(15), UINT32_C(15))
    };
    const vector<TokenInfo> modifier_tokens = {tokens[0]};
    unique_ptr<DataTypeNode> return_data_type(
        move(test_data.return_data_type.node));
    const TokenInfo func_name_token = tokens[2];
    const TokenInfo args_start_token = tokens[3];
    vector< unique_ptr<ArgDefNode> > args;
    unique_ptr<ArgDefNode> arg1(new ArgDefNode(
        move(test_data.arg_data_type1.node), tokens[5]));
    args.push_back(move(arg1));
    unique_ptr<ArgDefNode> arg2(new ArgDefNode(
        move(test_data.arg_data_type2.node), tokens[8]));
    args.push_back(move(arg2));
    const vector<TokenInfo> arg_separator_tokens = {tokens[6]};
    const TokenInfo args_end_token = tokens[9];
    const TokenInfo body_start_token = tokens[10];
    vector< unique_ptr<StmtNode> > body_stmts;
    unique_ptr<ExprNode> id1(new IdNode(tokens[11]));
    unique_ptr<StmtNode> body_stmt1(
        new ExprStmtNode(move(id1), tokens[12]));
    body_stmts.push_back(move(body_stmt1));
    unique_ptr<ExprNode> id2(new IdNode(tokens[13]));
    unique_ptr<StmtNode> body_stmt2(
        new ExprStmtNode(move(id2), tokens[14]));
    body_stmts.push_back(move(body_stmt2));
    const TokenInfo body_end_token = tokens[15];
    unique_ptr<ScopeNode> body(
        new ScopeNode(body_start_token, move(body_stmts), body_end_token));
    unique_ptr<StmtNode> func_def1(new FuncDefWithBodyNode(
        modifier_tokens,
        move(return_data_type),
        func_name_token,
        args_start_token,
        move(args),
        arg_separator_tokens,
        args_end_token,
        move(body)));
    TestNode<StmtNode> test_stmt = {tokens, move(func_def1)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, FuncDefWithBodyWithoutModifiersAndArgs) {
  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      test_data_type.token,
      TokenInfo(Token::kName, "myFunc", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(17), UINT32_C(18)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(19), UINT32_C(20)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(33), UINT32_C(34))
    };
    const vector<TokenInfo> modifier_tokens;
    unique_ptr<DataTypeNode> return_data_type(move(test_data_type.node));
    const TokenInfo name_token = tokens[1];
    const TokenInfo args_start_token = tokens[2];
    vector< unique_ptr<ArgDefNode> > args;
    const TokenInfo args_end_token = tokens[3];
    const vector<TokenInfo> arg_separator_tokens;
    vector< unique_ptr<StmtNode> > body_stmts;
    unique_ptr<ScopeNode> body(
        new ScopeNode(tokens[4], move(body_stmts), tokens[5]));
    unique_ptr<StmtNode> func_def1(new FuncDefWithBodyNode(
        modifier_tokens,
        move(return_data_type),
        name_token,
        args_start_token,
        move(args),
        arg_separator_tokens,
        args_end_token,
        move(body)));
    TestNode<StmtNode> test_stmt = {tokens, move(func_def1)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, FuncDefWithoutBodyWithModifiersAndArgs) {
  struct TestData {
    TestDataTypeNode return_data_type;
    TestDataTypeNode arg_data_type1;
    TestDataTypeNode arg_data_type2;
  };
  vector<TestData> test_data_suits;

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new IntDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kIntType, "int", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new IntDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kIntType, "int", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new IntDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kLongType, "long", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new LongDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kLongType, "long", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new LongDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kLongType, "long", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new LongDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kDoubleType, "double", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new DoubleDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kDoubleType, "double", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new DoubleDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kDoubleType, "double", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new DoubleDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kBoolType, "bool", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new BoolDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kBoolType, "bool", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new BoolDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kBoolType, "bool", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new BoolDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kCharType, "char", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new CharDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kCharType, "char", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new CharDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kCharType, "char", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new CharDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  {
    TokenInfo return_data_type_token =
        TokenInfo(Token::kStringType, "string", UINT32_C(1), UINT32_C(2));
    TestDataTypeNode test_return_data_type = {
      return_data_type_token,
      unique_ptr<PrimitiveDataTypeNode>(
          new StringDataTypeNode(return_data_type_token))
    };
    TokenInfo arg_data_type_token1 =
        TokenInfo(Token::kStringType, "string", UINT32_C(7), UINT32_C(8));
    TestDataTypeNode test_arg_data_type1 = {
      arg_data_type_token1,
      unique_ptr<PrimitiveDataTypeNode>(
          new StringDataTypeNode(arg_data_type_token1))
    };
    TokenInfo arg_data_type_token2 =
        TokenInfo(Token::kStringType, "string", UINT32_C(13), UINT32_C(14));
    TestDataTypeNode test_arg_data_type2 = {
      arg_data_type_token2,
      unique_ptr<PrimitiveDataTypeNode>(
          new StringDataTypeNode(arg_data_type_token2))
    };
    TestData test_data = {move(test_return_data_type),
                          move(test_arg_data_type1),
                          move(test_arg_data_type2)};
    test_data_suits.push_back(move(test_data));
  }

  for (TestData &test_data: test_data_suits) {
    vector<TokenInfo> tokens = {
      TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0)),
      test_data.return_data_type.token,
      TokenInfo(Token::kName, "myFunc", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(3)),
      test_data.arg_data_type1.token,
      TokenInfo(Token::kName, "arg1", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kSeparator, ",", UINT32_C(6), UINT32_C(6)),
      test_data.arg_data_type2.token,
      TokenInfo(Token::kName, "arg2", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(10), UINT32_C(10))
    };
    const vector<TokenInfo> modifier_tokens = {tokens[0]};
    unique_ptr<DataTypeNode> return_data_type(
        move(test_data.return_data_type.node));
    const TokenInfo func_name_token = tokens[2];
    const TokenInfo args_start_token = tokens[3];
    vector< unique_ptr<ArgDefNode> > args;
    unique_ptr<ArgDefNode> arg1(new ArgDefNode(
        move(test_data.arg_data_type1.node), tokens[5]));
    args.push_back(move(arg1));
    unique_ptr<ArgDefNode> arg2(new ArgDefNode(
        move(test_data.arg_data_type2.node), tokens[8]));
    args.push_back(move(arg2));
    const vector<TokenInfo> arg_separator_tokens = {tokens[6]};
    const TokenInfo args_end_token = tokens[9];
    const TokenInfo end_token = tokens[10];
    unique_ptr<StmtNode> func_def1(new FuncDefWithoutBodyNode(
        modifier_tokens,
        move(return_data_type),
        func_name_token,
        args_start_token,
        move(args),
        arg_separator_tokens,
        args_end_token,
        end_token));
    TestNode<StmtNode> test_stmt = {tokens, move(func_def1)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, VarDefWithoutInit) {
  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      test_data_type.token,
      TokenInfo(Token::kName, "myVar", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(6))
    };
    const TokenInfo name_token = tokens[1];
    const TokenInfo end_token = tokens[2];
    unique_ptr<DataTypeNode> data_type(move(test_data_type.node));
    unique_ptr<StmtNode> var_def(new VarDefWithoutInitNode(
        move(data_type), name_token, end_token));
    TestNode<StmtNode> test_stmt = {tokens, move(var_def)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, VarDefWithInit) {
  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      test_data_type.token,
      TokenInfo(Token::kName, "myVar", UINT32_C(3), UINT32_C(4)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(5), UINT32_C(6)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(7), UINT32_C(8)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(9), UINT32_C(10))
    };
    const TokenInfo name_token = tokens[1];
    unique_ptr<DataTypeNode> data_type(move(test_data_type.node));
    const TokenInfo assign_token = tokens[2];
    unique_ptr<ExprNode> value(new IntNode(tokens[3]));
    const TokenInfo end_token = tokens[4];
    unique_ptr<StmtNode> var_def(new VarDefWithInitNode(
        move(data_type), name_token, assign_token, move(value), end_token));
    TestNode<StmtNode> test_stmt = {tokens, move(var_def)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, ReturnValue) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2))
  };
  const TokenInfo &start_token = tokens[0];
  unique_ptr<ExprNode> value(new IntNode(tokens[1]));
  const TokenInfo &end_token = tokens[2];
  unique_ptr<StmtNode> return1(
      new ReturnValueNode(start_token, move(value), end_token));
  test_stmt_nodes.push_back({tokens, move(return1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, ReturnWithoutValue) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &end_token = tokens[1];
  unique_ptr<StmtNode> return1(
      new ReturnWithoutValueNode(start_token, end_token));
  test_stmt_nodes.push_back({tokens, move(return1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, Break) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kBreak, "break", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &end_token = tokens[1];
  unique_ptr<StmtNode> break1(new BreakNode(start_token, end_token));
  test_stmt_nodes.push_back({tokens, move(break1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, Continue) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kContinue, "continue", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &end_token = tokens[1];
  unique_ptr<StmtNode> continue1(new ContinueNode(start_token, end_token));
  test_stmt_nodes.push_back({tokens, move(continue1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, PreTestLoopWithBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kWhile, "while", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(10)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(11), UINT32_C(12)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(13), UINT32_C(14)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(15), UINT32_C(16)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(18)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(19), UINT32_C(20))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &cond_start_token = tokens[1];
  unique_ptr<ExprNode> cond(new BoolNode(tokens[2]));
  const TokenInfo &cond_end_token = tokens[3];
  vector< unique_ptr<StmtNode> > body_stmts;
  unique_ptr<ExprNode> int1(new IntNode(tokens[5]));
  unique_ptr<StmtNode> body_stmt1(new ExprStmtNode(move(int1), tokens[6]));
  body_stmts.push_back(move(body_stmt1));
  unique_ptr<ExprNode> int2(new IntNode(tokens[7]));
  unique_ptr<StmtNode> body_stmt2(new ExprStmtNode(move(int2), tokens[8]));
  body_stmts.push_back(move(body_stmt2));
  unique_ptr<ScopeNode> body(
      new ScopeNode(tokens[4], move(body_stmts), tokens[9]));
  unique_ptr<StmtNode> pre_test_loop1(new PreTestLoopNode(
      start_token,
      cond_start_token,
      move(cond),
      cond_end_token,
      move(body)));
  test_stmt_nodes.push_back({tokens, move(pre_test_loop1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, PreTestLoopWithoutBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kWhile, "while", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(10)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(19), UINT32_C(20))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &cond_start_token = tokens[1];
  unique_ptr<ExprNode> cond(new BoolNode(tokens[2]));
  const TokenInfo &cond_end_token = tokens[3];
  vector< unique_ptr<StmtNode> > body_stmts;
  unique_ptr<ScopeNode> body(
      new ScopeNode(tokens[4], move(body_stmts), tokens[5]));
  unique_ptr<StmtNode> pre_test_loop1(new PreTestLoopNode(
      start_token,
      cond_start_token,
      move(cond),
      cond_end_token,
      move(body)));
  test_stmt_nodes.push_back({tokens, move(pre_test_loop1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfWithBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(10)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(11), UINT32_C(12)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(13), UINT32_C(14)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(15), UINT32_C(16)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(18)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(19), UINT32_C(20))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &cond_start_token = tokens[1];
  unique_ptr<ExprNode> cond(new BoolNode(tokens[2]));
  const TokenInfo &cond_end_token = tokens[3];
  vector< unique_ptr<StmtNode> > body_stmts;
  unique_ptr<ExprNode> int1(new IntNode(tokens[5]));
  unique_ptr<StmtNode> body_stmt1(new ExprStmtNode(move(int1), tokens[6]));
  body_stmts.push_back(move(body_stmt1));
  unique_ptr<ExprNode> int2(new IntNode(tokens[7]));
  unique_ptr<StmtNode> body_stmt2(new ExprStmtNode(move(int2), tokens[8]));
  body_stmts.push_back(move(body_stmt2));
  unique_ptr<ScopeNode> body(
      new ScopeNode(tokens[4], move(body_stmts), tokens[9]));
  unique_ptr<IfNode> if1(new IfNode(
      start_token, cond_start_token, move(cond), cond_end_token, move(body)));
  vector< unique_ptr<ElseIfNode> > else_ifs;
  unique_ptr<StmtNode> if_else_if1(new IfElseIfNode(move(if1), move(else_ifs)));
  test_stmt_nodes.push_back({tokens, move(if_else_if1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfWithoutBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(1), UINT32_C(2)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(3), UINT32_C(4)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(5), UINT32_C(6)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(7), UINT32_C(8)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(10)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(19), UINT32_C(20))
  };
  const TokenInfo &start_token = tokens[0];
  const TokenInfo &cond_start_token = tokens[1];
  unique_ptr<ExprNode> cond(new BoolNode(tokens[2]));
  const TokenInfo &cond_end_token = tokens[3];
  vector< unique_ptr<StmtNode> > body_stmts;
  unique_ptr<ScopeNode> body(
      new ScopeNode(tokens[4], move(body_stmts), tokens[5]));
  unique_ptr<IfNode> if1(new IfNode(
      start_token, cond_start_token, move(cond), cond_end_token, move(body)));
  vector< unique_ptr<ElseIfNode> > else_ifs;
  unique_ptr<StmtNode> if_else_if1(new IfElseIfNode(move(if1), move(else_ifs)));
  test_stmt_nodes.push_back({tokens, move(if_else_if1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfElseIfWithBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(7), UINT32_C(7)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(9), UINT32_C(9)),

    TokenInfo(Token::kElse, "else", UINT32_C(10), UINT32_C(10)),
    TokenInfo(Token::kIf, "if", UINT32_C(11), UINT32_C(11)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(12), UINT32_C(12)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(13), UINT32_C(13)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(14), UINT32_C(14)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(15), UINT32_C(15)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(16), UINT32_C(16)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(17)),
    TokenInfo(Token::kIntLit, "4", UINT32_C(18), UINT32_C(18)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(19), UINT32_C(19)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(20), UINT32_C(20)),

    TokenInfo(Token::kElse, "else", UINT32_C(21), UINT32_C(21)),
    TokenInfo(Token::kIf, "if", UINT32_C(22), UINT32_C(22)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(23), UINT32_C(23)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(24), UINT32_C(24)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(25), UINT32_C(25)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(26), UINT32_C(26)),
    TokenInfo(Token::kIntLit, "5", UINT32_C(27), UINT32_C(27)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(28), UINT32_C(28)),
    TokenInfo(Token::kIntLit, "6", UINT32_C(29), UINT32_C(29)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(30), UINT32_C(30)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(31), UINT32_C(31))
  };
  unique_ptr<ExprNode> cond1(new BoolNode(tokens[2]));
  vector< unique_ptr<StmtNode> > body_stmts1;
  unique_ptr<ExprNode> int1(new IntNode(tokens[5]));
  unique_ptr<StmtNode> body_stmt1(new ExprStmtNode(move(int1), tokens[6]));
  body_stmts1.push_back(move(body_stmt1));
  unique_ptr<ExprNode> int2(new IntNode(tokens[7]));
  unique_ptr<StmtNode> body_stmt2(new ExprStmtNode(move(int2), tokens[8]));
  body_stmts1.push_back(move(body_stmt2));
  unique_ptr<ScopeNode> body1(
      new ScopeNode(tokens[4], move(body_stmts1), tokens[9]));
  unique_ptr<IfNode> if1(new IfNode(
      tokens[0], tokens[1], move(cond1), tokens[3], move(body1)));

  vector< unique_ptr<ElseIfNode> > else_ifs;
  unique_ptr<ExprNode> cond2(new BoolNode(tokens[13]));
  vector< unique_ptr<StmtNode> > body_stmts2;
  unique_ptr<ExprNode> int3(new IntNode(tokens[16]));
  unique_ptr<StmtNode> body_stmt3(new ExprStmtNode(move(int3), tokens[17]));
  body_stmts2.push_back(move(body_stmt3));
  unique_ptr<ExprNode> int4(new IntNode(tokens[18]));
  unique_ptr<StmtNode> body_stmt4(new ExprStmtNode(move(int4), tokens[19]));
  body_stmts2.push_back(move(body_stmt4));
  unique_ptr<ScopeNode> body2(
      new ScopeNode(tokens[15], move(body_stmts2), tokens[20]));
  unique_ptr<IfNode> if2(new IfNode(
      tokens[11], tokens[12], move(cond2), tokens[14], move(body2)));
  unique_ptr<ElseIfNode> else_if1(new ElseIfNode(tokens[10], move(if2)));
  else_ifs.push_back(move(else_if1));

  unique_ptr<ExprNode> cond3(new BoolNode(tokens[24]));
  vector< unique_ptr<StmtNode> > body_stmts3;
  unique_ptr<ExprNode> int5(new IntNode(tokens[27]));
  unique_ptr<StmtNode> body_stmt5(new ExprStmtNode(move(int5), tokens[28]));
  body_stmts3.push_back(move(body_stmt5));
  unique_ptr<ExprNode> int6(new IntNode(tokens[29]));
  unique_ptr<StmtNode> body_stmt6(new ExprStmtNode(move(int6), tokens[30]));
  body_stmts3.push_back(move(body_stmt6));
  unique_ptr<ScopeNode> body3(
      new ScopeNode(tokens[26], move(body_stmts3), tokens[31]));
  unique_ptr<IfNode> if3(new IfNode(
      tokens[22], tokens[23], move(cond3), tokens[25], move(body3)));
  unique_ptr<ElseIfNode> else_if2(new ElseIfNode(tokens[21], move(if3)));
  else_ifs.push_back(move(else_if2));

  unique_ptr<StmtNode> if_else_if1(new IfElseIfNode(move(if1), move(else_ifs)));
  test_stmt_nodes.push_back({tokens, move(if_else_if1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfElseIfWithoutBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),

    TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
    TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(9), UINT32_C(9)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(10), UINT32_C(10)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(11), UINT32_C(11)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(12), UINT32_C(12))
  };
  unique_ptr<ExprNode> cond1(new BoolNode(tokens[2]));
  vector< unique_ptr<StmtNode> > body_stmts1;
  unique_ptr<ScopeNode> body1(
      new ScopeNode(tokens[4], move(body_stmts1), tokens[5]));
  unique_ptr<IfNode> if1(new IfNode(
      tokens[0], tokens[1], move(cond1), tokens[3], move(body1)));

  vector< unique_ptr<ElseIfNode> > else_ifs;
  unique_ptr<ExprNode> cond2(new BoolNode(tokens[9]));
  vector< unique_ptr<StmtNode> > body_stmts2;
  unique_ptr<ScopeNode> body2(
      new ScopeNode(tokens[11], move(body_stmts2), tokens[12]));
  unique_ptr<IfNode> if2(new IfNode(
      tokens[7], tokens[8], move(cond2), tokens[10], move(body2)));
  unique_ptr<ElseIfNode> else_if1(new ElseIfNode(tokens[6], move(if2)));
  else_ifs.push_back(move(else_if1));

  unique_ptr<StmtNode> if_else_if1(new IfElseIfNode(move(if1), move(else_ifs)));
  test_stmt_nodes.push_back({tokens, move(if_else_if1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfElseWithBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(7), UINT32_C(7)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(9), UINT32_C(9)),

    TokenInfo(Token::kElse, "else", UINT32_C(10), UINT32_C(10)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(11), UINT32_C(11)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(12), UINT32_C(12)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(13), UINT32_C(13)),
    TokenInfo(Token::kIntLit, "4", UINT32_C(14), UINT32_C(14)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(15), UINT32_C(15)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(16), UINT32_C(16))
  };
  unique_ptr<ExprNode> cond1(new BoolNode(tokens[2]));
  vector< unique_ptr<StmtNode> > body_stmts1;
  unique_ptr<ExprNode> int1(new IntNode(tokens[5]));
  unique_ptr<StmtNode> body_stmt1(new ExprStmtNode(move(int1), tokens[6]));
  body_stmts1.push_back(move(body_stmt1));
  unique_ptr<ExprNode> int2(new IntNode(tokens[7]));
  unique_ptr<StmtNode> body_stmt2(new ExprStmtNode(move(int2), tokens[8]));
  body_stmts1.push_back(move(body_stmt2));
  unique_ptr<ScopeNode> body1(
      new ScopeNode(tokens[4], move(body_stmts1), tokens[9]));
  unique_ptr<IfNode> if1(new IfNode(
      tokens[0], tokens[1], move(cond1), tokens[3], move(body1)));

  vector< unique_ptr<ElseIfNode> > else_ifs;

  const TokenInfo &else_token = tokens[10];
  vector< unique_ptr<StmtNode> > body_stmts2;
  unique_ptr<ExprNode> int3(new IntNode(tokens[12]));
  unique_ptr<StmtNode> body_stmt3(new ExprStmtNode(move(int3), tokens[13]));
  body_stmts2.push_back(move(body_stmt3));
  unique_ptr<ExprNode> int4(new IntNode(tokens[14]));
  unique_ptr<StmtNode> body_stmt4(new ExprStmtNode(move(int4), tokens[15]));
  body_stmts2.push_back(move(body_stmt4));
  unique_ptr<ScopeNode> body3(
      new ScopeNode(tokens[11], move(body_stmts2), tokens[16]));

  unique_ptr<StmtNode> if_else1(new IfElseIfElseNode(
      move(if1), move(else_ifs), else_token, move(body3)));
  test_stmt_nodes.push_back({tokens, move(if_else1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfElseWithoutBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),

    TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(8), UINT32_C(8))
  };
  unique_ptr<ExprNode> cond1(new BoolNode(tokens[2]));
  vector< unique_ptr<StmtNode> > body_stmts1;
  unique_ptr<ScopeNode> body1(
      new ScopeNode(tokens[4], move(body_stmts1), tokens[5]));
  unique_ptr<IfNode> if1(new IfNode(
      tokens[0], tokens[1], move(cond1), tokens[3], move(body1)));

  vector< unique_ptr<ElseIfNode> > else_ifs;

  const TokenInfo &else_token = tokens[6];
  vector< unique_ptr<StmtNode> > body_stmts2;
  unique_ptr<ScopeNode> body3(
      new ScopeNode(tokens[7], move(body_stmts2), tokens[8]));

  unique_ptr<StmtNode> if_else1(new IfElseIfElseNode(
      move(if1), move(else_ifs), else_token, move(body3)));
  test_stmt_nodes.push_back({tokens, move(if_else1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfElseIfElseWithBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
    TokenInfo(Token::kIntLit, "1", UINT32_C(5), UINT32_C(5)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6)),
    TokenInfo(Token::kIntLit, "2", UINT32_C(7), UINT32_C(7)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(8), UINT32_C(8)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(9), UINT32_C(9)),

    TokenInfo(Token::kElse, "else", UINT32_C(10), UINT32_C(10)),
    TokenInfo(Token::kIf, "if", UINT32_C(11), UINT32_C(11)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(12), UINT32_C(12)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(13), UINT32_C(13)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(14), UINT32_C(14)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(15), UINT32_C(15)),
    TokenInfo(Token::kIntLit, "3", UINT32_C(16), UINT32_C(16)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(17), UINT32_C(17)),
    TokenInfo(Token::kIntLit, "4", UINT32_C(18), UINT32_C(18)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(19), UINT32_C(19)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(20), UINT32_C(20)),

    TokenInfo(Token::kElse, "else", UINT32_C(21), UINT32_C(21)),
    TokenInfo(Token::kIf, "if", UINT32_C(22), UINT32_C(22)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(23), UINT32_C(23)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(24), UINT32_C(24)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(25), UINT32_C(25)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(26), UINT32_C(26)),
    TokenInfo(Token::kIntLit, "5", UINT32_C(27), UINT32_C(27)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(28), UINT32_C(28)),
    TokenInfo(Token::kIntLit, "6", UINT32_C(29), UINT32_C(29)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(30), UINT32_C(30)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(31), UINT32_C(31)),

    TokenInfo(Token::kElse, "else", UINT32_C(32), UINT32_C(32)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(33), UINT32_C(33)),
    TokenInfo(Token::kIntLit, "7", UINT32_C(34), UINT32_C(34)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(35), UINT32_C(35)),
    TokenInfo(Token::kIntLit, "8", UINT32_C(36), UINT32_C(36)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(37), UINT32_C(37)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(38), UINT32_C(38))
  };
  unique_ptr<ExprNode> cond1(new BoolNode(tokens[2]));
  vector< unique_ptr<StmtNode> > body_stmts1;
  unique_ptr<ExprNode> int1(new IntNode(tokens[5]));
  unique_ptr<StmtNode> body_stmt1(new ExprStmtNode(move(int1), tokens[6]));
  body_stmts1.push_back(move(body_stmt1));
  unique_ptr<ExprNode> int2(new IntNode(tokens[7]));
  unique_ptr<StmtNode> body_stmt2(new ExprStmtNode(move(int2), tokens[8]));
  body_stmts1.push_back(move(body_stmt2));
  unique_ptr<ScopeNode> body1(
      new ScopeNode(tokens[4], move(body_stmts1), tokens[9]));
  unique_ptr<IfNode> if1(new IfNode(
      tokens[0], tokens[1], move(cond1), tokens[3], move(body1)));

  vector< unique_ptr<ElseIfNode> > else_ifs;
  unique_ptr<ExprNode> cond2(new BoolNode(tokens[13]));
  vector< unique_ptr<StmtNode> > body_stmts2;
  unique_ptr<ExprNode> int3(new IntNode(tokens[16]));
  unique_ptr<StmtNode> body_stmt3(new ExprStmtNode(move(int3), tokens[17]));
  body_stmts2.push_back(move(body_stmt3));
  unique_ptr<ExprNode> int4(new IntNode(tokens[18]));
  unique_ptr<StmtNode> body_stmt4(new ExprStmtNode(move(int4), tokens[19]));
  body_stmts2.push_back(move(body_stmt4));
  unique_ptr<ScopeNode> body2(
      new ScopeNode(tokens[15], move(body_stmts2), tokens[20]));
  unique_ptr<IfNode> if2(new IfNode(
      tokens[11], tokens[12], move(cond2), tokens[14], move(body2)));
  unique_ptr<ElseIfNode> else_if1(new ElseIfNode(tokens[10], move(if2)));
  else_ifs.push_back(move(else_if1));

  unique_ptr<ExprNode> cond3(new BoolNode(tokens[24]));
  vector< unique_ptr<StmtNode> > body_stmts3;
  unique_ptr<ExprNode> int5(new IntNode(tokens[27]));
  unique_ptr<StmtNode> body_stmt5(new ExprStmtNode(move(int5), tokens[28]));
  body_stmts3.push_back(move(body_stmt5));
  unique_ptr<ExprNode> int6(new IntNode(tokens[29]));
  unique_ptr<StmtNode> body_stmt6(new ExprStmtNode(move(int6), tokens[30]));
  body_stmts3.push_back(move(body_stmt6));
  unique_ptr<ScopeNode> body3(
      new ScopeNode(tokens[26], move(body_stmts3), tokens[31]));
  unique_ptr<IfNode> if3(new IfNode(
      tokens[22], tokens[23], move(cond3), tokens[25], move(body3)));
  unique_ptr<ElseIfNode> else_if2(new ElseIfNode(tokens[21], move(if3)));
  else_ifs.push_back(move(else_if2));

  const TokenInfo &final_else_token = tokens[32];
  vector< unique_ptr<StmtNode> > body_stmts4;
  unique_ptr<ExprNode> int7(new IntNode(tokens[34]));
  unique_ptr<StmtNode> body_stmt7(new ExprStmtNode(move(int7), tokens[35]));
  body_stmts4.push_back(move(body_stmt7));
  unique_ptr<ExprNode> int8(new IntNode(tokens[36]));
  unique_ptr<StmtNode> body_stmt8(new ExprStmtNode(move(int8), tokens[37]));
  body_stmts4.push_back(move(body_stmt8));
  unique_ptr<ScopeNode> body4(
      new ScopeNode(tokens[33], move(body_stmts4), tokens[38]));

  unique_ptr<StmtNode> if_else_if1(new IfElseIfElseNode(
      move(if1), move(else_ifs), final_else_token, move(body4)));
  test_stmt_nodes.push_back({tokens, move(if_else_if1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, IfElseIfElseWithoutBody) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(2), UINT32_C(2)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),

    TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
    TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
    TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
    TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(9), UINT32_C(9)),
    TokenInfo(Token::kGroupEnd, ")", UINT32_C(10), UINT32_C(10)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(11), UINT32_C(11)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(12), UINT32_C(12)),

    TokenInfo(Token::kElse, "else", UINT32_C(13), UINT32_C(13)),
    TokenInfo(Token::kScopeStart, "{", UINT32_C(14), UINT32_C(14)),
    TokenInfo(Token::kScopeEnd, "}", UINT32_C(15), UINT32_C(15))
  };
  unique_ptr<ExprNode> cond1(new BoolNode(tokens[2]));
  vector< unique_ptr<StmtNode> > body_stmts1;
  unique_ptr<ScopeNode> body1(
      new ScopeNode(tokens[4], move(body_stmts1), tokens[5]));
  unique_ptr<IfNode> if1(new IfNode(
      tokens[0], tokens[1], move(cond1), tokens[3], move(body1)));

  vector< unique_ptr<ElseIfNode> > else_ifs;
  unique_ptr<ExprNode> cond2(new BoolNode(tokens[9]));
  vector< unique_ptr<StmtNode> > body_stmts2;
  unique_ptr<ScopeNode> body2(
      new ScopeNode(tokens[11], move(body_stmts2), tokens[12]));
  unique_ptr<IfNode> if2(new IfNode(
      tokens[7], tokens[8], move(cond2), tokens[10], move(body2)));
  unique_ptr<ElseIfNode> else_if1(new ElseIfNode(tokens[6], move(if2)));
  else_ifs.push_back(move(else_if1));

  const TokenInfo &final_else_token = tokens[13];
  vector< unique_ptr<StmtNode> > body_stmts3;
  unique_ptr<ScopeNode> body3(
      new ScopeNode(tokens[14], move(body_stmts3), tokens[15]));

  unique_ptr<StmtNode> if_else_if1(new IfElseIfElseNode(
      move(if1), move(else_ifs), final_else_token, move(body3)));
  test_stmt_nodes.push_back({tokens, move(if_else_if1)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, Import) {
  vector< TestNode<StmtNode> > test_stmt_nodes;
  vector<TokenInfo> tokens = {
    TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
    TokenInfo(Token::kStringLit, "\"src/file.rt\"", UINT32_C(1), UINT32_C(1)),
    TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)),
  };
  const TokenInfo &start_token = tokens[0];
  unique_ptr<StringNode> file_path(new StringNode(tokens[1]));
  const TokenInfo &end_token = tokens[2];
  unique_ptr<StmtNode> import(
      new ImportNode(start_token, move(file_path), end_token));
  test_stmt_nodes.push_back({tokens, move(import)});

  for (TestNode<StmtNode> &test_stmt_node: test_stmt_nodes) {
    TestParse(StmtToProgram(test_stmt_node));
  }
}

TEST_F(SimpleParserTest, ArrayDataType) {
  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      test_data_type.token,
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kName, "myVar", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4))
    };
    unique_ptr<DataTypeNode> simple_data_type(move(test_data_type.node));
    unique_ptr<DataTypeNode> array_data_type(
        new ArrayDataTypeNode(move(simple_data_type), tokens[1], tokens[2]));
    unique_ptr<StmtNode> var_def(new VarDefWithoutInitNode(
        move(array_data_type), tokens[3], tokens[4]));
    TestNode<StmtNode> test_stmt = {tokens, move(var_def)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, ArrayOfArraysDataType) {
  for (TestDataTypeNode &test_data_type: GetTestDataTypes()) {
    vector<TokenInfo> tokens = {
      test_data_type.token,
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kName, "myVar", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(6), UINT32_C(6))
    };
    unique_ptr<DataTypeNode> simple_data_type(move(test_data_type.node));
    unique_ptr<DataTypeNode> array_data_type1(
        new ArrayDataTypeNode(move(simple_data_type), tokens[1], tokens[2]));
    unique_ptr<DataTypeNode> array_data_type2(
        new ArrayDataTypeNode(move(array_data_type1), tokens[3], tokens[4]));
    unique_ptr<StmtNode> var_def(new VarDefWithoutInitNode(
        move(array_data_type2), tokens[5], tokens[6]));
    TestNode<StmtNode> test_stmt = {tokens, move(var_def)};

    TestParse(StmtToProgram(test_stmt));
  }
}

TEST_F(SimpleParserTest, TwoOperandsInARowIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLongLit, "1L", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kLongLit, "2L", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kDivOp, "/", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, BinaryExprWithoutRightOperandIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, UnaryExprWithoutOperandIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kNotOp, "!", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, GroupWithoutExprIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ArrayAllocWithoutDataTypeIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ArrayAllocWithoutSizeIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ArrayAllocWithInitButWithoutValuesIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntType, "int", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(6), UINT32_C(6))
    };
    const TokenInfo &unexpected_token = tokens[5];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, SubscriptWithoutIndexIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "var", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "var", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "var", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, DefWithoutNameIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, VarDefWithInitButWithoutValueIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, VarDefWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "var", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ArrayDataTypeWithoutSubscriptEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kName, "var", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ExprWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(1))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, PreTestLoopWithoutConditionIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, PreTestLoopWithoutBodyIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "while", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[5];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ImportWithoutFilePathIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ImportWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kImport, "import", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStringLit, "\"file.rt\"", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, IfWithoutConditionIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(3), UINT32_C(3))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, IfWithoutBodyIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[5];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ElseIfWithoutConditionIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(10), UINT32_C(10))
    };
    const TokenInfo &unexpected_token = tokens[8];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(11), UINT32_C(11))
    };
    const TokenInfo &unexpected_token = tokens[9];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(12), UINT32_C(12))
    };
    const TokenInfo &unexpected_token = tokens[10];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(12), UINT32_C(12))
    };
    const TokenInfo &unexpected_token = tokens[9];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ElseIfWithoutBodyIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(11), UINT32_C(11))
    };
    const TokenInfo &unexpected_token = tokens[11];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kIf, "if", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(8), UINT32_C(8)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(9), UINT32_C(9)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(10), UINT32_C(10)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(11), UINT32_C(11)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(12), UINT32_C(12))
    };
    const TokenInfo &unexpected_token = tokens[12];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ElseWithoutBodyIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(7), UINT32_C(7))
    };
    const TokenInfo &unexpected_token = tokens[7];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kElse, "else", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(7), UINT32_C(7)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(8), UINT32_C(8))
    };
    const TokenInfo &unexpected_token = tokens[8];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ReturnWithoutValueAndWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(1))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ReturnValueWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kReturn, "return", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, BreakWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBreak, "break", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(1))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ContinueWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kContinue, "continue", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(1))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, FuncDefWithoutDataTypeIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNative, "native", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(2))
    };
    const TokenInfo &unexpected_token = tokens[1];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, FuncDefWithoutArgsIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[2];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, FuncDefWithoutEndIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(4), UINT32_C(4))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(5), UINT32_C(5))
    };
    const TokenInfo &unexpected_token = tokens[5];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ArgDefWithoutDataTypeIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kName, "arg", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(7), UINT32_C(7))
    };
    const TokenInfo &unexpected_token = tokens[3];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}

TEST_F(SimpleParserTest, ArgDefWithoutNameIsInvalid) {
  vector<MailformedTestTokens> mailformed_token_suits;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kName, "func", UINT32_C(1), UINT32_C(1)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(2), UINT32_C(2)),
      TokenInfo(Token::kIntType, "int", UINT32_C(3), UINT32_C(3)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(4), UINT32_C(4)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(5), UINT32_C(5)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(6), UINT32_C(6)),
      TokenInfo(Token::kFileEnd, "", UINT32_C(7), UINT32_C(7))
    };
    const TokenInfo &unexpected_token = tokens[4];
    const MailformedTestTokens mailformed_tokens = {tokens, unexpected_token};
    mailformed_token_suits.push_back(mailformed_tokens);
  }

  for (const MailformedTestTokens &mailformed_tokens: mailformed_token_suits) {
    TestFailingParse(mailformed_tokens);
  }
}
}
}
