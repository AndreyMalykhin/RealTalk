
#ifndef _REAL_TALK_PARSER_PARSER_H_
#define _REAL_TALK_PARSER_PARSER_H_

#include <memory>
#include "real_talk/parser/program_node.h"

namespace real_talk {
namespace parser {

class Parser {
 public:
  virtual ~Parser() {}
  virtual std::shared_ptr<ProgramNode> Parse() = 0;
};
}
}
#endif
