
#ifndef _REAL_TALK_LINKER_LIB_LINKER_H_
#define _REAL_TALK_LINKER_LIB_LINKER_H_

#include <string>
#include <vector>
#include "real_talk/linker/base_linker.h"

namespace real_talk {
namespace linker {

class LibLinker: public BaseLinker {
 public:
  virtual std::unique_ptr<real_talk::code::CodeContainer> Link(
      const Linker::Modules &modules,
      uint32_t output_version) const override;

 private:
  static void CollectFuncDefs(
      const std::vector<real_talk::code::IdAddress> &input_defs,
      uint32_t main_cmds_size,
      uint32_t main_cmds_current_address,
      uint32_t func_cmds_current_address,
      std::unordered_set<std::string> *output_ids,
      std::vector<real_talk::code::IdAddress> *output_defs);
};
}
}
#endif
