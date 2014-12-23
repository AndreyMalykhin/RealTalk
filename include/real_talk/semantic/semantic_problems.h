
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEMS_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEMS_H_

#include "real_talk/semantic/semantic_error.h"

namespace real_talk {
namespace parser {

class VarDefWithInitNode;
class VarDefNode;
}

namespace semantic {

class DataType;

class InitWithIncompatibleTypeError: public SemanticError {
 public:
  InitWithIncompatibleTypeError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::VarDefWithInitNode &var_def,
      const DataType &var_data_type,
      const DataType &value_data_type);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::VarDefWithInitNode &GetVarDef() const;
  const DataType &GetVarDataType() const;
  const DataType &GetValueDataType() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::VarDefWithInitNode &var_def_;
  const DataType &var_data_type_;
  const DataType &value_data_type_;
};

class VoidVarDefError: public SemanticError {
 public:
  VoidVarDefError(
      const boost::filesystem::path &file_path,
      const real_talk::parser::VarDefNode &var_def);
  virtual const boost::filesystem::path &GetFilePath() const override;
  const real_talk::parser::VarDefNode &GetVarDef() const;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  boost::filesystem::path file_path_;
  const real_talk::parser::VarDefNode &var_def_;
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
}
}
#endif
