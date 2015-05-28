
#include <cassert>
#include "real_talk/parser/var_def_with_init_node.h"
#include "real_talk/parser/func_def_with_body_node.h"
#include "real_talk/parser/func_def_without_body_node.h"
#include "real_talk/parser/call_node.h"
#include "real_talk/parser/pre_test_loop_node.h"
#include "real_talk/parser/binary_expr_node.h"
#include "real_talk/parser/unary_expr_node.h"
#include "real_talk/parser/return_value_node.h"
#include "real_talk/parser/return_without_value_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/parser/continue_node.h"
#include "real_talk/parser/break_node.h"
#include "real_talk/parser/branch_node.h"
#include "real_talk/parser/if_node.h"
#include "real_talk/parser/array_alloc_node.h"
#include "real_talk/parser/bounded_array_size_node.h"
#include "real_talk/parser/subscript_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/assign_node.h"
#include "real_talk/parser/char_node.h"
#include "real_talk/parser/int_node.h"
#include "real_talk/parser/long_node.h"
#include "real_talk/parser/double_node.h"
#include "real_talk/parser/array_data_type_node.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/semantic_problem_visitor.h"
#include "real_talk/semantic/semantic_problems.h"

using std::ostream;
using std::unique_ptr;
using real_talk::lexer::TokenInfo;
using real_talk::parser::VarDefWithInitNode;
using real_talk::parser::Node;
using real_talk::parser::DefNode;
using real_talk::parser::CallNode;
using real_talk::parser::BinaryExprNode;
using real_talk::parser::UnaryExprNode;
using real_talk::parser::FuncDefWithBodyNode;
using real_talk::parser::FuncDefWithoutBodyNode;
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
using real_talk::parser::SubscriptNode;
using real_talk::parser::IdNode;
using real_talk::parser::AssignNode;
using real_talk::parser::StringNode;
using real_talk::parser::CharNode;
using real_talk::parser::IntNode;
using real_talk::parser::LongNode;
using real_talk::parser::DoubleNode;
using real_talk::parser::ArrayDataTypeNode;

namespace real_talk {
namespace semantic {

ArrayAllocWithTooManyDimensionsError::ArrayAllocWithTooManyDimensionsError(
    const ArrayAllocNode &alloc,
    size_t max_count)
    : alloc_(alloc),
      max_count_(max_count) {
  assert(max_count > 0);
}

void ArrayAllocWithTooManyDimensionsError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitArrayAllocWithTooManyDimensionsError(*this);
}

const ArrayAllocNode &ArrayAllocWithTooManyDimensionsError::GetAlloc()
    const {
  return alloc_;
}

size_t ArrayAllocWithTooManyDimensionsError::GetMaxCount() const {
  return max_count_;
}

void ArrayAllocWithTooManyDimensionsError::Print(ostream &stream) const {
  stream << "alloc=" << alloc_ << "; max_count=" << max_count_;
}

bool ArrayAllocWithTooManyDimensionsError::IsEqual(
    const SemanticProblem &problem) const {
  const ArrayAllocWithTooManyDimensionsError &rhs =
      static_cast<const ArrayAllocWithTooManyDimensionsError&>(problem);
  return alloc_ == rhs.alloc_ && max_count_ == rhs.max_count_;
}

ArrayTypeWithTooManyDimensionsError::ArrayTypeWithTooManyDimensionsError(
    const ArrayDataTypeNode &array_type,
    size_t max_count)
    : array_type_(array_type),
      max_count_(max_count) {
  assert(max_count > 0);
}

void ArrayTypeWithTooManyDimensionsError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitArrayTypeWithTooManyDimensionsError(*this);
}

const ArrayDataTypeNode &ArrayTypeWithTooManyDimensionsError::GetArrayType()
    const {
  return array_type_;
}

size_t ArrayTypeWithTooManyDimensionsError::GetMaxCount() const {
  return max_count_;
}

void ArrayTypeWithTooManyDimensionsError::Print(ostream &stream) const {
  stream << "array_type=" << array_type_ << "; max_count=" << max_count_;
}

bool ArrayTypeWithTooManyDimensionsError::IsEqual(
    const SemanticProblem &problem) const {
  const ArrayTypeWithTooManyDimensionsError &rhs =
      static_cast<const ArrayTypeWithTooManyDimensionsError&>(problem);
  return array_type_ == rhs.array_type_ && max_count_ == rhs.max_count_;
}

