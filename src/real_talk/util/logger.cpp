
#include "real_talk/util/logger.h"

using std::cout;

namespace real_talk {
namespace util {

Logger g_logger;

void Logger::SetEnabled(bool is_enabled) noexcept {
  is_enabled_ = is_enabled;
}

void Logger::Log(MsgWriter writer) const {
  if (!is_enabled_) {
    return;
  }

  writer(&cout);
}
}
}
