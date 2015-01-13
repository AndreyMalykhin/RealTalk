
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "real_talk/util/errors.h"
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/lexer/token_info.h"

using std::ios;
using std::istringstream;
using std::istream;
using std::streambuf;
using std::string;
using std::vector;
using std::char_traits;
using testing::Test;
using testing::Throw;
using testing::Return;
using real_talk::util::IOError;

namespace real_talk {
namespace lexer {

struct TestTokens {
  string code;
  vector<TokenInfo> tokens;
};

struct MailformedTestToken {
  string chars;
  char unexpected_char;
  uint32_t unexpected_char_line_number;
  uint32_t unexpected_char_column_number;
};

class StreambufMock: public streambuf {
 public:
  MOCK_METHOD0(underflow, int());
};

class SimpleLexerTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

  void TestGetNextToken(const TestTokens &test_tokens) {
    istringstream code_stream(test_tokens.code);
    SimpleLexer lexer(code_stream);

    for (const TokenInfo &expected_token: test_tokens.tokens) {
      const TokenInfo &actual_token = lexer.GetNextToken();
      ASSERT_EQ(expected_token, actual_token);
    }
  }

  void TestFailingGetNextToken(const MailformedTestToken &mailformed_token) {
    istringstream code_stream(mailformed_token.chars);
    SimpleLexer lexer(code_stream);

    try {
      const TokenInfo &actual_token = lexer.GetNextToken();
      FAIL() << "expected_bad_char=" << mailformed_token.unexpected_char
             << "expected_bad_char_line_number="
             << mailformed_token.unexpected_char_line_number
             << "expected_bad_char_column_number="
             << mailformed_token.unexpected_char_column_number
             << "; actual_token=" << actual_token;
    } catch (const UnexpectedCharError &error) {
      ASSERT_EQ(mailformed_token.unexpected_char, error.GetChar());
      ASSERT_EQ(mailformed_token.unexpected_char_line_number,
                error.GetLineNumber());
      ASSERT_EQ(mailformed_token.unexpected_char_column_number,
                error.GetColumnNumber());
    }
  }
};

TEST_F(SimpleLexerTest, FileEndToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(0))
    };
    string code = "";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, StmtEndToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(0), UINT32_C(0))
    };
    string code = ";";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStmtEnd, ";", UINT32_C(0), UINT32_C(1))
    };
    string code = ";;";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, IfToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0))
    };
    string code = "if";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIf, "if", UINT32_C(0), UINT32_C(3))
    };
    string code = "if if";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, NameToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "i", UINT32_C(0), UINT32_C(0))
    };
    string code = "i";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "ii", UINT32_C(0), UINT32_C(0))
    };
    string code = "ii";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "i2", UINT32_C(0), UINT32_C(0))
    };
    string code = "i2";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "i_2", UINT32_C(0), UINT32_C(0))
    };
    string code = "i_2";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "i2_", UINT32_C(0), UINT32_C(0))
    };
    string code = "i2_";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kName, "i_i", UINT32_C(0), UINT32_C(0))
    };
    string code = "i_i";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, PreDecOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(0), UINT32_C(0))
    };
    string code = "--";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kPreDecOp, "--", UINT32_C(0), UINT32_C(3))
    };
    string code = "-- --";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, PreIncOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(0), UINT32_C(0))
    };
    string code = "++";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kPreIncOp, "++", UINT32_C(0), UINT32_C(3))
    };
    string code = "++ ++";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, AssignOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kAssignOp, "=", UINT32_C(0), UINT32_C(0))
    };
    string code = "=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kAssignOp, "=", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kAssignOp, "=", UINT32_C(0), UINT32_C(2))
    };
    string code = "= =";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, LessOrEqualOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(0), UINT32_C(0))
    };
    string code = "<=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kLessOrEqualOp, "<=", UINT32_C(0), UINT32_C(3))
    };
    string code = "<= <=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, LessOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLessOp, "<", UINT32_C(0), UINT32_C(0))
    };
    string code = "<";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLessOp, "<", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kLessOp, "<", UINT32_C(0), UINT32_C(2))
    };
    string code = "< <";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, GreaterOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(0), UINT32_C(0))
    };
    string code = ">";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGreaterOp, ">", UINT32_C(0), UINT32_C(2))
    };
    string code = "> >";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, GreaterOrEqualOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(0), UINT32_C(0))
    };
    string code = ">=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGreaterOrEqualOp, ">=", UINT32_C(0), UINT32_C(3))
    };
    string code = ">= >=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, NotOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(0))
    };
    string code = "!";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kNotOp, "!", UINT32_C(0), UINT32_C(1))
    };
    string code = "!!";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, NotEqualOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(0), UINT32_C(0))
    };
    string code = "!=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kNotEqualOp, "!=", UINT32_C(0), UINT32_C(3))
    };
    string code = "!= !=";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, EqualOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kEqualOp, "==", UINT32_C(0), UINT32_C(0))
    };
    string code = "==";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kEqualOp, "==", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kEqualOp, "==", UINT32_C(0), UINT32_C(3))
    };
    string code = "== ==";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, OrOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kOrOp, "||", UINT32_C(0), UINT32_C(0))
    };
    string code = "||";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kOrOp, "||", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kOrOp, "||", UINT32_C(0), UINT32_C(3))
    };
    string code = "|| ||";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, AndOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kAndOp, "&&", UINT32_C(0), UINT32_C(0))
    };
    string code = "&&";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kAndOp, "&&", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kAndOp, "&&", UINT32_C(0), UINT32_C(3))
    };
    string code = "&& &&";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, SubOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubOp, "-", UINT32_C(0), UINT32_C(0))
    };
    string code = "-";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubOp, "-", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubOp, "-", UINT32_C(0), UINT32_C(2))
    };
    string code = "- -";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, DivOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDivOp, "/", UINT32_C(0), UINT32_C(0))
    };
    string code = "/";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDivOp, "/", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kDivOp, "/", UINT32_C(0), UINT32_C(2))
    };
    string code = "/ /";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, MulOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kMulOp, "*", UINT32_C(0), UINT32_C(0))
    };
    string code = "*";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kMulOp, "*", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kMulOp, "*", UINT32_C(0), UINT32_C(2))
    };
    string code = "* *";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, SumOpToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSumOp, "+", UINT32_C(0), UINT32_C(0))
    };
    string code = "+";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSumOp, "+", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSumOp, "+", UINT32_C(0), UINT32_C(2))
    };
    string code = "+ +";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, ScopeStartToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0))
    };
    string code = "{";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kScopeStart, "{", UINT32_C(0), UINT32_C(1))
    };
    string code = "{{";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, ScopeEndToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0))
    };
    string code = "}";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kScopeEnd, "}", UINT32_C(0), UINT32_C(1))
    };
    string code = "}}";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, GroupStartToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0))
    };
    string code = "(";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupStart, "(", UINT32_C(0), UINT32_C(1))
    };
    string code = "((";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, GroupEndToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(0))
    };
    string code = ")";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kGroupEnd, ")", UINT32_C(0), UINT32_C(1))
    };
    string code = "))";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, SubscriptStartToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(0), UINT32_C(0))
    };
    string code = "[";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptStart, "[", UINT32_C(0), UINT32_C(1))
    };
    string code = "[[";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, SubscriptEndToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(0), UINT32_C(0))
    };
    string code = "]";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSubscriptEnd, "]", UINT32_C(0), UINT32_C(1))
    };
    string code = "]]";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, SeparatorToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSeparator, ",", UINT32_C(0), UINT32_C(0))
    };
    string code = ",";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kSeparator, ",", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kSeparator, ",", UINT32_C(0), UINT32_C(1))
    };
    string code = ",,";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, IntTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0))
    };
    string code = "int";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntType, "int", UINT32_C(0), UINT32_C(4))
    };
    string code = "int int";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, LongTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0))
    };
    string code = "long";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kLongType, "long", UINT32_C(0), UINT32_C(5))
    };
    string code = "long long";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, StringTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0))
    };
    string code = "string";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStringType, "string", UINT32_C(0), UINT32_C(7))
    };
    string code = "string string";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, VoidTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0))
    };
    string code = "void";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kVoidType, "void", UINT32_C(0), UINT32_C(5))
    };
    string code = "void void";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, BoolTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0))
    };
    string code = "bool";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kBoolType, "bool", UINT32_C(0), UINT32_C(5))
    };
    string code = "bool bool";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, DoubleTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0))
    };
    string code = "double";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kDoubleType, "double", UINT32_C(0), UINT32_C(7))
    };
    string code = "double double";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, CharTypeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0))
    };
    string code = "char";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kCharType, "char", UINT32_C(0), UINT32_C(5))
    };
    string code = "char char";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, IntLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(0))
    };
    string code = "1";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "12", UINT32_C(0), UINT32_C(0))
    };
    string code = "12";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "11", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kIntLit, "22", UINT32_C(0), UINT32_C(3))
    };
    string code = "11 22";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, LongLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLongLit, "1L", UINT32_C(0), UINT32_C(0))
    };
    string code = "1L";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLongLit, "12L", UINT32_C(0), UINT32_C(0))
    };
    string code = "12L";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kLongLit, "11L", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kLongLit, "22L", UINT32_C(0), UINT32_C(4))
    };
    string code = "11L 22L";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, DoubleLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDoubleLit, "1.2", UINT32_C(0), UINT32_C(0))
    };
    string code = "1.2";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDoubleLit, "11.22", UINT32_C(0), UINT32_C(0))
    };
    string code = "11.22";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kDoubleLit, "11.22", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kDoubleLit, "33.44", UINT32_C(0), UINT32_C(6))
    };
    string code = "11.22 33.44";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, BoolTrueLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(0), UINT32_C(0))
    };
    string code = "yeah";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kBoolTrueLit, "yeah", UINT32_C(0), UINT32_C(5))
    };
    string code = "yeah yeah";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, BoolFalseLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0))
    };
    string code = "nah";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kBoolFalseLit, "nah", UINT32_C(0), UINT32_C(4))
    };
    string code = "nah nah";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, CharLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0))
    };
    string code = "'a'";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharLit, "'a'", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kCharLit, "'b'", UINT32_C(0), UINT32_C(4))
    };
    string code = "'a' 'b'";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharLit, "''", UINT32_C(0), UINT32_C(0))
    };
    string code = "''";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharLit, "'\\''", UINT32_C(0), UINT32_C(0))
    };
    string code = "'\\''";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharLit, "'\\\\'", UINT32_C(0), UINT32_C(0))
    };
    string code = "'\\\\'";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, StringLitToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"a\"", UINT32_C(0), UINT32_C(0))
    };
    string code = "\"a\"";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"ab\"", UINT32_C(0), UINT32_C(0))
    };
    string code = "\"ab\"";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"ab\"", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kStringLit, "\"cd\"", UINT32_C(0), UINT32_C(5))
    };
    string code = "\"ab\" \"cd\"";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"\"", UINT32_C(0), UINT32_C(0))
    };
    string code = "\"\"";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"\\\"\"", UINT32_C(0), UINT32_C(0))
    };
    string code = "\"\\\"\"";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"\\\\\"", UINT32_C(0), UINT32_C(0))
    };
    string code = "\"\\\\\"";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, NewToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0))
    };
    string code = "fresh";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kNew, "fresh", UINT32_C(0), UINT32_C(6))
    };
    string code = "fresh fresh";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, ImportToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kImport, "checkthisout", UINT32_C(0), UINT32_C(0))
    };
    string code = "checkthisout";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kImport, "checkthisout", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kImport, "checkthisout", UINT32_C(0), UINT32_C(13))
    };
    string code = "checkthisout checkthisout";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, WhileToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "roll", UINT32_C(0), UINT32_C(0))
    };
    string code = "roll";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kWhile, "roll", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kWhile, "roll", UINT32_C(0), UINT32_C(5))
    };
    string code = "roll roll";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, ElseToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kElse, "else", UINT32_C(0), UINT32_C(0))
    };
    string code = "else";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kElse, "else", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kElse, "else", UINT32_C(0), UINT32_C(5))
    };
    string code = "else else";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, ContinueToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kContinue, "keeprollin", UINT32_C(0), UINT32_C(0))
    };
    string code = "keeprollin";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kContinue, "keeprollin", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kContinue, "keeprollin", UINT32_C(0), UINT32_C(11))
    };
    string code = "keeprollin keeprollin";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, BreakToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBreak, "holdon", UINT32_C(0), UINT32_C(0))
    };
    string code = "holdon";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kBreak, "holdon", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kBreak, "holdon", UINT32_C(0), UINT32_C(7))
    };
    string code = "holdon holdon";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, ReturnToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kReturn, "bringitback", UINT32_C(0), UINT32_C(0))
    };
    string code = "bringitback";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kReturn, "bringitback", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kReturn, "bringitback", UINT32_C(0), UINT32_C(12))
    };
    string code = "bringitback bringitback";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, NativeToken) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNative, "somewhere", UINT32_C(0), UINT32_C(0))
    };
    string code = "somewhere";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kNative, "somewhere", UINT32_C(0), UINT32_C(0)),
      TokenInfo(Token::kNative, "somewhere", UINT32_C(0), UINT32_C(10))
    };
    string code = "somewhere somewhere";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, SpaceIsIgnored) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(1))
    };
    string code = " ";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(2))
    };
    string code = "  ";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, LineEndResetsColumn) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kIntLit, "1", UINT32_C(0), UINT32_C(1)),
      TokenInfo(Token::kIntLit, "2", UINT32_C(1), UINT32_C(0))
    };
    string code = " 1\n2";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kStringLit, "\"\n\"", UINT32_C(0), UINT32_C(2)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(1))
    };
    string code = "  \"\n\"1";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kCharLit, "'\n'", UINT32_C(0), UINT32_C(2)),
      TokenInfo(Token::kIntLit, "1", UINT32_C(1), UINT32_C(1))
    };
    string code = "  '\n'1";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, LineEndIsIgnored) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(1), UINT32_C(0))
    };
    string code = "\n";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(2), UINT32_C(0))
    };
    string code = "\n\n";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, CarriageReturnIsIgnored) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(1))
    };
    string code = "\r";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(2))
    };
    string code = "\r\r";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, TabIsIgnored) {
  vector<TestTokens> test_token_suites;

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(1))
    };
    string code = "\t";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  {
    const vector<TokenInfo> tokens = {
      TokenInfo(Token::kFileEnd, "", UINT32_C(0), UINT32_C(2))
    };
    string code = "\t\t";
    TestTokens test_tokens = {code, tokens};
    test_token_suites.push_back(test_tokens);
  }

  for (const TestTokens &test_tokens: test_token_suites) {
    TestGetNextToken(test_tokens);
  }
}

