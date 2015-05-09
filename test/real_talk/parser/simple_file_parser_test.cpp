
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "real_talk/test_config.h"
#include "real_talk/lexer/lexer_factory.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/parser_factory.h"
#include "real_talk/parser/parser.h"
#include "real_talk/parser/simple_file_parser.h"
#include "real_talk/util/errors.h"

using std::unique_ptr;
using std::istream;
using std::vector;
using std::ios;
using testing::Test;
using testing::InSequence;
using testing::_;
using testing::Return;
using boost::filesystem::path;
using real_talk::TestConfig;
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
  virtual unique_ptr<Parser> Create() const override {
    return unique_ptr<Parser>(Create_());
  }

  MOCK_CONST_METHOD0(Create_, Parser*());
};

class LexerMock: public Lexer {
  MOCK_METHOD0(GetNextToken, TokenInfo());
};

class ParserMock: public Parser {
 public:
  virtual unique_ptr<ProgramNode> Parse(Lexer *lexer) override {
    return unique_ptr<ProgramNode>(Parse_(lexer));
  }

  MOCK_METHOD1(Parse_, ProgramNode*(Lexer*));
};

class SimpleFileParserTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleFileParserTest, Parse) {
  vector< unique_ptr<StmtNode> > stmts;
  ProgramNode *expected_program_ptr = new ProgramNode(move(stmts));
  unique_ptr<ProgramNode> expected_program(expected_program_ptr);
  LexerFactoryMock *lexer_factory_mock = new LexerFactoryMock();
  unique_ptr<LexerFactory> lexer_factory(lexer_factory_mock);
  ParserFactoryMock *parser_factory_mock = new ParserFactoryMock();
  unique_ptr<ParserFactory> parser_factory(parser_factory_mock);
  LexerMock *lexer_ptr = new LexerMock();
  unique_ptr<Lexer> lexer(lexer_ptr);
  ParserMock *parser_ptr = new ParserMock();
  unique_ptr<Parser> parser(parser_ptr);

  {
    InSequence sequence;

    EXPECT_CALL(*lexer_factory_mock, Create_(_))
        .Times(1)
        .WillOnce(Return(lexer.release()));
    EXPECT_CALL(*parser_factory_mock, Create_())
        .Times(1)
        .WillOnce(Return(parser.release()));
    EXPECT_CALL(*parser_ptr, Parse_(lexer_ptr))
        .Times(1)
        .WillOnce(Return(expected_program.release()));
  }

  path file_path(TestConfig::GetResourceDir() / "program.rt");
  SimpleFileParser file_parser(*lexer_factory, *parser_factory);
  unique_ptr<ProgramNode> actual_program = file_parser.Parse(file_path);
  ASSERT_EQ(expected_program_ptr, actual_program.get());
}

TEST_F(SimpleFileParserTest, ParseFailsIfCantOpenFile) {
  LexerFactoryMock *lexer_factory_ptr = new LexerFactoryMock();
  unique_ptr<LexerFactory> lexer_factory(lexer_factory_ptr);
  ParserFactoryMock *parser_factory_ptr = new ParserFactoryMock();
  unique_ptr<ParserFactory> parser_factory(parser_factory_ptr);

  EXPECT_CALL(*lexer_factory_ptr, Create_(_))
      .Times(0);
  EXPECT_CALL(*parser_factory_ptr, Create_())
      .Times(0);

  path file_path("i_am_not_here.rt");
  SimpleFileParser file_parser(*lexer_factory, *parser_factory);

  try {
    file_parser.Parse(file_path);
    FAIL();
  } catch (const IOError&) {}
}
}
}
