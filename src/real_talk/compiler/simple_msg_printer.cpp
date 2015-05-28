
#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/compiler/simple_msg_printer.h"

using std::ostream;
using std::string;
using boost::filesystem::path;
using real_talk::lexer::TokenInfo;
using real_talk::semantic::SemanticProblem;
using real_talk::semantic::SemanticAnalysis;
using real_talk::lexer::TokenInfo;

namespace real_talk {
namespace compiler {

SimpleMsgPrinter::SimpleMsgPrinter(ostream *stream): stream_(stream) {
  assert(stream);
}

void SimpleMsgPrinter::PrintSemanticProblems(
    const SemanticAnalysis::ProgramProblems &problems,
    const ProgramFilePaths &program_file_paths) const {
  assert(false);
}

void SimpleMsgPrinter::PrintSemanticProblem(
    const SemanticProblem &problem, const path &file_path) const {
  assert(false);
}

void SimpleMsgPrinter::PrintTokenError(
    const TokenInfo &token, const path &file_path, const string &msg) const {
  assert(false);
}

void SimpleMsgPrinter::PrintUnexpectedTokenError(
    const TokenInfo &token, const path &file_path) const {
  PrintFileError(file_path, token.GetLine(), token.GetColumn())
      << "unexpected token \"" << token.GetValue() << "\"\n";
}

void SimpleMsgPrinter::PrintUnexpectedCharError(
    char c, uint32_t line, uint32_t column, const path &file_path) const {
  PrintFileError(file_path, line, column)
      << "unexpected character \"" << c << "\"\n";
}

void SimpleMsgPrinter::PrintError(const string &error) const {
  *stream_ << "[Error] " << error << '\n';
}

void SimpleMsgPrinter::PrintHelp(const string &help) const {
  *stream_ << help << '\n';
}

ostream &SimpleMsgPrinter::PrintFileError(
    const path &file_path, uint32_t line, uint32_t column) const {
  return *stream_ << "[Error] " << file_path.string() << ':' << line << ':'
                  << column << ": ";
}
}
}
