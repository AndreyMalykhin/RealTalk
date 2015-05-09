
#include <boost/format.hpp>
#include <boost/filesystem/fstream.hpp>
#include "real_talk/lexer/lexer.h"
#include "real_talk/lexer/lexer_factory.h"
#include "real_talk/util/errors.h"
#include "real_talk/parser/parser.h"
#include "real_talk/parser/parser_factory.h"
#include "real_talk/parser/simple_file_parser.h"

using std::shared_ptr;
using std::unique_ptr;
using boost::filesystem::path;
using boost::filesystem::ifstream;
using boost::format;
using real_talk::lexer::LexerFactory;
using real_talk::lexer::Lexer;
using real_talk::util::IOError;

namespace real_talk {
namespace parser {

SimpleFileParser::SimpleFileParser(
    const LexerFactory &lexer_factory,
    const ParserFactory &parser_factory)
    : lexer_factory_(lexer_factory),
      parser_factory_(parser_factory) {
}

unique_ptr<ProgramNode> SimpleFileParser::Parse(const path &file_path) const {
  ifstream stream(file_path);

  if (!stream.is_open()) {
    throw IOError((format("Can't open file: %1%") % file_path).str());
  }

  unique_ptr<Lexer> lexer = lexer_factory_.Create(stream);
  unique_ptr<Parser> parser = parser_factory_.Create();
  return parser->Parse(lexer.get());
}
}
}
