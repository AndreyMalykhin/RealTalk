
#include <typeinfo>
#include "real_talk/semantic/semantic_problem.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const SemanticProblem &lhs, const SemanticProblem &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
}

ostream &operator<<(ostream &stream, const SemanticProblem &problem) {
  stream << "type=" << typeid(problem).name() << "; ";
  problem.Print(stream);
  return stream;
}
}
}
