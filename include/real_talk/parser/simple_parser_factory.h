
#ifndef _REAL_TALK_PARSER_SIMPLE_PARSER_FACTORY_H_
#define _REAL_TALK_PARSER_SIMPLE_PARSER_FACTORY_H_

#include "real_talk/parser/parser_factory.h"

namespace real_talk {
namespace parser {

class SimpleParserFactory: public ParserFactory {
 public:
  virtual std::unique_ptr<Parser> Create() const override;
};
}
}
#endif
