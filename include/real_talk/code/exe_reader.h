
#ifndef _REAL_TALK_CODE_EXE_READER_H_
#define _REAL_TALK_CODE_EXE_READER_H_

#include <memory>
#include <iostream>

namespace real_talk {
namespace code {

class Code;
class Exe;

class ExeReader {
 public:
  virtual ~ExeReader() {}
  virtual std::unique_ptr<Exe> ReadFromCode(Code *code) const = 0;

  /**
   * @throws real_talk::util::IOError
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  virtual std::unique_ptr<Exe> ReadFromStream(
      std::istream *stream) const = 0;
};
}
}
#endif
