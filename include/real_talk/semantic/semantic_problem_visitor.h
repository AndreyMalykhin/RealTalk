
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEM_VISITOR_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_PROBLEM_VISITOR_H_

namespace real_talk {
namespace semantic {

class ArrayAllocWithTooManyDimensionsError;
class ArrayTypeWithTooManyDimensionsError;
class DoubleWithOutOfRangeValueError;
class LongWithOutOfRangeValueError;
class IntWithOutOfRangeValueError;
class CharWithMultipleCharsError;
class CharWithEmptyHexValueError;
class CharWithOutOfRangeHexValueError;
class StringWithOutOfRangeHexValueError;
class StringWithEmptyHexValueError;
class UnaryExprWithUnsupportedTypeError;
class AssignWithRightValueAssigneeError;
class IdWithoutDefError;
class SubscriptWithUnsupportedIndexTypeError;
class SubscriptWithNonArrayError;
class ArrayAllocWithIncompatibleValueTypeError;
class ArrayAllocWithUnsupportedElementTypeError;
class ArrayAllocWithUnsupportedSizeTypeError;
class IfWithIncompatibleTypeError;
class BreakNotWithinLoopError;
class ContinueNotWithinLoopError;
class PreTestLoopWithIncompatibleTypeError;
class ImportIsNotFirstStmtError;
class VarDefWithIncompatibleValueTypeError;
class ReturnWithoutValueError;
class ReturnNotWithinFuncError;
class ReturnWithIncompatibleTypeError;
class BinaryExprWithUnsupportedTypesError;
class CallWithIncompatibleArgTypeError;
class DefWithUnsupportedTypeError;
class DuplicateDefError;
class FuncDefWithoutBodyNotNativeError;
class FuncDefWithBodyIsNativeError;
class FuncDefWithinNonGlobalScopeError;
class FuncDefWithoutReturnValueError;
class CallWithNonFuncError;
class CallWithInvalidArgsCountError;

class SemanticProblemVisitor {
 public:
  virtual ~SemanticProblemVisitor() {}
  virtual void VisitArrayAllocWithTooManyDimensionsError(
      const ArrayAllocWithTooManyDimensionsError &error) const = 0;
  virtual void VisitArrayTypeWithTooManyDimensionsError(
      const ArrayTypeWithTooManyDimensionsError &error) const = 0;
  virtual void VisitDoubleWithOutOfRangeValueError(
      const DoubleWithOutOfRangeValueError &error) const = 0;
  virtual void VisitLongWithOutOfRangeValueError(
      const LongWithOutOfRangeValueError &error) const = 0;
  virtual void VisitIntWithOutOfRangeValueError(
      const IntWithOutOfRangeValueError &error) const = 0;
  virtual void VisitCharWithMultipleCharsError(
      const CharWithMultipleCharsError &error) const = 0;
  virtual void VisitCharWithEmptyHexValueError(
      const CharWithEmptyHexValueError &error) const = 0;
  virtual void VisitCharWithOutOfRangeHexValueError(
      const CharWithOutOfRangeHexValueError &error) const = 0;
  virtual void VisitStringWithOutOfRangeHexValueError(
      const StringWithOutOfRangeHexValueError &error) const = 0;
  virtual void VisitStringWithEmptyHexValueError(
      const StringWithEmptyHexValueError &error) const = 0;
  virtual void VisitUnaryExprWithUnsupportedTypeError(
      const UnaryExprWithUnsupportedTypeError &error) const = 0;
  virtual void VisitAssignWithRightValueAssigneeError(
      const AssignWithRightValueAssigneeError &error) const = 0;
  virtual void VisitIdWithoutDefError(
      const IdWithoutDefError &error) const = 0;
  virtual void VisitSubscriptWithUnsupportedIndexTypeError(
      const SubscriptWithUnsupportedIndexTypeError &error) const = 0;
  virtual void VisitSubscriptWithNonArrayError(
      const SubscriptWithNonArrayError &error) const = 0;
  virtual void VisitArrayAllocWithIncompatibleValueTypeError(
      const ArrayAllocWithIncompatibleValueTypeError &error) const = 0;
  virtual void VisitArrayAllocWithUnsupportedElementTypeError(
      const ArrayAllocWithUnsupportedElementTypeError &error) const = 0;
  virtual void VisitArrayAllocWithUnsupportedSizeTypeError(
      const ArrayAllocWithUnsupportedSizeTypeError &error) const = 0;
  virtual void VisitIfWithIncompatibleTypeError(
      const IfWithIncompatibleTypeError &error) const = 0;
  virtual void VisitBreakNotWithinLoopError(
      const BreakNotWithinLoopError &error) const = 0;
  virtual void VisitContinueNotWithinLoopError(
      const ContinueNotWithinLoopError &error) const = 0;
  virtual void VisitPreTestLoopWithIncompatibleTypeError(
      const PreTestLoopWithIncompatibleTypeError &error) const = 0;
  virtual void VisitImportIsNotFirstStmtError(
      const ImportIsNotFirstStmtError &error) const = 0;
  virtual void VisitVarDefWithIncompatibleValueTypeError(
      const VarDefWithIncompatibleValueTypeError &error) const = 0;
  virtual void VisitReturnWithoutValueError(
      const ReturnWithoutValueError &error) const = 0;
  virtual void VisitReturnNotWithinFuncError(
      const ReturnNotWithinFuncError &error) const = 0;
  virtual void VisitReturnWithIncompatibleTypeError(
      const ReturnWithIncompatibleTypeError &error) const = 0;
  virtual void VisitBinaryExprWithUnsupportedTypesError(
      const BinaryExprWithUnsupportedTypesError &error) const = 0;
  virtual void VisitCallWithIncompatibleArgTypeError(
      const CallWithIncompatibleArgTypeError &error) const = 0;
  virtual void VisitDefWithUnsupportedTypeError(
      const DefWithUnsupportedTypeError &error) const = 0;
  virtual void VisitDuplicateDefError(
      const DuplicateDefError &error) const = 0;
  virtual void VisitFuncDefWithoutBodyNotNativeError(
      const FuncDefWithoutBodyNotNativeError &error) const = 0;
  virtual void VisitFuncDefWithBodyIsNativeError(
      const FuncDefWithBodyIsNativeError &error) const = 0;
  virtual void VisitFuncDefWithinNonGlobalScopeError(
      const FuncDefWithinNonGlobalScopeError &error) const = 0;
  virtual void VisitFuncDefWithoutReturnValueError(
      const FuncDefWithoutReturnValueError &error) const = 0;
  virtual void VisitCallWithNonFuncError(
      const CallWithNonFuncError &error) const = 0;
  virtual void VisitCallWithInvalidArgsCountError(
      const CallWithInvalidArgsCountError &error) const = 0;
};
}
}
#endif
