
#ifndef _REAL_TALK_SEMANTIC_ID_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_ID_ANALYSIS_H_

#include <memory>
#include <iostream>

namespace real_talk {
namespace parser {

class DefNode;
}

namespace semantic {

class IdAnalysis {
 public:
  explicit IdAnalysis(const real_talk::parser::DefNode* def);
  friend bool operator==(const IdAnalysis &lhs,
                         const IdAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const IdAnalysis &analysis);

 private:
  const real_talk::parser::DefNode* def_;
};
}
}
#endif
