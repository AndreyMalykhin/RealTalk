
#ifndef _REAL_TALK_COMPILER_MSG_PRINTER_H_
#define _REAL_TALK_COMPILER_MSG_PRINTER_H_

#include <boost/filesystem.hpp>
#include <unordered_map>
#include <cstdint>
#include <string>
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class ProgramNode;
}

namespace semantic {

class SemanticProblem;
}

namespace compiler {

class MsgPrinter {
 public:
  typedef std::unordered_map<const real_talk::parser::ProgramNode*,
                             boost::filesystem::path> ProgramFilePaths;

  virtual ~MsgPrinter() {}
  virtual void PrintSemanticProblems(
      const real_talk::semantic::SemanticAnalysis::ProgramProblems &problems,
      const ProgramFilePaths &program_file_paths) const = 0;
  virtual void PrintSemanticProblem(
      const real_talk::semantic::SemanticProblem &problem,
      const boost::filesystem::path &file_path) const = 0;
  virtual void PrintTokenError(
      const real_talk::lexer::TokenInfo &token,
      const boost::filesystem::path &file_path,
      const std::string &error) const = 0;
  virtual void PrintUnexpectedTokenError(
      const real_talk::lexer::TokenInfo &token,
      const boost::filesystem::path &file_path) const = 0;
  virtual void PrintUnexpectedCharError(
      char c,
      uint32_t line,
      uint32_t column,
      const boost::filesystem::path &file_path) const = 0;
  virtual void PrintError(const std::string &error) const = 0;
  virtual void PrintHelp(const std::string &help) const = 0;
};
}
}
#endif
