
#ifndef _REAL_TALK_PARSER_PARSER_FACTORY_H_
#define _REAL_TALK_PARSER_PARSER_FACTORY_H_

#include <memory>

namespace real_talk {
namespace parser {

class Parser;

class ParserFactory {
 public:
  virtual ~ParserFactory() {}
  virtual std::unique_ptr<Parser> Create() const = 0;
};
}
}
#endif
