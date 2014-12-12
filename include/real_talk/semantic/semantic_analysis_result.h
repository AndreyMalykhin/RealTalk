
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_ANALYSIS_RESULT_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_ANALYSIS_RESULT_H_

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace real_talk {
namespace parser {

class ExprNode;
}

namespace semantic {

class DataType;

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

class SemanticAnalysisResult {
 public:
  explicit SemanticAnalysisResult(
      std::vector< std::unique_ptr<SemanticError> > errors);
  friend bool operator==(const SemanticAnalysisResult &lhs,
                         const SemanticAnalysisResult &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const SemanticAnalysisResult &result);

 private:
  std::vector< std::unique_ptr<SemanticError> > errors_;
};
}
}
#endif
