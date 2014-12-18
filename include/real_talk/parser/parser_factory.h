
#ifndef _REAL_TALK_PARSER_PARSER_FACTORY_H_
#define _REAL_TALK_PARSER_PARSER_FACTORY_H_

#include <memory>
#include <string>
#include "real_talk/parser/parser.h"

namespace real_talk {
namespace parser {

class ParserFactory {
 public:
  virtual ~ParserFactory() {}
  virtual std::unique_ptr<Parser> Create(const std::string &file_path)
      const = 0;
};
}
}
#endif
