
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_

#include <string>
#include <memory>
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace parser {

class ProgramNode;
class FileParser;
}

namespace semantic {

class ImportFileSearcher;
class LitParser;

class SimpleSemanticAnalyzer {
 public:
  class IOError: public std::runtime_error {
   public:
    IOError(const boost::filesystem::path &file_path, const std::string &msg);
    const boost::filesystem::path &GetFilePath() const;
    friend bool operator==(
        const SimpleSemanticAnalyzer::IOError &lhs,
        const SimpleSemanticAnalyzer::IOError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream, const SimpleSemanticAnalyzer::IOError &error);

   private:
    boost::filesystem::path file_path_;
  };

  class UnexpectedTokenError: public std::runtime_error {
   public:
    UnexpectedTokenError(
        const real_talk::parser::Parser::UnexpectedTokenError &error,
        const boost::filesystem::path &file_path,
        const std::string &msg);
    const real_talk::parser::Parser::UnexpectedTokenError &GetError() const;
    const boost::filesystem::path &GetFilePath() const;
    friend bool operator==(
        const SimpleSemanticAnalyzer::UnexpectedTokenError &lhs,
        const SimpleSemanticAnalyzer::UnexpectedTokenError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream,
        const SimpleSemanticAnalyzer::UnexpectedTokenError &error);

   private:
    real_talk::parser::Parser::UnexpectedTokenError error_;
    boost::filesystem::path file_path_;
  };

  class UnexpectedCharError: public std::runtime_error {
   public:
    UnexpectedCharError(
        const real_talk::lexer::Lexer::UnexpectedCharError &error,
        const boost::filesystem::path &file_path,
        const std::string &msg);
    const boost::filesystem::path &GetFilePath() const;
    const real_talk::lexer::Lexer::UnexpectedCharError &GetError() const;
    friend bool operator==(
        const SimpleSemanticAnalyzer::UnexpectedCharError &lhs,
        const SimpleSemanticAnalyzer::UnexpectedCharError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream,
        const SimpleSemanticAnalyzer::UnexpectedCharError &error);

   private:
    real_talk::lexer::Lexer::UnexpectedCharError error_;
    boost::filesystem::path file_path_;
  };

  SimpleSemanticAnalyzer(
      std::shared_ptr<real_talk::parser::ProgramNode> program,
      const boost::filesystem::path &absolute_file_path,
      const real_talk::parser::FileParser &file_parser,
      const ImportFileSearcher &import_file_searcher,
      const LitParser &lit_parser);
  ~SimpleSemanticAnalyzer();
  SemanticAnalysis Analyze();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
