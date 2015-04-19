
#include <cassert>
#include "real_talk/parser/func_def_node.h"
#include "real_talk/semantic/return_analysis.h"

using real_talk::parser::FuncDefNode;

namespace real_talk {
namespace semantic {

ReturnAnalysis::ReturnAnalysis(const FuncDefNode *func_def)
    : func_def_(func_def) {
  assert(func_def_);
}

const FuncDefNode *ReturnAnalysis::GetFuncDef() const {
  return func_def_;
}

bool ReturnAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const ReturnAnalysis &rhs = static_cast<const ReturnAnalysis&>(analysis);
  return func_def_ == rhs.func_def_;
}

void ReturnAnalysis::Print(std::ostream &stream) const {
  stream << "func_def=" << *func_def_;
}
}
}
