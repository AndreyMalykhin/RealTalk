
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "real_talk/test/test_config.h"
#include "real_talk/lexer/lexer_factory.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/parser_factory.h"
#include "real_talk/parser/parser.h"
#include "real_talk/parser/simple_file_parser.h"
#include "real_talk/util/errors.h"

using std::unique_ptr;
using std::shared_ptr;
using std::istream;
using std::vector;
using std::ios;
using testing::Test;
using testing::InSequence;
using testing::_;
using testing::Return;
using boost::filesystem::path;
using real_talk::test::TestConfig;
using real_talk::lexer::LexerFactory;
using real_talk::lexer::Lexer;
using real_talk::lexer::TokenInfo;
using real_talk::parser::ParserFactory;
using real_talk::util::IOError;

namespace real_talk {
namespace parser {

class LexerFactoryMock: public LexerFactory {
 public:
  virtual unique_ptr<Lexer> Create(istream &stream) const override {
    return unique_ptr<Lexer>(Create_(stream));
  }

  MOCK_CONST_METHOD1(Create_, Lexer*(istream&));
};

class ParserFactoryMock: public ParserFactory {
 public:
  virtual unique_ptr<Parser> Create(unique_ptr<Lexer> lexer) const override {
    return unique_ptr<Parser>(Create_(lexer.release()));
  }

  MOCK_CONST_METHOD1(Create_, Parser*(Lexer*));
};

class LexerMock: public Lexer {
  MOCK_METHOD0(GetNextToken, TokenInfo());
};

class ParserMock: public Parser {
 public:
  MOCK_METHOD0(Parse, shared_ptr<ProgramNode>());
};

class SimpleFileParserTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }
};

TEST_F(SimpleFileParserTest, Parse) {
  vector< unique_ptr<StmtNode> > stmts;
  shared_ptr<ProgramNode> expected_program(new ProgramNode(move(stmts)));

  LexerFactoryMock *lexer_factory_mock = new LexerFactoryMock();
  unique_ptr<LexerFactory> lexer_factory(lexer_factory_mock);
  ParserFactoryMock *parser_factory_mock = new ParserFactoryMock();
  unique_ptr<ParserFactory> parser_factory(parser_factory_mock);
  LexerMock *lexer_mock = new LexerMock();
  unique_ptr<Lexer> lexer(lexer_mock);
  ParserMock *parser_mock = new ParserMock();
  unique_ptr<Parser> parser(parser_mock);

  {
    InSequence sequence;

    EXPECT_CALL(*lexer_factory_mock, Create_(_))
        .Times(1)
        .WillOnce(Return(lexer_mock));
    EXPECT_CALL(*parser_factory_mock, Create_(lexer_mock))
        .Times(1)
        .WillOnce(Return(parser_mock));
    EXPECT_CALL(*parser_mock, Parse())
        .Times(1)
        .WillOnce(Return(expected_program));
  }

  path file_path(TestConfig::GetResourceDir() / "program.rt");
  SimpleFileParser file_parser(*lexer_factory, *parser_factory);
  lexer.release();
  parser.release();

  shared_ptr<ProgramNode> actual_program = file_parser.Parse(file_path);
  ASSERT_EQ(expected_program, actual_program);
}

TEST_F(SimpleFileParserTest, ParseFailsIfCantOpenFile) {
  LexerFactoryMock *lexer_factory_mock = new LexerFactoryMock();
  unique_ptr<LexerFactory> lexer_factory(lexer_factory_mock);
  ParserFactoryMock *parser_factory_mock = new ParserFactoryMock();
  unique_ptr<ParserFactory> parser_factory(parser_factory_mock);

  EXPECT_CALL(*lexer_factory_mock, Create_(_))
      .Times(0);
  EXPECT_CALL(*parser_factory_mock, Create_(_))
      .Times(0);

  path file_path("i_am_not_here.rt");
  SimpleFileParser file_parser(*lexer_factory, *parser_factory);

  try {
    file_parser.Parse(file_path);
    FAIL();
  } catch (const IOError&) {
  }
}
}
}
