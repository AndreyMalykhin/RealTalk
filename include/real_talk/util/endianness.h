
#ifndef _REAL_TALK_UTIL_ENDIANNESS_H_
#define _REAL_TALK_UTIL_ENDIANNESS_H_

#include <cstdint>
#include <boost/detail/endian.hpp>

// #define BOOST_BIG_ENDIAN

namespace real_talk {
namespace util {

#if defined BOOST_BIG_ENDIAN

namespace {

inline uint32_t SwapBytes32(uint32_t input) {
  return ((input << 24) & 0xFF000000)
      | ((input << 8) & 0x00FF0000)
      | ((input >> 8) & 0x0000FF00)
      | ((input >> 24) & 0x000000FF);
}
}

inline uint32_t ToLittleEndian32(uint32_t input) {
  return SwapBytes32(input);
}

inline uint32_t FromLittleEndian32(uint32_t input) {
  return SwapBytes32(input);
}

#elif defined BOOST_LITTLE_ENDIAN

inline uint32_t ToLittleEndian32(uint32_t input) {
  return input;
}

inline uint32_t FromLittleEndian32(uint32_t input) {
  return input;
}

#else
#error Unsupported endianness
#endif
}
}
#endif
