
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/func_def_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/binary_expr_node.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/semantic_problems.h"

using std::ostream;
using boost::filesystem::path;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::Node;
using real_talk::parser::DefNode;
using real_talk::parser::CallNode;
using real_talk::parser::BinaryExprNode;
using real_talk::parser::FuncDefNode;

namespace real_talk {
namespace semantic {

InitWithIncompatibleTypeError::InitWithIncompatibleTypeError(
    const path &file_path,
    const VarDefWithInitNode &var_def,
    const DataType &dest_data_type,
    const DataType &src_data_type)
    : file_path_(file_path),
      var_def_(var_def),
      dest_data_type_(dest_data_type),
      src_data_type_(src_data_type) {
}

const path &InitWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const VarDefWithInitNode &InitWithIncompatibleTypeError::GetVarDef() const {
  return var_def_;
}

const DataType &InitWithIncompatibleTypeError::GetDestDataType() const {
  return dest_data_type_;
}

const DataType &InitWithIncompatibleTypeError::GetSrcDataType() const {
  return src_data_type_;
}

void InitWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "var_def=" << static_cast<const Node&>(var_def_)
         << "; dest_type=" << dest_data_type_ << "; src_type="
         << src_data_type_;
}

bool InitWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const InitWithIncompatibleTypeError &rhs =
      static_cast<const InitWithIncompatibleTypeError&>(problem);
  return var_def_ == static_cast<const Node&>(rhs.var_def_)
      && dest_data_type_ == rhs.dest_data_type_
      && src_data_type_ == rhs.src_data_type_;
}

BinaryExprWithIncompatibleTypeError::BinaryExprWithIncompatibleTypeError(
    const path &file_path,
    const BinaryExprNode &expr,
    const DataType &dest_data_type,
    const DataType &src_data_type)
    : file_path_(file_path),
      expr_(expr),
      dest_data_type_(dest_data_type),
      src_data_type_(src_data_type) {
}

const path &BinaryExprWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const BinaryExprNode &BinaryExprWithIncompatibleTypeError::GetExpr() const {
  return expr_;
}

const DataType &BinaryExprWithIncompatibleTypeError::GetDestDataType() const {
  return dest_data_type_;
}

const DataType &BinaryExprWithIncompatibleTypeError::GetSrcDataType() const {
  return src_data_type_;
}

void BinaryExprWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "expr=" << expr_ << "; dest_type=" << dest_data_type_
         << "; src_type=" << src_data_type_;
}

bool BinaryExprWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const BinaryExprWithIncompatibleTypeError &rhs =
      static_cast<const BinaryExprWithIncompatibleTypeError&>(problem);
  return expr_ == rhs.expr_
      && dest_data_type_ == rhs.dest_data_type_
      && src_data_type_ == rhs.src_data_type_;
}

BinaryExprWithUnsupportedTypesError::BinaryExprWithUnsupportedTypesError(
    const path &file_path,
    const BinaryExprNode &expr,
    const DataType &left_operand_data_type,
    const DataType &right_operand_data_type)
    : file_path_(file_path),
      expr_(expr),
      left_operand_data_type_(left_operand_data_type),
      right_operand_data_type_(right_operand_data_type) {
}

const path &BinaryExprWithUnsupportedTypesError::GetFilePath() const {
  return file_path_;
}

const BinaryExprNode &BinaryExprWithUnsupportedTypesError::GetExpr() const {
  return expr_;
}

const DataType
&BinaryExprWithUnsupportedTypesError::GetLeftOperandDataType() const {
  return left_operand_data_type_;
}

const DataType
&BinaryExprWithUnsupportedTypesError::GetRightOperandDataType() const {
  return right_operand_data_type_;
}

void BinaryExprWithUnsupportedTypesError::Print(ostream &stream) const {
  stream << "expr=" << expr_ << "; left_operand_type="
         << left_operand_data_type_ << "; right_operand_type="
         << right_operand_data_type_;
}

bool BinaryExprWithUnsupportedTypesError::IsEqual(
    const SemanticProblem &problem) const {
  const BinaryExprWithUnsupportedTypesError &rhs =
      static_cast<const BinaryExprWithUnsupportedTypesError&>(problem);
  return expr_ == rhs.expr_
      && left_operand_data_type_ == rhs.left_operand_data_type_
      && right_operand_data_type_ == rhs.right_operand_data_type_;
}

CallWithIncompatibleTypeError::CallWithIncompatibleTypeError(
    const path &file_path,
    const CallNode &call,
    size_t arg_index,
    const DataType &dest_data_type,
    const DataType &src_data_type)
    : file_path_(file_path),
      call_(call),
      arg_index_(arg_index),
      dest_data_type_(dest_data_type),
      src_data_type_(src_data_type) {
}

const path &CallWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const CallNode &CallWithIncompatibleTypeError::GetCall() const {
  return call_;
}

