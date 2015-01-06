
#include <cassert>
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/func_def_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/binary_expr_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/branch_node.h"
#include "real_talk/parser/if_node.h"
#include "real_talk/parser/array_alloc_node.h"
#include "real_talk/parser/bounded_array_size_node.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/semantic_problems.h"

using std::ostream;
using std::unique_ptr;
using boost::filesystem::path;
using real_talk::lexer::TokenInfo;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::Node;
using real_talk::parser::DefNode;
using real_talk::parser::CallNode;
using real_talk::parser::BinaryExprNode;
using real_talk::parser::FuncDefNode;
using real_talk::parser::ReturnValueNode;
using real_talk::parser::ReturnWithoutValueNode;
using real_talk::parser::ReturnNode;
using real_talk::parser::ImportNode;
using real_talk::parser::PreTestLoopNode;
using real_talk::parser::ContinueNode;
using real_talk::parser::BreakNode;
using real_talk::parser::BranchNode;
using real_talk::parser::IfNode;
using real_talk::parser::ArrayAllocNode;
using real_talk::parser::BoundedArraySizeNode;

namespace real_talk {
namespace semantic {

ArrayAllocWithIncompatibleValueTypeError::
ArrayAllocWithIncompatibleValueTypeError(
    const path &file_path,
    const ArrayAllocNode &alloc,
    size_t value_index,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      alloc_(alloc),
      value_index_(value_index),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &ArrayAllocWithIncompatibleValueTypeError::GetFilePath() const {
  return file_path_;
}

const ArrayAllocNode
&ArrayAllocWithIncompatibleValueTypeError::GetAlloc() const {
  return alloc_;
}

size_t ArrayAllocWithIncompatibleValueTypeError::GetValueIndex() const {
  return value_index_;
}

const DataType
&ArrayAllocWithIncompatibleValueTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType
&ArrayAllocWithIncompatibleValueTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void ArrayAllocWithIncompatibleValueTypeError::Print(ostream &stream) const {
  stream << "alloc=" << alloc_ << "; value_index=" << value_index_
         << "; dest_type=" << *dest_data_type_ << "; src_type="
         << *src_data_type_;
}

bool ArrayAllocWithIncompatibleValueTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const ArrayAllocWithIncompatibleValueTypeError &rhs =
      static_cast<const ArrayAllocWithIncompatibleValueTypeError&>(problem);
  return alloc_ == rhs.alloc_
      && value_index_ == rhs.value_index_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

ArrayAllocWithUnsupportedElementTypeError::
ArrayAllocWithUnsupportedElementTypeError(
    const path &file_path,
    const ArrayAllocNode &alloc,
    unique_ptr<DataType> data_type)
    : file_path_(file_path),
      alloc_(alloc),
      data_type_(move(data_type)) {
  assert(data_type_);
}

const path &ArrayAllocWithUnsupportedElementTypeError::GetFilePath() const {
  return file_path_;
}

const ArrayAllocNode
&ArrayAllocWithUnsupportedElementTypeError::GetAlloc() const {
  return alloc_;
}

const DataType &ArrayAllocWithUnsupportedElementTypeError::GetDataType() const {
  return *data_type_;
}

void ArrayAllocWithUnsupportedElementTypeError::Print(ostream &stream) const {
  stream << "alloc=" << alloc_ << "; data_type=" << *data_type_;
}

bool ArrayAllocWithUnsupportedElementTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const ArrayAllocWithUnsupportedElementTypeError &rhs =
      static_cast<const ArrayAllocWithUnsupportedElementTypeError&>(problem);
  return alloc_ == rhs.alloc_
      && *(data_type_) == *(rhs.data_type_);
}

ArrayAllocWithIncompatibleSizeTypeError::
ArrayAllocWithIncompatibleSizeTypeError(
    const path &file_path,
    const ArrayAllocNode &alloc,
    const BoundedArraySizeNode &size,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      alloc_(alloc),
      size_(size),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &ArrayAllocWithIncompatibleSizeTypeError::GetFilePath() const {
  return file_path_;
}

const ArrayAllocNode
&ArrayAllocWithIncompatibleSizeTypeError::GetAlloc() const {
  return alloc_;
}

const BoundedArraySizeNode &ArrayAllocWithIncompatibleSizeTypeError::GetSize() const {
  return size_;
}

const DataType
&ArrayAllocWithIncompatibleSizeTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType
&ArrayAllocWithIncompatibleSizeTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void ArrayAllocWithIncompatibleSizeTypeError::Print(ostream &stream) const {
  stream << "alloc=" << alloc_ << "; size=" << size_ << "; dest_type="
         << *dest_data_type_ << "; src_type=" << *src_data_type_;
}

bool ArrayAllocWithIncompatibleSizeTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const ArrayAllocWithIncompatibleSizeTypeError &rhs =
      static_cast<const ArrayAllocWithIncompatibleSizeTypeError&>(problem);
  return alloc_ == rhs.alloc_
      && size_ == rhs.size_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

IfWithIncompatibleTypeError::IfWithIncompatibleTypeError(
    const path &file_path,
    const BranchNode &branch_node,
    const IfNode &if_node,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      branch_(branch_node),
      if_(if_node),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &IfWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const BranchNode &IfWithIncompatibleTypeError::GetBranch() const {
  return branch_;
}

const IfNode &IfWithIncompatibleTypeError::GetIf() const {
  return if_;
}

const DataType &IfWithIncompatibleTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType &IfWithIncompatibleTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void IfWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "branch=" << branch_ << "; if=" << if_ << "; dest_type="
         << *dest_data_type_ << "; src_type=" << *src_data_type_;
}

bool IfWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const IfWithIncompatibleTypeError &rhs =
      static_cast<const IfWithIncompatibleTypeError&>(problem);
  return branch_ == rhs.branch_
      && if_ == rhs.if_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

BreakNotWithinLoopError::BreakNotWithinLoopError(
    const path &file_path, const BreakNode &break_node)
    : file_path_(file_path), break_(break_node) {
}

const path &BreakNotWithinLoopError::GetFilePath() const {
  return file_path_;
}

const BreakNode &BreakNotWithinLoopError::GetBreak() const {
  return break_;
}

void BreakNotWithinLoopError::Print(ostream &stream) const {
  stream << "break=" << break_;
}

bool BreakNotWithinLoopError::IsEqual(const SemanticProblem &problem) const {
  const BreakNotWithinLoopError &rhs =
      static_cast<const BreakNotWithinLoopError&>(problem);
  return break_ == rhs.break_;
}

ContinueNotWithinLoopError::ContinueNotWithinLoopError(
    const path &file_path, const ContinueNode &continue_node)
    : file_path_(file_path), continue_(continue_node) {
}

const path &ContinueNotWithinLoopError::GetFilePath() const {
  return file_path_;
}

const ContinueNode &ContinueNotWithinLoopError::GetContinue() const {
  return continue_;
}

void ContinueNotWithinLoopError::Print(ostream &stream) const {
  stream << "continue=" << continue_;
}

bool ContinueNotWithinLoopError::IsEqual(const SemanticProblem &problem) const {
  const ContinueNotWithinLoopError &rhs =
      static_cast<const ContinueNotWithinLoopError&>(problem);
  return continue_ == rhs.continue_;
}

PreTestLoopWithIncompatibleTypeError::PreTestLoopWithIncompatibleTypeError(
    const path &file_path,
    const PreTestLoopNode &loop,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      loop_(loop),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &PreTestLoopWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const PreTestLoopNode &PreTestLoopWithIncompatibleTypeError::GetLoop() const {
  return loop_;
}

const DataType &PreTestLoopWithIncompatibleTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType &PreTestLoopWithIncompatibleTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void PreTestLoopWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "loop=" << loop_ << "; dest_type=" << *dest_data_type_
         << "; src_type=" << *src_data_type_;
}

bool PreTestLoopWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const PreTestLoopWithIncompatibleTypeError &rhs =
      static_cast<const PreTestLoopWithIncompatibleTypeError&>(problem);
  return loop_ == rhs.loop_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

ImportWithUnexpectedCharError::ImportWithUnexpectedCharError(
    const path &src_file_path,
    const ImportNode &import,
    const path &import_file_path,
    char c,
    uint32_t line_number,
    uint32_t column_number)
    : src_file_path_(src_file_path),
      import_(import),
      import_file_path_(import_file_path),
      c_(c),
      line_number_(line_number),
      column_number_(column_number) {
}

const path &ImportWithUnexpectedCharError::GetFilePath() const {
  return src_file_path_;
}

const ImportNode &ImportWithUnexpectedCharError::GetImport() const {
  return import_;
}

const path &ImportWithUnexpectedCharError::GetImportFilePath() const {
  return import_file_path_;
}

char ImportWithUnexpectedCharError::GetChar() const {
  return c_;
}

uint32_t ImportWithUnexpectedCharError::GetLineNumber() const {
  return line_number_;
}

uint32_t ImportWithUnexpectedCharError::GetColumnNumber() const {
  return column_number_;
}

void ImportWithUnexpectedCharError::Print(std::ostream &stream) const {
  stream << "import=" << import_ << "; import_file_path=" << import_file_path_
         << "; char=" << c_ << "; line=" << line_number_ << "; column="
         << column_number_;
}

bool ImportWithUnexpectedCharError::IsEqual(const SemanticProblem &problem) const {
  const ImportWithUnexpectedCharError &rhs =
      static_cast<const ImportWithUnexpectedCharError&>(problem);
  return import_ == rhs.import_
      && import_file_path_ == rhs.import_file_path_
      && c_ == rhs.c_
      && line_number_ == rhs.line_number_
      && column_number_ == rhs.column_number_;
}

ImportWithUnexpectedTokenError::ImportWithUnexpectedTokenError(
    const path &src_file_path,
    const ImportNode &import,
    const path &import_file_path,
    const TokenInfo &token)
    : src_file_path_(src_file_path),
      import_(import),
      import_file_path_(import_file_path),
      token_(token) {
}

const path &ImportWithUnexpectedTokenError::GetFilePath() const {
  return src_file_path_;
}

const ImportNode &ImportWithUnexpectedTokenError::GetImport() const {
  return import_;
}

const path &ImportWithUnexpectedTokenError::GetImportFilePath() const {
  return import_file_path_;
}

const TokenInfo &ImportWithUnexpectedTokenError::GetToken() const {
  return token_;
}

void ImportWithUnexpectedTokenError::Print(ostream &stream) const {
  stream << "import=" << import_ << "; import_file_path=" << import_file_path_
         << "; token=" << token_;
}

bool ImportWithUnexpectedTokenError::IsEqual(const SemanticProblem &problem) const {
  const ImportWithUnexpectedTokenError &rhs =
      static_cast<const ImportWithUnexpectedTokenError&>(problem);
  return import_ == rhs.import_
      && import_file_path_ == rhs.import_file_path_
      && token_ == rhs.token_;
}

ImportWithIOError::ImportWithIOError(
    const path &src_file_path,
    const ImportNode &import,
    const path &import_file_path)
    : src_file_path_(src_file_path),
      import_(import),
      import_file_path_(import_file_path) {
}

const path &ImportWithIOError::GetFilePath() const {
  return src_file_path_;
}

const ImportNode &ImportWithIOError::GetImport() const {
  return import_;
}

const path &ImportWithIOError::GetImportFilePath() const {
  return import_file_path_;
}

void ImportWithIOError::Print(ostream &stream) const {
  stream << "import=" << import_ << "; import_file_path=" << import_file_path_;
}

bool ImportWithIOError::IsEqual(const SemanticProblem &problem) const {
  const ImportWithIOError &rhs =
      static_cast<const ImportWithIOError&>(problem);
  return import_ == rhs.import_ && import_file_path_ == rhs.import_file_path_;
}

ImportWithNotExistingFileError::ImportWithNotExistingFileError(
    const path &src_file_path,
    const ImportNode &import,
    const path &import_file_path)
    : src_file_path_(src_file_path),
      import_(import),
      import_file_path_(import_file_path) {
}

const path &ImportWithNotExistingFileError::GetFilePath() const {
  return src_file_path_;
}

const ImportNode &ImportWithNotExistingFileError::GetImport() const {
  return import_;
}

const path &ImportWithNotExistingFileError::GetImportFilePath() const {
  return import_file_path_;
}

void ImportWithNotExistingFileError::Print(ostream &stream) const {
  stream << "import=" << import_ << "; import_file_path=" << import_file_path_;
}

bool ImportWithNotExistingFileError::IsEqual(
    const SemanticProblem &problem) const {
  const ImportWithNotExistingFileError &rhs =
      static_cast<const ImportWithNotExistingFileError&>(problem);
  return import_ == rhs.import_ && import_file_path_ == rhs.import_file_path_;
}

ImportIsNotFirstStmtError::ImportIsNotFirstStmtError(
    const path &file_path, const ImportNode &import)
    : file_path_(file_path), import_(import) {
}

const path &ImportIsNotFirstStmtError::GetFilePath() const {
  return file_path_;
}

const ImportNode &ImportIsNotFirstStmtError::GetImport() const {
  return import_;
}

void ImportIsNotFirstStmtError::Print(ostream &stream) const {
  stream << "import=" << import_;
}

bool ImportIsNotFirstStmtError::IsEqual(const SemanticProblem &problem) const {
  const ImportIsNotFirstStmtError &rhs =
      static_cast<const ImportIsNotFirstStmtError&>(problem);
  return import_ == rhs.import_;
}

InitWithIncompatibleTypeError::InitWithIncompatibleTypeError(
    const path &file_path,
    const VarDefWithInitNode &var_def,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      var_def_(var_def),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &InitWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const VarDefWithInitNode &InitWithIncompatibleTypeError::GetVarDef() const {
  return var_def_;
}

const DataType &InitWithIncompatibleTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType &InitWithIncompatibleTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void InitWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "var_def=" << static_cast<const Node&>(var_def_)
         << "; dest_type=" << *dest_data_type_ << "; src_type="
         << *src_data_type_;
}

bool InitWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const InitWithIncompatibleTypeError &rhs =
      static_cast<const InitWithIncompatibleTypeError&>(problem);
  return var_def_ == static_cast<const Node&>(rhs.var_def_)
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

ReturnWithoutValueError::ReturnWithoutValueError(
    const path &file_path, const ReturnWithoutValueNode &return_node)
    : file_path_(file_path), return_(return_node) {
}

const path &ReturnWithoutValueError::GetFilePath() const {
  return file_path_;
}

const ReturnWithoutValueNode &ReturnWithoutValueError::GetReturn() const {
  return return_;
}

void ReturnWithoutValueError::Print(std::ostream &stream) const {
  stream << "return=" << return_;
}

bool ReturnWithoutValueError::IsEqual(const SemanticProblem &problem) const {
  const ReturnWithoutValueError &rhs =
      static_cast<const ReturnWithoutValueError&>(problem);
  return return_ == rhs.return_;
}

ReturnNotWithinFuncError::ReturnNotWithinFuncError(
    const path &file_path, const ReturnNode &return_node)
    : file_path_(file_path), return_(return_node) {
}

const path &ReturnNotWithinFuncError::GetFilePath() const {
  return file_path_;
}

const ReturnNode &ReturnNotWithinFuncError::GetReturn() const {
  return return_;
}

void ReturnNotWithinFuncError::Print(std::ostream &stream) const {
  stream << "return=" << return_;
}

bool ReturnNotWithinFuncError::IsEqual(const SemanticProblem &problem) const {
  const ReturnNotWithinFuncError &rhs =
      static_cast<const ReturnNotWithinFuncError&>(problem);
  return return_ == rhs.return_;
}

ReturnWithIncompatibleTypeError::ReturnWithIncompatibleTypeError(
    const path &file_path,
    const ReturnValueNode &return_node,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      return_(return_node),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &ReturnWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const ReturnValueNode &ReturnWithIncompatibleTypeError::GetReturn() const {
  return return_;
}

const DataType &ReturnWithIncompatibleTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType &ReturnWithIncompatibleTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void ReturnWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "return=" << return_ << "; dest_type=" << *dest_data_type_
         << "; src_type=" << *src_data_type_;
}

bool ReturnWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const ReturnWithIncompatibleTypeError &rhs =
      static_cast<const ReturnWithIncompatibleTypeError&>(problem);
  return return_ == rhs.return_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

BinaryExprWithIncompatibleTypeError::BinaryExprWithIncompatibleTypeError(
    const path &file_path,
    const BinaryExprNode &expr,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      expr_(expr),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

const path &BinaryExprWithIncompatibleTypeError::GetFilePath() const {
  return file_path_;
}

const BinaryExprNode &BinaryExprWithIncompatibleTypeError::GetExpr() const {
  return expr_;
}

const DataType &BinaryExprWithIncompatibleTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType &BinaryExprWithIncompatibleTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void BinaryExprWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "expr=" << expr_ << "; dest_type=" << *dest_data_type_
         << "; src_type=" << *src_data_type_;
}

bool BinaryExprWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const BinaryExprWithIncompatibleTypeError &rhs =
      static_cast<const BinaryExprWithIncompatibleTypeError&>(problem);
  return expr_ == rhs.expr_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

BinaryExprWithUnsupportedTypesError::BinaryExprWithUnsupportedTypesError(
    const path &file_path,
    const BinaryExprNode &expr,
    unique_ptr<DataType> left_operand_data_type,
    unique_ptr<DataType> right_operand_data_type)
    : file_path_(file_path),
      expr_(expr),
      left_operand_data_type_(move(left_operand_data_type)),
      right_operand_data_type_(move(right_operand_data_type)) {
  assert(left_operand_data_type_);
  assert(right_operand_data_type_);
}

const path &BinaryExprWithUnsupportedTypesError::GetFilePath() const {
  return file_path_;
}

const BinaryExprNode &BinaryExprWithUnsupportedTypesError::GetExpr() const {
  return expr_;
}

const DataType
&BinaryExprWithUnsupportedTypesError::GetLeftOperandDataType() const {
  return *left_operand_data_type_;
}

const DataType
&BinaryExprWithUnsupportedTypesError::GetRightOperandDataType() const {
  return *right_operand_data_type_;
}

void BinaryExprWithUnsupportedTypesError::Print(ostream &stream) const {
  stream << "expr=" << expr_ << "; left_operand_type="
         << *left_operand_data_type_ << "; right_operand_type="
         << *right_operand_data_type_;
}

bool BinaryExprWithUnsupportedTypesError::IsEqual(
    const SemanticProblem &problem) const {
  const BinaryExprWithUnsupportedTypesError &rhs =
      static_cast<const BinaryExprWithUnsupportedTypesError&>(problem);
  return expr_ == rhs.expr_
      && *left_operand_data_type_ == *(rhs.left_operand_data_type_)
      && *right_operand_data_type_ == *(rhs.right_operand_data_type_);
}

CallWithIncompatibleTypeError::CallWithIncompatibleTypeError(
    const path &file_path,
    const CallNode &call,
    size_t arg_index,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : file_path_(file_path),
      call_(call),
      arg_index_(arg_index),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
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
  return *dest_data_type_;
}

const DataType &CallWithIncompatibleTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void CallWithIncompatibleTypeError::Print(ostream &stream) const {
  stream << "call=" << call_ << "; arg_index=" << arg_index_
         << "; dest_type=" << *dest_data_type_ << "; src_type="
         << *src_data_type_;
}

bool CallWithIncompatibleTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const CallWithIncompatibleTypeError &rhs =
      static_cast<const CallWithIncompatibleTypeError&>(problem);
  return call_ == rhs.call_
      && arg_index_ == rhs.arg_index_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

DefWithUnsupportedTypeError::DefWithUnsupportedTypeError(
    const path &file_path, const DefNode &def, unique_ptr<DataType> data_type)
    : file_path_(file_path), def_(def), data_type_(move(data_type)) {
  assert(data_type_);
}

const path &DefWithUnsupportedTypeError::GetFilePath() const {
  return file_path_;
}

const DefNode &DefWithUnsupportedTypeError::GetDef() const {
  return def_;
}

const DataType &DefWithUnsupportedTypeError::GetDataType() const {
  return *data_type_;
}

void DefWithUnsupportedTypeError::Print(ostream &stream) const {
  stream << "def=" << def_ << "; type=" << *data_type_;
}

bool DefWithUnsupportedTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const DefWithUnsupportedTypeError &rhs =
      static_cast<const DefWithUnsupportedTypeError&>(problem);
  return def_ == rhs.def_ && *data_type_ == *(rhs.data_type_);
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

FuncDefWithinNonGlobalScope::FuncDefWithinNonGlobalScope(
    const path &file_path, const FuncDefNode &def)
    : file_path_(file_path), def_(def) {
}

const path &FuncDefWithinNonGlobalScope::GetFilePath() const {
  return file_path_;
}

const FuncDefNode &FuncDefWithinNonGlobalScope::GetDef() const {
  return def_;
}

void FuncDefWithinNonGlobalScope::Print(ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool FuncDefWithinNonGlobalScope::IsEqual(
    const SemanticProblem &problem) const {
  const FuncDefWithinNonGlobalScope &rhs =
      static_cast<const FuncDefWithinNonGlobalScope&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

FuncDefWithoutReturnValueError::FuncDefWithoutReturnValueError(
    const path &file_path, const FuncDefNode &def)
    : file_path_(file_path), def_(def) {
}

const path &FuncDefWithoutReturnValueError::GetFilePath() const {
  return file_path_;
}

const FuncDefNode &FuncDefWithoutReturnValueError::GetDef() const {
  return def_;
}

void FuncDefWithoutReturnValueError::Print(ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool FuncDefWithoutReturnValueError::IsEqual(
    const SemanticProblem &problem) const {
  const FuncDefWithoutReturnValueError &rhs =
      static_cast<const FuncDefWithoutReturnValueError&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

CallWithNonFuncError::CallWithNonFuncError(
    const path &file_path, const CallNode &call)
    : file_path_(file_path), call_(call) {
}

const path &CallWithNonFuncError::GetFilePath() const {
  return file_path_;
}

const CallNode &CallWithNonFuncError::GetCall() const {
  return call_;
}

void CallWithNonFuncError::Print(ostream &stream) const {
  stream << "call=" << call_;
}

bool CallWithNonFuncError::IsEqual(const SemanticProblem &problem) const {
  const CallWithNonFuncError &rhs =
      static_cast<const CallWithNonFuncError&>(problem);
  return call_ == rhs.call_;
}

CallWithInvalidArgsCount::CallWithInvalidArgsCount(
    const path &file_path,
    const CallNode &call,
    size_t expected_count,
    size_t actual_count)
    : file_path_(file_path),
      call_(call),
      expected_count_(expected_count),
      actual_count_(actual_count) {
}

const path &CallWithInvalidArgsCount::GetFilePath() const {
  return file_path_;
}

const CallNode &CallWithInvalidArgsCount::GetCall() const {
  return call_;
}

size_t CallWithInvalidArgsCount::GetExpectedCount() const {
  return expected_count_;
}

size_t CallWithInvalidArgsCount::GetActualCount() const {
  return actual_count_;
}

void CallWithInvalidArgsCount::Print(ostream &stream) const {
  stream << "call=" << call_ << "; expected=" << expected_count_
         << "; actual=" << actual_count_;
}

bool CallWithInvalidArgsCount::IsEqual(const SemanticProblem &problem) const {
  const CallWithInvalidArgsCount &rhs =
      static_cast<const CallWithInvalidArgsCount&>(problem);
  return call_ == rhs.call_
      && expected_count_ == rhs.expected_count_
      && actual_count_ == rhs.actual_count_;
}
}
}
