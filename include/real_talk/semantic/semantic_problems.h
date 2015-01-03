
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEMS_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEMS_H_

#include "real_talk/semantic/semantic_error.h"

namespace real_talk {
namespace parser {

class VarDefWithInitNode;
class VarDefNode;
class BinaryExprNode;
}

namespace semantic {

class DataType;

class ImportIOError: public SemanticError {
 public:
  ImportIOError(
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
      const DataType &dest_data_type,
      const DataType &src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::VarDefWithInitNode &GetVarDef() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::VarDefWithInitNode &var_def_;
  const DataType &dest_data_type_;
  const DataType &src_data_type_;
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
      const DataType &dest_data_type,
      const DataType &src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::ReturnValueNode &GetReturn() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::ReturnValueNode &return_;
  const DataType &dest_data_type_;
  const DataType &src_data_type_;
};

class CallWithIncompatibleTypeError: public SemanticError {
 public:
  CallWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::CallNode &call,
      size_t arg_index,
      const DataType &dest_data_type,
      const DataType &src_data_type);
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
  const DataType &dest_data_type_;
  const DataType &src_data_type_;
};

class BinaryExprWithIncompatibleTypeError: public SemanticError {
 public:
  BinaryExprWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::BinaryExprNode &expr,
      const DataType &dest_data_type,
      const DataType &src_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::BinaryExprNode &GetExpr() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::BinaryExprNode &expr_;
  const DataType &dest_data_type_;
  const DataType &src_data_type_;
};

class BinaryExprWithUnsupportedTypesError: public SemanticError {
 public:
  BinaryExprWithUnsupportedTypesError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::BinaryExprNode &expr,
      const DataType &left_operand_data_type,
      const DataType &right_operand_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::BinaryExprNode &GetExpr() const;
  const DataType &GetLeftOperandDataType() const;
  const DataType &GetRightOperandDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::BinaryExprNode &expr_;
  const DataType &left_operand_data_type_;
  const DataType &right_operand_data_type_;
};

class DefWithUnsupportedTypeError: public SemanticError {
 public:
  DefWithUnsupportedTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::DefNode &def,
      const DataType &data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::DefNode &GetDef() const;
  const DataType &GetDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::DefNode &def_;
  const DataType &data_type_;
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

class NestedFuncDefError: public SemanticError {
 public:
  NestedFuncDefError(
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

class NonFuncCallError: public SemanticError {
 public:
  NonFuncCallError(
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
