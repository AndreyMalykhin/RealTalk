
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
    const vector<std::string> &ids_of_native_func_defs,
    const vector<IdAddresses> &id_addresses_of_native_func_refs)
    : CodeContainer(version,
                    move(cmds_code),
                    main_cmds_code_size,
                    ids_of_native_func_defs,
                    id_addresses_of_native_func_refs) {}

void Exe::Accept(const CodeContainerVisitor &visitor) const {
  visitor.VisitExe(*this);
}

bool Exe::IsEqual(const CodeContainer&) const {return true;}

void Exe::Print(ostream&) {}
}
}