DoubleWithOutOfRangeValueError::DoubleWithOutOfRangeValueError(
    const DoubleNode &double_node): double_(double_node) {}

void DoubleWithOutOfRangeValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitDoubleWithOutOfRangeValueError(*this);
}

const DoubleNode &DoubleWithOutOfRangeValueError::GetDouble() const {
  return double_;
}

void DoubleWithOutOfRangeValueError::Print(ostream &stream) const {
  stream << "double=" << double_;
}

bool DoubleWithOutOfRangeValueError::IsEqual(
    const SemanticProblem &problem) const {
  const DoubleWithOutOfRangeValueError &rhs =
      static_cast<const DoubleWithOutOfRangeValueError&>(problem);
  return double_ == rhs.double_;
}

LongWithOutOfRangeValueError::LongWithOutOfRangeValueError(
    const LongNode &long_node): long_(long_node) {}

void LongWithOutOfRangeValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitLongWithOutOfRangeValueError(*this);
}

const LongNode &LongWithOutOfRangeValueError::GetLong() const {
  return long_;
}

void LongWithOutOfRangeValueError::Print(ostream &stream) const {
  stream << "long=" << long_;
}

bool LongWithOutOfRangeValueError::IsEqual(
    const SemanticProblem &problem) const {
  const LongWithOutOfRangeValueError &rhs =
      static_cast<const LongWithOutOfRangeValueError&>(problem);
  return long_ == rhs.long_;
}

IntWithOutOfRangeValueError::IntWithOutOfRangeValueError(
    const IntNode &int_node): int_(int_node) {}

void IntWithOutOfRangeValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitIntWithOutOfRangeValueError(*this);
}

const IntNode &IntWithOutOfRangeValueError::GetInt() const {
  return int_;
}

void IntWithOutOfRangeValueError::Print(ostream &stream) const {
  stream << "int=" << int_;
}

bool IntWithOutOfRangeValueError::IsEqual(
    const SemanticProblem &problem) const {
  const IntWithOutOfRangeValueError &rhs =
      static_cast<const IntWithOutOfRangeValueError&>(problem);
  return int_ == rhs.int_;
}

CharWithMultipleCharsError::CharWithMultipleCharsError(
    const CharNode &c): c_(c) {}

void CharWithMultipleCharsError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitCharWithMultipleCharsError(*this);
}

const CharNode &CharWithMultipleCharsError::GetChar() const {
  return c_;
}

void CharWithMultipleCharsError::Print(ostream &stream) const {
  stream << "char=" << c_;
}

bool CharWithMultipleCharsError::IsEqual(
    const SemanticProblem &problem) const {
  const CharWithMultipleCharsError &rhs =
      static_cast<const CharWithMultipleCharsError&>(problem);
  return c_ == rhs.c_;
}

CharWithEmptyHexValueError::CharWithEmptyHexValueError(
    const CharNode &c): c_(c) {}

void CharWithEmptyHexValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitCharWithEmptyHexValueError(*this);
}

const CharNode &CharWithEmptyHexValueError::GetChar() const {
  return c_;
}

void CharWithEmptyHexValueError::Print(ostream &stream) const {
  stream << "char=" << c_;
}

bool CharWithEmptyHexValueError::IsEqual(
    const SemanticProblem &problem) const {
  const CharWithEmptyHexValueError &rhs =
      static_cast<const CharWithEmptyHexValueError&>(problem);
  return c_ == rhs.c_;
}

CharWithOutOfRangeHexValueError::CharWithOutOfRangeHexValueError(
    const CharNode &c): c_(c) {}

void CharWithOutOfRangeHexValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitCharWithOutOfRangeHexValueError(*this);
}

const CharNode &CharWithOutOfRangeHexValueError::GetChar() const {
  return c_;
}

void CharWithOutOfRangeHexValueError::Print(ostream &stream) const {
  stream << "char=" << c_;
}

bool CharWithOutOfRangeHexValueError::IsEqual(
    const SemanticProblem &problem) const {
  const CharWithOutOfRangeHexValueError &rhs =
      static_cast<const CharWithOutOfRangeHexValueError&>(problem);
  return c_ == rhs.c_;
}

