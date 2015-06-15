
#include <boost/format.hpp>
#include <vector>
#include <string>
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/code_container.h"

using std::unique_ptr;
using std::vector;
using std::string;
using std::move;
using std::ostream;
using boost::format;

namespace real_talk {
namespace code {

CodeContainer::CodeContainer(
    uint32_t version,
    unique_ptr<Code> cmds_code,
    uint32_t main_cmds_code_size,
    const vector<string> &ids_of_native_func_defs,
    const vector<IdAddresses> &id_addresses_of_native_func_refs)
    : version_(version),
      cmds_code_(move(cmds_code)),
      main_cmds_code_size_(main_cmds_code_size),
      ids_of_native_func_defs_(ids_of_native_func_defs),
      id_addresses_of_native_func_refs_(id_addresses_of_native_func_refs) {
  assert(cmds_code_);
  assert(cmds_code_->GetSize() >= main_cmds_code_size_);
}

Code &CodeContainer::GetCmdsCode() {
  return *cmds_code_;
}

const Code &CodeContainer::GetCmdsCode() const {
  return *cmds_code_;
}

uint32_t CodeContainer::GetMainCmdsCodeSize() const {
  return main_cmds_code_size_;
}

uint32_t CodeContainer::GetFuncCmdsCodeSize() const {
  return cmds_code_->GetSize() - main_cmds_code_size_;
}

uint32_t CodeContainer::GetVersion() const {
  return version_;
}

const vector<IdAddresses> &CodeContainer::GetIdAddressesOfNativeFuncRefs()
    const {
  return id_addresses_of_native_func_refs_;
}

const vector<string> &CodeContainer::GetIdsOfNativeFuncDefs() const {
  return ids_of_native_func_defs_;
}

bool operator==(const CodeContainer &lhs, const CodeContainer &rhs) {
  return typeid(lhs) == typeid(rhs)
      && lhs.version_ == rhs.version_
      && lhs.main_cmds_code_size_ == rhs.main_cmds_code_size_
      && *(lhs.cmds_code_) == *(rhs.cmds_code_)
      && lhs.ids_of_native_func_defs_ == rhs.ids_of_native_func_defs_
      && lhs.id_addresses_of_native_func_refs_
      == rhs.id_addresses_of_native_func_refs_
      && lhs.IsEqual(rhs);
}

ostream &operator<<(ostream &stream, CodeContainer &container) {
  stream << typeid(container).name() << "; version=" << container.version_
         << "\nids_of_native_func_defs=\n";

  for (const string &id_of_native_func_def
           : container.ids_of_native_func_defs_) {
    stream << id_of_native_func_def << "\n";
  }

  stream << "id_addresses_of_native_func_refs=\n";

  for (const IdAddresses &id_addresses_of_native_func_ref
           : container.id_addresses_of_native_func_refs_) {
    stream << id_addresses_of_native_func_ref << "\n";
  }

  Code &cmds_code = *(container.cmds_code_);
  stream << "main_cmds_code_size=" << container.main_cmds_code_size_
         << "\nfunc_cmds_code_size=" << container.GetFuncCmdsCodeSize()
         << "\ntotal_cmds_code_size=" << cmds_code.GetSize()
         << "\ncmds_code_position=" << cmds_code.GetPosition()
         << '\n';

  if (cmds_code.GetSize()) {
    CmdReader cmd_reader;
    cmd_reader.SetCode(&cmds_code);
    stream << "cmds=\n";

    while (cmds_code.GetPosition() != cmds_code.GetSize()) {
      stream << cmds_code.GetPosition();
      const Cmd &cmd = cmd_reader.GetNextCmd();
      stream << " " << cmd << "\n";
    }
  }

  container.Print(stream);
  return stream;
}
}
}
