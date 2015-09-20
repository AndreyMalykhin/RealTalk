
#ifndef _REAL_TALK_UTIL_LOGGER_H_
#define _REAL_TALK_UTIL_LOGGER_H_

#include <functional>
#include <iostream>

namespace real_talk {
namespace util {

class Logger {
 public:
  typedef std::function<void(std::ostream *stream)> MsgWriter;

  void SetEnabled(bool is_enabled) noexcept;
  void Log(MsgWriter writer) const;

 private:
  bool is_enabled_ = false;
};

extern Logger g_logger;
}
}
#endif
