
#include <string>
#include "real_talk/semantic/lit_parser.h"

using std::string;
using std::runtime_error;

namespace real_talk {
namespace semantic {

LitParser::MultipleCharsError::MultipleCharsError(const string &msg)
    : runtime_error(msg) {
}

LitParser::EmptyHexValueError::EmptyHexValueError(const string &msg)
    : runtime_error(msg) {
}

LitParser::OutOfRange::OutOfRange(const string &msg): runtime_error(msg) {}
}
}
