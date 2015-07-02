
#include <cassert>
#include <vector>
#include "real_talk/parser/var_def_node.h"
#include "real_talk/semantic/control_flow_transfer_analysis.h"

using std::vector;
using std::ostream;
using real_talk::parser::VarDefNode;

namespace real_talk {
namespace semantic {

ControlFlowTransferAnalysis::ControlFlowTransferAnalysis(
    const vector<const VarDefNode*> &flow_local_var_defs)
    : flow_local_var_defs_(flow_local_var_defs) {}

const vector<const VarDefNode*>
&ControlFlowTransferAnalysis::GetFlowLocalVarDefs() const {
  return flow_local_var_defs_;
}

bool ControlFlowTransferAnalysis::IsEqual(const NodeSemanticAnalysis &analysis)
    const {
  const ControlFlowTransferAnalysis &rhs =
      static_cast<const ControlFlowTransferAnalysis&>(analysis);
  return flow_local_var_defs_ == rhs.flow_local_var_defs_;
}

void ControlFlowTransferAnalysis::Print(ostream &stream) const {
  stream << "flow_local_var_defs=\n";

  for (const VarDefNode *var_def: flow_local_var_defs_) {
    assert(var_def);
    stream << *var_def << '\n';
  }
}
}
}