size_t CallWithIncompatibleTypeError::GetArgIndex() const {
  return arg_index_;
}

const DataType &CallWithIncompatibleTypeError::GetDestDataType() const {
  return dest_data_type_;
}

const DataType &CallWithIncompatibleTypeError::GetSrcDataType() const {
  return src_data_type_;
}

void CallWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "call=" << call_ << "; arg_index=" << arg_index_
         << "; dest_type=" << dest_data_type_ << "; src_type="
         << src_data_type_;
}

bool CallWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const CallWithIncompatibleTypeError &rhs =
      static_cast<const CallWithIncompatibleTypeError&>(problem);
  return call_ == rhs.call_
      && arg_index_ == rhs.arg_index_
      && dest_data_type_ == rhs.dest_data_type_
      && src_data_type_ == rhs.src_data_type_;
}

DefWithUnsupportedDataTypeError::DefWithUnsupportedDataTypeError(
    const path &file_path, const DefNode &def, const DataType &data_type)
    : file_path_(file_path), def_(def), data_type_(data_type) {
}

const path &DefWithUnsupportedDataTypeError::GetFilePath() const {
  return file_path_;
}

const DefNode &DefWithUnsupportedDataTypeError::GetDef() const {
  return def_;
}

const DataType &DefWithUnsupportedDataTypeError::GetDataType() const {
  return data_type_;
}

void DefWithUnsupportedDataTypeError::Print(ostream &stream) const {
  stream << "def=" << def_ << "; type=" << data_type_;
}

bool DefWithUnsupportedDataTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const DefWithUnsupportedDataTypeError &rhs =
      static_cast<const DefWithUnsupportedDataTypeError&>(problem);
  return def_ == rhs.def_ && data_type_ == rhs.data_type_;
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

NestedFuncDefError::NestedFuncDefError(
    const path &file_path, const FuncDefNode &def)
    : file_path_(file_path), def_(def) {
}

const boost::filesystem::path &NestedFuncDefError::GetFilePath() const {
  return file_path_;
}

const real_talk::parser::FuncDefNode &NestedFuncDefError::GetDef() const {
  return def_;
}

void NestedFuncDefError::Print(std::ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool NestedFuncDefError::IsEqual(const SemanticProblem &problem) const {
  const NestedFuncDefError &rhs =
      static_cast<const NestedFuncDefError&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

FuncDefWithoutReturnValueError::FuncDefWithoutReturnValueError(
    const path &file_path, const FuncDefNode &def)
    : file_path_(file_path), def_(def) {
}

const boost::filesystem::path
&FuncDefWithoutReturnValueError::GetFilePath() const {
  return file_path_;
}

const real_talk::parser::FuncDefNode
&FuncDefWithoutReturnValueError::GetDef() const {
  return def_;
}

void FuncDefWithoutReturnValueError::Print(std::ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool FuncDefWithoutReturnValueError::IsEqual(
    const SemanticProblem &problem) const {
  const FuncDefWithoutReturnValueError &rhs =
      static_cast<const FuncDefWithoutReturnValueError&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

NonFuncCallError::NonFuncCallError(
    const path &file_path, const CallNode &call)
    : file_path_(file_path), call_(call) {
}

const path &NonFuncCallError::GetFilePath() const {
  return file_path_;
}

const CallNode &NonFuncCallError::GetCall() const {
  return call_;
}

void NonFuncCallError::Print(ostream &stream) const {
  stream << "call=" << call_;
}

bool NonFuncCallError::IsEqual(const SemanticProblem &problem) const {
  const NonFuncCallError &rhs = static_cast<const NonFuncCallError&>(problem);
  return call_ == rhs.call_;
}

InvalidArgsCountError::InvalidArgsCountError(
    const path &file_path,
    const CallNode &call,
    size_t expected_count,
    size_t actual_count)
    : file_path_(file_path),
      call_(call),
      expected_count_(expected_count),
      actual_count_(actual_count) {
}

const path &InvalidArgsCountError::GetFilePath() const {
  return file_path_;
}

const CallNode &InvalidArgsCountError::GetCall() const {
  return call_;
}

size_t InvalidArgsCountError::GetExpectedCount() const {
  return expected_count_;
}

size_t InvalidArgsCountError::GetActualCount() const {
  return actual_count_;
}

void InvalidArgsCountError::Print(ostream &stream) const {
  stream << "call=" << call_ << "; expected=" << expected_count_
         << "; actual=" << actual_count_;
}

bool InvalidArgsCountError::IsEqual(const SemanticProblem &problem) const {
  const InvalidArgsCountError &rhs =
      static_cast<const InvalidArgsCountError&>(problem);
  return call_ == rhs.call_
      && expected_count_ == rhs.expected_count_
      && actual_count_ == rhs.actual_count_;
}
}
}
