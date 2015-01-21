
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_LIT_PARSER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_LIT_PARSER_H_

#include <string>
#include "real_talk/semantic/lit_parser.h"

namespace real_talk {
namespace semantic {

class SimpleLitParser: public LitParser {
 public:
  virtual int32_t ParseInt(const std::string &str) const override;
  virtual int64_t ParseLong(const std::string &str) const override;
  virtual double ParseDouble(const std::string &str) const override;
  virtual bool ParseBool(const std::string &str) const override;
  virtual char ParseChar(const std::string &str) const override;
  virtual std::string ParseString(const std::string &str) const override;
 private:
  std::string DoParseString(const std::string &str) const;
  char ParseHexValue(std::string::const_iterator &start_it,
                     std::string::const_iterator end_it,
                     const std::string &str) const;
};
}
}
#endif