StringWithOutOfRangeHexValueError::StringWithOutOfRangeHexValueError(
    const StringNode &str): str_(str) {}

void StringWithOutOfRangeHexValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitStringWithOutOfRangeHexValueError(*this);
}

const StringNode &StringWithOutOfRangeHexValueError::GetString() const {
  return str_;
}

void StringWithOutOfRangeHexValueError::Print(ostream &stream) const {
  stream << "string=" << str_;
}

bool StringWithOutOfRangeHexValueError::IsEqual(
    const SemanticProblem &problem) const {
  const StringWithOutOfRangeHexValueError &rhs =
      static_cast<const StringWithOutOfRangeHexValueError&>(problem);
  return str_ == rhs.str_;
}

StringWithEmptyHexValueError::StringWithEmptyHexValueError(
    const StringNode &str): str_(str) {}

void StringWithEmptyHexValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitStringWithEmptyHexValueError(*this);
}

const StringNode &StringWithEmptyHexValueError::GetString() const {
  return str_;
}

void StringWithEmptyHexValueError::Print(ostream &stream) const {
  stream << "string=" << str_;
}

bool StringWithEmptyHexValueError::IsEqual(
    const SemanticProblem &problem) const {
  const StringWithEmptyHexValueError &rhs =
      static_cast<const StringWithEmptyHexValueError&>(problem);
  return str_ == rhs.str_;
}

UnaryExprWithUnsupportedTypeError::UnaryExprWithUnsupportedTypeError(
    const UnaryExprNode &expr, unique_ptr<DataType> data_type)
    : expr_(expr), data_type_(move(data_type)) {
  assert(data_type_);
}

void UnaryExprWithUnsupportedTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitUnaryExprWithUnsupportedTypeError(*this);
}

const UnaryExprNode &UnaryExprWithUnsupportedTypeError::GetExpr() const {
  return expr_;
}

const DataType &UnaryExprWithUnsupportedTypeError::GetDataType() const {
  return *data_type_;
}

void UnaryExprWithUnsupportedTypeError::Print(ostream &stream) const {
  stream << "expr=" << expr_ << "; data_type=" << *data_type_;
}

bool UnaryExprWithUnsupportedTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const UnaryExprWithUnsupportedTypeError &rhs =
      static_cast<const UnaryExprWithUnsupportedTypeError&>(problem);
  return expr_ == rhs.expr_ && *data_type_ == *(rhs.data_type_);
}

AssignWithRightValueAssigneeError::AssignWithRightValueAssigneeError(
    const AssignNode &assign): assign_(assign) {}

void AssignWithRightValueAssigneeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitAssignWithRightValueAssigneeError(*this);
}

const AssignNode &AssignWithRightValueAssigneeError::GetAssign() const {
  return assign_;
}

void AssignWithRightValueAssigneeError::Print(ostream &stream) const {
  stream << "assign=" << assign_;
}

bool AssignWithRightValueAssigneeError::IsEqual(
    const SemanticProblem &problem) const {
  const AssignWithRightValueAssigneeError &rhs =
      static_cast<const AssignWithRightValueAssigneeError&>(problem);
  return assign_ == rhs.assign_;
}

IdWithoutDefError::IdWithoutDefError(const IdNode &id): id_(id) {}

void IdWithoutDefError::Accept(const SemanticProblemVisitor *visitor) const {
  visitor->VisitIdWithoutDefError(*this);
}

const IdNode &IdWithoutDefError::GetId() const {
  return id_;
}

void IdWithoutDefError::Print(ostream &stream) const {
  stream << "id=" << id_;
}

bool IdWithoutDefError::IsEqual(const SemanticProblem &problem) const {
  const IdWithoutDefError &rhs = static_cast<const IdWithoutDefError&>(problem);
  return id_ == rhs.id_;
}

SubscriptWithUnsupportedIndexTypeError
::SubscriptWithUnsupportedIndexTypeError(
     const SubscriptNode &subscript, unique_ptr<DataType> data_type)
    : subscript_(subscript), data_type_(move(data_type)) {
  assert(data_type_);
}

void SubscriptWithUnsupportedIndexTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitSubscriptWithUnsupportedIndexTypeError(*this);
}

