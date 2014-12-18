
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_HINT_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_HINT_H_

#include <cstdint>
#include <string>
#include <iostream>

namespace real_talk {
namespace semantic {

class SemanticHint {
 public:
  SemanticHint(const std::string &message,
               std::uint32_t line_number,
               std::uint32_t column_number,
               const std::string &file_path);
  friend bool operator==(const SemanticHint &lhs,
                         const SemanticHint &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const SemanticHint &hint);

 private:
  std::string message_;
  std::uint32_t line_number_;
  std::uint32_t column_number_;
  std::string file_path_;
};
}
}
#endif
