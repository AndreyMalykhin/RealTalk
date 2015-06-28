
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
    const vector<IdAddress> &func_defs,
    const vector<string> &global_var_defs,
    const vector<string> &native_func_defs,
    const vector<IdAddresses> &func_refs,
    const vector<IdAddresses> &native_func_refs,
    const vector<IdAddresses> &global_var_refs)
    : CodeContainer(version,
                    move(cmds_code),
                    main_cmds_code_size,
                    native_func_defs,
                    native_func_refs),
      func_defs_(func_defs),
      global_var_defs_(global_var_defs),
      func_refs_(func_refs),
      global_var_refs_(global_var_refs) {}

void Module::Accept(const CodeContainerVisitor &visitor) const {
  visitor.VisitModule(*this);
}

const vector<IdAddress> &Module::GetFuncDefs() const {
  return func_defs_;
}

const vector<IdAddresses> &Module::GetFuncRefs() const {
  return func_refs_;
}

const vector<string> &Module::GetGlobalVarDefs() const {
  return global_var_defs_;
}

const vector<IdAddresses> &Module::GetGlobalVarRefs() const {
  return global_var_refs_;
}

bool Module::IsEqual(const CodeContainer &container) const {
  const Module &rhs = static_cast<const Module&>(container);
  return global_var_refs_
      == rhs.global_var_refs_
      && global_var_defs_ == rhs.global_var_defs_
      && func_refs_ == rhs.func_refs_
      && func_defs_ == rhs.func_defs_;
}

void Module::Print(ostream &stream) {
  stream << "func_defs=\n";

  for (const IdAddress &id_address_of_func_def: func_defs_) {
    stream << id_address_of_func_def << "\n";
  }

  stream << "func_refs=\n";

  for (const IdAddresses &id_addresses_of_func_ref
           : func_refs_) {
    stream << id_addresses_of_func_ref << "\n";
  }

  stream << "global_var_defs=\n";

  for (const string &id_of_global_var_def: global_var_defs_) {
    stream << id_of_global_var_def << "\n";
  }

  stream << "global_var_refs=\n";

  for (const IdAddresses &id_addresses_of_global_var_ref
           : global_var_refs_) {
    stream << id_addresses_of_global_var_ref << "\n";
  }
}
}
}
