
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
         const std::vector<IdAddress> &id_addresses_of_func_defs,
         const std::vector<std::string> &ids_of_global_var_defs,
         const std::vector<std::string> &ids_of_native_func_defs,
         const std::vector<IdAddresses> &id_addresses_of_func_refs,
         const std::vector<IdAddresses> &id_addresses_of_native_func_refs,
         const std::vector<IdAddresses> &id_addresses_of_global_var_refs);
  virtual void Accept(const CodeContainerVisitor &visitor) const override;
  const std::vector<IdAddress> &GetIdAddressesOfFuncDefs() const;
  const std::vector<IdAddresses> &GetIdAddressesOfFuncRefs() const;
  const std::vector<std::string> &GetIdsOfGlobalVarDefs() const;
  const std::vector<IdAddresses> &GetIdAddressesOfGlobalVarRefs() const;

 private:
  virtual bool IsEqual(const CodeContainer &container) const override;
  virtual void Print(std::ostream &stream) override;

  std::vector<IdAddress> id_addresses_of_func_defs_;
  std::vector<std::string> ids_of_global_var_defs_;
  std::vector<IdAddresses> id_addresses_of_func_refs_;
  std::vector<IdAddresses> id_addresses_of_global_var_refs_;
};
}
}
#endif