TEST_F(SimpleLexerTest, StringLitWithoutEndIsInvalid) {
  vector<MailformedTestToken> mailformed_tokens;

  {
    const string chars = "\"";
    const char unexpected_char = char_traits<char>::eof();
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  for (const MailformedTestToken &mailformed_token: mailformed_tokens) {
    TestFailingGetNextToken(mailformed_token);
  }
}

TEST_F(SimpleLexerTest, CharLitWithoutEndIsInvalid) {
  vector<MailformedTestToken> mailformed_tokens;

  {
    const string chars = "'";
    const char unexpected_char = char_traits<char>::eof();
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  for (const MailformedTestToken &mailformed_token: mailformed_tokens) {
    TestFailingGetNextToken(mailformed_token);
  }
}

TEST_F(SimpleLexerTest, OrOpWithoutSecondCharIsInvalid) {
  vector<MailformedTestToken> mailformed_tokens;

  {
    const string chars = "|";
    const char unexpected_char = char_traits<char>::eof();
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  {
    const string chars = "|a";
    const char unexpected_char = 'a';
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  for (const MailformedTestToken &mailformed_token: mailformed_tokens) {
    TestFailingGetNextToken(mailformed_token);
  }
}

TEST_F(SimpleLexerTest, AndOpWithoutSecondCharIsInvalid) {
  vector<MailformedTestToken> mailformed_tokens;

  {
    const string chars = "&";
    const char unexpected_char = char_traits<char>::eof();
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  {
    const string chars = "&a";
    const char unexpected_char = 'a';
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  for (const MailformedTestToken &mailformed_token: mailformed_tokens) {
    TestFailingGetNextToken(mailformed_token);
  }
}

TEST_F(SimpleLexerTest, UnexpectedChar) {
  vector<MailformedTestToken> mailformed_tokens;

  {
    const string chars = " $";
    const char unexpected_char = '$';
    const uint32_t unexpected_char_line_number = UINT32_C(0);
    const uint32_t unexpected_char_column_number = UINT32_C(1);
    const MailformedTestToken mailformed_token = {
      chars,
      unexpected_char,
      unexpected_char_line_number,
      unexpected_char_column_number
    };
    mailformed_tokens.push_back(mailformed_token);
  }

  for (const MailformedTestToken &mailformed_token: mailformed_tokens) {
    TestFailingGetNextToken(mailformed_token);
  }
}

TEST_F(SimpleLexerTest, IOError) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, underflow())
      .Times(1)
      .WillOnce(Throw(ios::failure("underflow() error")));
  istream code_stream(&stream_buffer);
  SimpleLexer lexer(code_stream);

  try {
    const TokenInfo &token = lexer.GetNextToken();
    FAIL() << "token=" << token;
  } catch (const IOError&) {
  }
}
}
}
