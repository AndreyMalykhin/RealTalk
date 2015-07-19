
#include <cassert>
#include <vector>
#include "real_talk/code/exe.h"
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/simple_vm.h"

using std::vector;
using real_talk::code::Exe;
using real_talk::code::Code;
using real_talk::code::CmdReader;
using real_talk::code::CmdVisitor;
using real_talk::code::AndCmd;
using real_talk::code::OrCmd;
using real_talk::code::CreateGlobalIntVarCmd;
using real_talk::code::CreateGlobalArrayVarCmd;
using real_talk::code::CreateGlobalLongVarCmd;
using real_talk::code::CreateGlobalDoubleVarCmd;
using real_talk::code::CreateGlobalCharVarCmd;
using real_talk::code::CreateGlobalStringVarCmd;
using real_talk::code::CreateGlobalBoolVarCmd;
using real_talk::code::CreateLocalIntVarCmd;
using real_talk::code::CreateLocalArrayVarCmd;
using real_talk::code::CreateLocalLongVarCmd;
using real_talk::code::CreateLocalDoubleVarCmd;
using real_talk::code::CreateLocalCharVarCmd;
using real_talk::code::CreateLocalStringVarCmd;
using real_talk::code::CreateLocalBoolVarCmd;
using real_talk::code::DestroyLocalIntVarCmd;
using real_talk::code::DestroyLocalArrayVarCmd;
using real_talk::code::DestroyLocalLongVarCmd;
using real_talk::code::DestroyLocalDoubleVarCmd;
using real_talk::code::DestroyLocalCharVarCmd;
using real_talk::code::DestroyLocalStringVarCmd;
using real_talk::code::DestroyLocalBoolVarCmd;
using real_talk::code::UnloadIntCmd;
using real_talk::code::UnloadArrayCmd;
using real_talk::code::UnloadLongCmd;
using real_talk::code::UnloadDoubleCmd;
using real_talk::code::UnloadCharCmd;
using real_talk::code::UnloadStringCmd;
using real_talk::code::UnloadBoolCmd;
using real_talk::code::LoadIntValueCmd;
using real_talk::code::LoadLongValueCmd;
using real_talk::code::LoadBoolValueCmd;
using real_talk::code::LoadCharValueCmd;
using real_talk::code::LoadStringValueCmd;
using real_talk::code::LoadDoubleValueCmd;
using real_talk::code::LoadFuncValueCmd;
using real_talk::code::LoadNativeFuncValueCmd;
using real_talk::code::CreateAndInitGlobalBoolVarCmd;
using real_talk::code::CreateAndInitGlobalStringVarCmd;
using real_talk::code::CreateAndInitGlobalCharVarCmd;
using real_talk::code::CreateAndInitGlobalDoubleVarCmd;
using real_talk::code::CreateAndInitGlobalLongVarCmd;
using real_talk::code::CreateAndInitGlobalArrayVarCmd;
using real_talk::code::CreateAndInitGlobalIntVarCmd;
using real_talk::code::CreateAndInitLocalIntVarCmd;
using real_talk::code::CreateAndInitLocalCharVarCmd;
using real_talk::code::CreateAndInitLocalStringVarCmd;
using real_talk::code::CreateAndInitLocalBoolVarCmd;
using real_talk::code::CreateAndInitLocalArrayVarCmd;
using real_talk::code::CreateAndInitLocalLongVarCmd;
using real_talk::code::CreateAndInitLocalDoubleVarCmd;
using real_talk::code::CreateIntArrayCmd;
using real_talk::code::CreateLongArrayCmd;
using real_talk::code::CreateBoolArrayCmd;
using real_talk::code::CreateCharArrayCmd;
using real_talk::code::CreateStringArrayCmd;
using real_talk::code::CreateDoubleArrayCmd;
using real_talk::code::CreateAndInitIntArrayCmd;
using real_talk::code::CreateAndInitLongArrayCmd;
using real_talk::code::CreateAndInitDoubleArrayCmd;
using real_talk::code::CreateAndInitBoolArrayCmd;
using real_talk::code::CreateAndInitCharArrayCmd;
using real_talk::code::CreateAndInitStringArrayCmd;
using real_talk::code::DirectJumpCmd;
using real_talk::code::ImplicitJumpIfCmd;
using real_talk::code::JumpIfNotCmd;
using real_talk::code::ImplicitJumpIfNotCmd;
using real_talk::code::ReturnCmd;
using real_talk::code::LoadGlobalIntVarValueCmd;
using real_talk::code::LoadGlobalArrayVarValueCmd;
using real_talk::code::LoadGlobalLongVarValueCmd;
using real_talk::code::LoadGlobalDoubleVarValueCmd;
using real_talk::code::LoadGlobalCharVarValueCmd;
using real_talk::code::LoadGlobalStringVarValueCmd;
using real_talk::code::LoadGlobalBoolVarValueCmd;
using real_talk::code::LoadLocalIntVarValueCmd;
using real_talk::code::LoadLocalArrayVarValueCmd;
using real_talk::code::LoadLocalLongVarValueCmd;
using real_talk::code::LoadLocalCharVarValueCmd;
using real_talk::code::LoadLocalStringVarValueCmd;
using real_talk::code::LoadLocalBoolVarValueCmd;
using real_talk::code::LoadLocalDoubleVarValueCmd;
using real_talk::code::LoadGlobalVarAddressCmd;
using real_talk::code::LoadLocalVarAddressCmd;
using real_talk::code::LoadArrayOfIntsElementValueCmd;
using real_talk::code::LoadArrayOfLongsElementValueCmd;
using real_talk::code::LoadArrayOfDoublesElementValueCmd;
using real_talk::code::LoadArrayOfCharsElementValueCmd;
using real_talk::code::LoadArrayOfStringsElementValueCmd;
using real_talk::code::LoadArrayOfBoolsElementValueCmd;
using real_talk::code::LoadArrayOfArraysElementValueCmd;
using real_talk::code::LoadArrayOfIntsElementAddressCmd;
using real_talk::code::LoadArrayOfDoublesElementAddressCmd;
using real_talk::code::LoadArrayOfBoolsElementAddressCmd;
using real_talk::code::LoadArrayOfStringsElementAddressCmd;
using real_talk::code::LoadArrayOfArraysElementAddressCmd;
using real_talk::code::LoadArrayOfLongsElementAddressCmd;
using real_talk::code::LoadArrayOfCharsElementAddressCmd;
using real_talk::code::StoreIntCmd;
using real_talk::code::StoreLongCmd;
using real_talk::code::StoreDoubleCmd;
using real_talk::code::StoreBoolCmd;
using real_talk::code::StoreCharCmd;
using real_talk::code::StoreStringCmd;
using real_talk::code::StoreArrayCmd;
using real_talk::code::CastCharToIntCmd;
using real_talk::code::CastCharToDoubleCmd;
using real_talk::code::CastCharToLongCmd;
using real_talk::code::CastCharToStringCmd;
using real_talk::code::CastIntToLongCmd;
using real_talk::code::CastIntToDoubleCmd;
using real_talk::code::CastLongToDoubleCmd;
using real_talk::code::CallNativeCmd;
using real_talk::code::CallCmd;
using real_talk::code::MulCharCmd;
using real_talk::code::MulIntCmd;
using real_talk::code::MulLongCmd;
using real_talk::code::MulDoubleCmd;
using real_talk::code::DivCharCmd;
using real_talk::code::DivIntCmd;
using real_talk::code::DivDoubleCmd;
using real_talk::code::DivLongCmd;
using real_talk::code::SumCharCmd;
using real_talk::code::SumLongCmd;
using real_talk::code::SumDoubleCmd;
using real_talk::code::SumStringCmd;
using real_talk::code::SumIntCmd;
using real_talk::code::SubCharCmd;
using real_talk::code::SubIntCmd;
using real_talk::code::SubLongCmd;
using real_talk::code::SubDoubleCmd;
using real_talk::code::EqualIntCmd;
using real_talk::code::EqualLongCmd;
using real_talk::code::EqualDoubleCmd;
using real_talk::code::EqualStringCmd;
using real_talk::code::EqualArrayCmd;
using real_talk::code::EqualBoolCmd;
using real_talk::code::EqualCharCmd;
using real_talk::code::NotEqualIntCmd;
using real_talk::code::NotEqualLongCmd;
using real_talk::code::NotEqualDoubleCmd;
using real_talk::code::NotEqualBoolCmd;
using real_talk::code::NotEqualStringCmd;
using real_talk::code::NotEqualArrayCmd;
using real_talk::code::NotEqualCharCmd;
using real_talk::code::GreaterCharCmd;
using real_talk::code::GreaterIntCmd;
using real_talk::code::GreaterLongCmd;
using real_talk::code::GreaterDoubleCmd;
using real_talk::code::GreaterOrEqualCharCmd;
using real_talk::code::GreaterOrEqualIntCmd;
using real_talk::code::GreaterOrEqualLongCmd;
using real_talk::code::GreaterOrEqualDoubleCmd;
using real_talk::code::LessCharCmd;
using real_talk::code::LessIntCmd;
using real_talk::code::LessLongCmd;
using real_talk::code::LessDoubleCmd;
using real_talk::code::LessOrEqualIntCmd;
using real_talk::code::LessOrEqualDoubleCmd;
using real_talk::code::LessOrEqualCharCmd;
using real_talk::code::LessOrEqualLongCmd;
using real_talk::code::LogicalNegateBoolCmd;
using real_talk::code::ArithmeticNegateIntCmd;
using real_talk::code::ArithmeticNegateLongCmd;
using real_talk::code::ArithmeticNegateDoubleCmd;
using real_talk::code::PreDecCharCmd;
using real_talk::code::PreDecIntCmd;
using real_talk::code::PreDecLongCmd;
using real_talk::code::PreDecDoubleCmd;
using real_talk::code::PreIncCharCmd;
using real_talk::code::PreIncIntCmd;
using real_talk::code::PreIncLongCmd;
using real_talk::code::PreIncDoubleCmd;

