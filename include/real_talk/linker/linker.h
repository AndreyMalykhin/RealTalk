
#ifndef _REAL_TALK_LINKER_LINKER_H_
#define _REAL_TALK_LINKER_LINKER_H_

#include <cstdint>
#include <memory>
#include <vector>

namespace real_talk {
namespace code {

class Module;
class CodeContainer;
}

namespace linker {

class Linker {
 public:
  typedef std::vector< std::unique_ptr<real_talk::code::Module> > Modules;

  virtual ~Linker() {}
  virtual std::unique_ptr<real_talk::code::CodeContainer> Link(
      const Linker::Modules &modules,
      uint32_t output_code_version) const = 0;
};
}
}
#endif
