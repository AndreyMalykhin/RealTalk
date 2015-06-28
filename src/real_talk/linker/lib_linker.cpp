
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
#include "real_talk/linker/lib_linker.h"

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
using real_talk::code::CmdId;
using real_talk::code::IdAddress;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace linker {

void LibLinker::CollectFuncDefs(
    const vector<IdAddress> &input_defs,
    uint32_t main_cmds_size,
    uint32_t main_cmds_start_address,
    uint32_t func_cmds_start_address,
    unordered_set<string> *output_ids,
    vector<IdAddress> *output_defs) {
  for (const IdAddress &input_def: input_defs) {
    const string &id = input_def.GetId();
    const bool is_duplicate_id = !output_ids->insert(id).second;

    if (is_duplicate_id) {
      ThrowDuplicateDefError(id);
    }

    const uint32_t address = GetAbsoluteAddress(input_def.GetAddress(),
                                                main_cmds_size,
                                                main_cmds_start_address,
                                                func_cmds_start_address);
    output_defs->push_back(IdAddress(id, address));
  }
}

unique_ptr<CodeContainer> LibLinker::Link(
    const Linker::Modules &modules, uint32_t output_version) const {
  uint32_t func_cmds_current_address = UINT32_C(0);
  uint32_t cmds_size = UINT32_C(0);

  for (const unique_ptr<Module> &module: modules) {
    CheckAddressOverflow(
        func_cmds_current_address, module->GetMainCmdsCodeSize());
    func_cmds_current_address += module->GetMainCmdsCodeSize();
    CheckAddressOverflow(cmds_size, module->GetCmdsCode().GetSize());
    cmds_size += module->GetCmdsCode().GetSize();
  }

  uint32_t main_cmds_current_address = UINT32_C(0);
  unique_ptr<Code> cmds(new Code(cmds_size));
  vector<IdAddresses> global_var_refs;
  IdIndexes id_indexes_of_global_var_refs;
  vector<IdAddresses> func_refs;
  IdIndexes id_indexes_of_func_refs;
  vector<IdAddresses> native_func_refs;
  IdIndexes id_indexes_of_native_func_refs;
  unordered_set<string> unique_global_var_defs;
  vector<string> ordered_global_var_defs;
  unordered_set<string> unique_native_func_defs;
  vector<string> ordered_native_func_defs;
  unordered_set<string> unique_func_defs;
  vector<IdAddress> ordered_func_defs;

  for (const unique_ptr<Module> &module: modules) {
    WriteModuleCmds(*module,
                    main_cmds_current_address,
                    func_cmds_current_address,
                    cmds.get());
    const uint32_t module_main_cmds_size = module->GetMainCmdsCodeSize();
    CollectDefs(module->GetGlobalVarDefs(),
                &unique_global_var_defs,
                &ordered_global_var_defs);
    CollectDefs(module->GetNativeFuncDefs(),
                &unique_native_func_defs,
                &ordered_native_func_defs);
    CollectFuncDefs(module->GetFuncDefs(),
                    module_main_cmds_size,
                    main_cmds_current_address,
                    func_cmds_current_address,
                    &unique_func_defs,
                    &ordered_func_defs);
    CollectRefs(module_main_cmds_size,
                main_cmds_current_address,
                func_cmds_current_address,
                module->GetGlobalVarRefs(),
                &id_indexes_of_global_var_refs,
                &global_var_refs);
    CollectRefs(module_main_cmds_size,
                main_cmds_current_address,
                func_cmds_current_address,
                module->GetFuncRefs(),
                &id_indexes_of_func_refs,
                &func_refs);
    CollectRefs(module_main_cmds_size,
                main_cmds_current_address,
                func_cmds_current_address,
                module->GetNativeFuncRefs(),
                &id_indexes_of_native_func_refs,
                &native_func_refs);
    main_cmds_current_address += module_main_cmds_size;
    func_cmds_current_address += module->GetFuncCmdsCodeSize();
  }

  for (IdAddresses &global_var_refs_item: global_var_refs) {
    sort(global_var_refs_item.GetAddresses().begin(),
         global_var_refs_item.GetAddresses().end());
  }

  for (IdAddresses &func_refs_item: func_refs) {
    sort(func_refs_item.GetAddresses().begin(),
         func_refs_item.GetAddresses().end());
  }

  for (IdAddresses &native_func_refs_item: native_func_refs) {
    sort(native_func_refs_item.GetAddresses().begin(),
         native_func_refs_item.GetAddresses().end());
  }

  cmds->SetPosition(UINT32_C(0));
  const uint32_t main_cmds_size = main_cmds_current_address;
  return unique_ptr<CodeContainer>(new Module(
      output_version,
      move(cmds),
      main_cmds_size,
      ordered_func_defs,
      ordered_global_var_defs,
      ordered_native_func_defs,
      func_refs,
      native_func_refs,
      global_var_refs));
}
}
}
