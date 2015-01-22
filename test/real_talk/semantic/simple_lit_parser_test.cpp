
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/semantic/simple_lit_parser.h"

using std::vector;
using std::string;
using std::to_string;
using testing::Test;

namespace real_talk {
namespace semantic {

class SimpleLitParserTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }
};

TEST_F(SimpleLitParserTest, ParseInt) {
  SimpleLitParser lit_parser;
  int32_t actual_int = lit_parser.ParseInt("7");
  ASSERT_EQ(INT32_C(7), actual_int);
}

TEST_F(SimpleLitParserTest, ParseLong) {
  SimpleLitParser lit_parser;
  int64_t actual_long = lit_parser.ParseLong("7L");
  ASSERT_EQ(INT64_C(7), actual_long);
}

TEST_F(SimpleLitParserTest, ParseDouble) {
  SimpleLitParser lit_parser;
  double actual_double = lit_parser.ParseDouble("7.7777777777");
  ASSERT_EQ(7.7777777777, actual_double);
}

TEST_F(SimpleLitParserTest, ParseBool) {
  {
    SimpleLitParser lit_parser;
    bool actual_bool = lit_parser.ParseBool("yeah");
    ASSERT_EQ(true, actual_bool);
  }

  {
    SimpleLitParser lit_parser;
    bool actual_bool = lit_parser.ParseBool("nah");
    ASSERT_EQ(false, actual_bool);
  }
}

TEST_F(SimpleLitParserTest, ParseChar) {
  struct TestData {
    string str;
    char chr;
  };

  vector<TestData> test_data_suits = {{"''", '\0'},
                                      {"'a'", 'a'},
                                      {"'\\z'", 'z'},
                                      {"'\\''", '\''},
                                      {"'\\\"'", '"'},
                                      {"'\\?'", '\?'},
                                      {"'\\\\'", '\\'},
                                      {"'\\a'", '\a'},
                                      {"'\\b'", '\b'},
                                      {"'\\f'", '\f'},
                                      {"'\\n'", '\n'},
                                      {"'\\r'", '\r'},
                                      {"'\\t'", '\t'},
                                      {"'\\v'", '\v'},
                                      {"'\\x61'", 'a'}};

  for (const TestData &test_data: test_data_suits) {
    SimpleLitParser lit_parser;
    char actual_char = lit_parser.ParseChar(test_data.str);
    ASSERT_EQ(test_data.chr, actual_char);
  }
}

TEST_F(SimpleLitParserTest, ParseString) {
  struct TestData {
    string str;
    string expected_str;
  };

  vector<TestData> test_data_suits = {{"\"ab\"", "ab"},
                                      {"\"\\yz\"", "yz"},
                                      {"\"\\'\\'\"", "''"},
                                      {"\"\\\"\\\"\"", "\"\""},
                                      {"\"\\?\\?\"", "\?\?"},
                                      {"\"\\\\\\\\\"", "\\\\"},
                                      {"\"\\a\\a\"", "\a\a"},
                                      {"\"\\b\\b\"", "\b\b"},
                                      {"\"\\f\\f\"", "\f\f"},
                                      {"\"\\n\\n\"", "\n\n"},
                                      {"\"\\r\\r\"", "\r\r"},
                                      {"\"\\t\\t\"", "\t\t"},
                                      {"\"\\v\\v\"", "\v\v"},
                                      {"\"\\x61\\x62\"", "ab"}};

  for (const TestData &test_data: test_data_suits) {
    SimpleLitParser lit_parser;
    const string &actual_str = lit_parser.ParseString(test_data.str);
    ASSERT_EQ(test_data.expected_str, actual_str);
  }
}

TEST_F(SimpleLitParserTest, ParseIntFailsIfNumberExceeds32Bits) {
  vector<string> test_strs = {"281474976710656", "18446744073709551616"};

  for (const string &str: test_strs) {
    SimpleLitParser lit_parser;

    try {
      lit_parser.ParseInt(str);
      FAIL() << "str=" << str;
    } catch (const LitParser::OutOfRange&) {
    }
  }
}

TEST_F(SimpleLitParserTest, ParseLongFailsIfNumberExceeds64Bits) {
  SimpleLitParser lit_parser;

  try {
    lit_parser.ParseLong("918446744073709551616L");
    FAIL();
  } catch (const LitParser::OutOfRange&) {
  }
}

TEST_F(SimpleLitParserTest, ParseDoubleFailsIfNumberExceeds64Bits) {
  SimpleLitParser lit_parser;
  const string &str = "918446744073709551616" + to_string(1.79769e+308);

  try {
    lit_parser.ParseDouble(str);
    FAIL() << "str=" << str;
  } catch (const LitParser::OutOfRange&) {
  }
}

TEST_F(SimpleLitParserTest, ParseCharFailsIfMultipleChars) {
  SimpleLitParser lit_parser;

  try {
    lit_parser.ParseChar("'ab'");
    FAIL();
  } catch (const LitParser::MultipleCharsError&) {
  }
}

TEST_F(SimpleLitParserTest, ParseCharFailsIfHexValueIsEmpty) {
  vector<string> test_strs = {"'\\x'", "'\\xZ'"};

  for (const string &str: test_strs) {
    SimpleLitParser lit_parser;

    try {
      lit_parser.ParseChar(str);
      FAIL();
    } catch (const LitParser::EmptyHexValueError&) {
    }
  }
}

TEST_F(SimpleLitParserTest, ParseCharFailsIfHexValueExceeds1Byte) {
  vector<string> test_strs = {"'\\xFFF'"};

  for (const string &str: test_strs) {
    SimpleLitParser lit_parser;

    try {
      lit_parser.ParseChar(str);
      FAIL();
    } catch (const LitParser::OutOfRange&) {
    }
  }
}

TEST_F(SimpleLitParserTest, ParseStringFailsIfHexValueIsEmpty) {
  vector<string> test_strs = {"\"\\x\"", "\"\\xZ\""};

  for (const string &str: test_strs) {
    SimpleLitParser lit_parser;

    try {
      lit_parser.ParseString(str);
      FAIL();
    } catch (const LitParser::EmptyHexValueError&) {
    }
  }
}

TEST_F(SimpleLitParserTest, ParseStringFailsIfHexValueExceeds1Byte) {
  vector<string> test_strs = {"\"\\xFFF\""};

  for (const string &str: test_strs) {
    SimpleLitParser lit_parser;

    try {
      lit_parser.ParseString(str);
      FAIL();
    } catch (const LitParser::OutOfRange&) {
    }
  }
}
}
}
