
#ifndef _REAL_TALK_SEMANTIC_LIT_PARSER_H_
#define _REAL_TALK_SEMANTIC_LIT_PARSER_H_

#include <cstdint>
#include <string>
#include <stdexcept>

namespace real_talk {
namespace semantic {

class LitParser {
 public:
  class MultipleCharsError: public std::runtime_error {
   public:
    explicit MultipleCharsError(const std::string &msg);
  };

  class EmptyHexValueError: public std::runtime_error {
   public:
    explicit EmptyHexValueError(const std::string &msg);
  };

  class OutOfRangeError: public std::runtime_error {
   public:
    explicit OutOfRangeError(const std::string &msg);
  };

  virtual ~LitParser() {}
  virtual bool ParseBool(const std::string &str) const = 0;

  /**
   * @throws real_talk::semantic::LitParser::OutOfRangeError
   */
  virtual int32_t ParseInt(const std::string &str) const = 0;

  /**
   * @throws real_talk::semantic::LitParser::OutOfRangeError
   */
  virtual int64_t ParseLong(const std::string &str) const = 0;

  /**
   * @throws real_talk::semantic::LitParser::OutOfRangeError
   */
  virtual double ParseDouble(const std::string &str) const = 0;

  /**
   * @throws real_talk::semantic::LitParser::OutOfRangeError
   * @throws real_talk::semantic::LitParser::EmptyHexValueError
   * @throws real_talk::semantic::LitParser::MultipleCharsError
   */
  virtual char ParseChar(const std::string &str) const = 0;

  /**
   * @throws real_talk::semantic::LitParser::OutOfRangeError
   * @throws real_talk::semantic::LitParser::EmptyHexValueError
   */
  virtual std::string ParseString(const std::string &str) const = 0;
};
}
}
#endif
