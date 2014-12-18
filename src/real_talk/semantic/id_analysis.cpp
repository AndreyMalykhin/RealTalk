
#include <cassert>
#include "real_talk/parser/def_node.h"
#include "real_talk/semantic/id_analysis.h"

using std::unique_ptr;
using std::ostream;
using real_talk::parser::DefNode;

namespace real_talk {
namespace semantic {

IdAnalysis::IdAnalysis(const DefNode* def): def_(def) {
  assert(def_);
}

bool operator==(const IdAnalysis &lhs, const IdAnalysis &rhs) {
  return lhs.def_ == rhs.def_;
}

ostream &operator<<(ostream &stream, const IdAnalysis &id_analysis) {
  return stream << "def=" << *(id_analysis.def_);
}
}
}
