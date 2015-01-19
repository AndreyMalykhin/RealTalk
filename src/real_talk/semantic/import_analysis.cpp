
#include <cassert>
#include "real_talk/semantic/import_analysis.h"

using std::ostream;
using std::shared_ptr;
using real_talk::parser::ProgramNode;

namespace real_talk {
namespace semantic {

ImportAnalysis::ImportAnalysis(shared_ptr<ProgramNode> program)
    : program_(program) {
  assert(program_);
}

bool ImportAnalysis::IsEqual(const NodeSemanticAnalysis &analysis) const {
  const ImportAnalysis &rhs = static_cast<const ImportAnalysis&>(analysis);
  return program_ == rhs.program_;
}

void ImportAnalysis::Print(ostream &stream) const {
  stream << "program=" << *program_;
}
}
}
