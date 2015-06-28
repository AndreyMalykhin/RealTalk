
#ifndef _REAL_TALK_LINKER_LINKER_FACTORY_H_
#define _REAL_TALK_LINKER_LINKER_FACTORY_H_

#include <memory>

namespace real_talk {
namespace linker {

class Linker;
enum class OutputFileType: uint8_t;

class LinkerFactory {
 public:
  virtual ~LinkerFactory() {}
  virtual std::unique_ptr<Linker> Create(
      OutputFileType output_file_type) const = 0;
};
}
}
#endif
