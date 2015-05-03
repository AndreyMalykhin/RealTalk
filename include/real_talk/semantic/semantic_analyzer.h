
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_ANALYZER_H_

#include <string>
#include "real_talk/parser/parser.h"
#include "real_talk/lexer/lexer.h"
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace semantic {

class SemanticAnalyzer {
 public:
  class IOError: public std::runtime_error {
   public:
    IOError(const boost::filesystem::path &file_path, const std::string &msg);
    const boost::filesystem::path &GetFilePath() const;
    friend bool operator==(
        const SemanticAnalyzer::IOError &lhs,
        const SemanticAnalyzer::IOError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream, const SemanticAnalyzer::IOError &error);

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
        const SemanticAnalyzer::UnexpectedTokenError &lhs,
        const SemanticAnalyzer::UnexpectedTokenError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream,
        const SemanticAnalyzer::UnexpectedTokenError &error);

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
        const SemanticAnalyzer::UnexpectedCharError &lhs,
        const SemanticAnalyzer::UnexpectedCharError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream,
        const SemanticAnalyzer::UnexpectedCharError &error);

   private:
    real_talk::lexer::Lexer::UnexpectedCharError error_;
    boost::filesystem::path file_path_;
  };

  virtual ~SemanticAnalyzer() {}

  /**
   * @throws real_talk::semantic::SemanticAnalyzer::UnexpectedCharError
   * @throws real_talk::semantic::SemanticAnalyzer::UnexpectedTokenError
   * @throws real_talk::semantic::SemanticAnalyzer::IOError
   */
  virtual SemanticAnalysis Analyze() = 0;
};
}
}
#endif
