
#ifndef _REAL_TALK_PARSER_NODE_H_
#define _REAL_TALK_PARSER_NODE_H_

#include <iostream>

namespace real_talk {
namespace parser {

class Node {
 public:
  Node() = default;
  Node(const Node&) = default;

  friend bool operator==(const Node &lhs, const Node &rhs) {
    if (typeid(lhs) != typeid(rhs)) {
      return false;
    }

    return lhs.IsEqual(rhs);
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const Node &node) {
    node.Print(stream);
    return stream;
  }

  virtual ~Node() {}
 private:
  virtual bool IsEqual(const Node &node) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}

#endif
