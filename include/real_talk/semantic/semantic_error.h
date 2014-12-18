
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_ERROR_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_ERROR_H_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "real_talk/semantic/semantic_hint.h"

namespace real_talk {
namespace semantic {

class SemanticError {
 public:
  SemanticError(const std::string &message,
                std::uint32_t line_number,
                std::uint32_t column_number,
                std::vector< std::unique_ptr<SemanticHint> > hints);
  friend bool operator==(const SemanticError &lhs,
                         const SemanticError &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const SemanticError &error);

 private:
  std::string message_;
  std::uint32_t line_number_;
  std::uint32_t column_number_;
  std::vector< std::unique_ptr<SemanticHint> > hints_;
};
}
}
#endif
