
#ifndef _REAL_TALK_LINKER_EXE_LINKER_H_
#define _REAL_TALK_LINKER_EXE_LINKER_H_

#include <vector>
#include <string>
#include "real_talk/linker/base_linker.h"

namespace real_talk {
namespace code {

class IdSize;
}

namespace linker {

class ExeLinker: public BaseLinker {
 public:
  virtual std::unique_ptr<real_talk::code::CodeContainer> Link(
      const Linker::Modules &modules,
      uint32_t output_version) const override;

 private:
  static void CollectGlobalVarDefs(
      const std::vector<real_talk::code::IdSize> &var_defs,
      std::unordered_map<std::string, uint32_t> *output_var_indexes,
      uint32_t *next_var_index);
  static void FillAddressPlaceholders(
      const std::vector<real_talk::code::IdAddresses> &placeholders,
      const std::unordered_map<std::string, uint32_t> &addresses,
      uint32_t main_cmds_size,
      uint32_t main_cmds_start_address,
      uint32_t func_cmds_start_address,
      real_talk::code::Code *cmds);

  /**
   * @return Func addresses
   */
  static std::unordered_map<std::string, uint32_t> CollectFuncDefs(
      const Linker::Modules &modules, uint32_t func_cmds_start_address);
};
}
}
#endif
