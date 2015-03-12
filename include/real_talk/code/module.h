
#ifndef _REAL_TALK_CODE_MODULE_H_
#define _REAL_TALK_CODE_MODULE_H_

#include <boost/filesystem.hpp>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include "real_talk/code/id_address.h"

namespace real_talk {
namespace code {

class Code;

class Module {
 public:
  Module(uint32_t version,
         std::unique_ptr<Code> cmds_code,
         const std::vector<IdAddress> &id_addresses_of_func_defs,
         const std::vector<std::string> &ids_of_global_var_defs,
         const std::vector<std::string> &ids_of_native_func_defs,
         const std::vector<IdAddress> &id_addresses_of_func_refs,
         const std::vector<IdAddress> &id_addresses_of_global_var_refs,
         const std::vector<boost::filesystem::path> &import_file_paths);
  Code &GetCmdsCode();
  const Code &GetCmdsCode() const;
  uint32_t GetVersion() const;
  const std::vector<IdAddress> &GetIdAddressesOfFuncDefs() const;
  const std::vector<IdAddress> &GetIdAddressesOfFuncRefs() const;
  const std::vector<std::string> &GetIdsOfGlobalVarDefs() const;
  const std::vector<std::string> &GetIdsOfNativeFuncDefs() const;
  const std::vector<IdAddress> &GetIdAddressesOfGlobalVarRefs() const;
  const std::vector<boost::filesystem::path> &GetImportFilePaths() const;
  friend bool operator==(const Module &lhs, const Module &rhs);
  friend std::ostream &operator<<(std::ostream &stream, Module &module);

 private:
  uint32_t version_;
  std::unique_ptr<Code> cmds_code_;
  std::vector<IdAddress> id_addresses_of_func_defs_;
  std::vector<std::string> ids_of_global_var_defs_;
  std::vector<std::string> ids_of_native_func_defs_;
  std::vector<IdAddress> id_addresses_of_func_refs_;
  std::vector<IdAddress> id_addresses_of_global_var_refs_;
  std::vector<boost::filesystem::path> import_file_paths_;
};
}
}
#endif
