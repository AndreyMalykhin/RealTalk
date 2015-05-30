
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
class UnaryExprNode;
class ArrayAllocNode;
class BoundedArraySizeNode;
class StringNode;
class ArrayDataTypeNode;
class ReturnNode;
class DefNode;
class FuncDefNode;
class DoubleNode;
class LongNode;
class IntNode;
class CharNode;
class AssignNode;
class IdNode;
class SubscriptNode;
class BreakNode;
class ContinueNode;
class PreTestLoopNode;
class ImportNode;
class ReturnWithoutValueNode;
class ReturnValueNode;
class CallNode;
class FuncDefWithoutBodyNode;
class FuncDefWithBodyNode;
}

namespace semantic {

class DataType;

class ArrayAllocWithTooManyDimensionsError: public SemanticError {
 public:
  ArrayAllocWithTooManyDimensionsError(
      const real_talk::parser::ArrayAllocNode &alloc, size_t max_count);
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  size_t GetMaxCount() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ArrayAllocNode &alloc_;
  size_t max_count_;
};

class ArrayTypeWithTooManyDimensionsError: public SemanticError {
 public:
  ArrayTypeWithTooManyDimensionsError(
      const real_talk::parser::ArrayDataTypeNode &array_type, size_t max_count);
  const real_talk::parser::ArrayDataTypeNode &GetArrayType() const;
  size_t GetMaxCount() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ArrayDataTypeNode &array_type_;
  size_t max_count_;
};

class DoubleWithOutOfRangeValueError: public SemanticError {
 public:
  DoubleWithOutOfRangeValueError(
      const real_talk::parser::DoubleNode &double_node);
  const real_talk::parser::DoubleNode &GetDouble() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::DoubleNode &double_;
};

class LongWithOutOfRangeValueError: public SemanticError {
 public:
  LongWithOutOfRangeValueError(
      const real_talk::parser::LongNode &long_node);
  const real_talk::parser::LongNode &GetLong() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::LongNode &long_;
};

class IntWithOutOfRangeValueError: public SemanticError {
 public:
  IntWithOutOfRangeValueError(
      const real_talk::parser::IntNode &int_node);
  const real_talk::parser::IntNode &GetInt() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::IntNode &int_;
};

class CharWithMultipleCharsError: public SemanticError {
 public:
  CharWithMultipleCharsError(
      const real_talk::parser::CharNode &c);
  const real_talk::parser::CharNode &GetChar() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::CharNode &c_;
};

class CharWithEmptyHexValueError: public SemanticError {
 public:
  CharWithEmptyHexValueError(
      const real_talk::parser::CharNode &c);
  const real_talk::parser::CharNode &GetChar() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::CharNode &c_;
};

class CharWithOutOfRangeHexValueError: public SemanticError {
 public:
  CharWithOutOfRangeHexValueError(
      const real_talk::parser::CharNode &c);
  const real_talk::parser::CharNode &GetChar() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::CharNode &c_;
};

class StringWithOutOfRangeHexValueError: public SemanticError {
 public:
  StringWithOutOfRangeHexValueError(
      const real_talk::parser::StringNode &str);
  const real_talk::parser::StringNode &GetString() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::StringNode &str_;
};

class StringWithEmptyHexValueError: public SemanticError {
 public:
  StringWithEmptyHexValueError(
      const real_talk::parser::StringNode &str);
  const real_talk::parser::StringNode &GetString() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::StringNode &str_;
};

class AssignWithRightValueAssigneeError: public SemanticError {
 public:
  AssignWithRightValueAssigneeError(
      const real_talk::parser::AssignNode &assign);
  const real_talk::parser::AssignNode &GetAssign() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::AssignNode &assign_;
};

class IdWithoutDefError: public SemanticError {
 public:
  IdWithoutDefError(
      const real_talk::parser::IdNode &id);
  const real_talk::parser::IdNode &GetId() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::IdNode &id_;
};

class SubscriptWithUnsupportedIndexTypeError: public SemanticError {
 public:
  SubscriptWithUnsupportedIndexTypeError(
      const real_talk::parser::SubscriptNode &subscript,
      std::unique_ptr<DataType> data_type);
  const real_talk::parser::SubscriptNode &GetSubscript() const;
  const DataType &GetDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::SubscriptNode &subscript_;
  std::unique_ptr<DataType> data_type_;
};

class SubscriptWithNonArrayError: public SemanticError {
 public:
  SubscriptWithNonArrayError(
      const real_talk::parser::SubscriptNode &subscript);
  const real_talk::parser::SubscriptNode &GetSubscript() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::SubscriptNode &subscript_;
};

class ArrayAllocWithIncompatibleValueTypeError: public SemanticError {
 public:
  ArrayAllocWithIncompatibleValueTypeError(
      const real_talk::parser::ArrayAllocNode &alloc,
      size_t value_index,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  size_t GetValueIndex() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ArrayAllocNode &alloc_;
  size_t value_index_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class ArrayAllocWithUnsupportedElementTypeError: public SemanticError {
 public:
  ArrayAllocWithUnsupportedElementTypeError(
      const real_talk::parser::ArrayAllocNode &alloc,
      std::unique_ptr<DataType> data_type);
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  const DataType &GetDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ArrayAllocNode &alloc_;
  std::unique_ptr<DataType> data_type_;
};

class ArrayAllocWithUnsupportedSizeTypeError: public SemanticError {
 public:
  ArrayAllocWithUnsupportedSizeTypeError(
      const real_talk::parser::ArrayAllocNode &alloc,
      const real_talk::parser::BoundedArraySizeNode &size,
      std::unique_ptr<DataType> data_type);
  const real_talk::parser::ArrayAllocNode &GetAlloc() const;
  const real_talk::parser::BoundedArraySizeNode &GetSize() const;
  const DataType &GetDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ArrayAllocNode &alloc_;
  const real_talk::parser::BoundedArraySizeNode &size_;
  std::unique_ptr<DataType> data_type_;
};

class IfWithIncompatibleTypeError: public SemanticError {
 public:
  IfWithIncompatibleTypeError(
      const real_talk::parser::BranchNode &branch_node,
      const real_talk::parser::IfNode &if_node,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  const real_talk::parser::BranchNode &GetBranch() const;
  const real_talk::parser::IfNode &GetIf() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::BranchNode &branch_;
  const real_talk::parser::IfNode &if_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class BreakNotWithinLoopError: public SemanticError {
 public:
  BreakNotWithinLoopError(
      const real_talk::parser::BreakNode &break_node);
  const real_talk::parser::BreakNode &GetBreak() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::BreakNode &break_;
};

class ContinueNotWithinLoopError: public SemanticError {
 public:
  ContinueNotWithinLoopError(
      const real_talk::parser::ContinueNode &continue_node);
  const real_talk::parser::ContinueNode &GetContinue() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ContinueNode &continue_;
};

class PreTestLoopWithIncompatibleTypeError: public SemanticError {
 public:
  PreTestLoopWithIncompatibleTypeError(
      const real_talk::parser::PreTestLoopNode &loop,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  const real_talk::parser::PreTestLoopNode &GetLoop() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::PreTestLoopNode &loop_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class ImportIsNotFirstStmtError: public SemanticError {
 public:
  ImportIsNotFirstStmtError(
      const real_talk::parser::ImportNode &import);
  const real_talk::parser::ImportNode &GetImport() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ImportNode &import_;
};

class VarDefWithIncompatibleValueTypeError: public SemanticError {
 public:
  VarDefWithIncompatibleValueTypeError(
      const real_talk::parser::VarDefWithInitNode &var_def,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  const real_talk::parser::VarDefWithInitNode &GetVarDef() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::VarDefWithInitNode &var_def_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class ReturnWithoutValueError: public SemanticError {
 public:
  ReturnWithoutValueError(
      const real_talk::parser::ReturnWithoutValueNode &return_node);
  const real_talk::parser::ReturnWithoutValueNode &GetReturn() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ReturnWithoutValueNode &return_;
};

class ReturnNotWithinFuncError: public SemanticError {
 public:
  ReturnNotWithinFuncError(
      const real_talk::parser::ReturnNode &return_node);
  const real_talk::parser::ReturnNode &GetReturn() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ReturnNode &return_;
};

class ReturnWithIncompatibleTypeError: public SemanticError {
 public:
  ReturnWithIncompatibleTypeError(
      const real_talk::parser::ReturnValueNode &return_node,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  const real_talk::parser::ReturnValueNode &GetReturn() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::ReturnValueNode &return_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class CallWithIncompatibleArgTypeError: public SemanticError {
 public:
  CallWithIncompatibleArgTypeError(
      const real_talk::parser::CallNode &call,
      size_t arg_index,
      std::unique_ptr<DataType> dest_data_type,
      std::unique_ptr<DataType> src_data_type);
  const real_talk::parser::CallNode &GetCall() const;
  size_t GetArgIndex() const;
  const DataType &GetDestDataType() const;
  const DataType &GetSrcDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::CallNode &call_;
  size_t arg_index_;
  std::unique_ptr<DataType> dest_data_type_;
  std::unique_ptr<DataType> src_data_type_;
};

class BinaryExprWithUnsupportedTypesError: public SemanticError {
 public:
  BinaryExprWithUnsupportedTypesError(
      const real_talk::parser::BinaryExprNode &expr,
      std::unique_ptr<DataType> left_operand_data_type,
      std::unique_ptr<DataType> right_operand_data_type);
  const real_talk::parser::BinaryExprNode &GetExpr() const;
  const DataType &GetLeftOperandDataType() const;
  const DataType &GetRightOperandDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::BinaryExprNode &expr_;
  std::unique_ptr<DataType> left_operand_data_type_;
  std::unique_ptr<DataType> right_operand_data_type_;
};

class UnaryExprWithUnsupportedTypeError: public SemanticError {
 public:
  UnaryExprWithUnsupportedTypeError(
      const real_talk::parser::UnaryExprNode &expr,
      std::unique_ptr<DataType> data_type);
  const real_talk::parser::UnaryExprNode &GetExpr() const;
  const DataType &GetDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::UnaryExprNode &expr_;
  std::unique_ptr<DataType> data_type_;
};

class DefWithUnsupportedTypeError: public SemanticError {
 public:
  DefWithUnsupportedTypeError(
      const real_talk::parser::DefNode &def,
      std::unique_ptr<DataType> data_type);
  const real_talk::parser::DefNode &GetDef() const;
  const DataType &GetDataType() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::DefNode &def_;
  std::unique_ptr<DataType> data_type_;
};

class DuplicateDefError: public SemanticError {
 public:
  DuplicateDefError(
      const real_talk::parser::DefNode &def);
  const real_talk::parser::DefNode &GetDef() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::DefNode &def_;
};

class FuncDefWithoutBodyNotNativeError: public SemanticError {
 public:
  FuncDefWithoutBodyNotNativeError(
      const real_talk::parser::FuncDefWithoutBodyNode &def);
  const real_talk::parser::FuncDefWithoutBodyNode &GetDef() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::FuncDefWithoutBodyNode &def_;
};

class FuncDefWithBodyIsNativeError: public SemanticError {
 public:
  FuncDefWithBodyIsNativeError(
      const real_talk::parser::FuncDefWithBodyNode &def);
  const real_talk::parser::FuncDefWithBodyNode &GetDef() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::FuncDefWithBodyNode &def_;
};

class FuncDefWithinNonGlobalScopeError: public SemanticError {
 public:
  FuncDefWithinNonGlobalScopeError(
      const real_talk::parser::FuncDefNode &def);
  const real_talk::parser::FuncDefNode &GetDef() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::FuncDefNode &def_;
};

class FuncDefWithoutReturnValueError: public SemanticError {
 public:
  FuncDefWithoutReturnValueError(
      const real_talk::parser::FuncDefNode &def);
  const real_talk::parser::FuncDefNode &GetDef() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::FuncDefNode &def_;
};

class CallWithNonFuncError: public SemanticError {
 public:
  CallWithNonFuncError(
      const real_talk::parser::CallNode &call);
  const real_talk::parser::CallNode &GetCall() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::CallNode &call_;
};

class CallWithInvalidArgsCountError: public SemanticError {
 public:
  CallWithInvalidArgsCountError(
      const real_talk::parser::CallNode &call,
      size_t expected_count,
      size_t actual_count);
  const real_talk::parser::CallNode &GetCall() const;
  size_t GetExpectedCount() const;
  size_t GetActualCount() const;
  virtual void Accept(const SemanticProblemVisitor *visitor) const override;

 private:
  virtual void Print(std::ostream &stream) const override;
  virtual bool IsEqual(const SemanticProblem &rhs) const override;

  const real_talk::parser::CallNode &call_;
  size_t expected_count_;
  size_t actual_count_;
};
}
}
#endif
