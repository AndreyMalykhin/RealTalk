
#ifndef _REAL_TALK_PARSER_NODE_H_
#define _REAL_TALK_PARSER_NODE_H_

#include <iostream>

namespace real_talk {
namespace parser {

class NodeVisitor;

class Node {
 public:
  virtual ~Node() {}
  virtual void Accept(NodeVisitor &visitor) const = 0;

  friend bool operator==(const Node &lhs, const Node &rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const Node &node) {
    node.Print(stream);
    return stream;
  }

 private:
  virtual bool IsEqual(const Node &node) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}

#endif
