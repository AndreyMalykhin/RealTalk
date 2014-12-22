
#include <typeinfo>
#include "real_talk/semantic/semantic_problem.h"

using std::ostream;

namespace real_talk {
namespace semantic {

bool operator==(const SemanticProblem &lhs, const SemanticProblem &rhs) {
  return typeid(lhs) == typeid(rhs)
      && lhs.GetFilePath() == rhs.GetFilePath()
      && lhs.IsEqual(rhs);
}

ostream &operator<<(ostream &stream, const SemanticProblem &problem) {
  stream << "file_path=" << problem.GetFilePath() << "; ";
  problem.Print(stream);
  return stream;
}
}
}
