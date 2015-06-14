
#ifndef _REAL_TALK_CODE_CODE_CONTAINER_H_
#define _REAL_TALK_CODE_CODE_CONTAINER_H_

#include <iostream>

namespace real_talk {
namespace code {

class CodeContainerVisitor;

class CodeContainer {
 public:
  virtual ~CodeContainer() {}
  virtual void Accept(const CodeContainerVisitor &visitor) const = 0;

  friend bool operator==(const CodeContainer &lhs, const CodeContainer &rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
  }

  friend std::ostream &operator<<(
      std::ostream &stream, CodeContainer &container) {
    stream << typeid(container).name() << "; ";
    container.Print(stream);
    return stream;
  }

 private:
  virtual bool IsEqual(const CodeContainer &container) const = 0;
  virtual void Print(std::ostream &stream) = 0;
};
}
}
#endif
