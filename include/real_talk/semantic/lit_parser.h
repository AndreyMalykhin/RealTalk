
#ifndef _REAL_TALK_SEMANTIC_LIT_PARSER_H_
#define _REAL_TALK_SEMANTIC_LIT_PARSER_H_

#include <cstdint>
#include <string>

namespace real_talk {
namespace semantic {

class LitParser {
 public:
  virtual ~LitParser() {}
  virtual bool ParseBool(const std::string &str) const = 0;

  /**
   * @throws std::out_of_range
   */
  virtual std::int32_t ParseInt(const std::string &str) const = 0;

  /**
   * @throws std::out_of_range
   */
  virtual std::int64_t ParseLong(const std::string &str) const = 0;

  /**
   * @throws std::out_of_range
   */
  virtual double ParseDouble(const std::string &str) const = 0;

  /**
   * @throws std::out_of_range
   * @throws real_talk::semantic::EmptyHexValueError
   * @throws real_talk::semantic::MultipleCharsError
   */
  virtual char ParseChar(const std::string &str) const = 0;

  /**
   * @throws std::out_of_range
   * @throws real_talk::semantic::EmptyHexValueError
   */
  virtual std::string ParseString(const std::string &str) const = 0;
};
}
}
#endif
