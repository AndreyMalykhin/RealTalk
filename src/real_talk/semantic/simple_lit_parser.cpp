
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>
#include <string>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/simple_lit_parser.h"

using std::string;
using boost::numeric_cast;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

int32_t SimpleLitParser::ParseInt(const string &str) const {
  try {
    return numeric_cast<int32_t>(stoi(str));
  } catch (...) {
    // TODO
    assert(false);
  }
}

int64_t SimpleLitParser::ParseLong(const string &str) const {
  try {
    return numeric_cast<int64_t>(stoll(str));
  } catch (...) {
    // TODO
    assert(false);
  }
}

double SimpleLitParser::ParseDouble(const string &str) const {
  try {
    return numeric_cast<double>(stod(str));
  } catch (...) {
    // TODO
    assert(false);
  }
}

bool SimpleLitParser::ParseBool(const string &str) const {
  if (str == SimpleLexer::kTrueKeyword) {
    return true;
  } else if (str == SimpleLexer::kFalseKeyword) {
    return false;
  }

  assert(false);
}

string SimpleLitParser::ParseString(const string &str) const {
  return DoParseString(str);
}

char SimpleLitParser::ParseChar(const string &str) const {
  const string &value = DoParseString(str);
  // TODO
  assert(value.size() == 1);
  return value.c_str()[0];
}

string SimpleLitParser::DoParseString(const string &str) const {
  assert(str.size() > 1);
  auto str_end_it = str.end() - 1;
  auto str_it = str.begin() + 1;
  string value;
  value.reserve(static_cast<size_t>(str_end_it - str_it));
  char previous_char = '\0';
  const char escape_char_ = SimpleLexer::kEscapeChar;

  while (str_it != str_end_it) {
    const char current_char = *str_it;

    if (current_char != escape_char_ || previous_char == escape_char_) {
      value += current_char;
    }

    previous_char = current_char;
    ++str_it;
  }

  return value;
}
}
}
