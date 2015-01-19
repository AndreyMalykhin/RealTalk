
#ifndef _REAL_TALK_SEMANTIC_IMPORT_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_IMPORT_ANALYSIS_H_

#include <memory>
#include "real_talk/parser/program_node.h"
#include "real_talk/semantic/node_semantic_analysis.h"

namespace real_talk {
namespace semantic {

class ImportAnalysis: public NodeSemanticAnalysis {
 public:
  explicit ImportAnalysis(
      std::shared_ptr<real_talk::parser::ProgramNode> program);

 private:
  virtual bool IsEqual(const NodeSemanticAnalysis &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::shared_ptr<real_talk::parser::ProgramNode> program_;
};
}
}
#endif
