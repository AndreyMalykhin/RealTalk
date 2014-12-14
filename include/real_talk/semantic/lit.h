
#ifndef _REAL_TALK_SEMANTIC_LIT_H_
#define _REAL_TALK_SEMANTIC_LIT_H_

#include <iostream>

namespace real_talk {
namespace semantic {

class Lit {
 public:
  virtual ~Lit() {}
  friend bool operator==(const Lit &lhs, const Lit &rhs);
  friend bool operator!=(const Lit &lhs, const Lit &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const Lit &lit);

 private:
  virtual bool IsEqual(const Lit &rhs) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}
#endif
