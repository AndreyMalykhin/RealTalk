
#ifndef _REAL_TALK_PARSER_SIMPLE_FILE_PARSER_H_
#define _REAL_TALK_PARSER_SIMPLE_FILE_PARSER_H_

#include "real_talk/parser/file_parser.h"

namespace real_talk {
namespace lexer {

class LexerFactory;
}

namespace parser {

class ParserFactory;

class SimpleFileParser: public FileParser {
 public:
  SimpleFileParser(const real_talk::lexer::LexerFactory &lexer_factory,
                   const ParserFactory &parser_factory);
  virtual std::shared_ptr<ProgramNode> Parse(
      const boost::filesystem::path &file_path) const override;

 private:
  const real_talk::lexer::LexerFactory &lexer_factory_;
  const ParserFactory &parser_factory_;
};
}
}
#endif
