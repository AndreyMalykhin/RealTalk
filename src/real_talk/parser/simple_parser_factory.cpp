
#include "real_talk/parser/simple_parser.h"
#include "real_talk/parser/simple_parser_factory.h"

using std::unique_ptr;

namespace real_talk {
namespace parser {

unique_ptr<Parser> SimpleParserFactory::Create() const {
  return unique_ptr<Parser>(new SimpleParser());
}
}
}
