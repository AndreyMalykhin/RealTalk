
#include <typeinfo>
#include "real_talk/semantic/node_semantic_analysis.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const NodeSemanticAnalysis &lhs,
                const NodeSemanticAnalysis &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
}

ostream &operator<<(ostream &stream,
                    const NodeSemanticAnalysis &analysis) {
  stream << typeid(analysis).name() << "; ";
  analysis.Print(stream);
  return stream;
}
}
}
