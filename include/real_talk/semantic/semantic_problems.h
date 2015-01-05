
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEMS_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEMS_H_

#include "real_talk/semantic/semantic_error.h"

namespace real_talk {
namespace parser {

class VarDefWithInitNode;
class VarDefNode;
class BranchNode;
class IfNode;
class BinaryExprNode;
class ArrayAllocNode;
class ArraySizeNode;
}

namespace semantic {

class DataType;

class ArrayAllocWithIncompatibleValueTypeError: public SemanticError {
 public:
  ArrayAllocWithIncompatibleValueTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ArrayAllocNode &alloc,
      size_t value_index,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  size_t GetValueIndex() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ArrayAllocNode &alloc_;
  size_t value_index_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class ArrayAllocWithUnsupportedElementTypeError: public SemanticError {
 public:
  ArrayAllocWithUnsupportedElementTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ArrayAllocNode &alloc,
      std::unique_ptr<DataType> data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  const DataType &GetDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ArrayAllocNode &alloc_;
  std::unique_ptr<DataType> data_type_;
};

class ArrayAllocWithIncompatibleSizeTypeError: public SemanticError {
 public:
  ArrayAllocWithIncompatibleSizeTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ArrayAllocNode &alloc,
      const real_talk::parser::ArraySizeNode &size,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  const real_talk::parser::ArraySizeNode &GetSize() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ArrayAllocNode &alloc_;
  const real_talk::parser::ArraySizeNode &size_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class IfWithIncompatibleTypeError: public SemanticError {
 public:
  IfWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::BranchNode &branch_node,
      const real_talk::parser::IfNode &if_node,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::BranchNode &GetBranch() const;
  const real_talk::parser::IfNode &GetIf() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::BranchNode &branch_;
  const real_talk::parser::IfNode &if_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class BreakNotWithinLoopError: public SemanticError {
 public:
  BreakNotWithinLoopError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::BreakNode &break_node);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::BreakNode &GetBreak() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::BreakNode &break_;
};

class ContinueNotWithinLoopError: public SemanticError {
 public:
  ContinueNotWithinLoopError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ContinueNode &continue_node);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ContinueNode &GetContinue() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ContinueNode &continue_;
};

class PreTestLoopWithIncompatibleTypeError: public SemanticError {
 public:
  PreTestLoopWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::PreTestLoopNode &loop,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::PreTestLoopNode &GetLoop() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::PreTestLoopNode &loop_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class ImportWithUnexpectedCharError: public SemanticError {
 public:
  ImportWithUnexpectedCharError(
      const boost::filesystem::path &src_file_path,
      const real_talk::parser::ImportNode &import,
      const boost::filesystem::path &import_file_path,
      char c,
      uint32_t line_number,
      uint32_t column_number);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ImportNode &GetImport() const;
  const boost::filesystem::path &GetImportFilePath() const;
  char GetChar() const;
  uint32_t GetLineNumber() const;
  uint32_t GetColumnNumber() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path src_file_path_;
  const real_talk::parser::ImportNode &import_;
  boost::filesystem::path import_file_path_;
  char c_;
  uint32_t line_number_;
  uint32_t column_number_;
};

class ImportWithUnexpectedTokenError: public SemanticError {
 public:
  ImportWithUnexpectedTokenError(
      const boost::filesystem::path &src_file_path,
      const real_talk::parser::ImportNode &import,
      const boost::filesystem::path &import_file_path,
      const real_talk::lexer::TokenInfo &token);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ImportNode &GetImport() const;
  const boost::filesystem::path &GetImportFilePath() const;
  const real_talk::lexer::TokenInfo &GetToken() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path src_file_path_;
  const real_talk::parser::ImportNode &import_;
  boost::filesystem::path import_file_path_;
  real_talk::lexer::TokenInfo token_;
};

class ImportWithIOError: public SemanticError {
 public:
  ImportWithIOError(
      const boost::filesystem::path &src_file_path,
      const real_talk::parser::ImportNode &import,
      const boost::filesystem::path &import_file_path);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ImportNode &GetImport() const;
  const boost::filesystem::path &GetImportFilePath() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path src_file_path_;
  const real_talk::parser::ImportNode &import_;
  boost::filesystem::path import_file_path_;
};

class ImportWithNotExistingFileError: public SemanticError {
 public:
  ImportWithNotExistingFileError(
      const boost::filesystem::path &src_file_path,
      const real_talk::parser::ImportNode &import,
      const boost::filesystem::path &import_file_path);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ImportNode &GetImport() const;
  const boost::filesystem::path &GetImportFilePath() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path src_file_path_;
  const real_talk::parser::ImportNode &import_;
  boost::filesystem::path import_file_path_;
};

class ImportIsNotFirstStmtError: public SemanticError {
 public:
  ImportIsNotFirstStmtError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ImportNode &import);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ImportNode &GetImport() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ImportNode &import_;
};

class InitWithIncompatibleTypeError: public SemanticError {
 public:
  InitWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::VarDefWithInitNode &var_def,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::VarDefWithInitNode &GetVarDef() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::VarDefWithInitNode &var_def_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class ReturnWithoutValueError: public SemanticError {
 public:
  ReturnWithoutValueError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ReturnWithoutValueNode &return_node);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ReturnWithoutValueNode &GetReturn() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ReturnWithoutValueNode &return_;
};

class ReturnNotWithinFuncError: public SemanticError {
 public:
  ReturnNotWithinFuncError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ReturnNode &return_node);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ReturnNode &GetReturn() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ReturnNode &return_;
};

class ReturnWithIncompatibleTypeError: public SemanticError {
 public:
  ReturnWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::ReturnValueNode &return_node,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ReturnValueNode &GetReturn() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ReturnValueNode &return_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class CallWithIncompatibleTypeError: public SemanticError {
 public:
  CallWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::CallNode &call,
      size_t arg_index,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::CallNode &GetCall() const;
  size_t GetArgIndex() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::CallNode &call_;
  size_t arg_index_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class BinaryExprWithIncompatibleTypeError: public SemanticError {
 public:
  BinaryExprWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::BinaryExprNode &expr,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::BinaryExprNode &GetExpr() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::BinaryExprNode &expr_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class BinaryExprWithUnsupportedTypesError: public SemanticError {
 public:
  BinaryExprWithUnsupportedTypesError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::BinaryExprNode &expr,
      std::unique_ptr<DataType> left_operand_data_type,
      std::unique_ptr<DataType> right_operand_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::BinaryExprNode &GetExpr() const;
  const DataType &GetLeftOperandDataType() const;
  const DataType &GetRightOperandDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::BinaryExprNode &expr_;
  std::unique_ptr<DataType> left_operand_data_type_;
  std::unique_ptr<DataType> right_operand_data_type_;
};

class DefWithUnsupportedTypeError: public SemanticError {
 public:
  DefWithUnsupportedTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::DefNode &def,
      std::unique_ptr<DataType> data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::DefNode &GetDef() const;
  const DataType &GetDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::DefNode &def_;
  std::unique_ptr<DataType> data_type_;
};

class DuplicateDefError: public SemanticError {
 public:
  DuplicateDefError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::DefNode &def);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::DefNode &GetDef() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::DefNode &def_;
};

class FuncDefWithinNonGlobalScope: public SemanticError {
 public:
  FuncDefWithinNonGlobalScope(
      const boost::filesystem::path &file_path,
      const real_talk::parser::FuncDefNode &def);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::FuncDefNode &GetDef() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::FuncDefNode &def_;
};

class FuncDefWithoutReturnValueError: public SemanticError {
 public:
  FuncDefWithoutReturnValueError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::FuncDefNode &def);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::FuncDefNode &GetDef() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::FuncDefNode &def_;
};

class CallWithNonFuncError: public SemanticError {
 public:
  CallWithNonFuncError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::CallNode &call);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::CallNode &GetCall() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::CallNode &call_;
};

class CallWithInvalidArgsCount: public SemanticError {
 public:
  CallWithInvalidArgsCount(
      const boost::filesystem::path &file_path,
      const real_talk::parser::CallNode &call,
      size_t expected_count,
      size_t actual_count);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::CallNode &GetCall() const;
  size_t GetExpectedCount() const;
  size_t GetActualCount() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::CallNode &call_;
  size_t expected_count_;
  size_t actual_count_;
};
}
}
#endif
