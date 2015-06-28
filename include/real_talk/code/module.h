
#ifndef _REAL_TALK_CODE_MODULE_H_
#define _REAL_TALK_CODE_MODULE_H_

#include <vector>
#include <string>
#include "real_talk/code/id_address.h"
#include "real_talk/code/code_container.h"

namespace real_talk {
namespace code {

class Module: public CodeContainer {
 public:
  Module(uint32_t version,
         std::unique_ptr<Code> cmds_code,
         uint32_t main_cmds_code_size,
         const std::vector<IdAddress> &func_defs,
         const std::vector<std::string> &global_var_defs,
         const std::vector<std::string> &native_func_defs,
         const std::vector<IdAddresses> &func_refs,
         const std::vector<IdAddresses> &native_func_refs,
         const std::vector<IdAddresses> &global_var_refs);
  virtual void Accept(const CodeContainerVisitor &visitor) const override;
  const std::vector<IdAddress> &GetFuncDefs() const;
  const std::vector<IdAddresses> &GetFuncRefs() const;
  const std::vector<std::string> &GetGlobalVarDefs() const;
  const std::vector<IdAddresses> &GetGlobalVarRefs() const;

 private:
  virtual bool IsEqual(const CodeContainer &container) const override;
  virtual void Print(std::ostream &stream) override;

  std::vector<IdAddress> func_defs_;
  std::vector<std::string> global_var_defs_;
  std::vector<IdAddresses> func_refs_;
  std::vector<IdAddresses> global_var_refs_;
};
}
}
#endif
