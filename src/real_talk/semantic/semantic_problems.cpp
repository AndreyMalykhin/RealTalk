
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/semantic_problems.h"

using std::ostream;
using boost::filesystem::path;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::Node;
using real_talk::parser::DefNode;

namespace real_talk {
namespace semantic {

InitWithIncompatibleTypeError::InitWithIncompatibleTypeError(
    const path &file_path,
    const VarDefWithInitNode &var_def,
    const DataType &var_data_type,
    const DataType &value_data_type)
    : file_path_(file_path),
      var_def_(var_def),
      var_data_type_(var_data_type),
      value_data_type_(value_data_type) {
}

const path &InitWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const VarDefWithInitNode &InitWithIncompatibleTypeError::GetVarDef() const {
  return var_def_;
}

const DataType &InitWithIncompatibleTypeError::GetVarDataType() const {
  return var_data_type_;
}

const DataType &InitWithIncompatibleTypeError::GetValueDataType() const {
  return value_data_type_;
}

void InitWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "var_def=" << static_cast<const Node&>(var_def_)
         << "; var_data_type=" << var_data_type_ << "; value_data_type="
         << value_data_type_;
}

bool InitWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const InitWithIncompatibleTypeError &rhs =
      static_cast<const InitWithIncompatibleTypeError&>(problem);
  return var_def_ == static_cast<const Node&>(rhs.var_def_)
      && var_data_type_ == rhs.var_data_type_
      && value_data_type_ == rhs.value_data_type_;
}

VoidVarDefError::VoidVarDefError(
    const boost::filesystem::path &file_path,
    const real_talk::parser::VarDefNode &var_def)
    : file_path_(file_path), var_def_(var_def) {
}

const boost::filesystem::path &VoidVarDefError::GetFilePath() const {
  return file_path_;
}

const real_talk::parser::VarDefNode &VoidVarDefError::GetVarDef() const {
  return var_def_;
}

void VoidVarDefError::Print(std::ostream &stream) const {
  stream << "var_def=" << var_def_;
}

bool VoidVarDefError::IsEqual(const SemanticProblem &problem) const {
  const VoidVarDefError &rhs = static_cast<const VoidVarDefError&>(problem);
  return var_def_ == rhs.var_def_;
}

DuplicateDefError::DuplicateDefError(
    const path &file_path, const DefNode &def)
    : file_path_(file_path), def_(def) {
}

const boost::filesystem::path &DuplicateDefError::GetFilePath() const {
  return file_path_;
}

const real_talk::parser::DefNode &DuplicateDefError::GetDef() const {
  return def_;
}

void DuplicateDefError::Print(std::ostream &stream) const {
  stream << "def=" << def_;
}

bool DuplicateDefError::IsEqual(const SemanticProblem &problem) const {
  const DuplicateDefError &rhs = static_cast<const DuplicateDefError&>(problem);
  return def_ == rhs.def_;
}
}
}
