
#ifndef _REAL_TALK_SEMANTIC_IMPORT_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_IMPORT_ANALYSIS_H_

#include <memory>
#include <iostream>
#include "real_talk/parser/program_node.h"

namespace real_talk {
namespace semantic {

class ImportAnalysis {
 public:
  explicit ImportAnalysis(
      std::shared_ptr<real_talk::parser::ProgramNode> program);
  friend bool operator==(const ImportAnalysis &lhs,
                         const ImportAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const ImportAnalysis &analysis);

 private:
  std::shared_ptr<real_talk::parser::ProgramNode> program_;
};
}
}
#endif
