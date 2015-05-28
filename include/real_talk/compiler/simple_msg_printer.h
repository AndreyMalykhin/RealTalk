
#ifndef _REAL_TALK_COMPILER_SIMPLE_MSG_PRINTER_H_
#define _REAL_TALK_COMPILER_SIMPLE_MSG_PRINTER_H_

#include <string>
#include <iostream>
#include "real_talk/compiler/msg_printer.h"

namespace real_talk {
namespace compiler {

class SimpleMsgPrinter: public MsgPrinter {
 public:
  explicit SimpleMsgPrinter(std::ostream *stream);
  virtual void PrintSemanticProblems(
      const real_talk::semantic::SemanticAnalysis::ProgramProblems &problems,
      const ProgramFilePaths &program_file_paths) const override;
  virtual void PrintSemanticProblem(
      const real_talk::semantic::SemanticProblem &problem,
      const boost::filesystem::path &file_path) const override;
  virtual void PrintTokenError(
      const real_talk::lexer::TokenInfo &token,
      const boost::filesystem::path &file_path,
      const std::string &msg) const override;
  virtual void PrintUnexpectedTokenError(
      const real_talk::lexer::TokenInfo &token,
      const boost::filesystem::path &file_path) const override;
  virtual void PrintUnexpectedCharError(
      char c,
      uint32_t line,
      uint32_t column,
      const boost::filesystem::path &file_path) const override;
  virtual void PrintError(const std::string &error) const override;
  virtual void PrintHelp(const std::string &help) const override;

 private:
  std::ostream &PrintFileError(const boost::filesystem::path &file_path,
                               uint32_t line,
                               uint32_t column) const;

  std::ostream *stream_;
};
}
}
#endif
