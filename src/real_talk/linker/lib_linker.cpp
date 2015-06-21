
#include <unordered_map>
#include <cassert>
#include <set>
#include <vector>
#include <string>
#include <limits>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/linker/lib_linker.h"

using std::vector;
using std::set;
using std::unordered_map;
using std::string;
using std::numeric_limits;
using std::unique_ptr;
using real_talk::code::CodeContainer;
using real_talk::code::Code;
using real_talk::code::Module;
using real_talk::code::CmdId;
using real_talk::code::IdAddress;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace linker {

unique_ptr<CodeContainer> LibLinker::Link(
    const Linker::Modules &modules, uint32_t output_version) const {
  uint32_t func_cmds_current_address = UINT32_C(0);
  uint32_t cmds_size = UINT32_C(0);

  for (const unique_ptr<Module> &module: modules) {
    func_cmds_current_address += module->GetMainCmdsCodeSize();
    cmds_size += module->GetCmdsCode().GetSize();
  }

  uint32_t main_cmds_current_address = UINT32_C(0);
  unique_ptr<Code> output_cmds(new Code(cmds_size));
  unordered_map< string, set<uint32_t> > id_addresses_of_global_var_refs_map;
  vector<string> ids_of_global_var_refs;

  for (const unique_ptr<Module> &module: modules) {
    output_cmds->SetPosition(main_cmds_current_address);
    const unsigned char *input_cmds = module->GetCmdsCode().GetData();
    const uint32_t main_cmds_size = module->GetMainCmdsCodeSize();
    output_cmds->WriteBytes(input_cmds, main_cmds_size);
    output_cmds->SetPosition(func_cmds_current_address);
    const uint32_t func_cmds_size = module->GetFuncCmdsCodeSize();
    output_cmds->WriteBytes(input_cmds + main_cmds_size, func_cmds_size);

    for (const IdAddresses &id_addresses
             : module->GetIdAddressesOfGlobalVarRefs()) {
      const string &id = id_addresses.GetId();
      auto id_addresses_of_global_var_refs_map_it =
          id_addresses_of_global_var_refs_map.find(id);

      if (id_addresses_of_global_var_refs_map_it
          == id_addresses_of_global_var_refs_map.cend()) {
        id_addresses_of_global_var_refs_map_it =
            id_addresses_of_global_var_refs_map.insert(
                make_pair(id, set<uint32_t>())).first;
        ids_of_global_var_refs.push_back(id);
      }

      set<uint32_t> &addresses = id_addresses_of_global_var_refs_map_it->second;

      for (const uint32_t old_address: id_addresses.GetAddresses()) {
        const bool is_address_in_main_cmds_segment =
            old_address < main_cmds_size;
        const uint32_t new_address = old_address
                          + (is_address_in_main_cmds_segment
                             ? main_cmds_current_address
                             : (func_cmds_current_address - main_cmds_size));
        addresses.insert(new_address);
      }
    }

    main_cmds_current_address += main_cmds_size;
    func_cmds_current_address += func_cmds_size;
  }

  vector<IdAddresses> id_addresses_of_global_var_refs_list;

  for (const string &id: ids_of_global_var_refs) {
    const auto id_addresses_of_global_var_refs_map_it =
        id_addresses_of_global_var_refs_map.find(id);
    assert(id_addresses_of_global_var_refs_map_it
           != id_addresses_of_global_var_refs_map.cend());
    const set<uint32_t> &addresses =
        id_addresses_of_global_var_refs_map_it->second;
    id_addresses_of_global_var_refs_list.push_back(
        IdAddresses(id, vector<uint32_t>(addresses.begin(), addresses.end())));
  }

  vector<string> ids_of_global_var_defs;
  vector<IdAddress> id_addresses_of_func_defs;
  vector<string> ids_of_native_func_defs;
  vector<IdAddresses> id_addresses_of_func_refs;
  vector<IdAddresses> id_addresses_of_native_func_refs;
  const uint32_t main_cmds_size = main_cmds_current_address;
  return unique_ptr<CodeContainer>(new Module(
      output_version,
      move(output_cmds),
      main_cmds_size,
      id_addresses_of_func_defs,
      ids_of_global_var_defs,
      ids_of_native_func_defs,
      id_addresses_of_func_refs,
      id_addresses_of_native_func_refs,
      id_addresses_of_global_var_refs_list));
}
}
}
