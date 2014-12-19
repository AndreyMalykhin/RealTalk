
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/lexer/simple_lexer_factory.h"

using std::unique_ptr;
using std::istream;

namespace real_talk {
namespace lexer {

unique_ptr<Lexer> SimpleLexerFactory::Create(istream &stream) const {
  return unique_ptr<Lexer>(new SimpleLexer(stream));
}
}
}
