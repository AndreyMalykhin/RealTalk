
#include <cassert>
#include <vector>
#include "real_talk/parser/func_def_node.h"
#include "real_talk/parser/var_def_node.h"
#include "real_talk/semantic/return_analysis.h"

using std::vector;
using real_talk::parser::FuncDefNode;
using real_talk::parser::VarDefNode;

namespace real_talk {
namespace semantic {

ReturnAnalysis::ReturnAnalysis(
    const FuncDefNode *func_def,
    const vector<const VarDefNode*> &flow_local_var_defs)
    : func_def_(func_def), flow_local_var_defs_(flow_local_var_defs) {
  assert(func_def_);
}

const FuncDefNode *ReturnAnalysis::GetFuncDef() const {
  return func_def_;
}

const vector<const VarDefNode*>
&ReturnAnalysis::GetFlowLocalVarDefs() const {
  return flow_local_var_defs_;
}

bool ReturnAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const ReturnAnalysis &rhs = static_cast<const ReturnAnalysis&>(analysis);
  return func_def_ == rhs.func_def_
      && flow_local_var_defs_ == rhs.flow_local_var_defs_;
}

void ReturnAnalysis::Print(std::ostream &stream) const {
  stream << "func_def=" << *func_def_ << "; flow_local_var_defs=\n";

  for (const VarDefNode *var_def: flow_local_var_defs_) {
    assert(var_def);
    stream << *var_def << '\n';
  }
}
}
}