namespace real_talk {
namespace vm {

class SimpleVM::Impl: private CmdVisitor {
 public:
  Impl(Exe *exe, const vector<NativeFunc> &native_funcs);
  void Execute();
  const DataStorage &GetGlobalVars() const;
  const DataStorage &GetLocalVars() const;
  const DataStorage &GetOperands() const;
  const FuncFrames &GetFuncFrames() const;

 private:
  virtual void VisitCreateGlobalIntVar(
      const CreateGlobalIntVarCmd &cmd) override;
  virtual void VisitCreateGlobalArrayVar(
      const CreateGlobalArrayVarCmd &cmd) override;
  virtual void VisitCreateGlobalLongVar(
      const CreateGlobalLongVarCmd &cmd) override;
  virtual void VisitCreateGlobalDoubleVar(
      const CreateGlobalDoubleVarCmd &cmd) override;
  virtual void VisitCreateGlobalCharVar(
      const CreateGlobalCharVarCmd &cmd) override;
  virtual void VisitCreateGlobalStringVar(
      const CreateGlobalStringVarCmd &cmd) override;
  virtual void VisitCreateGlobalBoolVar(
      const CreateGlobalBoolVarCmd &cmd) override;
  virtual void VisitCreateLocalIntVar(
      const CreateLocalIntVarCmd &cmd) override;
  virtual void VisitCreateLocalArrayVar(
      const CreateLocalArrayVarCmd &cmd) override;
  virtual void VisitCreateLocalLongVar(
      const CreateLocalLongVarCmd &cmd) override;
  virtual void VisitCreateLocalDoubleVar(
      const CreateLocalDoubleVarCmd &cmd) override;
  virtual void VisitCreateLocalCharVar(
      const CreateLocalCharVarCmd &cmd) override;
  virtual void VisitCreateLocalStringVar(
      const CreateLocalStringVarCmd &cmd) override;
  virtual void VisitCreateLocalBoolVar(
      const CreateLocalBoolVarCmd &cmd) override;
  virtual void VisitDestroyLocalIntVar(
      const DestroyLocalIntVarCmd &cmd) override;
  virtual void VisitDestroyLocalArrayVar(
      const DestroyLocalArrayVarCmd &cmd) override;
  virtual void VisitDestroyLocalLongVar(
      const DestroyLocalLongVarCmd &cmd) override;
  virtual void VisitDestroyLocalDoubleVar(
      const DestroyLocalDoubleVarCmd &cmd) override;
  virtual void VisitDestroyLocalCharVar(
      const DestroyLocalCharVarCmd &cmd) override;
  virtual void VisitDestroyLocalStringVar(
      const DestroyLocalStringVarCmd &cmd) override;
  virtual void VisitDestroyLocalBoolVar(
      const DestroyLocalBoolVarCmd &cmd) override;
  virtual void VisitUnloadInt(
      const UnloadIntCmd &cmd) override;
  virtual void VisitUnloadArray(
      const UnloadArrayCmd &cmd) override;
  virtual void VisitUnloadLong(
      const UnloadLongCmd &cmd) override;
  virtual void VisitUnloadDouble(
      const UnloadDoubleCmd &cmd) override;
  virtual void VisitUnloadChar(
      const UnloadCharCmd &cmd) override;
  virtual void VisitUnloadString(
      const UnloadStringCmd &cmd) override;
  virtual void VisitUnloadBool(
      const UnloadBoolCmd &cmd) override;
  virtual void VisitLoadIntValue(
      const LoadIntValueCmd &cmd) override;
  virtual void VisitLoadLongValue(
      const LoadLongValueCmd &cmd) override;
  virtual void VisitLoadBoolValue(
      const LoadBoolValueCmd &cmd) override;
  virtual void VisitLoadCharValue(
      const LoadCharValueCmd &cmd) override;
  virtual void VisitLoadStringValue(
      const LoadStringValueCmd &cmd) override;
  virtual void VisitLoadDoubleValue(
      const LoadDoubleValueCmd &cmd) override;
  virtual void VisitLoadFuncValue(
      const LoadFuncValueCmd &cmd) override;
  virtual void VisitLoadNativeFuncValue(
      const LoadNativeFuncValueCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalBoolVar(
      const CreateAndInitGlobalBoolVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalStringVar(
      const CreateAndInitGlobalStringVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalCharVar(
      const CreateAndInitGlobalCharVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalDoubleVar(
      const CreateAndInitGlobalDoubleVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalLongVar(
      const CreateAndInitGlobalLongVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalArrayVar(
      const CreateAndInitGlobalArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalIntVar(
      const CreateAndInitGlobalIntVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalIntVar(
      const CreateAndInitLocalIntVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalCharVar(
      const CreateAndInitLocalCharVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalStringVar(
      const CreateAndInitLocalStringVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalBoolVar(
      const CreateAndInitLocalBoolVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalArrayVar(
      const CreateAndInitLocalArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalLongVar(
      const CreateAndInitLocalLongVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalDoubleVar(
      const CreateAndInitLocalDoubleVarCmd &cmd) override;
  virtual void VisitCreateIntArray(
      const CreateIntArrayCmd &cmd) override;
  virtual void VisitCreateLongArray(
      const CreateLongArrayCmd &cmd) override;
  virtual void VisitCreateBoolArray(
      const CreateBoolArrayCmd &cmd) override;
  virtual void VisitCreateCharArray(
      const CreateCharArrayCmd &cmd) override;
  virtual void VisitCreateDoubleArray(
      const CreateDoubleArrayCmd &cmd) override;
  virtual void VisitCreateStringArray(
      const CreateStringArrayCmd &cmd) override;
  virtual void VisitCreateAndInitIntArray(
      const CreateAndInitIntArrayCmd &cmd) override;
  virtual void VisitCreateAndInitLongArray(
      const CreateAndInitLongArrayCmd &cmd) override;
  virtual void VisitCreateAndInitDoubleArray(
      const CreateAndInitDoubleArrayCmd &cmd) override;
  virtual void VisitCreateAndInitBoolArray(
      const CreateAndInitBoolArrayCmd &cmd) override;
  virtual void VisitCreateAndInitCharArray(
      const CreateAndInitCharArrayCmd &cmd) override;
  virtual void VisitCreateAndInitStringArray(
      const CreateAndInitStringArrayCmd &cmd) override;
  virtual void VisitDirectJump(
      const DirectJumpCmd &cmd) override;
  virtual void VisitImplicitJumpIf(
      const ImplicitJumpIfCmd &cmd) override;
  virtual void VisitJumpIfNot(
      const JumpIfNotCmd &cmd) override;
  virtual void VisitImplicitJumpIfNot(
      const ImplicitJumpIfNotCmd &cmd) override;
  virtual void VisitReturn(
      const ReturnCmd &cmd) override;
  virtual void VisitLoadGlobalIntVarValue(
      const LoadGlobalIntVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalArrayVarValue(
      const LoadGlobalArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalLongVarValue(
      const LoadGlobalLongVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalDoubleVarValue(
      const LoadGlobalDoubleVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalCharVarValue(
      const LoadGlobalCharVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalStringVarValue(
      const LoadGlobalStringVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalBoolVarValue(
      const LoadGlobalBoolVarValueCmd &cmd) override;
  virtual void VisitLoadLocalIntVarValue(
      const LoadLocalIntVarValueCmd &cmd) override;
  virtual void VisitLoadLocalArrayVarValue(
      const LoadLocalArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalLongVarValue(
      const LoadLocalLongVarValueCmd &cmd) override;
  virtual void VisitLoadLocalCharVarValue(
      const LoadLocalCharVarValueCmd &cmd) override;
  virtual void VisitLoadLocalStringVarValue(
      const LoadLocalStringVarValueCmd &cmd) override;
  virtual void VisitLoadLocalBoolVarValue(
      const LoadLocalBoolVarValueCmd &cmd) override;
  virtual void VisitLoadLocalDoubleVarValue(
      const LoadLocalDoubleVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalVarAddress(
      const LoadGlobalVarAddressCmd &cmd) override;
  virtual void VisitLoadLocalVarAddress(
      const LoadLocalVarAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfIntsElementValue(
      const LoadArrayOfIntsElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfLongsElementValue(
      const LoadArrayOfLongsElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfDoublesElementValue(
      const LoadArrayOfDoublesElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfCharsElementValue(
      const LoadArrayOfCharsElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfStringsElementValue(
      const LoadArrayOfStringsElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfBoolsElementValue(
      const LoadArrayOfBoolsElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfArraysElementValue(
      const LoadArrayOfArraysElementValueCmd &cmd) override;
  virtual void VisitLoadArrayOfIntsElementAddress(
      const LoadArrayOfIntsElementAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfDoublesElementAddress(
      const LoadArrayOfDoublesElementAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfBoolsElementAddress(
      const LoadArrayOfBoolsElementAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfStringsElementAddress(
      const LoadArrayOfStringsElementAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfArraysElementAddress(
      const LoadArrayOfArraysElementAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfLongsElementAddress(
      const LoadArrayOfLongsElementAddressCmd &cmd) override;
  virtual void VisitLoadArrayOfCharsElementAddress(
      const LoadArrayOfCharsElementAddressCmd &cmd) override;
  virtual void VisitStoreInt(
      const StoreIntCmd &cmd) override;
  virtual void VisitStoreLong(
      const StoreLongCmd &cmd) override;
  virtual void VisitStoreDouble(
      const StoreDoubleCmd &cmd) override;
  virtual void VisitStoreBool(
      const StoreBoolCmd &cmd) override;
  virtual void VisitStoreChar(
      const StoreCharCmd &cmd) override;
  virtual void VisitStoreString(
      const StoreStringCmd &cmd) override;
  virtual void VisitStoreArray(
      const StoreArrayCmd &cmd) override;
  virtual void VisitCastCharToInt(
      const CastCharToIntCmd &cmd) override;
  virtual void VisitCastCharToDouble(
      const CastCharToDoubleCmd &cmd) override;
  virtual void VisitCastCharToLong(
      const CastCharToLongCmd &cmd) override;
  virtual void VisitCastCharToString(
      const CastCharToStringCmd &cmd) override;
  virtual void VisitCastIntToLong(
      const CastIntToLongCmd &cmd) override;
  virtual void VisitCastIntToDouble(
      const CastIntToDoubleCmd &cmd) override;
  virtual void VisitCastLongToDouble(
      const CastLongToDoubleCmd &cmd) override;
  virtual void VisitCallNative(
      const CallNativeCmd &cmd) override;
  virtual void VisitCall(
      const CallCmd &cmd) override;
  virtual void VisitMulChar(
      const MulCharCmd &cmd) override;
  virtual void VisitMulInt(
      const MulIntCmd &cmd) override;
  virtual void VisitMulLong(
      const MulLongCmd &cmd) override;
  virtual void VisitMulDouble(
      const MulDoubleCmd &cmd) override;
  virtual void VisitDivChar(
      const DivCharCmd &cmd) override;
  virtual void VisitDivInt(
      const DivIntCmd &cmd) override;
  virtual void VisitDivDouble(
      const DivDoubleCmd &cmd) override;
  virtual void VisitDivLong(
      const DivLongCmd &cmd) override;
  virtual void VisitSumChar(
      const SumCharCmd &cmd) override;
  virtual void VisitSumLong(
      const SumLongCmd &cmd) override;
  virtual void VisitSumDouble(
      const SumDoubleCmd &cmd) override;
  virtual void VisitSumString(
      const SumStringCmd &cmd) override;
  virtual void VisitSumInt(
      const SumIntCmd &cmd) override;
  virtual void VisitSubChar(
      const SubCharCmd &cmd) override;
  virtual void VisitSubInt(
      const SubIntCmd &cmd) override;
  virtual void VisitSubLong(
      const SubLongCmd &cmd) override;
  virtual void VisitSubDouble(
      const SubDoubleCmd &cmd) override;
  virtual void VisitEqualInt(
      const EqualIntCmd &cmd) override;
  virtual void VisitEqualLong(
      const EqualLongCmd &cmd) override;
  virtual void VisitEqualDouble(
      const EqualDoubleCmd &cmd) override;
  virtual void VisitEqualString(
      const EqualStringCmd &cmd) override;
  virtual void VisitEqualArray(
      const EqualArrayCmd &cmd) override;
  virtual void VisitEqualBool(
      const EqualBoolCmd &cmd) override;
  virtual void VisitEqualChar(
      const EqualCharCmd &cmd) override;
  virtual void VisitNotEqualInt(
      const NotEqualIntCmd &cmd) override;
  virtual void VisitNotEqualLong(
      const NotEqualLongCmd &cmd) override;
  virtual void VisitNotEqualDouble(
      const NotEqualDoubleCmd &cmd) override;
  virtual void VisitNotEqualBool(
      const NotEqualBoolCmd &cmd) override;
  virtual void VisitNotEqualString(
      const NotEqualStringCmd &cmd) override;
  virtual void VisitNotEqualArray(
      const NotEqualArrayCmd &cmd) override;
  virtual void VisitNotEqualChar(
      const NotEqualCharCmd &cmd) override;
  virtual void VisitGreaterChar(
      const GreaterCharCmd &cmd) override;
  virtual void VisitGreaterInt(
      const GreaterIntCmd &cmd) override;
  virtual void VisitGreaterLong(
      const GreaterLongCmd &cmd) override;
  virtual void VisitGreaterDouble(
      const GreaterDoubleCmd &cmd) override;
  virtual void VisitGreaterOrEqualChar(
      const GreaterOrEqualCharCmd &cmd) override;
  virtual void VisitGreaterOrEqualInt(
      const GreaterOrEqualIntCmd &cmd) override;
  virtual void VisitGreaterOrEqualLong(
      const GreaterOrEqualLongCmd &cmd) override;
  virtual void VisitGreaterOrEqualDouble(
      const GreaterOrEqualDoubleCmd &cmd) override;
  virtual void VisitLessChar(
      const LessCharCmd &cmd) override;
  virtual void VisitLessInt(
      const LessIntCmd &cmd) override;
  virtual void VisitLessLong(
      const LessLongCmd &cmd) override;
  virtual void VisitLessDouble(
      const LessDoubleCmd &cmd) override;
  virtual void VisitLessOrEqualInt(
      const LessOrEqualIntCmd &cmd) override;
  virtual void VisitLessOrEqualDouble(
      const LessOrEqualDoubleCmd &cmd) override;
  virtual void VisitLessOrEqualChar(
      const LessOrEqualCharCmd &cmd) override;
  virtual void VisitLessOrEqualLong(
      const LessOrEqualLongCmd &cmd) override;
  virtual void VisitLogicalNegateBool(
      const LogicalNegateBoolCmd &cmd) override;
  virtual void VisitArithmeticNegateInt(
      const ArithmeticNegateIntCmd &cmd) override;
  virtual void VisitArithmeticNegateLong(
      const ArithmeticNegateLongCmd &cmd) override;
  virtual void VisitArithmeticNegateDouble(
      const ArithmeticNegateDoubleCmd &cmd) override;
  virtual void VisitPreDecChar(
      const PreDecCharCmd &cmd) override;
  virtual void VisitPreDecInt(
      const PreDecIntCmd &cmd) override;
  virtual void VisitPreDecLong(
      const PreDecLongCmd &cmd) override;
  virtual void VisitPreDecDouble(
      const PreDecDoubleCmd &cmd) override;
  virtual void VisitPreIncChar(
      const PreIncCharCmd &cmd) override;
  virtual void VisitPreIncInt(
      const PreIncIntCmd &cmd) override;
  virtual void VisitPreIncLong(
      const PreIncLongCmd &cmd) override;
  virtual void VisitPreIncDouble(
      const PreIncDoubleCmd &cmd) override;
  virtual void VisitAnd(
      const AndCmd &cmd) override;
  virtual void VisitOr(
      const OrCmd &cmd) override;

  Exe *exe_;
  const vector<NativeFunc> &native_funcs_;
  Code &cmds_code_;
  CmdReader cmd_reader_;
  DataStorage global_vars_;
  DataStorage local_vars_;
  DataStorage operands_;
  FuncFrames func_frames_;
};

SimpleVM::Impl::Impl(Exe *exe, const vector<NativeFunc> &native_funcs)
    : exe_(exe),
      native_funcs_(native_funcs),
      cmds_code_(exe->GetCmdsCode()),
      global_vars_(exe->GetGlobalVarsSize()) {
  assert(exe_);
  assert(cmds_code_.GetPosition() <= exe_->GetMainCmdsCodeSize());
  cmd_reader_.SetCode(&cmds_code_);
}

void SimpleVM::Impl::Execute() {
  assert(func_frames_.empty());
  const uint32_t main_cmds_end_position = exe_->GetMainCmdsCodeSize();
  const size_t local_vars_start_index = 0;
  const uint32_t return_address = UINT32_C(0);
  const FuncFrame main_func_frame(local_vars_start_index, return_address);
  func_frames_.push_back(main_func_frame);

  while (true) {
    const bool is_in_main_func = func_frames_.size() == 1;

    if (cmds_code_.GetPosition() == main_cmds_end_position && is_in_main_func) {
      break;
    }

    cmd_reader_.GetNextCmd().Accept(this);
  }
}

const DataStorage &SimpleVM::Impl::GetGlobalVars() const {
  return global_vars_;
}

const DataStorage &SimpleVM::Impl::GetLocalVars() const {
  return local_vars_;
}

const DataStorage &SimpleVM::Impl::GetOperands() const {
  return operands_;
}

const SimpleVM::FuncFrames &SimpleVM::Impl::GetFuncFrames() const {
  return func_frames_;
}

void SimpleVM::Impl::VisitCreateGlobalIntVar(
    const CreateGlobalIntVarCmd &cmd) {
  global_vars_.GetInt(cmd.GetVarIndex()) = IntValue();
}

void SimpleVM::Impl::VisitCreateGlobalArrayVar(
    const CreateGlobalArrayVarCmd &cmd) {
  global_vars_.GetArray(cmd.GetVarIndex()) = ArrayValue();
}

void SimpleVM::Impl::VisitCreateGlobalLongVar(
    const CreateGlobalLongVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateGlobalDoubleVar(
    const CreateGlobalDoubleVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateGlobalCharVar(
    const CreateGlobalCharVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateGlobalStringVar(
    const CreateGlobalStringVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateGlobalBoolVar(
    const CreateGlobalBoolVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalIntVar(
    const CreateLocalIntVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalArrayVar(
    const CreateLocalArrayVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalLongVar(
    const CreateLocalLongVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalDoubleVar(
    const CreateLocalDoubleVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalCharVar(
    const CreateLocalCharVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalStringVar(
    const CreateLocalStringVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLocalBoolVar(
    const CreateLocalBoolVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalIntVar(
    const DestroyLocalIntVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalArrayVar(
    const DestroyLocalArrayVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalLongVar(
    const DestroyLocalLongVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalDoubleVar(
    const DestroyLocalDoubleVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalCharVar(
    const DestroyLocalCharVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalStringVar(
    const DestroyLocalStringVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitDestroyLocalBoolVar(
    const DestroyLocalBoolVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadInt(
    const UnloadIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadArray(
    const UnloadArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadLong(
    const UnloadLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadDouble(
    const UnloadDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadChar(
    const UnloadCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadString(
    const UnloadStringCmd&) {assert(false);}

void SimpleVM::Impl::VisitUnloadBool(
    const UnloadBoolCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadIntValue(
    const LoadIntValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLongValue(
    const LoadLongValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadBoolValue(
    const LoadBoolValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadCharValue(
    const LoadCharValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadStringValue(
    const LoadStringValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadDoubleValue(
    const LoadDoubleValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadFuncValue(
    const LoadFuncValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadNativeFuncValue(
    const LoadNativeFuncValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalBoolVar(
    const CreateAndInitGlobalBoolVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalStringVar(
    const CreateAndInitGlobalStringVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalCharVar(
    const CreateAndInitGlobalCharVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalDoubleVar(
    const CreateAndInitGlobalDoubleVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalLongVar(
    const CreateAndInitGlobalLongVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalArrayVar(
    const CreateAndInitGlobalArrayVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitGlobalIntVar(
    const CreateAndInitGlobalIntVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalIntVar(
    const CreateAndInitLocalIntVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalCharVar(
    const CreateAndInitLocalCharVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalStringVar(
    const CreateAndInitLocalStringVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalBoolVar(
    const CreateAndInitLocalBoolVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalArrayVar(
    const CreateAndInitLocalArrayVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalLongVar(
    const CreateAndInitLocalLongVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLocalDoubleVar(
    const CreateAndInitLocalDoubleVarCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateIntArray(
    const CreateIntArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateLongArray(
    const CreateLongArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateBoolArray(
    const CreateBoolArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateCharArray(
    const CreateCharArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateDoubleArray(
    const CreateDoubleArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateStringArray(
    const CreateStringArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitIntArray(
    const CreateAndInitIntArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitLongArray(
    const CreateAndInitLongArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitDoubleArray(
    const CreateAndInitDoubleArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitBoolArray(
    const CreateAndInitBoolArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitCharArray(
    const CreateAndInitCharArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCreateAndInitStringArray(
    const CreateAndInitStringArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitDirectJump(
    const DirectJumpCmd&) {assert(false);}

void SimpleVM::Impl::VisitImplicitJumpIf(
    const ImplicitJumpIfCmd&) {assert(false);}

void SimpleVM::Impl::VisitJumpIfNot(
    const JumpIfNotCmd&) {assert(false);}

void SimpleVM::Impl::VisitImplicitJumpIfNot(
    const ImplicitJumpIfNotCmd&) {assert(false);}

void SimpleVM::Impl::VisitReturn(
    const ReturnCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalIntVarValue(
    const LoadGlobalIntVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalArrayVarValue(
    const LoadGlobalArrayVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalLongVarValue(
    const LoadGlobalLongVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalDoubleVarValue(
    const LoadGlobalDoubleVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalCharVarValue(
    const LoadGlobalCharVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalStringVarValue(
    const LoadGlobalStringVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalBoolVarValue(
    const LoadGlobalBoolVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalIntVarValue(
    const LoadLocalIntVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalArrayVarValue(
    const LoadLocalArrayVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalLongVarValue(
    const LoadLocalLongVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalCharVarValue(
    const LoadLocalCharVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalStringVarValue(
    const LoadLocalStringVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalBoolVarValue(
    const LoadLocalBoolVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalDoubleVarValue(
    const LoadLocalDoubleVarValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadGlobalVarAddress(
    const LoadGlobalVarAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadLocalVarAddress(
    const LoadLocalVarAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfIntsElementValue(
    const LoadArrayOfIntsElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfLongsElementValue(
    const LoadArrayOfLongsElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfDoublesElementValue(
    const LoadArrayOfDoublesElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfCharsElementValue(
    const LoadArrayOfCharsElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfStringsElementValue(
    const LoadArrayOfStringsElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfBoolsElementValue(
    const LoadArrayOfBoolsElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfArraysElementValue(
    const LoadArrayOfArraysElementValueCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfIntsElementAddress(
    const LoadArrayOfIntsElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfDoublesElementAddress(
    const LoadArrayOfDoublesElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfBoolsElementAddress(
    const LoadArrayOfBoolsElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfStringsElementAddress(
    const LoadArrayOfStringsElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfArraysElementAddress(
    const LoadArrayOfArraysElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfLongsElementAddress(
    const LoadArrayOfLongsElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitLoadArrayOfCharsElementAddress(
    const LoadArrayOfCharsElementAddressCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreInt(
    const StoreIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreLong(
    const StoreLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreDouble(
    const StoreDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreBool(
    const StoreBoolCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreChar(
    const StoreCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreString(
    const StoreStringCmd&) {assert(false);}

void SimpleVM::Impl::VisitStoreArray(
    const StoreArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastCharToInt(
    const CastCharToIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastCharToDouble(
    const CastCharToDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastCharToLong(
    const CastCharToLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastCharToString(
    const CastCharToStringCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastIntToLong(
    const CastIntToLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastIntToDouble(
    const CastIntToDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitCastLongToDouble(
    const CastLongToDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitCallNative(
    const CallNativeCmd&) {assert(false);}

void SimpleVM::Impl::VisitCall(
    const CallCmd&) {assert(false);}

void SimpleVM::Impl::VisitMulChar(
    const MulCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitMulInt(
    const MulIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitMulLong(
    const MulLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitMulDouble(
    const MulDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitDivChar(
    const DivCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitDivInt(
    const DivIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitDivDouble(
    const DivDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitDivLong(
    const DivLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitSumChar(
    const SumCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitSumLong(
    const SumLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitSumDouble(
    const SumDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitSumString(
    const SumStringCmd&) {assert(false);}

void SimpleVM::Impl::VisitSumInt(
    const SumIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitSubChar(
    const SubCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitSubInt(
    const SubIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitSubLong(
    const SubLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitSubDouble(
    const SubDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualInt(
    const EqualIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualLong(
    const EqualLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualDouble(
    const EqualDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualString(
    const EqualStringCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualArray(
    const EqualArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualBool(
    const EqualBoolCmd&) {assert(false);}

void SimpleVM::Impl::VisitEqualChar(
    const EqualCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualInt(
    const NotEqualIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualLong(
    const NotEqualLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualDouble(
    const NotEqualDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualBool(
    const NotEqualBoolCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualString(
    const NotEqualStringCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualArray(
    const NotEqualArrayCmd&) {assert(false);}

void SimpleVM::Impl::VisitNotEqualChar(
    const NotEqualCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterChar(
    const GreaterCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterInt(
    const GreaterIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterLong(
    const GreaterLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterDouble(
    const GreaterDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterOrEqualChar(
    const GreaterOrEqualCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterOrEqualInt(
    const GreaterOrEqualIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterOrEqualLong(
    const GreaterOrEqualLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitGreaterOrEqualDouble(
    const GreaterOrEqualDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessChar(
    const LessCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessInt(
    const LessIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessLong(
    const LessLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessDouble(
    const LessDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessOrEqualInt(
    const LessOrEqualIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessOrEqualDouble(
    const LessOrEqualDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessOrEqualChar(
    const LessOrEqualCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitLessOrEqualLong(
    const LessOrEqualLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitLogicalNegateBool(
    const LogicalNegateBoolCmd&) {assert(false);}

void SimpleVM::Impl::VisitArithmeticNegateInt(
    const ArithmeticNegateIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitArithmeticNegateLong(
    const ArithmeticNegateLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitArithmeticNegateDouble(
    const ArithmeticNegateDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreDecChar(
    const PreDecCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreDecInt(
    const PreDecIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreDecLong(
    const PreDecLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreDecDouble(
    const PreDecDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreIncChar(
    const PreIncCharCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreIncInt(
    const PreIncIntCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreIncLong(
    const PreIncLongCmd&) {assert(false);}

void SimpleVM::Impl::VisitPreIncDouble(
    const PreIncDoubleCmd&) {assert(false);}

void SimpleVM::Impl::VisitAnd(
    const AndCmd&) {assert(false);}

void SimpleVM::Impl::VisitOr(
    const OrCmd&) {assert(false);}

SimpleVM::SimpleVM(Exe *exe, const vector<NativeFunc> &native_funcs)
    : impl_(new Impl(exe, native_funcs)) {}

SimpleVM::~SimpleVM() {}

void SimpleVM::Execute() {
  impl_->Execute();
}

const DataStorage &SimpleVM::GetGlobalVars() const {
  return impl_->GetGlobalVars();
}

const DataStorage &SimpleVM::GetLocalVars() const {
  return impl_->GetLocalVars();
}

const DataStorage &SimpleVM::GetOperands() const {
  return impl_->GetOperands();
}

const SimpleVM::FuncFrames &SimpleVM::GetFuncFrames() const {
  return impl_->GetFuncFrames();
}
}
}
