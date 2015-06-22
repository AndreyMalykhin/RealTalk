
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <set>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/linker/lib_linker.h"

using std::sort;
using std::vector;
using std::set;
using std::unordered_map;
using std::unordered_set;
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
namespace {

typedef unordered_map<string, size_t> IdIndexes;

uint32_t GetNewAddress(uint32_t old_address,
                       uint32_t main_cmds_size,
                       uint32_t main_cmds_current_address,
                       uint32_t func_cmds_current_address) {
  const bool is_address_in_main_cmds_segment = old_address < main_cmds_size;
  return old_address + (is_address_in_main_cmds_segment
                        ? main_cmds_current_address
                        : (func_cmds_current_address - main_cmds_size));
}

void MergeIdAddressesList(uint32_t main_cmds_size,
                          uint32_t main_cmds_current_address,
                          uint32_t func_cmds_current_address,
                          const vector<IdAddresses> &input_id_addresses,
                          IdIndexes *id_indexes,
                          vector<IdAddresses> *output_id_addresses) {
  for (const IdAddresses &id_addresses_item: input_id_addresses) {
    const string &id = id_addresses_item.GetId();
    size_t id_index = output_id_addresses->size();
    const auto insertion_result = id_indexes->insert(make_pair(id, id_index));
    const bool is_id_already_exists = !insertion_result.second;

    if (is_id_already_exists) {
      const auto id_indexes_it = insertion_result.first;
      id_index = id_indexes_it->second;
    } else {
      output_id_addresses->push_back(IdAddresses(id, vector<uint32_t>()));
    }

    assert(id_index < output_id_addresses->size());
    vector<uint32_t> &output_addresses =
        (*output_id_addresses)[id_index].GetAddresses();

    for (const uint32_t old_address: id_addresses_item.GetAddresses()) {
      const uint32_t new_address = GetNewAddress(old_address,
                                                 main_cmds_size,
                                                 main_cmds_current_address,
                                                 func_cmds_current_address);
      output_addresses.push_back(new_address);
    }
  }
}

void MergeIdAddressList(const vector<IdAddress> &input_id_addresses,
                        uint32_t main_cmds_size,
                        uint32_t main_cmds_current_address,
                        uint32_t func_cmds_current_address,
                        unordered_set<string> *ids,
                        vector<IdAddress> *output_id_addresses) {
  for (const IdAddress &id_address: input_id_addresses) {
    const bool is_duplicate_id =
        !ids->insert(id_address.GetId()).second;
    assert(!is_duplicate_id);
    const uint32_t new_address = GetNewAddress(id_address.GetAddress(),
                                               main_cmds_size,
                                               main_cmds_current_address,
                                               func_cmds_current_address);
    output_id_addresses->push_back(IdAddress(id_address.GetId(), new_address));
  }
}

void MergeIds(const vector<string> &input_id_list,
              unordered_set<string> *output_id_set,
              vector<string> *output_id_list) {
  for (const string &id: input_id_list) {
    const bool is_duplicate_id = !output_id_set->insert(id).second;
    assert(!is_duplicate_id);
    output_id_list->push_back(id);
  }
}
}

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
  vector<IdAddresses> id_addresses_of_global_var_refs;
  IdIndexes id_indexes_of_global_var_refs;
  vector<IdAddresses> id_addresses_of_func_refs;
  IdIndexes id_indexes_of_func_refs;
  vector<IdAddresses> id_addresses_of_native_func_refs;
  IdIndexes id_indexes_of_native_func_refs;
  unordered_set<string> ids_of_global_var_defs_set;
  vector<string> ids_of_global_var_defs_list;
  unordered_set<string> ids_of_native_func_defs_set;
  vector<string> ids_of_native_func_defs_list;
  unordered_set<string> ids_of_func_defs;
  vector<IdAddress> id_addresses_of_func_defs;

  for (const unique_ptr<Module> &module: modules) {
    output_cmds->SetPosition(main_cmds_current_address);
    const unsigned char *input_cmds = module->GetCmdsCode().GetData();
    const uint32_t main_cmds_size = module->GetMainCmdsCodeSize();
    output_cmds->WriteBytes(input_cmds, main_cmds_size);
    output_cmds->SetPosition(func_cmds_current_address);
    const uint32_t func_cmds_size = module->GetFuncCmdsCodeSize();
    output_cmds->WriteBytes(input_cmds + main_cmds_size, func_cmds_size);
    MergeIds(module->GetIdsOfGlobalVarDefs(),
             &ids_of_global_var_defs_set,
             &ids_of_global_var_defs_list);
    MergeIds(module->GetIdsOfNativeFuncDefs(),
             &ids_of_native_func_defs_set,
             &ids_of_native_func_defs_list);
    MergeIdAddressList(module->GetIdAddressesOfFuncDefs(),
                       main_cmds_size,
                       main_cmds_current_address,
                       func_cmds_current_address,
                       &ids_of_func_defs,
                       &id_addresses_of_func_defs);
    MergeIdAddressesList(main_cmds_size,
                         main_cmds_current_address,
                         func_cmds_current_address,
                         module->GetIdAddressesOfGlobalVarRefs(),
                         &id_indexes_of_global_var_refs,
                         &id_addresses_of_global_var_refs);
    MergeIdAddressesList(main_cmds_size,
                         main_cmds_current_address,
                         func_cmds_current_address,
                         module->GetIdAddressesOfFuncRefs(),
                         &id_indexes_of_func_refs,
                         &id_addresses_of_func_refs);
    MergeIdAddressesList(main_cmds_size,
                         main_cmds_current_address,
                         func_cmds_current_address,
                         module->GetIdAddressesOfNativeFuncRefs(),
                         &id_indexes_of_native_func_refs,
                         &id_addresses_of_native_func_refs);
    main_cmds_current_address += main_cmds_size;
    func_cmds_current_address += func_cmds_size;
  }

  for (IdAddresses &id_addresses: id_addresses_of_global_var_refs) {
    sort(id_addresses.GetAddresses().begin(),
         id_addresses.GetAddresses().end());
  }

  for (IdAddresses &id_addresses: id_addresses_of_func_refs) {
    sort(id_addresses.GetAddresses().begin(),
         id_addresses.GetAddresses().end());
  }

  for (IdAddresses &id_addresses: id_addresses_of_native_func_refs) {
    sort(id_addresses.GetAddresses().begin(),
         id_addresses.GetAddresses().end());
  }

  const uint32_t main_cmds_size = main_cmds_current_address;
  return unique_ptr<CodeContainer>(new Module(
      output_version,
      move(output_cmds),
      main_cmds_size,
      id_addresses_of_func_defs,
      ids_of_global_var_defs_list,
      ids_of_native_func_defs_list,
      id_addresses_of_func_refs,
      id_addresses_of_native_func_refs,
      id_addresses_of_global_var_refs));
}
}
}
