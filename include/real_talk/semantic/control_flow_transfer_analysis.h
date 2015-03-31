
#ifndef _REAL_TALK_SEMANTIC_CONTROL_FLOW_TRANSFER_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_CONTROL_FLOW_TRANSFER_ANALYSIS_H_

#include <cstdint>
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace semantic {

class ControlFlowTransferAnalysis: public NodeSemanticAnalysis {
 public:
  explicit ControlFlowTransferAnalysis(uint32_t flow_local_vars_count);
  uint32_t GetFlowLocalVarsCount() const;

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  uint32_t flow_local_vars_count_;
};
}
}
#endif