const SubscriptNode
&SubscriptWithUnsupportedIndexTypeError::GetSubscript() const {
  return subscript_;
}

const DataType
&SubscriptWithUnsupportedIndexTypeError::GetDataType() const {
  return *data_type_;
}

void SubscriptWithUnsupportedIndexTypeError::Print(ostream &stream) const {
  stream << "subscript=" << subscript_ << "; data_type=" << *data_type_;
}

bool SubscriptWithUnsupportedIndexTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const SubscriptWithUnsupportedIndexTypeError &rhs =
      static_cast<const SubscriptWithUnsupportedIndexTypeError&>(problem);
  return subscript_ == rhs.subscript_
      && *data_type_ == *(rhs.data_type_);
}

SubscriptWithNonArrayError::SubscriptWithNonArrayError(
    const SubscriptNode &subscript): subscript_(subscript) {}

void SubscriptWithNonArrayError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitSubscriptWithNonArrayError(*this);
}

const SubscriptNode &SubscriptWithNonArrayError::GetSubscript() const {
  return subscript_;
}

void SubscriptWithNonArrayError::Print(ostream &stream) const {
  stream << "subscript=" << subscript_;
}

bool SubscriptWithNonArrayError::IsEqual(const SemanticProblem &problem) const {
  const SubscriptWithNonArrayError &rhs =
      static_cast<const SubscriptWithNonArrayError&>(problem);
  return subscript_ == rhs.subscript_;
}

ArrayAllocWithIncompatibleValueTypeError::
ArrayAllocWithIncompatibleValueTypeError(
    const ArrayAllocNode &alloc,
    size_t value_index,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : alloc_(alloc),
      value_index_(value_index),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

void ArrayAllocWithIncompatibleValueTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitArrayAllocWithIncompatibleValueTypeError(*this);
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
    const ArrayAllocNode &alloc, unique_ptr<DataType> data_type)
    : alloc_(alloc), data_type_(move(data_type)) {
  assert(data_type_);
}

void ArrayAllocWithUnsupportedElementTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitArrayAllocWithUnsupportedElementTypeError(*this);
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

ArrayAllocWithUnsupportedSizeTypeError::
ArrayAllocWithUnsupportedSizeTypeError(
    const ArrayAllocNode &alloc,
    const BoundedArraySizeNode &size,
    unique_ptr<DataType> data_type)
    : alloc_(alloc),
      size_(size),
      data_type_(move(data_type)) {
  assert(data_type_);
}

void ArrayAllocWithUnsupportedSizeTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitArrayAllocWithUnsupportedSizeTypeError(*this);
}

const ArrayAllocNode
&ArrayAllocWithUnsupportedSizeTypeError::GetAlloc() const {
  return alloc_;
}

const BoundedArraySizeNode
&ArrayAllocWithUnsupportedSizeTypeError::GetSize() const {
  return size_;
}

const DataType
&ArrayAllocWithUnsupportedSizeTypeError::GetDataType() const {
  return *data_type_;
}

void ArrayAllocWithUnsupportedSizeTypeError::Print(ostream &stream) const {
  stream << "alloc=" << alloc_ << "; size=" << size_ << "; data_type="
         << *data_type_;
}

bool ArrayAllocWithUnsupportedSizeTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const ArrayAllocWithUnsupportedSizeTypeError &rhs =
      static_cast<const ArrayAllocWithUnsupportedSizeTypeError&>(problem);
  return alloc_ == rhs.alloc_
      && size_ == rhs.size_
      && *data_type_ == *(rhs.data_type_);
}

IfWithIncompatibleTypeError::IfWithIncompatibleTypeError(
    const BranchNode &branch_node,
    const IfNode &if_node,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : branch_(branch_node),
      if_(if_node),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

void IfWithIncompatibleTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitIfWithIncompatibleTypeError(*this);
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
    const BreakNode &break_node): break_(break_node) {}

void BreakNotWithinLoopError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitBreakNotWithinLoopError(*this);
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
    const ContinueNode &continue_node): continue_(continue_node) {}

void ContinueNotWithinLoopError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitContinueNotWithinLoopError(*this);
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
    const PreTestLoopNode &loop,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : loop_(loop),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

void PreTestLoopWithIncompatibleTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitPreTestLoopWithIncompatibleTypeError(*this);
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

