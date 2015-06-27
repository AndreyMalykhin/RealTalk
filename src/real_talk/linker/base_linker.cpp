
#include <boost/format.hpp>
#include <cassert>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/linker/base_linker.h"

using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::string;
using boost::format;
using real_talk::code::Code;
using real_talk::code::Module;
using real_talk::code::IdAddresses;
using real_talk::code::IdAddress;

namespace real_talk {
namespace linker {

void BaseLinker::ThrowDuplicateDefError(const string &id) {
  throw Linker::DuplicateDefError(
      id, (format("Duplicate definition; id=%1%") % id).str());
}

void BaseLinker::ThrowMissingDefError(const string &id) {
  throw Linker::MissingDefError(
      id, (format("Missing definition; id=%1%") % id).str());
}

uint32_t BaseLinker::GetAbsoluteAddress(uint32_t relative_address,
                                        uint32_t main_cmds_size,
                                        uint32_t main_cmds_start_address,
                                        uint32_t func_cmds_start_address) {
  const bool is_address_in_main_cmds_segment =
      relative_address < main_cmds_size;
  return relative_address + (is_address_in_main_cmds_segment
                             ? main_cmds_start_address
                             : (func_cmds_start_address - main_cmds_size));
}

void BaseLinker::CheckAddressOverflow(uint32_t address, uint32_t addition) {
  if (address + addition < address) {
    throw Code::CodeSizeOverflowError("Code size exceeds 32 bits");
  }
}

void BaseLinker::CollectRefs(
    uint32_t main_cmds_size,
    uint32_t main_cmds_start_address,
    uint32_t func_cmds_start_address,
    const vector<IdAddresses> &input_refs,
    IdIndexes *output_id_indexes,
    vector<IdAddresses> *output_refs) {
  for (const IdAddresses &input_refs_item: input_refs) {
    const string &id = input_refs_item.GetId();
    size_t id_index = output_refs->size();
    const auto insertion_result =
        output_id_indexes->insert(make_pair(id, id_index));
    const bool is_id_already_exists = !insertion_result.second;

    if (is_id_already_exists) {
      const auto id_indexes_it = insertion_result.first;
      id_index = id_indexes_it->second;
    } else {
      output_refs->push_back(IdAddresses(id, vector<uint32_t>()));
    }

    assert(id_index < output_refs->size());
    vector<uint32_t> &output_addresses =
        (*output_refs)[id_index].GetAddresses();

    for (const uint32_t relative_address: input_refs_item.GetAddresses()) {
      const uint32_t absolute_address = GetAbsoluteAddress(
          relative_address,
          main_cmds_size,
          main_cmds_start_address,
          func_cmds_start_address);
      output_addresses.push_back(absolute_address);
    }
  }
}

void BaseLinker::CollectDefs(const vector<string> &input_defs,
                             unordered_set<string> *output_unique_defs,
                             vector<string> *output_ordered_defs) {
  for (const string &id: input_defs) {
    const bool is_duplicate_id = !output_unique_defs->insert(id).second;

    if (is_duplicate_id) {
      ThrowDuplicateDefError(id);
    }

    output_ordered_defs->push_back(id);
  }
}

void BaseLinker::WriteModuleCmds(const Module &module,
                                 uint32_t main_cmds_start_address,
                                 uint32_t func_cmds_start_address,
                                 Code *output_cmds) {
  output_cmds->SetPosition(main_cmds_start_address);
  const unsigned char * const module_main_cmds = module.GetCmdsCode().GetData();
  const uint32_t module_main_cmds_size = module.GetMainCmdsCodeSize();
  output_cmds->WriteBytes(module_main_cmds, module_main_cmds_size);
  output_cmds->SetPosition(func_cmds_start_address);
  const unsigned char * const module_func_cmds =
      module_main_cmds + module_main_cmds_size;
  output_cmds->WriteBytes(module_func_cmds, module.GetFuncCmdsCodeSize());
}
}
}
