
#ifndef _REAL_TALK_CODE_CODE_H_
#define _REAL_TALK_CODE_CODE_H_

#include <boost/filesystem.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "real_talk/code/cmd.h"
#include "real_talk/code/id_addresses.h"
#include "real_talk/code/id_address.h"

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

  Code(const unsigned char *bytes, uint32_t size);
  Code();
  unsigned char *GetData() noexcept;
  const unsigned char *GetData() const noexcept;
  unsigned char *GetDataAtPosition() noexcept;
  const unsigned char *GetDataAtPosition() const noexcept;
  uint32_t GetSize() const noexcept;
  uint32_t GetPosition() const noexcept;
  void SetPosition(uint32_t position) noexcept;
  uint8_t ReadUint8() noexcept;
  uint32_t ReadUint32() noexcept;
  uint64_t ReadUint64() noexcept;
  int32_t ReadInt32() noexcept;
  int64_t ReadInt64() noexcept;
  double ReadDouble() noexcept;
  bool ReadBool() noexcept;
  char ReadChar() noexcept;
  std::string ReadString();
  CmdId ReadCmdId() noexcept;
  IdAddresses ReadIdAddresses();
  IdAddress ReadIdAddress();
  boost::filesystem::path ReadFilePath();
  friend bool operator==(const Code &lhs, const Code &rhs);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void Skip(uint32_t bytes_count);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteUint8(uint8_t value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteUint32(uint32_t value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteUint64(uint64_t value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteInt32(int32_t value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteInt64(int64_t value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteDouble(double value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteBool(bool value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteChar(char value);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteBytes(const unsigned char *bytes, uint32_t count);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteString(const std::string &str);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteCmdId(CmdId id);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteIdAddresses(const IdAddresses &id_address);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteIdAddress(const IdAddress &id_address);

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  void WriteFilePath(const boost::filesystem::path &file_path);

 private:
  bool HasEnoughCapacity(uint32_t bytes_count) const;
  bool HasEnoughSize(uint32_t bytes_count) const;
  void EnsureCapacity(uint32_t bytes_count);
  void AfterWrite(uint32_t count);

  uint32_t size_;
  uint32_t capacity_;
  std::unique_ptr<unsigned char[]> bytes_;
  unsigned char *current_byte_;
};
}
}
#endif
