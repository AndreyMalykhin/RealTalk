
#ifndef _REAL_TALK_CODE_CMD_VISITOR_H_
#define _REAL_TALK_CODE_CMD_VISITOR_H_

namespace real_talk {
namespace code {

class AndCmd;
class OrCmd;
class CreateGlobalIntVarCmd;
class CreateGlobalArrayVarCmd;
class CreateGlobalLongVarCmd;
class CreateGlobalDoubleVarCmd;
class CreateGlobalCharVarCmd;
class CreateGlobalStringVarCmd;
class CreateGlobalBoolVarCmd;
class CreateLocalIntVarCmd;
class CreateLocalArrayVarCmd;
class CreateLocalLongVarCmd;
class CreateLocalDoubleVarCmd;
class CreateLocalCharVarCmd;
class CreateLocalStringVarCmd;
class CreateLocalBoolVarCmd;
class DestroyLocalIntVarCmd;
class DestroyLocalArrayVarCmd;
class DestroyLocalLongVarCmd;
class DestroyLocalDoubleVarCmd;
class DestroyLocalCharVarCmd;
class DestroyLocalStringVarCmd;
class DestroyLocalBoolVarCmd;
class UnloadIntCmd;
class UnloadArrayCmd;
class UnloadLongCmd;
class UnloadDoubleCmd;
class UnloadCharCmd;
class UnloadStringCmd;
class UnloadBoolCmd;
class LoadIntValueCmd;
class LoadLongValueCmd;
class LoadBoolValueCmd;
class LoadCharValueCmd;
class LoadStringValueCmd;
class LoadDoubleValueCmd;
class LoadFuncValueCmd;
class LoadNativeFuncValueCmd;
class CreateAndInitGlobalBoolVarCmd;
class CreateAndInitGlobalStringVarCmd;
class CreateAndInitGlobalCharVarCmd;
class CreateAndInitGlobalDoubleVarCmd;
class CreateAndInitGlobalLongVarCmd;
class CreateAndInitGlobalArrayVarCmd;
class CreateAndInitGlobalIntVarCmd;
class CreateAndInitLocalIntVarCmd;
class CreateAndInitLocalCharVarCmd;
class CreateAndInitLocalStringVarCmd;
class CreateAndInitLocalBoolVarCmd;
class CreateAndInitLocalArrayVarCmd;
class CreateAndInitLocalLongVarCmd;
class CreateAndInitLocalDoubleVarCmd;
class CreateIntArrayCmd;
class CreateLongArrayCmd;
class CreateBoolArrayCmd;
class CreateCharArrayCmd;
class CreateStringArrayCmd;
class CreateDoubleArrayCmd;
class CreateAndInitIntArrayCmd;
class CreateAndInitLongArrayCmd;
class CreateAndInitDoubleArrayCmd;
class CreateAndInitBoolArrayCmd;
class CreateAndInitCharArrayCmd;
class CreateAndInitStringArrayCmd;
class DirectJumpCmd;
class ImplicitJumpIfCmd;
class JumpIfNotCmd;
class ImplicitJumpIfNotCmd;
class ReturnCmd;
class LoadGlobalIntVarValueCmd;
class LoadGlobalArrayVarValueCmd;
class LoadGlobalLongVarValueCmd;
class LoadGlobalDoubleVarValueCmd;
class LoadGlobalCharVarValueCmd;
class LoadGlobalStringVarValueCmd;
class LoadGlobalBoolVarValueCmd;
class LoadLocalIntVarValueCmd;
class LoadLocalArrayVarValueCmd;
class LoadLocalLongVarValueCmd;
class LoadLocalCharVarValueCmd;
class LoadLocalStringVarValueCmd;
class LoadLocalBoolVarValueCmd;
class LoadLocalDoubleVarValueCmd;
class LoadGlobalVarAddressCmd;
class LoadLocalVarAddressCmd;
class LoadArrayOfIntsElementValueCmd;
class LoadArrayOfLongsElementValueCmd;
class LoadArrayOfDoublesElementValueCmd;
class LoadArrayOfCharsElementValueCmd;
class LoadArrayOfStringsElementValueCmd;
class LoadArrayOfBoolsElementValueCmd;
class LoadArrayOfIntsElementAddressCmd;
class LoadArrayOfDoublesElementAddressCmd;
class LoadArrayOfBoolsElementAddressCmd;
class LoadArrayOfStringsElementAddressCmd;
class LoadArrayOfArraysElementAddressCmd;
class LoadArrayOfLongsElementAddressCmd;
class LoadArrayOfCharsElementAddressCmd;
class StoreIntCmd;
class StoreLongCmd;
class StoreDoubleCmd;
class StoreBoolCmd;
class StoreCharCmd;
class StoreStringCmd;
class StoreArrayCmd;
class CastCharToIntCmd;
class CastCharToDoubleCmd;
class CastCharToLongCmd;
class CastCharToStringCmd;
class CastIntToLongCmd;
class CastIntToDoubleCmd;
class CastLongToDoubleCmd;
class CallNativeCmd;
class CallCmd;
class MulCharCmd;
class MulIntCmd;
class MulLongCmd;
class MulDoubleCmd;
class DivCharCmd;
class DivIntCmd;
class DivDoubleCmd;
class DivLongCmd;
class SumCharCmd;
class SumLongCmd;
class SumDoubleCmd;
class SumStringCmd;
class SumIntCmd;
class SubCharCmd;
class SubIntCmd;
class SubLongCmd;
class SubDoubleCmd;
class EqualIntCmd;
class EqualLongCmd;
class EqualDoubleCmd;
class EqualStringCmd;
class EqualArrayCmd;
class EqualBoolCmd;
class EqualCharCmd;
class NotEqualIntCmd;
class NotEqualLongCmd;
class NotEqualDoubleCmd;
class NotEqualBoolCmd;
class NotEqualStringCmd;
class NotEqualArrayCmd;
class NotEqualCharCmd;
class GreaterCharCmd;
class GreaterIntCmd;
class GreaterLongCmd;
class GreaterDoubleCmd;
class GreaterOrEqualCharCmd;
class GreaterOrEqualIntCmd;
class GreaterOrEqualLongCmd;
class GreaterOrEqualDoubleCmd;
class LessCharCmd;
class LessIntCmd;
class LessLongCmd;
class LessDoubleCmd;
class LessOrEqualIntCmd;
class LessOrEqualDoubleCmd;
class LessOrEqualCharCmd;
class LessOrEqualLongCmd;
class LogicalNegateBoolCmd;
class ArithmeticNegateIntCmd;
class ArithmeticNegateLongCmd;
class ArithmeticNegateDoubleCmd;
class PreDecCharCmd;
class PreDecIntCmd;
class PreDecLongCmd;
class PreDecDoubleCmd;
class PreIncCharCmd;
class PreIncIntCmd;
class PreIncLongCmd;
class PreIncDoubleCmd;

class CmdVisitor {
 public:
  virtual ~CmdVisitor() {}
  virtual void VisitCreateGlobalIntVar(
      const CreateGlobalIntVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalArrayVar(
      const CreateGlobalArrayVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalLongVar(
      const CreateGlobalLongVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalDoubleVar(
      const CreateGlobalDoubleVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalCharVar(
      const CreateGlobalCharVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalStringVar(
      const CreateGlobalStringVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalBoolVar(
      const CreateGlobalBoolVarCmd &cmd) = 0;
  virtual void VisitCreateLocalIntVar(
      const CreateLocalIntVarCmd &cmd) = 0;
  virtual void VisitCreateLocalArrayVar(
      const CreateLocalArrayVarCmd &cmd) = 0;
  virtual void VisitCreateLocalLongVar(
      const CreateLocalLongVarCmd &cmd) = 0;
  virtual void VisitCreateLocalDoubleVar(
      const CreateLocalDoubleVarCmd &cmd) = 0;
  virtual void VisitCreateLocalCharVar(
      const CreateLocalCharVarCmd &cmd) = 0;
  virtual void VisitCreateLocalStringVar(
      const CreateLocalStringVarCmd &cmd) = 0;
  virtual void VisitCreateLocalBoolVar(
      const CreateLocalBoolVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalIntVar(
      const DestroyLocalIntVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalArrayVar(
      const DestroyLocalArrayVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalLongVar(
      const DestroyLocalLongVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalDoubleVar(
      const DestroyLocalDoubleVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalCharVar(
      const DestroyLocalCharVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalStringVar(
      const DestroyLocalStringVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalBoolVar(
      const DestroyLocalBoolVarCmd &cmd) = 0;
  virtual void VisitUnloadInt(
      const UnloadIntCmd &cmd) = 0;
  virtual void VisitUnloadArray(
      const UnloadArrayCmd &cmd) = 0;
  virtual void VisitUnloadLong(
      const UnloadLongCmd &cmd) = 0;
  virtual void VisitUnloadDouble(
      const UnloadDoubleCmd &cmd) = 0;
  virtual void VisitUnloadChar(
      const UnloadCharCmd &cmd) = 0;
  virtual void VisitUnloadString(
      const UnloadStringCmd &cmd) = 0;
  virtual void VisitUnloadBool(
      const UnloadBoolCmd &cmd) = 0;
  virtual void VisitLoadIntValue(
      const LoadIntValueCmd &cmd) = 0;
  virtual void VisitLoadLongValue(
      const LoadLongValueCmd &cmd) = 0;
  virtual void VisitLoadBoolValue(
      const LoadBoolValueCmd &cmd) = 0;
  virtual void VisitLoadCharValue(
      const LoadCharValueCmd &cmd) = 0;
  virtual void VisitLoadStringValue(
      const LoadStringValueCmd &cmd) = 0;
  virtual void VisitLoadDoubleValue(
      const LoadDoubleValueCmd &cmd) = 0;
  virtual void VisitLoadFuncValue(
      const LoadFuncValueCmd &cmd) = 0;
  virtual void VisitLoadNativeFuncValue(
      const LoadNativeFuncValueCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalBoolVar(
      const CreateAndInitGlobalBoolVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalStringVar(
      const CreateAndInitGlobalStringVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalCharVar(
      const CreateAndInitGlobalCharVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalDoubleVar(
      const CreateAndInitGlobalDoubleVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalLongVar(
      const CreateAndInitGlobalLongVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalArrayVar(
      const CreateAndInitGlobalArrayVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitGlobalIntVar(
      const CreateAndInitGlobalIntVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalIntVar(
      const CreateAndInitLocalIntVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalCharVar(
      const CreateAndInitLocalCharVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalStringVar(
      const CreateAndInitLocalStringVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalBoolVar(
      const CreateAndInitLocalBoolVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalArrayVar(
      const CreateAndInitLocalArrayVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalLongVar(
      const CreateAndInitLocalLongVarCmd &cmd) = 0;
  virtual void VisitCreateAndInitLocalDoubleVar(
      const CreateAndInitLocalDoubleVarCmd &cmd) = 0;
  virtual void VisitCreateIntArray(
      const CreateIntArrayCmd &cmd) = 0;
  virtual void VisitCreateLongArray(
      const CreateLongArrayCmd &cmd) = 0;
  virtual void VisitCreateBoolArray(
      const CreateBoolArrayCmd &cmd) = 0;
  virtual void VisitCreateCharArray(
      const CreateCharArrayCmd &cmd) = 0;
  virtual void VisitCreateDoubleArray(
      const CreateDoubleArrayCmd &cmd) = 0;
  virtual void VisitCreateStringArray(
      const CreateStringArrayCmd &cmd) = 0;
  virtual void VisitCreateAndInitIntArray(
      const CreateAndInitIntArrayCmd &cmd) = 0;
  virtual void VisitCreateAndInitLongArray(
      const CreateAndInitLongArrayCmd &cmd) = 0;
  virtual void VisitCreateAndInitDoubleArray(
      const CreateAndInitDoubleArrayCmd &cmd) = 0;
  virtual void VisitCreateAndInitBoolArray(
      const CreateAndInitBoolArrayCmd &cmd) = 0;
  virtual void VisitCreateAndInitCharArray(
      const CreateAndInitCharArrayCmd &cmd) = 0;
  virtual void VisitCreateAndInitStringArray(
      const CreateAndInitStringArrayCmd &cmd) = 0;
  virtual void VisitDirectJump(
      const DirectJumpCmd &cmd) = 0;
  virtual void VisitImplicitJumpIf(
      const ImplicitJumpIfCmd &cmd) = 0;
  virtual void VisitJumpIfNot(
      const JumpIfNotCmd &cmd) = 0;
  virtual void VisitImplicitJumpIfNot(
      const ImplicitJumpIfNotCmd &cmd) = 0;
  virtual void VisitReturn(
      const ReturnCmd &cmd) = 0;
  virtual void VisitLoadGlobalIntVarValue(
      const LoadGlobalIntVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalArrayVarValue(
      const LoadGlobalArrayVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalLongVarValue(
      const LoadGlobalLongVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalDoubleVarValue(
      const LoadGlobalDoubleVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalCharVarValue(
      const LoadGlobalCharVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalStringVarValue(
      const LoadGlobalStringVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalBoolVarValue(
      const LoadGlobalBoolVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalIntVarValue(
      const LoadLocalIntVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalArrayVarValue(
      const LoadLocalArrayVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalLongVarValue(
      const LoadLocalLongVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalCharVarValue(
      const LoadLocalCharVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalStringVarValue(
      const LoadLocalStringVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalBoolVarValue(
      const LoadLocalBoolVarValueCmd &cmd) = 0;
  virtual void VisitLoadLocalDoubleVarValue(
      const LoadLocalDoubleVarValueCmd &cmd) = 0;
  virtual void VisitLoadGlobalVarAddress(
      const LoadGlobalVarAddressCmd &cmd) = 0;
  virtual void VisitLoadLocalVarAddress(
      const LoadLocalVarAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfIntsElementValue(
      const LoadArrayOfIntsElementValueCmd &cmd) = 0;
  virtual void VisitLoadArrayOfLongsElementValue(
      const LoadArrayOfLongsElementValueCmd &cmd) = 0;
  virtual void VisitLoadArrayOfDoublesElementValue(
      const LoadArrayOfDoublesElementValueCmd &cmd) = 0;
  virtual void VisitLoadArrayOfCharsElementValue(
      const LoadArrayOfCharsElementValueCmd &cmd) = 0;
  virtual void VisitLoadArrayOfStringsElementValue(
      const LoadArrayOfStringsElementValueCmd &cmd) = 0;
  virtual void VisitLoadArrayOfBoolsElementValue(
      const LoadArrayOfBoolsElementValueCmd &cmd) = 0;
  virtual void VisitLoadArrayOfIntsElementAddress(
      const LoadArrayOfIntsElementAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfDoublesElementAddress(
      const LoadArrayOfDoublesElementAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfBoolsElementAddress(
      const LoadArrayOfBoolsElementAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfStringsElementAddress(
      const LoadArrayOfStringsElementAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfArraysElementAddress(
      const LoadArrayOfArraysElementAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfLongsElementAddress(
      const LoadArrayOfLongsElementAddressCmd &cmd) = 0;
  virtual void VisitLoadArrayOfCharsElementAddress(
      const LoadArrayOfCharsElementAddressCmd &cmd) = 0;
  virtual void VisitStoreInt(
      const StoreIntCmd &cmd) = 0;
  virtual void VisitStoreLong(
      const StoreLongCmd &cmd) = 0;
  virtual void VisitStoreDouble(
      const StoreDoubleCmd &cmd) = 0;
  virtual void VisitStoreBool(
      const StoreBoolCmd &cmd) = 0;
  virtual void VisitStoreChar(
      const StoreCharCmd &cmd) = 0;
  virtual void VisitStoreString(
      const StoreStringCmd &cmd) = 0;
  virtual void VisitStoreArray(
      const StoreArrayCmd &cmd) = 0;
  virtual void VisitCastCharToInt(
      const CastCharToIntCmd &cmd) = 0;
  virtual void VisitCastCharToDouble(
      const CastCharToDoubleCmd &cmd) = 0;
  virtual void VisitCastCharToLong(
      const CastCharToLongCmd &cmd) = 0;
  virtual void VisitCastCharToString(
      const CastCharToStringCmd &cmd) = 0;
  virtual void VisitCastIntToLong(
      const CastIntToLongCmd &cmd) = 0;
  virtual void VisitCastIntToDouble(
      const CastIntToDoubleCmd &cmd) = 0;
  virtual void VisitCastLongToDouble(
      const CastLongToDoubleCmd &cmd) = 0;
  virtual void VisitCallNative(
      const CallNativeCmd &cmd) = 0;
  virtual void VisitCall(
      const CallCmd &cmd) = 0;
  virtual void VisitMulChar(
      const MulCharCmd &cmd) = 0;
  virtual void VisitMulInt(
      const MulIntCmd &cmd) = 0;
  virtual void VisitMulLong(
      const MulLongCmd &cmd) = 0;
  virtual void VisitMulDouble(
      const MulDoubleCmd &cmd) = 0;
  virtual void VisitDivChar(
      const DivCharCmd &cmd) = 0;
  virtual void VisitDivInt(
      const DivIntCmd &cmd) = 0;
  virtual void VisitDivDouble(
      const DivDoubleCmd &cmd) = 0;
  virtual void VisitDivLong(
      const DivLongCmd &cmd) = 0;
  virtual void VisitSumChar(
      const SumCharCmd &cmd) = 0;
  virtual void VisitSumLong(
      const SumLongCmd &cmd) = 0;
  virtual void VisitSumDouble(
      const SumDoubleCmd &cmd) = 0;
  virtual void VisitSumString(
      const SumStringCmd &cmd) = 0;
  virtual void VisitSumInt(
      const SumIntCmd &cmd) = 0;
  virtual void VisitSubChar(
      const SubCharCmd &cmd) = 0;
  virtual void VisitSubInt(
      const SubIntCmd &cmd) = 0;
  virtual void VisitSubLong(
      const SubLongCmd &cmd) = 0;
  virtual void VisitSubDouble(
      const SubDoubleCmd &cmd) = 0;
  virtual void VisitEqualInt(
      const EqualIntCmd &cmd) = 0;
  virtual void VisitEqualLong(
      const EqualLongCmd &cmd) = 0;
  virtual void VisitEqualDouble(
      const EqualDoubleCmd &cmd) = 0;
  virtual void VisitEqualString(
      const EqualStringCmd &cmd) = 0;
  virtual void VisitEqualArray(
      const EqualArrayCmd &cmd) = 0;
  virtual void VisitEqualBool(
      const EqualBoolCmd &cmd) = 0;
  virtual void VisitEqualChar(
      const EqualCharCmd &cmd) = 0;
  virtual void VisitNotEqualInt(
      const NotEqualIntCmd &cmd) = 0;
  virtual void VisitNotEqualLong(
      const NotEqualLongCmd &cmd) = 0;
  virtual void VisitNotEqualDouble(
      const NotEqualDoubleCmd &cmd) = 0;
  virtual void VisitNotEqualBool(
      const NotEqualBoolCmd &cmd) = 0;
  virtual void VisitNotEqualString(
      const NotEqualStringCmd &cmd) = 0;
  virtual void VisitNotEqualArray(
      const NotEqualArrayCmd &cmd) = 0;
  virtual void VisitNotEqualChar(
      const NotEqualCharCmd &cmd) = 0;
  virtual void VisitGreaterChar(
      const GreaterCharCmd &cmd) = 0;
  virtual void VisitGreaterInt(
      const GreaterIntCmd &cmd) = 0;
  virtual void VisitGreaterLong(
      const GreaterLongCmd &cmd) = 0;
  virtual void VisitGreaterDouble(
      const GreaterDoubleCmd &cmd) = 0;
  virtual void VisitGreaterOrEqualChar(
      const GreaterOrEqualCharCmd &cmd) = 0;
  virtual void VisitGreaterOrEqualInt(
      const GreaterOrEqualIntCmd &cmd) = 0;
  virtual void VisitGreaterOrEqualLong(
      const GreaterOrEqualLongCmd &cmd) = 0;
  virtual void VisitGreaterOrEqualDouble(
      const GreaterOrEqualDoubleCmd &cmd) = 0;
  virtual void VisitLessChar(
      const LessCharCmd &cmd) = 0;
  virtual void VisitLessInt(
      const LessIntCmd &cmd) = 0;
  virtual void VisitLessLong(
      const LessLongCmd &cmd) = 0;
  virtual void VisitLessDouble(
      const LessDoubleCmd &cmd) = 0;
  virtual void VisitLessOrEqualInt(
      const LessOrEqualIntCmd &cmd) = 0;
  virtual void VisitLessOrEqualDouble(
      const LessOrEqualDoubleCmd &cmd) = 0;
  virtual void VisitLessOrEqualChar(
      const LessOrEqualCharCmd &cmd) = 0;
  virtual void VisitLessOrEqualLong(
      const LessOrEqualLongCmd &cmd) = 0;
  virtual void VisitLogicalNegateBool(
      const LogicalNegateBoolCmd &cmd) = 0;
  virtual void VisitArithmeticNegateInt(
      const ArithmeticNegateIntCmd &cmd) = 0;
  virtual void VisitArithmeticNegateLong(
      const ArithmeticNegateLongCmd &cmd) = 0;
  virtual void VisitArithmeticNegateDouble(
      const ArithmeticNegateDoubleCmd &cmd) = 0;
  virtual void VisitPreDecChar(
      const PreDecCharCmd &cmd) = 0;
  virtual void VisitPreDecInt(
      const PreDecIntCmd &cmd) = 0;
  virtual void VisitPreDecLong(
      const PreDecLongCmd &cmd) = 0;
  virtual void VisitPreDecDouble(
      const PreDecDoubleCmd &cmd) = 0;
  virtual void VisitPreIncChar(
      const PreIncCharCmd &cmd) = 0;
  virtual void VisitPreIncInt(
      const PreIncIntCmd &cmd) = 0;
  virtual void VisitPreIncLong(
      const PreIncLongCmd &cmd) = 0;
  virtual void VisitPreIncDouble(
      const PreIncDoubleCmd &cmd) = 0;
  virtual void VisitAnd(
      const AndCmd &cmd) = 0;
  virtual void VisitOr(
      const OrCmd &cmd) = 0;
};
}
}
#endif
