
#include <string>
#include <vector>
#include "real_talk/code/code_container_visitor.h"
#include "real_talk/code/exe.h"

using std::unique_ptr;
using std::vector;
using std::string;
using std::ostream;

namespace real_talk {
namespace code {

Exe::Exe(
    uint32_t version,
    unique_ptr<Code> cmds_code,
    uint32_t main_cmds_code_size,
    uint32_t global_vars_size,
    const vector<std::string> &native_func_defs,
    const vector<IdAddresses> &native_func_refs)
    : CodeContainer(version,
                    move(cmds_code),
                    main_cmds_code_size,
                    native_func_defs,
                    native_func_refs),
      global_vars_size_(global_vars_size) {}

uint32_t Exe::GetGlobalVarsSize() const {
  return global_vars_size_;
}

void Exe::Accept(const CodeContainerVisitor &visitor) const {
  visitor.VisitExe(*this);
}

bool Exe::IsEqual(const CodeContainer&) const {return true;}

void Exe::Print(ostream&) {}
}
}
