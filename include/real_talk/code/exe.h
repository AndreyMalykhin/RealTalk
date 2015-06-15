
#ifndef _REAL_TALK_CODE_EXE_H_
#define _REAL_TALK_CODE_EXE_H_

#include <vector>
#include <string>
#include "real_talk/code/code_container.h"

namespace real_talk {
namespace code {

class Exe: public CodeContainer {
 public:
  Exe(uint32_t version,
      std::unique_ptr<Code> cmds_code,
      uint32_t main_cmds_code_size,
      const std::vector<std::string> &ids_of_native_func_defs,
      const std::vector<IdAddresses> &id_addresses_of_native_func_refs);
  virtual void Accept(const CodeContainerVisitor &visitor) const override;

 private:
  virtual bool IsEqual(const CodeContainer &container) const override;
  virtual void Print(std::ostream &stream) override;
};
}
}
#endif
