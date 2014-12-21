
#include <boost/numeric/conversion/cast.hpp>
#include <boost/format.hpp>
#include <cassert>
#include <cctype>
#include <string>
#include <stdexcept>
#include "real_talk/lexer/simple_lexer.h"
#include "real_talk/semantic/simple_lit_parser.h"

using std::string;
using std::out_of_range;
using std::runtime_error;
using boost::numeric_cast;
using boost::numeric::bad_numeric_cast;
using boost::format;
using real_talk::lexer::SimpleLexer;

namespace real_talk {
namespace semantic {

int32_t SimpleLitParser::ParseInt(const string &str) const {
  try {
    return numeric_cast<int32_t>(stol(str));
  } catch (const bad_numeric_cast&) {
    throw out_of_range((format("Number %1% exceeds 32 bits") % str).str());
  }
}

int64_t SimpleLitParser::ParseLong(const string &str) const {
  try {
    return numeric_cast<int64_t>(stoll(str));
  } catch (const bad_numeric_cast&) {
    throw out_of_range((format("Number %1% exceeds 64 bits") % str).str());
  }
}

double SimpleLitParser::ParseDouble(const string &str) const {
  try {
    return numeric_cast<double>(stod(str));
  } catch (const bad_numeric_cast&) {
    throw out_of_range((format("Number %1% exceeds 64 bits") % str).str());
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

  if (value.size() > 1) {
    throw MultipleCharsError(str);
  } else if (value.size() == 0) {
    return '\0';
  }

  return value.c_str()[0];
}

string SimpleLitParser::DoParseString(const string &str) const {
  assert(str.size() > 1);
  auto str_end_it = str.end() - 1;
  auto str_it = str.begin() + 1;
  string value;
  value.reserve(static_cast<size_t>(str_end_it - str_it));
  const char escape_char = SimpleLexer::kEscapeChar;

  while (str_it != str_end_it) {
    char current_char = *str_it;

    if (current_char == escape_char) {
      ++str_it;
      assert(str_it != str_end_it);
      current_char = *str_it;

      switch (current_char) {
        case 'a':
          value += '\a';
          break;
        case 'b':
          value += '\b';
          break;
        case 'f':
          value += '\f';
          break;
        case 'n':
          value += '\n';
          break;
        case 'r':
          value += '\r';
          break;
        case 't':
          value += '\t';
          break;
        case 'v':
          value += '\v';
          break;
        case '\\':
          value += escape_char;
          break;
        case 'x': {
          ++str_it;
          value += ParseHexValue(str_it, str_end_it, str);
          break;
        }
        default:
          value += current_char;
          break;
      }
    } else {
      value += current_char;
    }

    ++str_it;
  }

  return value;
}

char SimpleLitParser::ParseHexValue(string::const_iterator &start_it,
                                    string::const_iterator end_it,
                                    const string &str) const {
  const size_t hex_value_start_pos =
      static_cast<size_t>(start_it - str.begin());
  size_t hex_value_length = 0;
  string::const_iterator &str_it = start_it;

  while (str_it != end_it) {
    const char current_char = *str_it;

    if (!isxdigit(current_char)) {
      break;
    }

    ++hex_value_length;
    ++str_it;
  }

  --str_it;

  if (hex_value_length == 0) {
    throw EmptyHexValueError(str);
  }

  const string &hex_value = str.substr(hex_value_start_pos, hex_value_length);

  try {
    return numeric_cast<char>(stoi(hex_value, 0, 16));
  } catch (const bad_numeric_cast&) {
    throw out_of_range(
        (format("Hex value %1% exceeds 1 byte") % hex_value).str());
  }
}

MultipleCharsError::MultipleCharsError(const std::string &msg)
    : runtime_error(msg) {
}

EmptyHexValueError::EmptyHexValueError(const std::string &msg)
    : runtime_error(msg) {
}
}
}
