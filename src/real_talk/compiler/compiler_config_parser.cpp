
#include <string>
#include "real_talk/compiler/compiler_config_parser.h"

using std::string;
using std::runtime_error;

namespace real_talk {
namespace compiler {

CompilerConfigParser::BadArgsError::BadArgsError(const string &msg)
    : runtime_error(msg) {}
}
}
