
#include <cassert>
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/lit.h"

using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

LitAnalysis::LitAnalysis(unique_ptr<Lit> lit): lit_(move(lit)) {
  assert(lit_);
}

const Lit &LitAnalysis::GetLit() const {
  return *lit_;
}

bool operator==(const LitAnalysis &lhs, const LitAnalysis &rhs) {
  return *(lhs.lit_) == *(rhs.lit_);
}

ostream &operator<<(ostream &stream, const LitAnalysis &lit_analysis) {
  return stream << *(lit_analysis.lit_);
}
}
}
