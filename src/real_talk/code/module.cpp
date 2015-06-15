
#include <boost/format.hpp>
#include <vector>
#include <string>
#include "real_talk/code/code_container_visitor.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"

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
    : CodeContainer(version,
                    move(cmds_code),
                    main_cmds_code_size,
                    ids_of_native_func_defs,
                    id_addresses_of_native_func_refs),
      id_addresses_of_func_defs_(id_addresses_of_func_defs),
      ids_of_global_var_defs_(ids_of_global_var_defs),
      id_addresses_of_func_refs_(id_addresses_of_func_refs),
      id_addresses_of_global_var_refs_(id_addresses_of_global_var_refs) {}

void Module::Accept(const CodeContainerVisitor &visitor) const {
  visitor.VisitModule(*this);
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

const vector<IdAddresses> &Module::GetIdAddressesOfGlobalVarRefs() const {
  return id_addresses_of_global_var_refs_;
}

bool Module::IsEqual(const CodeContainer &container) const {
  const Module &rhs = static_cast<const Module&>(container);
  return id_addresses_of_global_var_refs_
      == rhs.id_addresses_of_global_var_refs_
      && ids_of_global_var_defs_ == rhs.ids_of_global_var_defs_
      && id_addresses_of_func_refs_ == rhs.id_addresses_of_func_refs_
      && id_addresses_of_func_defs_ == rhs.id_addresses_of_func_defs_;
}

void Module::Print(ostream &stream) {
  stream << "id_addresses_of_func_defs=\n";

  for (const IdAddress &id_address_of_func_def: id_addresses_of_func_defs_) {
    stream << id_address_of_func_def << "\n";
  }

  stream << "id_addresses_of_func_refs=\n";

  for (const IdAddresses &id_addresses_of_func_ref
           : id_addresses_of_func_refs_) {
    stream << id_addresses_of_func_ref << "\n";
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
}
}
}
