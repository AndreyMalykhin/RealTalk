
#include <cassert>
#include <vector>
#include "real_talk/parser/var_def_node.h"
#include "real_talk/semantic/scope_analysis.h"

using std::ostream;
using std::vector;
using real_talk::parser::VarDefNode;

namespace real_talk {
namespace semantic {

ScopeAnalysis::ScopeAnalysis(const vector<const VarDefNode*> &local_var_defs)
    : local_var_defs_(local_var_defs) {}

const vector<const VarDefNode*> &ScopeAnalysis::GetLocalVarDefs() const {
  return local_var_defs_;
}

bool ScopeAnalysis::IsEqual(const NodeSemanticAnalysis &node) const {
  const ScopeAnalysis &rhs = static_cast<const ScopeAnalysis&>(node);
  return local_var_defs_ == rhs.local_var_defs_;
}

void ScopeAnalysis::Print(ostream &stream) const {
  stream << "local_var_defs=\n";

  for (const VarDefNode *var_def: local_var_defs_) {
    assert(var_def);
    stream << *var_def << '\n';
  }
}
}
}
