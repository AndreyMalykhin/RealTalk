
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

inline uint64_t SwapBytes64(uint64_t input) {
  return ((input << 56) & 0xFF00000000000000)
      | ((input << 40) & 0x00FF000000000000)
      | ((input << 24) & 0x0000FF0000000000)
      | ((input << 8) & 0x000000FF00000000)
      | ((input >> 8) & 0x00000000FF000000)
      | ((input >> 24) & 0x0000000000FF0000)
      | ((input >> 40) & 0x000000000000FF00)
      | ((input >> 56) & 0x00000000000000FF);
}
}

inline uint32_t ToLittleEndian32(uint32_t input) {
  return SwapBytes32(input);
}

inline uint32_t FromLittleEndian32(uint32_t input) {
  return SwapBytes32(input);
}

inline uint64_t ToLittleEndian64(uint64_t input) {
  return SwapBytes64(input);
}

inline uint64_t FromLittleEndian64(uint64_t input) {
  return SwapBytes64(input);
}

#elif defined BOOST_LITTLE_ENDIAN

inline uint32_t ToLittleEndian32(uint32_t input) {
  return input;
}

inline uint32_t FromLittleEndian32(uint32_t input) {
  return input;
}

inline uint64_t ToLittleEndian64(uint64_t input) {
  return input;
}

inline uint64_t FromLittleEndian64(uint64_t input) {
  return input;
}

#else
#error Unsupported endianness
#endif
}
}
#endif