ImportIsNotFirstStmtError::ImportIsNotFirstStmtError(
    const ImportNode &import): import_(import) {}

void ImportIsNotFirstStmtError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitImportIsNotFirstStmtError(*this);
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

VarDefWithIncompatibleValueTypeError
::VarDefWithIncompatibleValueTypeError(
    const VarDefWithInitNode &var_def,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : var_def_(var_def),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

void VarDefWithIncompatibleValueTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitVarDefWithIncompatibleValueTypeError(*this);
}

const VarDefWithInitNode
&VarDefWithIncompatibleValueTypeError::GetVarDef() const {
  return var_def_;
}

const DataType
&VarDefWithIncompatibleValueTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType
&VarDefWithIncompatibleValueTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void VarDefWithIncompatibleValueTypeError::Print(ostream &stream) const {
  stream << "var_def=" << static_cast<const Node&>(var_def_)
         << "; dest_type=" << *dest_data_type_ << "; src_type="
         << *src_data_type_;
}

bool VarDefWithIncompatibleValueTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const VarDefWithIncompatibleValueTypeError &rhs =
      static_cast<const VarDefWithIncompatibleValueTypeError&>(problem);
  return var_def_ == static_cast<const Node&>(rhs.var_def_)
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

ReturnWithoutValueError::ReturnWithoutValueError(
    const ReturnWithoutValueNode &return_node): return_(return_node) {}

void ReturnWithoutValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitReturnWithoutValueError(*this);
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
    const ReturnNode &return_node): return_(return_node) {}

void ReturnNotWithinFuncError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitReturnNotWithinFuncError(*this);
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
    const ReturnValueNode &return_node,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : return_(return_node),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

void ReturnWithIncompatibleTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitReturnWithIncompatibleTypeError(*this);
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

BinaryExprWithUnsupportedTypesError::BinaryExprWithUnsupportedTypesError(
    const BinaryExprNode &expr,
    unique_ptr<DataType> left_operand_data_type,
    unique_ptr<DataType> right_operand_data_type)
    : expr_(expr),
      left_operand_data_type_(move(left_operand_data_type)),
      right_operand_data_type_(move(right_operand_data_type)) {
  assert(left_operand_data_type_);
  assert(right_operand_data_type_);
}

void BinaryExprWithUnsupportedTypesError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitBinaryExprWithUnsupportedTypesError(*this);
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

CallWithIncompatibleArgTypeError::CallWithIncompatibleArgTypeError(
    const CallNode &call,
    size_t arg_index,
    unique_ptr<DataType> dest_data_type,
    unique_ptr<DataType> src_data_type)
    : call_(call),
      arg_index_(arg_index),
      dest_data_type_(move(dest_data_type)),
      src_data_type_(move(src_data_type)) {
  assert(dest_data_type_);
  assert(src_data_type_);
}

void CallWithIncompatibleArgTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitCallWithIncompatibleArgTypeError(*this);
}

const CallNode &CallWithIncompatibleArgTypeError::GetCall() const {
  return call_;
}

size_t CallWithIncompatibleArgTypeError::GetArgIndex() const {
  return arg_index_;
}

const DataType &CallWithIncompatibleArgTypeError::GetDestDataType() const {
  return *dest_data_type_;
}

const DataType &CallWithIncompatibleArgTypeError::GetSrcDataType() const {
  return *src_data_type_;
}

void CallWithIncompatibleArgTypeError::Print(ostream &stream) const {
  stream << "call=" << call_ << "; arg_index=" << arg_index_
         << "; dest_type=" << *dest_data_type_ << "; src_type="
         << *src_data_type_;
}

bool CallWithIncompatibleArgTypeError::IsEqual(
    const SemanticProblem &problem) const {
  const CallWithIncompatibleArgTypeError &rhs =
      static_cast<const CallWithIncompatibleArgTypeError&>(problem);
  return call_ == rhs.call_
      && arg_index_ == rhs.arg_index_
      && *dest_data_type_ == *(rhs.dest_data_type_)
      && *src_data_type_ == *(rhs.src_data_type_);
}

DefWithUnsupportedTypeError::DefWithUnsupportedTypeError(
    const DefNode &def, unique_ptr<DataType> data_type)
    : def_(def), data_type_(move(data_type)) {
  assert(data_type_);
}

void DefWithUnsupportedTypeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitDefWithUnsupportedTypeError(*this);
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

DuplicateDefError::DuplicateDefError(const DefNode &def): def_(def) {}

void DuplicateDefError::Accept(const SemanticProblemVisitor *visitor) const {
  visitor->VisitDuplicateDefError(*this);
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

FuncDefWithoutBodyNotNativeError::FuncDefWithoutBodyNotNativeError(
    const FuncDefWithoutBodyNode &def): def_(def) {}

void FuncDefWithoutBodyNotNativeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitFuncDefWithoutBodyNotNativeError(*this);
}

const FuncDefWithoutBodyNode &FuncDefWithoutBodyNotNativeError::GetDef() const {
  return def_;
}

void FuncDefWithoutBodyNotNativeError::Print(ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool FuncDefWithoutBodyNotNativeError::IsEqual(
    const SemanticProblem &problem) const {
  const FuncDefWithoutBodyNotNativeError &rhs =
      static_cast<const FuncDefWithoutBodyNotNativeError&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

FuncDefWithBodyIsNativeError::FuncDefWithBodyIsNativeError(
    const FuncDefWithBodyNode &def): def_(def) {}

void FuncDefWithBodyIsNativeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitFuncDefWithBodyIsNativeError(*this);
}

const FuncDefWithBodyNode &FuncDefWithBodyIsNativeError::GetDef() const {
  return def_;
}

void FuncDefWithBodyIsNativeError::Print(ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool FuncDefWithBodyIsNativeError::IsEqual(
    const SemanticProblem &problem) const {
  const FuncDefWithBodyIsNativeError &rhs =
      static_cast<const FuncDefWithBodyIsNativeError&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

FuncDefWithinNonGlobalScopeError::FuncDefWithinNonGlobalScopeError(
    const FuncDefNode &def): def_(def) {}

void FuncDefWithinNonGlobalScopeError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitFuncDefWithinNonGlobalScopeError(*this);
}

const FuncDefNode &FuncDefWithinNonGlobalScopeError::GetDef() const {
  return def_;
}

void FuncDefWithinNonGlobalScopeError::Print(ostream &stream) const {
  stream << "def=" << static_cast<const Node&>(def_);
}

bool FuncDefWithinNonGlobalScopeError::IsEqual(
    const SemanticProblem &problem) const {
  const FuncDefWithinNonGlobalScopeError &rhs =
      static_cast<const FuncDefWithinNonGlobalScopeError&>(problem);
  return def_ == static_cast<const Node&>(rhs.def_);
}

FuncDefWithoutReturnValueError::FuncDefWithoutReturnValueError(
    const FuncDefNode &def): def_(def) {}

void FuncDefWithoutReturnValueError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitFuncDefWithoutReturnValueError(*this);
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
    const CallNode &call): call_(call) {}

void CallWithNonFuncError::Accept(const SemanticProblemVisitor *visitor) const {
  visitor->VisitCallWithNonFuncError(*this);
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

CallWithInvalidArgsCountError::CallWithInvalidArgsCountError(
    const CallNode &call,
    size_t expected_count,
    size_t actual_count)
    : call_(call),
      expected_count_(expected_count),
      actual_count_(actual_count) {
}

void CallWithInvalidArgsCountError::Accept(
    const SemanticProblemVisitor *visitor) const {
  visitor->VisitCallWithInvalidArgsCountError(*this);
}

const CallNode &CallWithInvalidArgsCountError::GetCall() const {
  return call_;
}

size_t CallWithInvalidArgsCountError::GetExpectedCount() const {
  return expected_count_;
}

size_t CallWithInvalidArgsCountError::GetActualCount() const {
  return actual_count_;
}

void CallWithInvalidArgsCountError::Print(ostream &stream) const {
  stream << "call=" << call_ << "; expected=" << expected_count_
         << "; actual=" << actual_count_;
}

bool CallWithInvalidArgsCountError::IsEqual(const SemanticProblem &problem) const {
  const CallWithInvalidArgsCountError &rhs =
      static_cast<const CallWithInvalidArgsCountError&>(problem);
  return call_ == rhs.call_
      && expected_count_ == rhs.expected_count_
      && actual_count_ == rhs.actual_count_;
}
}
}
