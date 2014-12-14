
#ifndef _REAL_TALK_PARSER_DEF_NODE_H_
#define _REAL_TALK_PARSER_DEF_NODE_H_

#include <string>

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class DataTypeNode;

class DefNode {
 public:
  virtual ~DefNode() {}
  virtual const std::unique_ptr<DataTypeNode> &GetDataType() const = 0;
  virtual const real_talk::lexer::TokenInfo &GetNameToken() const = 0;

  friend bool operator==(const DefNode &lhs, const DefNode &rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
  }

  friend std::ostream &operator<<(
      std::ostream &stream, const DefNode &node) {
    node.Print(stream);
    return stream;
  }

 private:
  virtual bool IsEqual(const DefNode &node) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}
#endif
