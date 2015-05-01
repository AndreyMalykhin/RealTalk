
#include <boost/format.hpp>
#include <vector>
#include <string>
#include "real_talk/code/module.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/code.h"
#include "real_talk/code/end_cmd.h"

using std::unique_ptr;
using std::vector;
using std::string;
using std::move;
using std::ostream;
using boost::filesystem::path;
using boost::format;

namespace real_talk {
namespace code {

Module::Module(
    uint32_t version,
    unique_ptr<Code> cmds_code,
    const vector<IdAddress> &id_addresses_of_func_defs,
    const vector<string> &ids_of_global_var_defs,
    const vector<string> &ids_of_native_func_defs,
    const vector<IdAddresses> &id_addresses_of_func_refs,
    const vector<IdAddresses> &id_addresses_of_global_var_refs,
    const vector<path> &import_file_paths)
    : version_(version),
      cmds_code_(move(cmds_code)),
      id_addresses_of_func_defs_(id_addresses_of_func_defs),
      ids_of_global_var_defs_(ids_of_global_var_defs),
      ids_of_native_func_defs_(ids_of_native_func_defs),
      id_addresses_of_func_refs_(id_addresses_of_func_refs),
      id_addresses_of_global_var_refs_(id_addresses_of_global_var_refs),
      import_file_paths_(import_file_paths) {
  assert(cmds_code_);
}

Code &Module::GetCmdsCode() {
  return *cmds_code_;
}

const Code &Module::GetCmdsCode() const {
  return *cmds_code_;
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

const vector<string> &Module::GetIdsOfGlobalVarDefs() const {
  return ids_of_global_var_defs_;
}

const vector<string> &Module::GetIdsOfNativeFuncDefs() const {
  return ids_of_native_func_defs_;
}

const vector<IdAddresses> &Module::GetIdAddressesOfGlobalVarRefs() const {
  return id_addresses_of_global_var_refs_;
}

const vector<path> &Module::GetImportFilePaths() const {
  return import_file_paths_;
}

bool operator==(const Module &lhs, const Module &rhs) {
  return lhs.version_ == rhs.version_
      && *(lhs.cmds_code_) == *(rhs.cmds_code_)
      && lhs.import_file_paths_ == rhs.import_file_paths_
      && lhs.id_addresses_of_global_var_refs_ ==
      rhs.id_addresses_of_global_var_refs_
      && lhs.ids_of_native_func_defs_ == rhs.ids_of_native_func_defs_
      && lhs.ids_of_global_var_defs_ == rhs.ids_of_global_var_defs_
      && lhs.id_addresses_of_func_refs_ == rhs.id_addresses_of_func_refs_
      && lhs.id_addresses_of_func_defs_ == rhs.id_addresses_of_func_defs_;
}

ostream &operator<<(ostream &stream, Module &module) {
  stream << "version=" << module.GetVersion() << "\n"
         << "import_file_paths=\n";

  for (const path &import_file_path:
           module.GetImportFilePaths()) {
    stream << import_file_path << "\n";
  }

  stream << "id_addresses_of_func_defs=\n";

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

  stream << "cmds_code_position=" << module.GetCmdsCode().GetPosition()
         << "\ncmds_code=";

  for (const unsigned char *it = module.GetCmdsCode().GetData();
       it != module.GetCmdsCode().GetData() + module.GetCmdsCode().GetSize();
       ++it) {
    stream << (format("%1$#5x") % static_cast<uint32_t>(*it)).str();
  }

  if (module.GetCmdsCode().GetSize() != UINT32_C(0)) {
    CmdReader cmd_reader;
    cmd_reader.SetCode(&module.GetCmdsCode());
    stream << "\ncmds=\n";

    do {
      stream << cmd_reader.GetCode()->GetPosition();
      const Cmd &cmd = cmd_reader.GetNextCmd();
      stream << " " << cmd << "\n";

      if (cmd == EndFuncsCmd()) {
        break;
      }
    } while (true);
  }

  return stream;
}
}
}
