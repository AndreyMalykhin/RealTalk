
#include <string>
#include "real_talk/util/errors.h"

using std::string;

namespace real_talk {
namespace util {

IOError::IOError(const string &msg): runtime_error(msg) {
}

FileNotFoundError::FileNotFoundError(const string &msg): runtime_error(msg) {
}
}
}
