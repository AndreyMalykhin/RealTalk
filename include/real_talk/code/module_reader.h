
#ifndef _REAL_TALK_CODE_MODULE_READER_H_
#define _REAL_TALK_CODE_MODULE_READER_H_

#include <memory>
#include <iostream>

namespace real_talk {
namespace code {

class Module;
class Code;

class ModuleReader {
 public:
  virtual ~ModuleReader() {}
  virtual std::unique_ptr<Module> ReadFromCode(Code *code) const = 0;

  /**
   * @throws real_talk::util::IOError
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  virtual std::unique_ptr<Module> ReadFromStream(
      std::istream *code_stream) const = 0;
};
}
}
#endif
