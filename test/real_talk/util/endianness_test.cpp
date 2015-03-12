
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/format.hpp>
#include <boost/detail/endian.hpp>
#include <string>
#include <algorithm>
#include <iostream>
#include "real_talk/util/endianness.h"

// #define BOOST_BIG_ENDIAN

using std::ostringstream;
using std::string;
using std::equal;
using boost::format;
using testing::Test;

namespace real_talk {
namespace util {

class EndiannessTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

  string PrintBytes(const unsigned char *bytes, size_t size) {
    ostringstream result;

    for (size_t i = 0; i != size; ++i) {
      result << (format("%02x") % static_cast<uint32_t>(bytes[i]));
    }

    return result.str();
  }

  void AssertBytesEqual(const unsigned char *expected_bytes,
                        const unsigned char *actual_bytes) {
    ASSERT_TRUE(equal(expected_bytes,
                      expected_bytes + sizeof(uint32_t),
                      actual_bytes))
        << "expected_bytes=" << PrintBytes(expected_bytes, sizeof(uint32_t))
        << "; actual_bytes=" << PrintBytes(actual_bytes, sizeof(uint32_t));    
  }
};

TEST_F(EndiannessTest, ToLittleEndian32) {
  unsigned char test_bytes[] = {0x00, 0x00, 0x00, 0x01};
  uint32_t test_value = *(reinterpret_cast<uint32_t*>(test_bytes));

#if defined BOOST_BIG_ENDIAN
  unsigned char expected_bytes[] = {0x01, 0x00, 0x00, 0x00};
#elif defined BOOST_LITTLE_ENDIAN
  unsigned char expected_bytes[] = {0x00, 0x00, 0x00, 0x01};
#else
#error Unsupported endianness
#endif

  uint32_t actual_value = ToLittleEndian32(test_value);
  unsigned char *actual_bytes = reinterpret_cast<unsigned char*>(&actual_value);
  AssertBytesEqual(expected_bytes, actual_bytes);
}

TEST_F(EndiannessTest, FromLittleEndian32) {
  unsigned char test_bytes[] = {0x01, 0x00, 0x00, 0x00};
  uint32_t test_value = *(reinterpret_cast<uint32_t*>(test_bytes));

#if defined BOOST_BIG_ENDIAN
  unsigned char expected_bytes[] = {0x00, 0x00, 0x00, 0x01};
#elif defined BOOST_LITTLE_ENDIAN
  unsigned char expected_bytes[] = {0x01, 0x00, 0x00, 0x00};
#else
#error Unsupported endianness
#endif

  uint32_t actual_value = FromLittleEndian32(test_value);
  unsigned char *actual_bytes = reinterpret_cast<unsigned char*>(&actual_value);
  AssertBytesEqual(expected_bytes, actual_bytes);
}
}
}
