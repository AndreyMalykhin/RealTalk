
#include <boost/format.hpp>
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
#include "real_talk/code/exe.h"
#include "real_talk/linker/exe_linker.h"

using std::sort;
using std::vector;
using std::set;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::numeric_limits;
using std::unique_ptr;
using boost::format;
using real_talk::code::CodeContainer;
using real_talk::code::Code;
using real_talk::code::Module;
using real_talk::code::Exe;
using real_talk::code::CmdId;
using real_talk::code::IdSize;
using real_talk::code::IdAddress;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace linker {

void ExeLinker::CollectGlobalVarDefs(
    const vector<IdSize> &var_defs,
    unordered_map<string, uint32_t> *output_var_indexes,
    uint32_t *next_var_index) {
  for (const IdSize &var_def: var_defs) {
    if (*next_var_index + 1 < *next_var_index) {
      ThrowDefsCountOverflowError();
    }

    const bool is_duplicate_id = !output_var_indexes->insert(
        make_pair(var_def.GetId(), *next_var_index)).second;
    const uint8_t var_size = static_cast<uint8_t>(var_def.GetSize());
    *next_var_index += var_size;

    if (is_duplicate_id) {
      ThrowDuplicateDefError(var_def.GetId());
    }
  }
}

unordered_map<string, uint32_t> ExeLinker::CollectFuncDefs(
    const Linker::Modules &modules, uint32_t func_cmds_start_address) {
  uint32_t main_cmds_current_address = UINT32_C(0);
  uint32_t func_cmds_current_address = func_cmds_start_address;
  unordered_map<string, uint32_t> func_addresses;

  for (const unique_ptr<Module> &module: modules) {
    const uint32_t module_main_cmds_size = module->GetMainCmdsCodeSize();

    for (const IdAddress &func_def: module->GetFuncDefs()) {
      const uint32_t func_address = GetAbsoluteAddress(
          func_def.GetAddress(),
          module_main_cmds_size,
          main_cmds_current_address,
          func_cmds_current_address);
      const bool is_duplicate_id = !func_addresses.insert(
          make_pair(func_def.GetId(), func_address)).second;

      if (is_duplicate_id) {
        ThrowDuplicateDefError(func_def.GetId());
      }
    }

    main_cmds_current_address += module_main_cmds_size;
    func_cmds_current_address += module->GetFuncCmdsCodeSize();
  }

  return func_addresses;
}

void ExeLinker::FillAddressPlaceholders(
    const vector<IdAddresses> &placeholders,
    const unordered_map<string, uint32_t> &addresses,
    uint32_t main_cmds_size,
    uint32_t main_cmds_start_address,
    uint32_t func_cmds_start_address,
    Code *cmds) {
  for (const IdAddresses &placeholder: placeholders) {
    const string &var_def = placeholder.GetId();
    const auto addresses_it = addresses.find(var_def);

    if (addresses_it == addresses.cend()) {
      ThrowMissingDefError(var_def);
    }

    const uint32_t address = addresses_it->second;

    for (const uint32_t relative_placeholder_address
             : placeholder.GetAddresses()) {
      const uint32_t absolute_placeholder_address = GetAbsoluteAddress(
          relative_placeholder_address,
          main_cmds_size,
          main_cmds_start_address,
          func_cmds_start_address);
      cmds->SetPosition(absolute_placeholder_address);
      cmds->WriteUint32(address);
    }
  }
}

unique_ptr<CodeContainer> ExeLinker::Link(
    const Linker::Modules &modules, uint32_t output_version) const {
  uint32_t func_cmds_current_address = UINT32_C(0);
  uint32_t cmds_size = UINT32_C(0);
  uint32_t next_global_var_index = UINT32_C(0);
  unordered_map<string, uint32_t> global_var_indexes;

  for (const unique_ptr<Module> &module: modules) {
    CheckAddressOverflow(
        func_cmds_current_address, module->GetMainCmdsCodeSize());
    func_cmds_current_address += module->GetMainCmdsCodeSize();
    CheckAddressOverflow(cmds_size, module->GetCmdsCode().GetSize());
    cmds_size += module->GetCmdsCode().GetSize();
    CollectGlobalVarDefs(module->GetGlobalVarDefs(),
                         &global_var_indexes,
                         &next_global_var_index);
  }

  const uint32_t func_cmds_start_address = func_cmds_current_address;
  unordered_map<string, uint32_t> func_addresses =
      CollectFuncDefs(modules, func_cmds_start_address);
  uint32_t main_cmds_current_address = UINT32_C(0);
  unique_ptr<Code> cmds(new Code(cmds_size));
  vector<IdAddresses> native_func_refs;
  IdIndexes id_indexes_of_native_func_refs;
  unordered_set<string> unique_native_func_defs;
  vector<string> ordered_native_func_defs;

  for (const unique_ptr<Module> &module: modules) {
    WriteModuleCmds(*module,
                    main_cmds_current_address,
                    func_cmds_current_address,
                    cmds.get());
    const uint32_t module_main_cmds_size = module->GetMainCmdsCodeSize();
    FillAddressPlaceholders(module->GetGlobalVarRefs(),
                            global_var_indexes,
                            module_main_cmds_size,
                            main_cmds_current_address,
                            func_cmds_current_address,
                            cmds.get());
    FillAddressPlaceholders(module->GetFuncRefs(),
                            func_addresses,
                            module_main_cmds_size,
                            main_cmds_current_address,
                            func_cmds_current_address,
                            cmds.get());
    CollectNativeFuncDefs(module->GetNativeFuncDefs(),
                          &unique_native_func_defs,
                          &ordered_native_func_defs);
    CollectRefs(module_main_cmds_size,
                main_cmds_current_address,
                func_cmds_current_address,
                module->GetNativeFuncRefs(),
                &id_indexes_of_native_func_refs,
                &native_func_refs);
    main_cmds_current_address += module_main_cmds_size;
    func_cmds_current_address += module->GetFuncCmdsCodeSize();
  }

  for (IdAddresses &native_func_refs_item: native_func_refs) {
    const string &var_def = native_func_refs_item.GetId();
    const bool is_id_defined = unique_native_func_defs.count(var_def) == 1;

    if (!is_id_defined) {
      ThrowMissingDefError(var_def);
    }

    sort(native_func_refs_item.GetAddresses().begin(),
         native_func_refs_item.GetAddresses().end());
  }

  cmds->SetPosition(UINT32_C(0));
  const uint32_t main_cmds_size = main_cmds_current_address;
  const uint32_t global_vars_size = next_global_var_index;
  return unique_ptr<CodeContainer>(new Exe(
      output_version,
      move(cmds),
      main_cmds_size,
      global_vars_size,
      ordered_native_func_defs,
      native_func_refs));
}
}
}
