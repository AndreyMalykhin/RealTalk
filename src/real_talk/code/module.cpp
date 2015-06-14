
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

bool Module::IsEqual(const CodeContainer &container) const {
  const Module &rhs = static_cast<const Module&>(container);
  return version_ == rhs.version_
      && main_cmds_code_size_ == rhs.main_cmds_code_size_
      && *(cmds_code_) == *(rhs.cmds_code_)
      && id_addresses_of_global_var_refs_
      == rhs.id_addresses_of_global_var_refs_
      && ids_of_native_func_defs_ == rhs.ids_of_native_func_defs_
      && ids_of_global_var_defs_ == rhs.ids_of_global_var_defs_
      && id_addresses_of_func_refs_ == rhs.id_addresses_of_func_refs_
      && id_addresses_of_native_func_refs_
      == rhs.id_addresses_of_native_func_refs_
      && id_addresses_of_func_defs_ == rhs.id_addresses_of_func_defs_;
}

void Module::Print(ostream &stream) {
  stream << "version=" << version_ << "\n"
         << "id_addresses_of_func_defs=\n";

  for (const IdAddress &id_address_of_func_def: id_addresses_of_func_defs_) {
    stream << id_address_of_func_def << "\n";
  }

  stream << "ids_of_native_func_defs=\n";

  for (const string &id_of_native_func_def: ids_of_native_func_defs_) {
    stream << id_of_native_func_def << "\n";
  }

  stream << "id_addresses_of_func_refs=\n";

  for (const IdAddresses &id_addresses_of_func_ref
           : id_addresses_of_func_refs_) {
    stream << id_addresses_of_func_ref << "\n";
  }

  stream << "id_addresses_of_native_func_refs=\n";

  for (const IdAddresses &id_addresses_of_native_func_ref
           : id_addresses_of_native_func_refs_) {
    stream << id_addresses_of_native_func_ref << "\n";
  }

  stream << "ids_of_global_var_defs=\n";

  for (const string &id_of_global_var_def: ids_of_global_var_defs_) {
    stream << id_of_global_var_def << "\n";
  }

  stream << "id_addresses_of_global_var_refs=\n";

  for (const IdAddresses &id_addresses_of_global_var_ref
           : id_addresses_of_global_var_refs_) {
    stream << id_addresses_of_global_var_ref << "\n";
  }

  stream << "main_cmds_code_size=" << main_cmds_code_size_
         << "\nfunc_cmds_code_size=" << GetFuncCmdsCodeSize()
         << "\ntotal_cmds_code_size=" << cmds_code_->GetSize()
         << "\ncmds_code_position=" << cmds_code_->GetPosition();

  if (cmds_code_->GetSize()) {
    CmdReader cmd_reader;
    cmd_reader.SetCode(cmds_code_.get());
    stream << "\ncmds=\n";

    while (cmds_code_->GetPosition() != cmds_code_->GetSize()) {
      stream << cmds_code_->GetPosition();
      const Cmd &cmd = cmd_reader.GetNextCmd();
      stream << " " << cmd << "\n";
    }
  }
}
}
}
