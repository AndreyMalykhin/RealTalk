
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEM_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEM_H_

#include <iostream>

namespace real_talk {
namespace semantic {

class SemanticProblemVisitor;

class SemanticProblem {
 public:
  virtual ~SemanticProblem() {}
  virtual void Accept(const SemanticProblemVisitor *visitor) const = 0;
  friend bool operator==(
      const SemanticProblem &lhs, const SemanticProblem &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const SemanticProblem &error);

 private:
  virtual void Print(std::ostream &stream) const = 0;
  virtual bool IsEqual(const SemanticProblem &rhs) const = 0;
};
}
}
#endif
