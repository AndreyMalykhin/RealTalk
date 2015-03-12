
#ifndef _REAL_TALK_CODE_MODULE_READER_H_
#define _REAL_TALK_CODE_MODULE_READER_H_

#include <memory>
#include <iostream>

namespace real_talk {
namespace code {

class Module;

class ModuleReader {
 public:
  /**
   * @throws real_talk::util::IOError
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  std::unique_ptr<Module> Read(std::istream &code_stream);
};
}
}
#endif
