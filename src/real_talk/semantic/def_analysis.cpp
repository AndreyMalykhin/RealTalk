
#include <typeinfo>
#include "real_talk/semantic/def_analysis.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const DefAnalysis &lhs, const DefAnalysis &rhs) {
  return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
}

ostream &operator<<(ostream &stream, const DefAnalysis &def_analysis) {
  def_analysis.Print(stream);
  return stream;
}
}
}
