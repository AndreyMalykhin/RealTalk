
#ifndef _REAL_TALK_COMPILER_MSG_PRINTER_H_
#define _REAL_TALK_COMPILER_MSG_PRINTER_H_

#include <string>
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace compiler {

class MsgPrinter {
 public:
  typedef std::unordered_map<const real_talk::parser::ProgramNode*,
                             boost::filesystem::path> ProgramFilePaths;

  virtual ~MsgPrinter() {}
  virtual void PrintSemanticProblems(
      const real_talk::semantic::SemanticAnalysis::ProgramProblems &problems,
      const ProgramFilePaths &program_file_paths) const = 0;
  virtual void PrintError(const std::string &error) const = 0;
};
}
}
#endif
