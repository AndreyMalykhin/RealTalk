
#include <boost/format.hpp>
#include <vector>
#include <string>
#include "real_talk/code/module.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/code.h"

using std::unique_ptr;
using std::vector;
using std::string;
using std::move;
using std::ostream;
using boost::format;

namespace real_talk {
namespace code {

Module::Module(
    uint32_t version,
    unique_ptr<Code> cmds_code,
    uint32_t main_cmds_code_size,
    const vector<IdAddress> &id_addresses_of_func_defs,
    const vector<string> &ids_of_global_var_defs,
    const vector<string> &ids_of_native_func_defs,
    const vector<IdAddresses> &id_addresses_of_func_refs,
    const vector<IdAddresses> &id_addresses_of_native_func_refs,
    const vector<IdAddresses> &id_addresses_of_global_var_refs)
    : version_(version),
      cmds_code_(move(cmds_code)),
      main_cmds_code_size_(main_cmds_code_size),
      id_addresses_of_func_defs_(id_addresses_of_func_defs),
      ids_of_global_var_defs_(ids_of_global_var_defs),
      ids_of_native_func_defs_(ids_of_native_func_defs),
      id_addresses_of_func_refs_(id_addresses_of_func_refs),
      id_addresses_of_native_func_refs_(id_addresses_of_native_func_refs),
      id_addresses_of_global_var_refs_(id_addresses_of_global_var_refs) {
  assert(cmds_code_);
  assert(cmds_code_->GetSize() >= main_cmds_code_size_);
}

Code &Module::GetCmdsCode() {
  return *cmds_code_;
}

const Code &Module::GetCmdsCode() const {
  return *cmds_code_;
}

uint32_t Module::GetMainCmdsCodeSize() const {
  return main_cmds_code_size_;
}

uint32_t Module::GetFuncCmdsCodeSize() const {
  return cmds_code_->GetSize() - main_cmds_code_size_;
}

uint32_t Module::GetVersion() const {
  return version_;
}

const vector<IdAddress> &Module::GetIdAddressesOfFuncDefs() const {
  return id_addresses_of_func_defs_;
}

const vector<IdAddresses> &Module::GetIdAddressesOfFuncRefs() const {
  return id_addresses_of_func_refs_;
}

const vector<IdAddresses> &Module::GetIdAddressesOfNativeFuncRefs() const {
  return id_addresses_of_native_func_refs_;
}

const vector<string> &Module::GetIdsOfGlobalVarDefs() const {
  return ids_of_global_var_defs_;
}

const vector<string> &Module::GetIdsOfNativeFuncDefs() const {
  return ids_of_native_func_defs_;
}

const vector<IdAddresses> &Module::GetIdAddressesOfGlobalVarRefs() const {
  return id_addresses_of_global_var_refs_;
}

bool operator==(const Module &lhs, const Module &rhs) {
  return lhs.version_ == rhs.version_
      && lhs.main_cmds_code_size_ == rhs.main_cmds_code_size_
      && *(lhs.cmds_code_) == *(rhs.cmds_code_)
      && lhs.id_addresses_of_global_var_refs_
      == rhs.id_addresses_of_global_var_refs_
      && lhs.ids_of_native_func_defs_ == rhs.ids_of_native_func_defs_
      && lhs.ids_of_global_var_defs_ == rhs.ids_of_global_var_defs_
      && lhs.id_addresses_of_func_refs_ == rhs.id_addresses_of_func_refs_
      && lhs.id_addresses_of_native_func_refs_
      == rhs.id_addresses_of_native_func_refs_
      && lhs.id_addresses_of_func_defs_ == rhs.id_addresses_of_func_defs_;
}

ostream &operator<<(ostream &stream, Module &module) {
  stream << "version=" << module.GetVersion() << "\n"
         << "id_addresses_of_func_defs=\n";

  for (const IdAddress &id_address_of_func_def:
           module.GetIdAddressesOfFuncDefs()) {
    stream << id_address_of_func_def << "\n";
  }

  stream << "ids_of_native_func_defs=\n";

  for (const string &id_of_native_func_def:
           module.GetIdsOfNativeFuncDefs()) {
    stream << id_of_native_func_def << "\n";
  }

  stream << "id_addresses_of_func_refs=\n";

  for (const IdAddresses &id_addresses_of_func_ref:
           module.GetIdAddressesOfFuncRefs()) {
    stream << id_addresses_of_func_ref << "\n";
  }

  stream << "id_addresses_of_native_func_refs=\n";

  for (const IdAddresses &id_addresses_of_native_func_ref:
           module.GetIdAddressesOfNativeFuncRefs()) {
    stream << id_addresses_of_native_func_ref << "\n";
  }

  stream << "ids_of_global_var_defs=\n";

  for (const string &id_of_global_var_def:
           module.GetIdsOfGlobalVarDefs()) {
    stream << id_of_global_var_def << "\n";
  }

  stream << "id_addresses_of_global_var_refs=\n";

  for (const IdAddresses &id_addresses_of_global_var_ref:
           module.GetIdAddressesOfGlobalVarRefs()) {
    stream << id_addresses_of_global_var_ref << "\n";
  }

  Code &cmds_code = module.GetCmdsCode();
  stream << "main_cmds_code_size=" << module.GetMainCmdsCodeSize()
         << "\nfunc_cmds_code_size=" << module.GetFuncCmdsCodeSize()
         << "\ntotal_cmds_code_size=" << cmds_code.GetSize()
         << "\ncmds_code_position=" << cmds_code.GetPosition()
         << "\ncmds_code=";

  for (const unsigned char *it = cmds_code.GetData();
       it != cmds_code.GetData() + cmds_code.GetSize();
       ++it) {
    stream << (format("%1$#5x") % static_cast<uint32_t>(*it)).str();
  }

  if (cmds_code.GetSize()) {
    CmdReader cmd_reader;
    cmd_reader.SetCode(&cmds_code);
    stream << "\ncmds=\n";

    while (cmds_code.GetPosition() != cmds_code.GetSize()) {
      stream << cmds_code.GetPosition();
      const Cmd &cmd = cmd_reader.GetNextCmd();
      stream << " " << cmd << "\n";
    }
  }

  return stream;
}
}
}
