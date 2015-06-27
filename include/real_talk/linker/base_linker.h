
#ifndef _REAL_TALK_LINKER_BASE_LINKER_H_
#define _REAL_TALK_LINKER_BASE_LINKER_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "real_talk/linker/linker.h"

namespace real_talk {
namespace code {

class Module;
class Code;
class IdAddresses;
class IdAddress;
}

namespace linker {

class BaseLinker: public Linker {
 protected:
  typedef std::unordered_map<std::string, size_t> IdIndexes;
  static void ThrowDuplicateDefError(const std::string &id);
  static void ThrowMissingDefError(const std::string &id);
  static uint32_t GetAbsoluteAddress(uint32_t relative_address,
                                     uint32_t main_cmds_size,
                                     uint32_t main_cmds_start_address,
                                     uint32_t func_cmds_start_address);
  static void CheckAddressOverflow(uint32_t address, uint32_t addition);
  static void CollectRefs(
      uint32_t main_cmds_size,
      uint32_t main_cmds_start_address,
      uint32_t func_cmds_start_address,
      const std::vector<real_talk::code::IdAddresses> &input_refs,
      std::unordered_map<std::string, size_t> *output_id_indexes,
      std::vector<real_talk::code::IdAddresses> *output_refs);
  static void CollectDefs(const std::vector<std::string> &input_defs,
                          std::unordered_set<std::string> *output_unique_defs,
                          std::vector<std::string> *output_ordered_defs);
  static void WriteModuleCmds(const real_talk::code::Module &module,
                              uint32_t main_cmds_start_address,
                              uint32_t func_cmds_start_address,
                              real_talk::code::Code *output_cmds);
};
}
}
#endif
