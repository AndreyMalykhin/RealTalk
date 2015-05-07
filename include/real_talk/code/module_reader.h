
#ifndef _REAL_TALK_CODE_MODULE_READER_H_
#define _REAL_TALK_CODE_MODULE_READER_H_

#include <memory>
#include <iostream>
#include "real_talk/code/module.h"

namespace real_talk {
namespace code {

class Code;

class ModuleReader {
 public:
  Module ReadFromCode(Code *code);

  /**
   * @throws real_talk::util::IOError
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  Module ReadFromStream(std::istream *code_stream);
};
}
}
#endif
