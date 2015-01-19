
#ifndef _REAL_TALK_SEMANTIC_NODE_SEMANTIC_ANALYSIS_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_NODE_SEMANTIC_ANALYSIS_ANALYSIS_H_

#include <iostream>

namespace real_talk {
namespace semantic {

class NodeSemanticAnalysis {
 public:
  virtual ~NodeSemanticAnalysis() {}
  friend bool operator==(const NodeSemanticAnalysis &lhs,
                         const NodeSemanticAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const NodeSemanticAnalysis &analysis);

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &node) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}
#endif
