
#ifndef _REAL_TALK_CODE_SIMPLE_EXE_READER_H_
#define _REAL_TALK_CODE_SIMPLE_EXE_READER_H_

#include "real_talk/code/exe_reader.h"

namespace real_talk {
namespace code {

class SimpleExeReader: public ExeReader {
 public:
  virtual std::unique_ptr<Exe> ReadFromCode(Code *code) const override;
  virtual std::unique_ptr<Exe> ReadFromStream(
      std::istream *stream) const override;
};
}
}
#endif
