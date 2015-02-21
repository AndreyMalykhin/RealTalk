
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/parser/simple_parser.h"
#include "real_talk/parser/simple_parser_factory.h"

using std::unique_ptr;
using real_talk::lexer::Lexer;

namespace real_talk {
namespace parser {

unique_ptr<Parser> SimpleParserFactory::Create(
    unique_ptr<Lexer> lexer) const {
  return unique_ptr<Parser>(new SimpleParser(move(lexer)));
}
}
}
