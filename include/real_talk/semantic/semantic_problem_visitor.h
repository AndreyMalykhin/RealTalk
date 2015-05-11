
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
class FuncDefWithinNonGlobalScope;
class FuncDefWithoutReturnValueError;
class CallWithNonFuncError;
class CallWithInvalidArgsCount;

class SemanticProblemVisitor {
 public:
  virtual ~SemanticProblemVisitor() {}
  virtual void VisitArrayAllocWithTooManyDimensionsError(
      const ArrayAllocWithTooManyDimensionsError &error) = 0;
  virtual void VisitArrayTypeWithTooManyDimensionsError(
      const ArrayTypeWithTooManyDimensionsError &error) = 0;
  virtual void VisitDoubleWithOutOfRangeValueError(
      const DoubleWithOutOfRangeValueError &error) = 0;
  virtual void VisitLongWithOutOfRangeValueError(
      const LongWithOutOfRangeValueError &error) = 0;
  virtual void VisitIntWithOutOfRangeValueError(
      const IntWithOutOfRangeValueError &error) = 0;
  virtual void VisitCharWithMultipleCharsError(
      const CharWithMultipleCharsError &error) = 0;
  virtual void VisitCharWithEmptyHexValueError(
      const CharWithEmptyHexValueError &error) = 0;
  virtual void VisitCharWithOutOfRangeHexValueError(
      const CharWithOutOfRangeHexValueError &error) = 0;
  virtual void VisitStringWithOutOfRangeHexValueError(
      const StringWithOutOfRangeHexValueError &error) = 0;
  virtual void VisitStringWithEmptyHexValueError(
      const StringWithEmptyHexValueError &error) = 0;
  virtual void VisitUnaryExprWithUnsupportedTypeError(
      const UnaryExprWithUnsupportedTypeError &error) = 0;
  virtual void VisitAssignWithRightValueAssigneeError(
      const AssignWithRightValueAssigneeError &error) = 0;
  virtual void VisitIdWithoutDefError(
      const IdWithoutDefError &error) = 0;
  virtual void VisitSubscriptWithUnsupportedIndexTypeError(
      const SubscriptWithUnsupportedIndexTypeError &error) = 0;
  virtual void VisitSubscriptWithNonArrayError(
      const SubscriptWithNonArrayError &error) = 0;
  virtual void VisitArrayAllocWithIncompatibleValueTypeError(
      const ArrayAllocWithIncompatibleValueTypeError &error) = 0;
  virtual void VisitArrayAllocWithUnsupportedElementTypeError(
      const ArrayAllocWithUnsupportedElementTypeError &error) = 0;
  virtual void VisitArrayAllocWithUnsupportedSizeTypeError(
      const ArrayAllocWithUnsupportedSizeTypeError &error) = 0;
  virtual void VisitIfWithIncompatibleTypeError(
      const IfWithIncompatibleTypeError &error) = 0;
  virtual void VisitBreakNotWithinLoopError(
      const BreakNotWithinLoopError &error) = 0;
  virtual void VisitContinueNotWithinLoopError(
      const ContinueNotWithinLoopError &error) = 0;
  virtual void VisitPreTestLoopWithIncompatibleTypeError(
      const PreTestLoopWithIncompatibleTypeError &error) = 0;
  virtual void VisitImportIsNotFirstStmtError(
      const ImportIsNotFirstStmtError &error) = 0;
  virtual void VisitVarDefWithIncompatibleValueTypeError(
      const VarDefWithIncompatibleValueTypeError &error) = 0;
  virtual void VisitReturnWithoutValueError(
      const ReturnWithoutValueError &error) = 0;
  virtual void VisitReturnNotWithinFuncError(
      const ReturnNotWithinFuncError &error) = 0;
  virtual void VisitReturnWithIncompatibleTypeError(
      const ReturnWithIncompatibleTypeError &error) = 0;
  virtual void VisitBinaryExprWithUnsupportedTypesError(
      const BinaryExprWithUnsupportedTypesError &error) = 0;
  virtual void VisitCallWithIncompatibleArgTypeError(
      const CallWithIncompatibleArgTypeError &error) = 0;
  virtual void VisitDefWithUnsupportedTypeError(
      const DefWithUnsupportedTypeError &error) = 0;
  virtual void VisitDuplicateDefError(
      const DuplicateDefError &error) = 0;
  virtual void VisitFuncDefWithoutBodyNotNativeError(
      const FuncDefWithoutBodyNotNativeError &error) = 0;
  virtual void VisitFuncDefWithBodyIsNativeError(
      const FuncDefWithBodyIsNativeError &error) = 0;
  virtual void VisitFuncDefWithinNonGlobalScope(
      const FuncDefWithinNonGlobalScope &error) = 0;
  virtual void VisitFuncDefWithoutReturnValueError(
      const FuncDefWithoutReturnValueError &error) = 0;
  virtual void VisitCallWithNonFuncError(
      const CallWithNonFuncError &error) = 0;
  virtual void VisitCallWithInvalidArgsCount(
      const CallWithInvalidArgsCount &error) = 0;
};
}
}
#endif
