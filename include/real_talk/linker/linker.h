
#ifndef _REAL_TALK_LINKER_LINKER_H_
#define _REAL_TALK_LINKER_LINKER_H_

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

namespace real_talk {
namespace code {

class Module;
class CodeContainer;
}

namespace linker {

class Linker {
 public:
  typedef std::vector< std::unique_ptr<real_talk::code::Module> > Modules;

  class DefsCountOverflowError: public std::runtime_error {
   public:
    explicit DefsCountOverflowError(const std::string &msg)
        : std::runtime_error(msg) {}
  };

  class DuplicateDefError: public std::runtime_error {
   public:
    DuplicateDefError(const std::string &id, const std::string &msg)
        : std::runtime_error(msg), id_(id) {}
    const std::string &GetId() const {return id_;}

   private:
    std::string id_;
  };

  class MissingDefError: public std::runtime_error {
   public:
    MissingDefError(const std::string &id, const std::string &msg)
        : std::runtime_error(msg), id_(id) {}
    const std::string &GetId() const {return id_;}

   private:
    std::string id_;
  };

  virtual ~Linker() {}

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   * @throws real_talk::linker::Linker::DuplicateDefError
   * @throws real_talk::linker::Linker::MissingDefError
   */
  virtual std::unique_ptr<real_talk::code::CodeContainer> Link(
      const Linker::Modules &modules, uint32_t output_version) const = 0;
};
}
}
#endif
