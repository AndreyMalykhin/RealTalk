
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
      uint32_t global_vars_size,
      const std::vector<std::string> &native_func_defs,
      const std::vector<IdAddresses> &native_func_refs);
  uint32_t GetGlobalVarsSize() const;
  virtual void Accept(const CodeContainerVisitor &visitor) const override;

 private:
  virtual bool IsEqual(const CodeContainer &container) const override;
  virtual void Print(std::ostream &stream) override;

  uint32_t global_vars_size_;
};
}
}
#endif
