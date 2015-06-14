
#ifndef _REAL_TALK_CODE_CODE_CONTAINER_WRITER_H_
#define _REAL_TALK_CODE_CODE_CONTAINER_WRITER_H_

namespace real_talk {
namespace code {

class CodeContainer;
class Code;

class CodeContainerWriter {
 public:
  virtual ~CodeContainerWriter() {}
  virtual void Write(const CodeContainer &container, Code *code) const = 0;
};
}
}
#endif
