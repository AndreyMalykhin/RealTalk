
#include <string>
#include "real_talk/semantic/semantic_analyzer.h"

using std::ostream;
using std::string;
using boost::filesystem::path;
using real_talk::parser::Parser;
using real_talk::lexer::Lexer;

namespace real_talk {
namespace semantic {

SemanticAnalyzer::UnexpectedTokenError::UnexpectedTokenError(
    const Parser::UnexpectedTokenError &error,
    const path &file_path,
    const string &msg)
    : runtime_error(msg),
      error_(error),
      file_path_(file_path) {
}

const Parser::UnexpectedTokenError
&SemanticAnalyzer::UnexpectedTokenError::GetError() const {
  return error_;
}

const path &SemanticAnalyzer::UnexpectedTokenError::GetFilePath() const {
  return file_path_;
}

bool operator==(
    const SemanticAnalyzer::UnexpectedTokenError &lhs,
    const SemanticAnalyzer::UnexpectedTokenError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.error_ == rhs.error_
      && lhs.file_path_ == rhs.file_path_;
}

ostream &operator<<(
    ostream &stream,
    const SemanticAnalyzer::UnexpectedTokenError &error) {
  return stream << "error=" << error.error_ << "; file_path="
                << error.file_path_ << "; msg=" << error.what();
}

SemanticAnalyzer::UnexpectedCharError::UnexpectedCharError(
    const Lexer::UnexpectedCharError &error,
    const path &file_path,
    const string &msg)
    : runtime_error(msg), error_(error), file_path_(file_path) {
}

const Lexer::UnexpectedCharError
&SemanticAnalyzer::UnexpectedCharError::GetError() const {
  return error_;
}

const path &SemanticAnalyzer::UnexpectedCharError::GetFilePath() const {
  return file_path_;
}

bool operator==(
    const SemanticAnalyzer::UnexpectedCharError &lhs,
    const SemanticAnalyzer::UnexpectedCharError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.file_path_ == rhs.file_path_
      && lhs.error_ == rhs.error_;
}

ostream &operator<<(ostream &stream,
                    const SemanticAnalyzer::UnexpectedCharError &error) {
  return stream << "error=" << error.error_ << "; file_path="
                << error.file_path_ << "; msg=" << error.what();
}

SemanticAnalyzer::IOError::IOError(
    const path &file_path, const string &msg)
    : runtime_error(msg), file_path_(file_path) {
}

const path &SemanticAnalyzer::IOError::GetFilePath() const {
  return file_path_;
}

bool operator==(
    const SemanticAnalyzer::IOError &lhs,
    const SemanticAnalyzer::IOError &rhs) {
  return strcmp(lhs.what(), rhs.what()) == 0
      && lhs.file_path_ == rhs.file_path_;
}

ostream &operator<<(
    ostream &stream, const SemanticAnalyzer::IOError &error) {
  return stream << "file_path=" << error.file_path_ << "; msg=" << error.what();
}
}
}
