
#ifndef _REAL_TALK_LINKER_SIMPLE_LINKER_CONFIG_PARSER_H_
#define _REAL_TALK_LINKER_SIMPLE_LINKER_CONFIG_PARSER_H_

#include <cstdint>
#include <stdexcept>
#include <string>
#include "real_talk/linker/linker_config_parser.h"

namespace real_talk {
namespace linker {

class SimpleLinkerConfigParser: public LinkerConfigParser {
 public:
  virtual void Parse(int argc,
                     const char *argv[],
                     LinkerConfig *config,
                     LinkerCmd *cmd) const override;
  virtual std::string GetHelp() const override;
};
}
}
#endif
