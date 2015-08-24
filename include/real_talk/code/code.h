
#ifndef _REAL_TALK_CODE_CODE_H_
#define _REAL_TALK_CODE_CODE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "real_talk/code/cmd.h"
#include "real_talk/code/id_addresses.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/id_size.h"

namespace real_talk {
namespace code {

class Code {
 public:
  class CodeSizeOverflowError: public std::runtime_error {
   public:
    explicit CodeSizeOverflowError(const std::string &msg);
  };

  /**
   * @throws real_talk::util::IOError
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  explicit Code(std::istream &stream);

  explicit Code(uint32_t size);
  Code(const unsigned char *bytes, uint32_t size);
  Code();
  unsigned char *GetData() noexcept;
  const unsigned char *GetData() const noexcept;
  unsigned char *GetDataAtPosition() noexcept;
  const unsigned char *GetDataAtPosition() const noexcept;
  uint32_t GetSize() const noexcept;
  uint32_t GetPosition() const noexcept;
  void SetPosition(uint32_t position) noexcept;
  template<typename T> T Read();

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  template<typename T> void Write(const T &value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void Write(const unsigned char *bytes, uint32_t count);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void Skip(uint32_t bytes_count);
  friend bool operator==(const Code &lhs, const Code &rhs);

 private:
  bool HasEnoughCapacity(uint32_t bytes_count) const noexcept;
  bool HasEnoughSize(uint32_t bytes_count) const noexcept;
  void EnsureCapacity(uint32_t bytes_count);
  void AfterWrite(uint32_t count) noexcept;

  uint32_t size_;
  uint32_t capacity_;
  std::unique_ptr<unsigned char[]> bytes_;
  unsigned char *current_byte_;
};
}
}
#endif
