
#include "real_talk/semantic/control_flow_transfer_analysis.h"

using std::ostream;

namespace real_talk {
namespace semantic {

ControlFlowTransferAnalysis::ControlFlowTransferAnalysis(
    uint32_t flow_local_vars_count)
    : flow_local_vars_count_(flow_local_vars_count) {
}

uint32_t ControlFlowTransferAnalysis::GetFlowLocalVarsCount() const {
  return flow_local_vars_count_;
}

bool ControlFlowTransferAnalysis::IsEqual(const NodeSemanticAnalysis &analysis)
    const {
  const ControlFlowTransferAnalysis &rhs =
      static_cast<const ControlFlowTransferAnalysis&>(analysis);
  return flow_local_vars_count_ == rhs.flow_local_vars_count_;
}

void ControlFlowTransferAnalysis::Print(ostream &stream) const {
  stream << "flow_local_vars_count=" << flow_local_vars_count_;
}
}
}
