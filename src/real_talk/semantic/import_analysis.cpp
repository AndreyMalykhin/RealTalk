
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

bool operator==(const ImportAnalysis &lhs, const ImportAnalysis &rhs) {
  return *(lhs.program_) == *(rhs.program_);
}

ostream &operator<<(ostream &stream, const ImportAnalysis &analysis) {
  return stream << *(analysis.program_);
}
}
}
