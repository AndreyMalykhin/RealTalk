
#include "real_talk/test/path_printer.h"

namespace boost {
namespace filesystem {

void PrintTo(const path &path, std::ostream *stream) {
  *stream << path;
}
}
}
