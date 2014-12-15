
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
class DefNode;
class LitNode;
class IdNode;
}

namespace semantic {

class DataType;
class Lit;

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

class DefAnalysis {
 public:
  explicit DefAnalysis(std::unique_ptr<DataType> data_type);
  friend bool operator==(const DefAnalysis &lhs,
                         const DefAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const DefAnalysis &analysis);

 private:
  std::unique_ptr<DataType> data_type_;
};

class ExprAnalysis {
 public:
  explicit ExprAnalysis(std::unique_ptr<DataType> data_type);
  const DataType &GetDataType() const;
  friend bool operator==(const ExprAnalysis &lhs,
                         const ExprAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const ExprAnalysis &analysis);

 private:
  std::unique_ptr<DataType> data_type_;
};

class LitAnalysis {
 public:
  explicit LitAnalysis(std::unique_ptr<Lit> lit);
  const Lit &GetLit() const;
  friend bool operator==(const LitAnalysis &lhs,
                         const LitAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const LitAnalysis &analysis);

 private:
  std::unique_ptr<Lit> lit_;
};

class IdAnalysis {
 public:
  explicit IdAnalysis(const real_talk::parser::DefNode* def);
  friend bool operator==(const IdAnalysis &lhs,
                         const IdAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const IdAnalysis &analysis);

 private:
  const real_talk::parser::DefNode* def_;
};

class SemanticAnalysisResult {
 public:
  typedef std::unordered_map<const real_talk::parser::DefNode*,
                             DefAnalysis> DefAnalyzes;
  typedef std::unordered_map<const real_talk::parser::ExprNode*,
                             ExprAnalysis> ExprAnalyzes;
  typedef std::unordered_map<const real_talk::parser::LitNode*,
                             LitAnalysis> LitAnalyzes;
  typedef std::unordered_map<const real_talk::parser::IdNode*,
                             IdAnalysis> IdAnalyzes;

  SemanticAnalysisResult(
      std::vector< std::unique_ptr<SemanticError> > errors,
      DefAnalyzes def_analyzes,
      ExprAnalyzes expr_analyzes,
      LitAnalyzes lit_analyzes,
      const IdAnalyzes &id_analyzes);
  friend bool operator==(const SemanticAnalysisResult &lhs,
                         const SemanticAnalysisResult &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const SemanticAnalysisResult &result);

 private:
  std::vector< std::unique_ptr<SemanticError> > errors_;
  DefAnalyzes def_analyzes_;
  ExprAnalyzes expr_analyzes_;
  LitAnalyzes lit_analyzes_;
  IdAnalyzes id_analyzes_;
};
}
}
#endif
