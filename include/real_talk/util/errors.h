
#ifndef _REAL_TALK_UTIL_ERRORS_H_
#define _REAL_TALK_UTIL_ERRORS_H_

#include <stdexcept>
#include <string>

namespace real_talk {
namespace util {

class IOError: public std::runtime_error {
 public:
  explicit IOError(const std::string &msg);
};

class FileNotFoundError: public IOError {
 public:
  explicit FileNotFoundError(const std::string &msg);
};
}
}
#endif
