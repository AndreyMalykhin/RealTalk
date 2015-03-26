
#include "real_talk/semantic/scope_analysis.h"

using std::ostream;

namespace real_talk {
namespace semantic {

ScopeAnalysis::ScopeAnalysis(uint32_t local_vars_count)
    : local_vars_count_(local_vars_count) {}

uint32_t ScopeAnalysis::GetLocalVarsCount() const {
  return local_vars_count_;
}

bool ScopeAnalysis::IsEqual(const NodeSemanticAnalysis &node) const {
  const ScopeAnalysis &rhs = static_cast<const ScopeAnalysis&>(node);
  return local_vars_count_ == rhs.local_vars_count_;
}

void ScopeAnalysis::Print(ostream &stream) const {
  stream << "local_vars_count=" << local_vars_count_;
}
}
}
