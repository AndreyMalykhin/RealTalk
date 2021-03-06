
#include <cassert>
#include <vector>
#include <memory>
#include <string>
#include "real_talk/util/logger.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/create_and_init_global_var_cmd.h"
#include "real_talk/code/create_local_var_cmd.h"
#include "real_talk/code/create_and_init_local_var_cmd.h"
#include "real_talk/code/create_array_cmd.h"
#include "real_talk/code/create_and_init_array_cmd.h"
#include "real_talk/code/destroy_local_var_cmd.h"
#include "real_talk/code/jump_cmd.h"
#include "real_talk/code/load_value_cmd.h"
#include "real_talk/code/load_local_var_value_cmd.h"
#include "real_talk/code/load_global_var_value_cmd.h"
#include "real_talk/code/load_global_var_address_cmd.h"
#include "real_talk/code/load_local_var_address_cmd.h"
#include "real_talk/code/load_array_element_value_cmd.h"
#include "real_talk/code/load_array_element_address_cmd.h"
#include "real_talk/code/store_cmd.h"
#include "real_talk/code/unload_cmd.h"
#include "real_talk/code/cast_cmd.h"
#include "real_talk/code/mul_cmd.h"
#include "real_talk/code/div_cmd.h"
#include "real_talk/code/sum_cmd.h"
#include "real_talk/code/sub_cmd.h"
#include "real_talk/code/equal_cmd.h"
#include "real_talk/code/not_equal_cmd.h"
#include "real_talk/code/greater_cmd.h"
#include "real_talk/code/greater_or_equal_cmd.h"
#include "real_talk/code/less_cmd.h"
#include "real_talk/code/less_or_equal_cmd.h"
#include "real_talk/code/logical_negate_cmd.h"
#include "real_talk/code/arithmetic_negate_cmd.h"
#include "real_talk/code/pre_dec_cmd.h"
#include "real_talk/code/pre_inc_cmd.h"
#include "real_talk/code/and_cmd.h"
#include "real_talk/code/or_cmd.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/simple_vm.h"

using std::ostream;
using std::vector;
using std::string;
using std::move;
using std::unique_ptr;
using std::exception;
using real_talk::util::g_logger;
using real_talk::code::Exe;
using real_talk::code::Code;
using real_talk::code::CmdReader;
using real_talk::code::CmdVisitor;
using real_talk::code::AndCmd;
using real_talk::code::OrCmd;
using real_talk::code::CreateGlobalVarCmd;
using real_talk::code::CreateGlobalIntVarCmd;
using real_talk::code::CreateGlobalArrayVarCmd;
using real_talk::code::CreateGlobalLongVarCmd;
using real_talk::code::CreateGlobalDoubleVarCmd;
using real_talk::code::CreateGlobalCharVarCmd;
using real_talk::code::CreateGlobalStringVarCmd;
using real_talk::code::CreateGlobalBoolVarCmd;
using real_talk::code::CreateLocalVarCmd;
using real_talk::code::CreateLocalIntVarCmd;
using real_talk::code::CreateLocalArrayVarCmd;
using real_talk::code::CreateLocalLongVarCmd;
using real_talk::code::CreateLocalDoubleVarCmd;
using real_talk::code::CreateLocalCharVarCmd;
using real_talk::code::CreateLocalStringVarCmd;
using real_talk::code::CreateLocalBoolVarCmd;
using real_talk::code::DestroyLocalVarCmd;
using real_talk::code::DestroyLocalIntVarCmd;
using real_talk::code::DestroyLocalLongVarCmd;
using real_talk::code::DestroyLocalDoubleVarCmd;
using real_talk::code::DestroyLocalCharVarCmd;
using real_talk::code::DestroyLocalStringVarCmd;
using real_talk::code::DestroyLocalBoolVarCmd;
using real_talk::code::DestroyLocalArrayVarCmd;
using real_talk::code::DestroyLocalIntArrayVarCmd;
using real_talk::code::DestroyLocalLongArrayVarCmd;
using real_talk::code::DestroyLocalDoubleArrayVarCmd;
using real_talk::code::DestroyLocalCharArrayVarCmd;
using real_talk::code::DestroyLocalStringArrayVarCmd;
using real_talk::code::DestroyLocalBoolArrayVarCmd;
using real_talk::code::UnloadCmd;
using real_talk::code::UnloadIntCmd;
using real_talk::code::UnloadLongCmd;
using real_talk::code::UnloadDoubleCmd;
using real_talk::code::UnloadCharCmd;
using real_talk::code::UnloadStringCmd;
using real_talk::code::UnloadBoolCmd;
using real_talk::code::UnloadArrayCmd;
using real_talk::code::UnloadIntArrayCmd;
using real_talk::code::UnloadLongArrayCmd;
using real_talk::code::UnloadDoubleArrayCmd;
using real_talk::code::UnloadCharArrayCmd;
using real_talk::code::UnloadStringArrayCmd;
using real_talk::code::UnloadBoolArrayCmd;
using real_talk::code::LoadIntValueCmd;
using real_talk::code::LoadLongValueCmd;
using real_talk::code::LoadBoolValueCmd;
using real_talk::code::LoadCharValueCmd;
using real_talk::code::LoadStringValueCmd;
using real_talk::code::LoadDoubleValueCmd;
using real_talk::code::LoadFuncValueCmd;
using real_talk::code::LoadNativeFuncValueCmd;
using real_talk::code::CreateAndInitGlobalVarCmd;
using real_talk::code::CreateAndInitGlobalBoolVarCmd;
using real_talk::code::CreateAndInitGlobalStringVarCmd;
using real_talk::code::CreateAndInitGlobalCharVarCmd;
using real_talk::code::CreateAndInitGlobalDoubleVarCmd;
using real_talk::code::CreateAndInitGlobalLongVarCmd;
using real_talk::code::CreateAndInitGlobalIntVarCmd;
using real_talk::code::CreateAndInitGlobalArrayVarCmd;
using real_talk::code::CreateAndInitGlobalBoolArrayVarCmd;
using real_talk::code::CreateAndInitGlobalStringArrayVarCmd;
using real_talk::code::CreateAndInitGlobalCharArrayVarCmd;
using real_talk::code::CreateAndInitGlobalDoubleArrayVarCmd;
using real_talk::code::CreateAndInitGlobalLongArrayVarCmd;
using real_talk::code::CreateAndInitGlobalIntArrayVarCmd;
using real_talk::code::CreateAndInitLocalVarCmd;
using real_talk::code::CreateAndInitLocalIntVarCmd;
using real_talk::code::CreateAndInitLocalCharVarCmd;
using real_talk::code::CreateAndInitLocalStringVarCmd;
using real_talk::code::CreateAndInitLocalBoolVarCmd;
using real_talk::code::CreateAndInitLocalLongVarCmd;
using real_talk::code::CreateAndInitLocalDoubleVarCmd;
using real_talk::code::CreateAndInitLocalArrayVarCmd;
using real_talk::code::CreateAndInitLocalIntArrayVarCmd;
using real_talk::code::CreateAndInitLocalCharArrayVarCmd;
using real_talk::code::CreateAndInitLocalStringArrayVarCmd;
using real_talk::code::CreateAndInitLocalBoolArrayVarCmd;
using real_talk::code::CreateAndInitLocalLongArrayVarCmd;
using real_talk::code::CreateAndInitLocalDoubleArrayVarCmd;
using real_talk::code::CreateArrayCmd;
using real_talk::code::CreateIntArrayCmd;
using real_talk::code::CreateLongArrayCmd;
using real_talk::code::CreateBoolArrayCmd;
using real_talk::code::CreateCharArrayCmd;
using real_talk::code::CreateStringArrayCmd;
using real_talk::code::CreateDoubleArrayCmd;
using real_talk::code::CreateAndInitArrayCmd;
using real_talk::code::CreateAndInitIntArrayCmd;
using real_talk::code::CreateAndInitLongArrayCmd;
using real_talk::code::CreateAndInitDoubleArrayCmd;
using real_talk::code::CreateAndInitBoolArrayCmd;
using real_talk::code::CreateAndInitCharArrayCmd;
using real_talk::code::CreateAndInitStringArrayCmd;
using real_talk::code::JumpCmd;
using real_talk::code::DirectJumpCmd;
using real_talk::code::ImplicitJumpIfCmd;
using real_talk::code::JumpIfNotCmd;
using real_talk::code::ImplicitJumpIfNotCmd;
using real_talk::code::ReturnCmd;
using real_talk::code::LoadGlobalVarValueCmd;
using real_talk::code::LoadGlobalIntVarValueCmd;
using real_talk::code::LoadGlobalLongVarValueCmd;
using real_talk::code::LoadGlobalDoubleVarValueCmd;
using real_talk::code::LoadGlobalCharVarValueCmd;
using real_talk::code::LoadGlobalStringVarValueCmd;
using real_talk::code::LoadGlobalBoolVarValueCmd;
using real_talk::code::LoadGlobalIntArrayVarValueCmd;
using real_talk::code::LoadGlobalLongArrayVarValueCmd;
using real_talk::code::LoadGlobalCharArrayVarValueCmd;
using real_talk::code::LoadGlobalStringArrayVarValueCmd;
using real_talk::code::LoadGlobalBoolArrayVarValueCmd;
using real_talk::code::LoadGlobalDoubleArrayVarValueCmd;
using real_talk::code::LoadLocalVarValueCmd;
using real_talk::code::LoadLocalIntVarValueCmd;
using real_talk::code::LoadLocalLongVarValueCmd;
using real_talk::code::LoadLocalCharVarValueCmd;
using real_talk::code::LoadLocalStringVarValueCmd;
using real_talk::code::LoadLocalBoolVarValueCmd;
using real_talk::code::LoadLocalDoubleVarValueCmd;
using real_talk::code::LoadLocalIntArrayVarValueCmd;
using real_talk::code::LoadLocalLongArrayVarValueCmd;
using real_talk::code::LoadLocalCharArrayVarValueCmd;
using real_talk::code::LoadLocalStringArrayVarValueCmd;
using real_talk::code::LoadLocalBoolArrayVarValueCmd;
using real_talk::code::LoadLocalDoubleArrayVarValueCmd;
using real_talk::code::LoadGlobalVarAddressCmd;
using real_talk::code::LoadLocalVarAddressCmd;
using real_talk::code::LoadArrayElementValueCmd;
using real_talk::code::LoadIntArrayElementValueCmd;
using real_talk::code::LoadLongArrayElementValueCmd;
using real_talk::code::LoadDoubleArrayElementValueCmd;
using real_talk::code::LoadCharArrayElementValueCmd;
using real_talk::code::LoadStringArrayElementValueCmd;
using real_talk::code::LoadBoolArrayElementValueCmd;
using real_talk::code::LoadArrayElementAddressCmd;
using real_talk::code::LoadIntArrayElementAddressCmd;
using real_talk::code::LoadDoubleArrayElementAddressCmd;
using real_talk::code::LoadBoolArrayElementAddressCmd;
using real_talk::code::LoadStringArrayElementAddressCmd;
using real_talk::code::LoadLongArrayElementAddressCmd;
using real_talk::code::LoadCharArrayElementAddressCmd;
using real_talk::code::StoreCmd;
using real_talk::code::StoreIntCmd;
using real_talk::code::StoreLongCmd;
using real_talk::code::StoreDoubleCmd;
using real_talk::code::StoreBoolCmd;
using real_talk::code::StoreCharCmd;
using real_talk::code::StoreStringCmd;
using real_talk::code::StoreArrayCmd;
using real_talk::code::StoreIntArrayCmd;
using real_talk::code::StoreLongArrayCmd;
using real_talk::code::StoreDoubleArrayCmd;
using real_talk::code::StoreBoolArrayCmd;
using real_talk::code::StoreCharArrayCmd;
using real_talk::code::StoreStringArrayCmd;
using real_talk::code::CastCmd;
using real_talk::code::CastCharToIntCmd;
using real_talk::code::CastCharToDoubleCmd;
using real_talk::code::CastCharToLongCmd;
using real_talk::code::CastCharToStringCmd;
using real_talk::code::CastIntToLongCmd;
using real_talk::code::CastIntToDoubleCmd;
using real_talk::code::CastLongToDoubleCmd;
using real_talk::code::CallNativeCmd;
using real_talk::code::CallCmd;
using real_talk::code::MulCmd;
using real_talk::code::MulCharCmd;
using real_talk::code::MulIntCmd;
using real_talk::code::MulLongCmd;
using real_talk::code::MulDoubleCmd;
using real_talk::code::DivCmd;
using real_talk::code::DivCharCmd;
using real_talk::code::DivIntCmd;
using real_talk::code::DivDoubleCmd;
using real_talk::code::DivLongCmd;
using real_talk::code::SumCmd;
using real_talk::code::SumCharCmd;
using real_talk::code::SumLongCmd;
using real_talk::code::SumDoubleCmd;
using real_talk::code::SumStringCmd;
using real_talk::code::SumIntCmd;
using real_talk::code::SubCmd;
using real_talk::code::SubCharCmd;
using real_talk::code::SubIntCmd;
using real_talk::code::SubLongCmd;
using real_talk::code::SubDoubleCmd;
using real_talk::code::EqualCmd;
using real_talk::code::EqualIntCmd;
using real_talk::code::EqualLongCmd;
using real_talk::code::EqualDoubleCmd;
using real_talk::code::EqualStringCmd;
using real_talk::code::EqualBoolCmd;
using real_talk::code::EqualCharCmd;
using real_talk::code::EqualArrayCmd;
using real_talk::code::EqualIntArrayCmd;
using real_talk::code::EqualLongArrayCmd;
using real_talk::code::EqualDoubleArrayCmd;
using real_talk::code::EqualStringArrayCmd;
using real_talk::code::EqualBoolArrayCmd;
using real_talk::code::EqualCharArrayCmd;
using real_talk::code::NotEqualCmd;
using real_talk::code::NotEqualIntCmd;
using real_talk::code::NotEqualLongCmd;
using real_talk::code::NotEqualDoubleCmd;
using real_talk::code::NotEqualBoolCmd;
using real_talk::code::NotEqualStringCmd;
using real_talk::code::NotEqualCharCmd;
using real_talk::code::NotEqualArrayCmd;
using real_talk::code::NotEqualIntArrayCmd;
using real_talk::code::NotEqualLongArrayCmd;
using real_talk::code::NotEqualDoubleArrayCmd;
using real_talk::code::NotEqualStringArrayCmd;
using real_talk::code::NotEqualBoolArrayCmd;
using real_talk::code::NotEqualCharArrayCmd;
using real_talk::code::GreaterCmd;
using real_talk::code::GreaterCharCmd;
using real_talk::code::GreaterIntCmd;
using real_talk::code::GreaterLongCmd;
using real_talk::code::GreaterDoubleCmd;
using real_talk::code::GreaterOrEqualCmd;
using real_talk::code::GreaterOrEqualCharCmd;
using real_talk::code::GreaterOrEqualIntCmd;
using real_talk::code::GreaterOrEqualLongCmd;
using real_talk::code::GreaterOrEqualDoubleCmd;
using real_talk::code::LessCmd;
using real_talk::code::LessCharCmd;
using real_talk::code::LessIntCmd;
using real_talk::code::LessLongCmd;
using real_talk::code::LessDoubleCmd;
using real_talk::code::LessOrEqualCmd;
using real_talk::code::LessOrEqualIntCmd;
using real_talk::code::LessOrEqualDoubleCmd;
using real_talk::code::LessOrEqualCharCmd;
using real_talk::code::LessOrEqualLongCmd;
using real_talk::code::LogicalNegateBoolCmd;
using real_talk::code::ArithmeticNegateCmd;
using real_talk::code::ArithmeticNegateIntCmd;
using real_talk::code::ArithmeticNegateLongCmd;
using real_talk::code::ArithmeticNegateDoubleCmd;
using real_talk::code::PreDecCmd;
using real_talk::code::PreDecCharCmd;
using real_talk::code::PreDecIntCmd;
using real_talk::code::PreDecLongCmd;
using real_talk::code::PreDecDoubleCmd;
using real_talk::code::PreIncCmd;
using real_talk::code::PreIncCharCmd;
using real_talk::code::PreIncIntCmd;
using real_talk::code::PreIncLongCmd;
using real_talk::code::PreIncDoubleCmd;
using real_talk::code::Cmd;

namespace real_talk {
namespace vm {

class SimpleVM::Impl: private CmdVisitor {
 public:
  Impl(Exe *exe, const vector<NativeFuncValue> &native_funcs, SimpleVM *vm);
  void Execute();
  const DataStorage &GetGlobalVars() const noexcept;
  const DataStorage &GetLocalVars() const noexcept;
  const DataStorage &GetOperands() const noexcept;
  DataStorage &GetOperands() noexcept;
  const FuncFrames &GetFuncFrames() const noexcept;

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
  virtual void VisitDestroyLocalIntArrayVar(
      const DestroyLocalIntArrayVarCmd &cmd) override;
  virtual void VisitDestroyLocalLongArrayVar(
      const DestroyLocalLongArrayVarCmd &cmd) override;
  virtual void VisitDestroyLocalDoubleArrayVar(
      const DestroyLocalDoubleArrayVarCmd &cmd) override;
  virtual void VisitDestroyLocalCharArrayVar(
      const DestroyLocalCharArrayVarCmd &cmd) override;
  virtual void VisitDestroyLocalStringArrayVar(
      const DestroyLocalStringArrayVarCmd &cmd) override;
  virtual void VisitDestroyLocalBoolArrayVar(
      const DestroyLocalBoolArrayVarCmd &cmd) override;
  virtual void VisitUnloadInt(
      const UnloadIntCmd &cmd) override;
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
  virtual void VisitUnloadIntArray(
      const UnloadIntArrayCmd &cmd) override;
  virtual void VisitUnloadLongArray(
      const UnloadLongArrayCmd &cmd) override;
  virtual void VisitUnloadDoubleArray(
      const UnloadDoubleArrayCmd &cmd) override;
  virtual void VisitUnloadCharArray(
      const UnloadCharArrayCmd &cmd) override;
  virtual void VisitUnloadStringArray(
      const UnloadStringArrayCmd &cmd) override;
  virtual void VisitUnloadBoolArray(
      const UnloadBoolArrayCmd &cmd) override;
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
  virtual void VisitCreateAndInitGlobalIntVar(
      const CreateAndInitGlobalIntVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalBoolArrayVar(
      const CreateAndInitGlobalBoolArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalStringArrayVar(
      const CreateAndInitGlobalStringArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalCharArrayVar(
      const CreateAndInitGlobalCharArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalDoubleArrayVar(
      const CreateAndInitGlobalDoubleArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalLongArrayVar(
      const CreateAndInitGlobalLongArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitGlobalIntArrayVar(
      const CreateAndInitGlobalIntArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalIntVar(
      const CreateAndInitLocalIntVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalCharVar(
      const CreateAndInitLocalCharVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalStringVar(
      const CreateAndInitLocalStringVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalBoolVar(
      const CreateAndInitLocalBoolVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalLongVar(
      const CreateAndInitLocalLongVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalDoubleVar(
      const CreateAndInitLocalDoubleVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalIntArrayVar(
      const CreateAndInitLocalIntArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalCharArrayVar(
      const CreateAndInitLocalCharArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalStringArrayVar(
      const CreateAndInitLocalStringArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalBoolArrayVar(
      const CreateAndInitLocalBoolArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalLongArrayVar(
      const CreateAndInitLocalLongArrayVarCmd &cmd) override;
  virtual void VisitCreateAndInitLocalDoubleArrayVar(
      const CreateAndInitLocalDoubleArrayVarCmd &cmd) override;
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
  virtual void VisitLoadGlobalIntArrayVarValue(
      const LoadGlobalIntArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalLongArrayVarValue(
      const LoadGlobalLongArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalDoubleArrayVarValue(
      const LoadGlobalDoubleArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalCharArrayVarValue(
      const LoadGlobalCharArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalBoolArrayVarValue(
      const LoadGlobalBoolArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalStringArrayVarValue(
      const LoadGlobalStringArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalIntVarValue(
      const LoadLocalIntVarValueCmd &cmd) override;
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
  virtual void VisitLoadLocalIntArrayVarValue(
      const LoadLocalIntArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalLongArrayVarValue(
      const LoadLocalLongArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalDoubleArrayVarValue(
      const LoadLocalDoubleArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalCharArrayVarValue(
      const LoadLocalCharArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalBoolArrayVarValue(
      const LoadLocalBoolArrayVarValueCmd &cmd) override;
  virtual void VisitLoadLocalStringArrayVarValue(
      const LoadLocalStringArrayVarValueCmd &cmd) override;
  virtual void VisitLoadGlobalVarAddress(
      const LoadGlobalVarAddressCmd &cmd) override;
  virtual void VisitLoadLocalVarAddress(
      const LoadLocalVarAddressCmd &cmd) override;
  virtual void VisitLoadIntArrayElementValue(
      const LoadIntArrayElementValueCmd &cmd) override;
  virtual void VisitLoadLongArrayElementValue(
      const LoadLongArrayElementValueCmd &cmd) override;
  virtual void VisitLoadDoubleArrayElementValue(
      const LoadDoubleArrayElementValueCmd &cmd) override;
  virtual void VisitLoadCharArrayElementValue(
      const LoadCharArrayElementValueCmd &cmd) override;
  virtual void VisitLoadStringArrayElementValue(
      const LoadStringArrayElementValueCmd &cmd) override;
  virtual void VisitLoadBoolArrayElementValue(
      const LoadBoolArrayElementValueCmd &cmd) override;
  virtual void VisitLoadIntArrayElementAddress(
      const LoadIntArrayElementAddressCmd &cmd) override;
  virtual void VisitLoadDoubleArrayElementAddress(
      const LoadDoubleArrayElementAddressCmd &cmd) override;
  virtual void VisitLoadBoolArrayElementAddress(
      const LoadBoolArrayElementAddressCmd &cmd) override;
  virtual void VisitLoadStringArrayElementAddress(
      const LoadStringArrayElementAddressCmd &cmd) override;
  virtual void VisitLoadLongArrayElementAddress(
      const LoadLongArrayElementAddressCmd &cmd) override;
  virtual void VisitLoadCharArrayElementAddress(
      const LoadCharArrayElementAddressCmd &cmd) override;
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
  virtual void VisitStoreIntArray(
      const StoreIntArrayCmd &cmd) override;
  virtual void VisitStoreLongArray(
      const StoreLongArrayCmd &cmd) override;
  virtual void VisitStoreDoubleArray(
      const StoreDoubleArrayCmd &cmd) override;
  virtual void VisitStoreBoolArray(
      const StoreBoolArrayCmd &cmd) override;
  virtual void VisitStoreCharArray(
      const StoreCharArrayCmd &cmd) override;
  virtual void VisitStoreStringArray(
      const StoreStringArrayCmd &cmd) override;
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
  virtual void VisitEqualBool(
      const EqualBoolCmd &cmd) override;
  virtual void VisitEqualChar(
      const EqualCharCmd &cmd) override;
  virtual void VisitEqualIntArray(
      const EqualIntArrayCmd &cmd) override;
  virtual void VisitEqualLongArray(
      const EqualLongArrayCmd &cmd) override;
  virtual void VisitEqualDoubleArray(
      const EqualDoubleArrayCmd &cmd) override;
  virtual void VisitEqualStringArray(
      const EqualStringArrayCmd &cmd) override;
  virtual void VisitEqualBoolArray(
      const EqualBoolArrayCmd &cmd) override;
  virtual void VisitEqualCharArray(
      const EqualCharArrayCmd &cmd) override;
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
  virtual void VisitNotEqualChar(
      const NotEqualCharCmd &cmd) override;
  virtual void VisitNotEqualIntArray(
      const NotEqualIntArrayCmd &cmd) override;
  virtual void VisitNotEqualLongArray(
      const NotEqualLongArrayCmd &cmd) override;
  virtual void VisitNotEqualDoubleArray(
      const NotEqualDoubleArrayCmd &cmd) override;
  virtual void VisitNotEqualStringArray(
      const NotEqualStringArrayCmd &cmd) override;
  virtual void VisitNotEqualBoolArray(
      const NotEqualBoolArrayCmd &cmd) override;
  virtual void VisitNotEqualCharArray(
      const NotEqualCharArrayCmd &cmd) override;
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
  void Jump(const JumpCmd &cmd) noexcept;
  vector<size_t> GetArrayDimensions(const CreateArrayCmd &cmd);
  template<typename T> void VisitCreateArray(const CreateArrayCmd &cmd);
  template<typename T> void VisitCreateAndInitArray(
      const CreateAndInitArrayCmd &cmd);
  template<typename T> void VisitCreateAndInitGlobalArrayVar(
      const CreateAndInitGlobalArrayVarCmd &cmd);
  template<typename T> void VisitCreateAndInitGlobalVar(
      const CreateAndInitGlobalVarCmd &cmd);
  template<typename T> void VisitCreateGlobalVar(
      const CreateGlobalVarCmd &cmd);
  template<typename T> void VisitCreateLocalVar(
      const CreateLocalVarCmd &cmd);
  template<typename T> void VisitCreateAndInitLocalVar(
      const CreateAndInitLocalVarCmd &cmd);
  template<typename T> void VisitCreateAndInitLocalArrayVar(
      const CreateAndInitLocalArrayVarCmd &cmd);
  template<typename T> void VisitDestroyLocalVar(
      const DestroyLocalVarCmd &cmd) noexcept;
  template<typename T> void VisitDestroyLocalArrayVar(
      const DestroyLocalArrayVarCmd &cmd) noexcept;
  template<typename T> void VisitLoadLocalVarValue(
      const LoadLocalVarValueCmd &cmd);
  template<typename T> void VisitLoadLocalArrayVarValue(
      const LoadLocalVarValueCmd &cmd);
  template<typename T> void VisitUnload(const UnloadCmd &cmd) noexcept;
  template<typename T> void VisitUnloadArray(const UnloadArrayCmd &cmd)
      noexcept;
  template<typename T> void VisitLoadGlobalVarValue(
      const LoadGlobalVarValueCmd &cmd);
  template<typename T> void VisitLoadGlobalArrayVarValue(
      const LoadGlobalVarValueCmd &cmd);
  template<typename T> void VisitLoadArrayElementValue(
      const LoadArrayElementValueCmd &cmd);
  template<typename T> void VisitLoadArrayElementAddress(
      const LoadArrayElementAddressCmd &cmd);
  template<typename T> void VisitStore(const StoreCmd &cmd) noexcept;
  template<typename T> void VisitStoreArray(const StoreArrayCmd &cmd) noexcept;
  template<typename TDest, typename TSource> void VisitCast(const CastCmd &cmd);
  template<typename T> void VisitMul(const MulCmd &cmd);
  template<typename T> void VisitDiv(const DivCmd &cmd);
  template<typename T> void VisitSum(const SumCmd &cmd);
  template<typename T> void VisitSub(const SubCmd &cmd);
  template<typename T> void VisitEqual(const EqualCmd &cmd);
  template<typename T> void VisitEqualArray(const EqualArrayCmd &cmd);
  template<typename T> void VisitNotEqual(const NotEqualCmd &cmd);
  template<typename T> void VisitNotEqualArray(const NotEqualArrayCmd &cmd);
  template<typename T> void VisitGreater(const GreaterCmd &cmd);
  template<typename T> void VisitGreaterOrEqual(const GreaterOrEqualCmd &cmd);
  template<typename T> void VisitLess(const LessCmd &cmd);
  template<typename T> void VisitLessOrEqual(const LessOrEqualCmd &cmd);
  template<typename T> void VisitArithmeticNegate(
      const ArithmeticNegateCmd &cmd);
  template<typename T> void VisitPreDec(const PreDecCmd &cmd);
  template<typename T> void VisitPreInc(const PreIncCmd &cmd);
  template<typename T> size_t GetLocalVarIndex(const T &cmd) const noexcept;

  Exe *exe_;
  const vector<NativeFuncValue> &native_funcs_;
  SimpleVM *vm_;
  Code &cmds_code_;
  CmdReader cmd_reader_;
  DataStorage global_vars_;
  DataStorage local_vars_;
  DataStorage operands_;
  FuncFrames func_frames_;
};

SimpleVM::SimpleVM(Exe *exe, const vector<NativeFuncValue> &native_funcs)
    : impl_(new Impl(exe, native_funcs, this)) {}

SimpleVM::~SimpleVM() {}

void SimpleVM::Execute() {impl_->Execute();}

const DataStorage &SimpleVM::GetGlobalVars() const noexcept {
  return impl_->GetGlobalVars();
}

const DataStorage &SimpleVM::GetLocalVars() const noexcept {
  return impl_->GetLocalVars();
}

const DataStorage &SimpleVM::GetOperands() const noexcept {
  return impl_->GetOperands();
}

DataStorage &SimpleVM::GetOperands() noexcept {
  return impl_->GetOperands();
}

const SimpleVM::FuncFrames &SimpleVM::GetFuncFrames() const noexcept {
  return impl_->GetFuncFrames();
}

SimpleVM::Impl::Impl(
    Exe *exe, const vector<NativeFuncValue> &native_funcs, SimpleVM *vm)
    : exe_(exe),
      native_funcs_(native_funcs),
      vm_(vm),
      cmds_code_(exe->GetCmdsCode()),
      global_vars_(exe->GetGlobalVarsSize(), exe->GetGlobalVarsSize()) {
  assert(exe_);
  assert(vm_);
  assert(cmds_code_.GetPosition() == UINT32_C(0));
  cmd_reader_.SetCode(&cmds_code_);
}

void SimpleVM::Impl::Execute() {
  assert(func_frames_.empty());
  g_logger.Log([this](ostream *stream) {
      *stream << "[input]\n\n" << *(this->exe_) << "\n\n";
    });
  const uint32_t main_cmds_end_position = exe_->GetMainCmdsCodeSize();
  const size_t local_vars_start_index = 0;
  const uint32_t return_address = UINT32_C(0);
  const FuncFrame main_func_frame(local_vars_start_index, return_address);
  func_frames_.push_back(main_func_frame);

  while (cmds_code_.GetPosition() != main_cmds_end_position
         || func_frames_.size() != 1) {
    const uint32_t code_position = cmds_code_.GetPosition();

    try {
      const Cmd &cmd = cmd_reader_.GetNextCmd();
      g_logger.Log([code_position, &cmd](ostream *stream) {
          *stream << code_position << ' ' << cmd << '\n';
        });
      cmd.Accept(this);
    } catch (const ExecutionError&) {
      throw;
    } catch (const exception &error) {
      throw ExecutionError(error.what());
    }
  }

  assert(func_frames_.size() == 1);
}

const DataStorage &SimpleVM::Impl::GetGlobalVars() const noexcept {
  return global_vars_;
}

const DataStorage &SimpleVM::Impl::GetLocalVars() const noexcept {
  return local_vars_;
}

const DataStorage &SimpleVM::Impl::GetOperands() const noexcept {
  return operands_;
}

DataStorage &SimpleVM::Impl::GetOperands() noexcept {
  return operands_;
}

const SimpleVM::FuncFrames &SimpleVM::Impl::GetFuncFrames() const noexcept {
  return func_frames_;
}

void SimpleVM::Impl::VisitCreateGlobalIntVar(
    const CreateGlobalIntVarCmd &cmd) {
  VisitCreateGlobalVar<IntValue>(cmd);
}

void SimpleVM::Impl::VisitCreateGlobalArrayVar(
    const CreateGlobalArrayVarCmd &cmd) {
  VisitCreateGlobalVar< ArrayValue<IntValue> >(cmd);
}

void SimpleVM::Impl::VisitCreateGlobalLongVar(
    const CreateGlobalLongVarCmd &cmd) {
  VisitCreateGlobalVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateGlobalDoubleVar(
    const CreateGlobalDoubleVarCmd &cmd) {
  VisitCreateGlobalVar<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitCreateGlobalCharVar(
    const CreateGlobalCharVarCmd &cmd) {
  VisitCreateGlobalVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateGlobalStringVar(
    const CreateGlobalStringVarCmd &cmd) {
  VisitCreateGlobalVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitCreateGlobalBoolVar(
    const CreateGlobalBoolVarCmd &cmd) {
  VisitCreateGlobalVar<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateGlobalVar(
    const CreateGlobalVarCmd &cmd) {
  global_vars_.Create<T>(cmd.GetVarIndex());
}

void SimpleVM::Impl::VisitCreateLocalIntVar(
    const CreateLocalIntVarCmd &cmd) {
  VisitCreateLocalVar<IntValue>(cmd);
}

void SimpleVM::Impl::VisitCreateLocalArrayVar(
    const CreateLocalArrayVarCmd &cmd) {
  VisitCreateLocalVar< ArrayValue<IntValue> >(cmd);
}

void SimpleVM::Impl::VisitCreateLocalLongVar(
    const CreateLocalLongVarCmd &cmd) {
  VisitCreateLocalVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateLocalDoubleVar(
    const CreateLocalDoubleVarCmd &cmd) {
  VisitCreateLocalVar<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitCreateLocalCharVar(
    const CreateLocalCharVarCmd &cmd) {
  VisitCreateLocalVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateLocalStringVar(
    const CreateLocalStringVarCmd &cmd) {
  VisitCreateLocalVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitCreateLocalBoolVar(
    const CreateLocalBoolVarCmd &cmd) {
  VisitCreateLocalVar<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateLocalVar(
    const CreateLocalVarCmd&) {
  local_vars_.Push(T());
}

void SimpleVM::Impl::VisitDestroyLocalIntVar(
    const DestroyLocalIntVarCmd &cmd) {
  VisitDestroyLocalVar<IntValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalLongVar(
    const DestroyLocalLongVarCmd &cmd) {
  VisitDestroyLocalVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalDoubleVar(
    const DestroyLocalDoubleVarCmd &cmd) {
  VisitDestroyLocalVar<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalCharVar(
    const DestroyLocalCharVarCmd &cmd) {
  VisitDestroyLocalVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalStringVar(
    const DestroyLocalStringVarCmd &cmd) {
  VisitDestroyLocalVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalBoolVar(
    const DestroyLocalBoolVarCmd &cmd) {
  VisitDestroyLocalVar<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitDestroyLocalVar(
    const DestroyLocalVarCmd&) noexcept {
  local_vars_.Pop<T>();
}

void SimpleVM::Impl::VisitDestroyLocalIntArrayVar(
    const DestroyLocalIntArrayVarCmd &cmd) {
  VisitDestroyLocalArrayVar<IntValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalLongArrayVar(
    const DestroyLocalLongArrayVarCmd &cmd) {
  VisitDestroyLocalArrayVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalDoubleArrayVar(
    const DestroyLocalDoubleArrayVarCmd &cmd) {
  VisitDestroyLocalArrayVar<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalCharArrayVar(
    const DestroyLocalCharArrayVarCmd &cmd) {
  VisitDestroyLocalArrayVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalStringArrayVar(
    const DestroyLocalStringArrayVarCmd &cmd) {
  VisitDestroyLocalArrayVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitDestroyLocalBoolArrayVar(
    const DestroyLocalBoolArrayVarCmd &cmd) {
  VisitDestroyLocalArrayVar<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitDestroyLocalArrayVar(
    const DestroyLocalArrayVarCmd &cmd) noexcept {
  local_vars_.Pop< ArrayValue<T> >().Destroy(cmd.GetDimensionsCount());
}

void SimpleVM::Impl::VisitUnloadInt(const UnloadIntCmd &cmd) {
  VisitUnload<IntValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadLong(const UnloadLongCmd &cmd) {
  VisitUnload<LongValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadDouble(const UnloadDoubleCmd &cmd) {
  VisitUnload<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadChar(const UnloadCharCmd &cmd) {
  VisitUnload<CharValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadString(const UnloadStringCmd &cmd) {
  VisitUnload<StringValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadBool(const UnloadBoolCmd &cmd) {
  VisitUnload<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitUnload(const UnloadCmd&)
    noexcept {
  operands_.Pop<T>();
}

void SimpleVM::Impl::VisitUnloadIntArray(const UnloadIntArrayCmd &cmd) {
  VisitUnloadArray<IntValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadLongArray(const UnloadLongArrayCmd &cmd) {
  VisitUnloadArray<LongValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadDoubleArray(const UnloadDoubleArrayCmd &cmd) {
  VisitUnloadArray<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadCharArray(const UnloadCharArrayCmd &cmd) {
  VisitUnloadArray<CharValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadStringArray(const UnloadStringArrayCmd &cmd) {
  VisitUnloadArray<StringValue>(cmd);
}

void SimpleVM::Impl::VisitUnloadBoolArray(const UnloadBoolArrayCmd &cmd) {
  VisitUnloadArray<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitUnloadArray(
    const UnloadArrayCmd &cmd) noexcept {
  operands_.Pop< ArrayValue<T> >().Destroy(cmd.GetDimensionsCount());
}

void SimpleVM::Impl::VisitLoadIntValue(const LoadIntValueCmd &cmd) {
  operands_.Push(IntValue(cmd.GetValue()));
}

void SimpleVM::Impl::VisitLoadLongValue(const LoadLongValueCmd &cmd) {
  operands_.Push(LongValue(cmd.GetValue()));
}

void SimpleVM::Impl::VisitLoadBoolValue(const LoadBoolValueCmd &cmd) {
  operands_.Push(BoolValue(cmd.GetValue()));
}

void SimpleVM::Impl::VisitLoadCharValue(const LoadCharValueCmd &cmd) {
  operands_.Push(CharValue(cmd.GetValue()));
}

void SimpleVM::Impl::VisitLoadStringValue(const LoadStringValueCmd &cmd) {
  operands_.Push(StringValue(cmd.GetValue()));
}

void SimpleVM::Impl::VisitLoadDoubleValue(const LoadDoubleValueCmd &cmd) {
  operands_.Push(DoubleValue(cmd.GetValue()));
}

void SimpleVM::Impl::VisitLoadFuncValue(const LoadFuncValueCmd &cmd) {
  operands_.Push(FuncValue(cmd.GetAddress()));
}

void SimpleVM::Impl::VisitLoadNativeFuncValue(
    const LoadNativeFuncValueCmd &cmd) {
  assert(cmd.GetFuncIndex() < native_funcs_.size());
  operands_.Push(native_funcs_[cmd.GetFuncIndex()]);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalBoolVar(
    const CreateAndInitGlobalBoolVarCmd &cmd) {
  VisitCreateAndInitGlobalVar<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalStringVar(
    const CreateAndInitGlobalStringVarCmd &cmd) {
  VisitCreateAndInitGlobalVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalCharVar(
    const CreateAndInitGlobalCharVarCmd &cmd) {
  VisitCreateAndInitGlobalVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalDoubleVar(
    const CreateAndInitGlobalDoubleVarCmd &cmd) {
  VisitCreateAndInitGlobalVar<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalLongVar(
    const CreateAndInitGlobalLongVarCmd &cmd) {
  VisitCreateAndInitGlobalVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalIntVar(
    const CreateAndInitGlobalIntVarCmd &cmd) {
  VisitCreateAndInitGlobalVar<IntValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateAndInitGlobalVar(
    const CreateAndInitGlobalVarCmd &cmd) {
  global_vars_.Create(cmd.GetVarIndex(), operands_.Pop<T>());
}

void SimpleVM::Impl::VisitCreateAndInitGlobalBoolArrayVar(
    const CreateAndInitGlobalBoolArrayVarCmd &cmd) {
  VisitCreateAndInitGlobalArrayVar<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalStringArrayVar(
    const CreateAndInitGlobalStringArrayVarCmd &cmd) {
  VisitCreateAndInitGlobalArrayVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalCharArrayVar(
    const CreateAndInitGlobalCharArrayVarCmd &cmd) {
  VisitCreateAndInitGlobalArrayVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalDoubleArrayVar(
    const CreateAndInitGlobalDoubleArrayVarCmd &cmd) {
  VisitCreateAndInitGlobalArrayVar<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalLongArrayVar(
    const CreateAndInitGlobalLongArrayVarCmd &cmd) {
  VisitCreateAndInitGlobalArrayVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitGlobalIntArrayVar(
    const CreateAndInitGlobalIntArrayVarCmd &cmd) {
  VisitCreateAndInitGlobalArrayVar<IntValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateAndInitGlobalArrayVar(
    const CreateAndInitGlobalArrayVarCmd &cmd) {
  global_vars_.Create(
      cmd.GetVarIndex(), operands_.Pop< ArrayValue<T> >());
}

void SimpleVM::Impl::VisitCreateAndInitLocalIntVar(
    const CreateAndInitLocalIntVarCmd &cmd) {
  VisitCreateAndInitLocalVar<IntValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalCharVar(
    const CreateAndInitLocalCharVarCmd &cmd) {
  VisitCreateAndInitLocalVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalStringVar(
    const CreateAndInitLocalStringVarCmd &cmd) {
  VisitCreateAndInitLocalVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalBoolVar(
    const CreateAndInitLocalBoolVarCmd &cmd) {
  VisitCreateAndInitLocalVar<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalLongVar(
    const CreateAndInitLocalLongVarCmd &cmd) {
  VisitCreateAndInitLocalVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalDoubleVar(
    const CreateAndInitLocalDoubleVarCmd &cmd) {
  VisitCreateAndInitLocalVar<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateAndInitLocalVar(
    const CreateAndInitLocalVarCmd&) {
  local_vars_.Push(operands_.Pop<T>());
}

void SimpleVM::Impl::VisitCreateAndInitLocalIntArrayVar(
    const CreateAndInitLocalIntArrayVarCmd &cmd) {
  VisitCreateAndInitLocalArrayVar<IntValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalCharArrayVar(
    const CreateAndInitLocalCharArrayVarCmd &cmd) {
  VisitCreateAndInitLocalArrayVar<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalStringArrayVar(
    const CreateAndInitLocalStringArrayVarCmd &cmd) {
  VisitCreateAndInitLocalArrayVar<StringValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalBoolArrayVar(
    const CreateAndInitLocalBoolArrayVarCmd &cmd) {
  VisitCreateAndInitLocalArrayVar<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalLongArrayVar(
    const CreateAndInitLocalLongArrayVarCmd &cmd) {
  VisitCreateAndInitLocalArrayVar<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLocalDoubleArrayVar(
    const CreateAndInitLocalDoubleArrayVarCmd &cmd) {
  VisitCreateAndInitLocalArrayVar<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateAndInitLocalArrayVar(
    const CreateAndInitLocalArrayVarCmd&) {
  local_vars_.Push(operands_.Pop< ArrayValue<T> >());
}

void SimpleVM::Impl::VisitCreateIntArray(const CreateIntArrayCmd &cmd) {
  VisitCreateArray<IntValue>(cmd);
}

void SimpleVM::Impl::VisitCreateLongArray(const CreateLongArrayCmd &cmd) {
  VisitCreateArray<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateBoolArray(const CreateBoolArrayCmd &cmd) {
  VisitCreateArray<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitCreateCharArray(const CreateCharArrayCmd &cmd) {
  VisitCreateArray<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateDoubleArray(const CreateDoubleArrayCmd &cmd) {
  VisitCreateArray<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitCreateStringArray(const CreateStringArrayCmd &cmd) {
  VisitCreateArray<StringValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateArray(
    const CreateArrayCmd &cmd) {
  vector<size_t> dimensions = GetArrayDimensions(cmd);
  operands_.Push(ArrayValue<T>::Multidimensional(
      dimensions.begin(), dimensions.end()));
}

vector<size_t> SimpleVM::Impl::GetArrayDimensions(const CreateArrayCmd &cmd) {
  const uint8_t dimensions_count = cmd.GetDimensionsCount();
  vector<size_t> dimensions(dimensions_count);

  for (uint8_t i = 0; i != dimensions_count; ++i) {
    const auto size = operands_.Pop<IntValue>();
    assert(size >= 0);
    assert(i < dimensions.size());
    dimensions[i] = static_cast<size_t>(size);
  }

  return dimensions;
}

void SimpleVM::Impl::VisitCreateAndInitIntArray(
    const CreateAndInitIntArrayCmd &cmd) {
  VisitCreateAndInitArray<IntValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitLongArray(
    const CreateAndInitLongArrayCmd &cmd) {
  VisitCreateAndInitArray<LongValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitDoubleArray(
    const CreateAndInitDoubleArrayCmd &cmd) {
  VisitCreateAndInitArray<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitBoolArray(
    const CreateAndInitBoolArrayCmd &cmd) {
  VisitCreateAndInitArray<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitCharArray(
    const CreateAndInitCharArrayCmd &cmd) {
  VisitCreateAndInitArray<CharValue>(cmd);
}

void SimpleVM::Impl::VisitCreateAndInitStringArray(
    const CreateAndInitStringArrayCmd &cmd) {
  VisitCreateAndInitArray<StringValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitCreateAndInitArray(
    const CreateAndInitArrayCmd &cmd) {
  auto array = ArrayValue<T>::Multidimensional(
      cmd.GetDimensionsCount(),
      static_cast<size_t>(cmd.GetValuesCount()),
      &operands_);
  operands_.Push(move(array));
}

void SimpleVM::Impl::VisitDirectJump(const DirectJumpCmd &cmd) {
  Jump(cmd);
}

void SimpleVM::Impl::VisitJumpIfNot(const JumpIfNotCmd &cmd) {
  if (operands_.Pop<BoolValue>()) {
    return;
  }

  Jump(cmd);
}

void SimpleVM::Impl::VisitImplicitJumpIfNot(const ImplicitJumpIfNotCmd &cmd) {
  if (operands_.GetTop<BoolValue>()) {
    return;
  }

  Jump(cmd);
}

void SimpleVM::Impl::VisitImplicitJumpIf(const ImplicitJumpIfCmd &cmd) {
  if (!operands_.GetTop<BoolValue>()) {
    return;
  }

  Jump(cmd);
}

void SimpleVM::Impl::Jump(const JumpCmd &cmd) noexcept {
  cmd_reader_.GetCode()->MovePosition(cmd.GetOffset());
}

void SimpleVM::Impl::VisitReturn(const ReturnCmd&) {
  const uint32_t return_address = func_frames_.back().GetReturnAddress();
  func_frames_.pop_back();
  cmd_reader_.GetCode()->SetPosition(return_address);
}

void SimpleVM::Impl::VisitLoadGlobalIntVarValue(
    const LoadGlobalIntVarValueCmd &cmd) {
  VisitLoadGlobalVarValue<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalLongVarValue(
    const LoadGlobalLongVarValueCmd &cmd) {
  VisitLoadGlobalVarValue<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalDoubleVarValue(
    const LoadGlobalDoubleVarValueCmd &cmd) {
  VisitLoadGlobalVarValue<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalCharVarValue(
    const LoadGlobalCharVarValueCmd &cmd) {
  VisitLoadGlobalVarValue<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalStringVarValue(
    const LoadGlobalStringVarValueCmd &cmd) {
  VisitLoadGlobalVarValue<StringValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalBoolVarValue(
    const LoadGlobalBoolVarValueCmd &cmd) {
  VisitLoadGlobalVarValue<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLoadGlobalVarValue(
    const LoadGlobalVarValueCmd &cmd) {
  operands_.Push(global_vars_.Get<T>(cmd.GetVarIndex()));
}

void SimpleVM::Impl::VisitLoadGlobalIntArrayVarValue(
    const LoadGlobalIntArrayVarValueCmd &cmd) {
  VisitLoadGlobalArrayVarValue<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalLongArrayVarValue(
    const LoadGlobalLongArrayVarValueCmd &cmd) {
  VisitLoadGlobalArrayVarValue<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalDoubleArrayVarValue(
    const LoadGlobalDoubleArrayVarValueCmd &cmd) {
  VisitLoadGlobalArrayVarValue<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalCharArrayVarValue(
    const LoadGlobalCharArrayVarValueCmd &cmd) {
  VisitLoadGlobalArrayVarValue<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalBoolArrayVarValue(
    const LoadGlobalBoolArrayVarValueCmd &cmd) {
  VisitLoadGlobalArrayVarValue<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitLoadGlobalStringArrayVarValue(
    const LoadGlobalStringArrayVarValueCmd &cmd) {
  VisitLoadGlobalArrayVarValue<StringValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLoadGlobalArrayVarValue(
    const LoadGlobalVarValueCmd &cmd) {
  operands_.Push(
      global_vars_.Get< ArrayValue<T> >(cmd.GetVarIndex()).Clone());
}

void SimpleVM::Impl::VisitLoadLocalIntVarValue(
    const LoadLocalIntVarValueCmd &cmd) {
  VisitLoadLocalVarValue<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalLongVarValue(
    const LoadLocalLongVarValueCmd &cmd) {
  VisitLoadLocalVarValue<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalCharVarValue(
    const LoadLocalCharVarValueCmd &cmd) {
  VisitLoadLocalVarValue<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalStringVarValue(
    const LoadLocalStringVarValueCmd &cmd) {
  VisitLoadLocalVarValue<StringValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalBoolVarValue(
    const LoadLocalBoolVarValueCmd &cmd) {
  VisitLoadLocalVarValue<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalDoubleVarValue(
    const LoadLocalDoubleVarValueCmd &cmd) {
  VisitLoadLocalVarValue<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLoadLocalVarValue(
    const LoadLocalVarValueCmd &cmd) {
  operands_.Push(local_vars_.Get<T>(GetLocalVarIndex(cmd)));
}

template<typename T> size_t SimpleVM::Impl::GetLocalVarIndex(const T &cmd)
    const noexcept {
  assert(!func_frames_.empty());
  return func_frames_.back().GetLocalVarsStartIndex() + cmd.GetVarIndex();
}

void SimpleVM::Impl::VisitLoadLocalIntArrayVarValue(
    const LoadLocalIntArrayVarValueCmd &cmd) {
  VisitLoadLocalArrayVarValue<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalLongArrayVarValue(
    const LoadLocalLongArrayVarValueCmd &cmd) {
  VisitLoadLocalArrayVarValue<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalDoubleArrayVarValue(
    const LoadLocalDoubleArrayVarValueCmd &cmd) {
  VisitLoadLocalArrayVarValue<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalCharArrayVarValue(
    const LoadLocalCharArrayVarValueCmd &cmd) {
  VisitLoadLocalArrayVarValue<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalBoolArrayVarValue(
    const LoadLocalBoolArrayVarValueCmd &cmd) {
  VisitLoadLocalArrayVarValue<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLocalStringArrayVarValue(
    const LoadLocalStringArrayVarValueCmd &cmd) {
  VisitLoadLocalArrayVarValue<StringValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLoadLocalArrayVarValue(
    const LoadLocalVarValueCmd &cmd) {
  operands_.Push(
      local_vars_.Get< ArrayValue<T> >(GetLocalVarIndex(cmd)).Clone());
}

void SimpleVM::Impl::VisitLoadGlobalVarAddress(
    const LoadGlobalVarAddressCmd &cmd) {
  operands_.Push(global_vars_.GetAddress(cmd.GetVarIndex()));
}

void SimpleVM::Impl::VisitLoadLocalVarAddress(
    const LoadLocalVarAddressCmd &cmd) {
  operands_.Push(local_vars_.GetAddress(GetLocalVarIndex(cmd)));
}

void SimpleVM::Impl::VisitLoadIntArrayElementValue(
    const LoadIntArrayElementValueCmd &cmd) {
  VisitLoadArrayElementValue<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLongArrayElementValue(
    const LoadLongArrayElementValueCmd &cmd) {
  VisitLoadArrayElementValue<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLoadDoubleArrayElementValue(
    const LoadDoubleArrayElementValueCmd &cmd) {
  VisitLoadArrayElementValue<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitLoadCharArrayElementValue(
    const LoadCharArrayElementValueCmd &cmd) {
  VisitLoadArrayElementValue<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLoadStringArrayElementValue(
    const LoadStringArrayElementValueCmd &cmd) {
  VisitLoadArrayElementValue<StringValue>(cmd);
}

void SimpleVM::Impl::VisitLoadBoolArrayElementValue(
    const LoadBoolArrayElementValueCmd &cmd) {
  VisitLoadArrayElementValue<BoolValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLoadArrayElementValue(
    const LoadArrayElementValueCmd &cmd) {
  auto array = operands_.Pop< ArrayValue<T> >();
  const auto index = operands_.Pop<IntValue>();
  assert(index >= 0);
  const uint8_t dimensions_count = cmd.GetDimensionsCount();
  auto array_destroyer = array.MakeDestroyer(dimensions_count);

  if (dimensions_count == 1) {
    operands_.Push(array.GetItem(static_cast<size_t>(index)));
    return;
  }

  operands_.Push(array.GetItemArray(static_cast<size_t>(index)).Clone());
}

void SimpleVM::Impl::VisitLoadIntArrayElementAddress(
    const LoadIntArrayElementAddressCmd &cmd) {
  VisitLoadArrayElementAddress<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLoadDoubleArrayElementAddress(
    const LoadDoubleArrayElementAddressCmd &cmd) {
  VisitLoadArrayElementAddress<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitLoadBoolArrayElementAddress(
    const LoadBoolArrayElementAddressCmd &cmd) {
  VisitLoadArrayElementAddress<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitLoadStringArrayElementAddress(
    const LoadStringArrayElementAddressCmd &cmd) {
  VisitLoadArrayElementAddress<StringValue>(cmd);
}

void SimpleVM::Impl::VisitLoadLongArrayElementAddress(
    const LoadLongArrayElementAddressCmd &cmd) {
  VisitLoadArrayElementAddress<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLoadCharArrayElementAddress(
    const LoadCharArrayElementAddressCmd &cmd) {
  VisitLoadArrayElementAddress<CharValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLoadArrayElementAddress(
    const LoadArrayElementAddressCmd &cmd) {
  auto array = operands_.Pop< ArrayValue<T> >();
  const auto index = operands_.Pop<IntValue>();
  assert(index >= 0);
  const uint8_t dimensions_count = cmd.GetDimensionsCount();
  auto array_destroyer = array.MakeDestroyer(dimensions_count);

  if (dimensions_count == 1) {
    operands_.Push<VarAddressValue>(
        &(array.GetItem(static_cast<size_t>(index))));
    return;
  }

  operands_.Push<VarAddressValue>(
      &(array.GetItemArray(static_cast<size_t>(index))));
}

void SimpleVM::Impl::VisitStoreInt(const StoreIntCmd &cmd) {
  VisitStore<IntValue>(cmd);
}

void SimpleVM::Impl::VisitStoreLong(const StoreLongCmd &cmd) {
  VisitStore<LongValue>(cmd);
}

void SimpleVM::Impl::VisitStoreDouble(const StoreDoubleCmd &cmd) {
  VisitStore<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitStoreBool(const StoreBoolCmd &cmd) {
  VisitStore<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitStoreChar(const StoreCharCmd &cmd) {
  VisitStore<CharValue>(cmd);
}

void SimpleVM::Impl::VisitStoreString(const StoreStringCmd &cmd) {
  VisitStore<StringValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitStore(const StoreCmd&) noexcept {
  auto var_address = static_cast<T*>(operands_.Pop<VarAddressValue>());
  assert(var_address);
  *var_address = operands_.Pop<T>();
}

void SimpleVM::Impl::VisitStoreIntArray(const StoreIntArrayCmd &cmd) {
  VisitStoreArray<IntValue>(cmd);
}

void SimpleVM::Impl::VisitStoreLongArray(const StoreLongArrayCmd &cmd) {
  VisitStoreArray<LongValue>(cmd);
}

void SimpleVM::Impl::VisitStoreDoubleArray(const StoreDoubleArrayCmd &cmd) {
  VisitStoreArray<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitStoreBoolArray(const StoreBoolArrayCmd &cmd) {
  VisitStoreArray<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitStoreCharArray(const StoreCharArrayCmd &cmd) {
  VisitStoreArray<CharValue>(cmd);
}

void SimpleVM::Impl::VisitStoreStringArray(const StoreStringArrayCmd &cmd) {
  VisitStoreArray<StringValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitStoreArray(
    const StoreArrayCmd &cmd) noexcept {
  auto var_address =
      static_cast<ArrayValue<T>*>(operands_.Pop<VarAddressValue>());
  assert(var_address);
  var_address->Set(
      operands_.Pop< ArrayValue<T> >(), cmd.GetDimensionsCount());
}

void SimpleVM::Impl::VisitCastCharToInt(const CastCharToIntCmd &cmd) {
  VisitCast<IntValue, CharValue>(cmd);
}

void SimpleVM::Impl::VisitCastCharToDouble(const CastCharToDoubleCmd &cmd) {
  VisitCast<DoubleValue, CharValue>(cmd);
}

void SimpleVM::Impl::VisitCastCharToLong(const CastCharToLongCmd &cmd) {
  VisitCast<LongValue, CharValue>(cmd);
}

void SimpleVM::Impl::VisitCastCharToString(const CastCharToStringCmd &cmd) {
  VisitCast<StringValue, CharValue>(cmd);
}

void SimpleVM::Impl::VisitCastIntToLong(const CastIntToLongCmd &cmd) {
  VisitCast<LongValue, IntValue>(cmd);
}

void SimpleVM::Impl::VisitCastIntToDouble(const CastIntToDoubleCmd &cmd) {
  VisitCast<DoubleValue, IntValue>(cmd);
}

void SimpleVM::Impl::VisitCastLongToDouble(const CastLongToDoubleCmd &cmd) {
  VisitCast<DoubleValue, LongValue>(cmd);
}

template<typename TDest, typename TSource> void SimpleVM::Impl::VisitCast(
    const CastCmd&) {
  operands_.Push(TDest(operands_.Pop<TSource>()));
}

void SimpleVM::Impl::VisitCallNative(const CallNativeCmd&) {
  const size_t local_vars_start_index = local_vars_.GetSize();
  const uint32_t return_address = cmd_reader_.GetCode()->GetPosition();
  func_frames_.push_back(FuncFrame(local_vars_start_index, return_address));
  const auto native_func = operands_.Pop<NativeFuncValue>();
  (*native_func)(vm_);
  func_frames_.pop_back();
  assert(cmd_reader_.GetCode()->GetPosition() == return_address);
}

void SimpleVM::Impl::VisitCall(const CallCmd&) {
  const size_t local_vars_start_index = local_vars_.GetSize();
  const uint32_t return_address = cmd_reader_.GetCode()->GetPosition();
  func_frames_.push_back(FuncFrame(local_vars_start_index, return_address));
  cmd_reader_.GetCode()->SetPosition(operands_.Pop<FuncValue>());
}

void SimpleVM::Impl::VisitMulChar(const MulCharCmd &cmd) {
  VisitMul<CharValue>(cmd);
}

void SimpleVM::Impl::VisitMulInt(const MulIntCmd &cmd) {
  VisitMul<IntValue>(cmd);
}

void SimpleVM::Impl::VisitMulLong(const MulLongCmd &cmd) {
  VisitMul<LongValue>(cmd);
}

void SimpleVM::Impl::VisitMulDouble(const MulDoubleCmd &cmd) {
  VisitMul<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitMul(const MulCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(T(lhs * rhs));
}

void SimpleVM::Impl::VisitDivChar(const DivCharCmd &cmd) {
  VisitDiv<CharValue>(cmd);
}

void SimpleVM::Impl::VisitDivInt(const DivIntCmd &cmd) {
  VisitDiv<IntValue>(cmd);
}

void SimpleVM::Impl::VisitDivDouble(const DivDoubleCmd &cmd) {
  VisitDiv<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitDivLong(const DivLongCmd &cmd) {
  VisitDiv<LongValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitDiv(const DivCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(T(lhs / rhs));
}

void SimpleVM::Impl::VisitSumChar(const SumCharCmd &cmd) {
  VisitSum<CharValue>(cmd);
}

void SimpleVM::Impl::VisitSumLong(const SumLongCmd &cmd) {
  VisitSum<LongValue>(cmd);
}

void SimpleVM::Impl::VisitSumDouble(const SumDoubleCmd &cmd) {
  VisitSum<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitSumString(const SumStringCmd &cmd) {
  VisitSum<StringValue>(cmd);
}

void SimpleVM::Impl::VisitSumInt(const SumIntCmd &cmd) {
  VisitSum<IntValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitSum(const SumCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(T(lhs + rhs));
}

void SimpleVM::Impl::VisitSubChar(const SubCharCmd &cmd) {
  VisitSub<CharValue>(cmd);
}

void SimpleVM::Impl::VisitSubInt(const SubIntCmd &cmd) {
  VisitSub<IntValue>(cmd);
}

void SimpleVM::Impl::VisitSubLong(const SubLongCmd &cmd) {
  VisitSub<LongValue>(cmd);
}

void SimpleVM::Impl::VisitSubDouble(const SubDoubleCmd &cmd) {
  VisitSub<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitSub(const SubCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(T(lhs - rhs));
}

void SimpleVM::Impl::VisitEqualInt(const EqualIntCmd &cmd) {
  VisitEqual<IntValue>(cmd);
}

void SimpleVM::Impl::VisitEqualLong(const EqualLongCmd &cmd) {
  VisitEqual<LongValue>(cmd);
}

void SimpleVM::Impl::VisitEqualDouble(const EqualDoubleCmd &cmd) {
  VisitEqual<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitEqualString(const EqualStringCmd &cmd) {
  VisitEqual<StringValue>(cmd);
}

void SimpleVM::Impl::VisitEqualBool(const EqualBoolCmd &cmd) {
  VisitEqual<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitEqualChar(const EqualCharCmd &cmd) {
  VisitEqual<CharValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitEqual(const EqualCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(BoolValue(lhs == rhs));
}

void SimpleVM::Impl::VisitEqualIntArray(const EqualIntArrayCmd &cmd) {
  VisitEqualArray<IntValue>(cmd);
}

void SimpleVM::Impl::VisitEqualLongArray(const EqualLongArrayCmd &cmd) {
  VisitEqualArray<LongValue>(cmd);
}

void SimpleVM::Impl::VisitEqualDoubleArray(const EqualDoubleArrayCmd &cmd) {
  VisitEqualArray<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitEqualStringArray(const EqualStringArrayCmd &cmd) {
  VisitEqualArray<StringValue>(cmd);
}

void SimpleVM::Impl::VisitEqualBoolArray(const EqualBoolArrayCmd &cmd) {
  VisitEqualArray<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitEqualCharArray(const EqualCharArrayCmd &cmd) {
  VisitEqualArray<CharValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitEqualArray(
    const EqualArrayCmd &cmd) {
  auto rhs = operands_.Pop< ArrayValue<T> >();
  auto lhs = operands_.Pop< ArrayValue<T> >();
  const BoolValue is_equal = lhs == rhs;
  const uint8_t dimensions_count = cmd.GetDimensionsCount();
  lhs.Destroy(dimensions_count);
  rhs.Destroy(dimensions_count);
  operands_.Push(is_equal);
}

void SimpleVM::Impl::VisitNotEqualInt(const NotEqualIntCmd &cmd) {
  VisitNotEqual<IntValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualLong(const NotEqualLongCmd &cmd) {
  VisitNotEqual<LongValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualDouble(const NotEqualDoubleCmd &cmd) {
  VisitNotEqual<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualBool(const NotEqualBoolCmd &cmd) {
  VisitNotEqual<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualString(const NotEqualStringCmd &cmd) {
  VisitNotEqual<StringValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualChar(const NotEqualCharCmd &cmd) {
  VisitNotEqual<CharValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitNotEqual(const NotEqualCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(BoolValue(lhs != rhs));
}

void SimpleVM::Impl::VisitNotEqualIntArray(const NotEqualIntArrayCmd &cmd) {
  VisitNotEqualArray<IntValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualLongArray(const NotEqualLongArrayCmd &cmd) {
  VisitNotEqualArray<LongValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualDoubleArray(
    const NotEqualDoubleArrayCmd &cmd) {
  VisitNotEqualArray<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualStringArray(
    const NotEqualStringArrayCmd &cmd) {
  VisitNotEqualArray<StringValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualBoolArray(const NotEqualBoolArrayCmd &cmd) {
  VisitNotEqualArray<BoolValue>(cmd);
}

void SimpleVM::Impl::VisitNotEqualCharArray(const NotEqualCharArrayCmd &cmd) {
  VisitNotEqualArray<CharValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitNotEqualArray(
    const NotEqualArrayCmd &cmd) {
  auto rhs = operands_.Pop< ArrayValue<T> >();
  auto lhs = operands_.Pop< ArrayValue<T> >();
  const BoolValue is_equal = lhs != rhs;
  const uint8_t dimensions_count = cmd.GetDimensionsCount();
  lhs.Destroy(dimensions_count);
  rhs.Destroy(dimensions_count);
  operands_.Push(is_equal);
}

void SimpleVM::Impl::VisitGreaterChar(const GreaterCharCmd &cmd) {
  VisitGreater<CharValue>(cmd);
}

void SimpleVM::Impl::VisitGreaterInt(const GreaterIntCmd &cmd) {
  VisitGreater<IntValue>(cmd);
}

void SimpleVM::Impl::VisitGreaterLong(const GreaterLongCmd &cmd) {
  VisitGreater<LongValue>(cmd);
}

void SimpleVM::Impl::VisitGreaterDouble(const GreaterDoubleCmd &cmd) {
  VisitGreater<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitGreater(const GreaterCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(BoolValue(lhs > rhs));
}

void SimpleVM::Impl::VisitGreaterOrEqualChar(const GreaterOrEqualCharCmd &cmd) {
  VisitGreaterOrEqual<CharValue>(cmd);
}

void SimpleVM::Impl::VisitGreaterOrEqualInt(const GreaterOrEqualIntCmd &cmd) {
  VisitGreaterOrEqual<IntValue>(cmd);
}

void SimpleVM::Impl::VisitGreaterOrEqualLong(const GreaterOrEqualLongCmd &cmd) {
  VisitGreaterOrEqual<LongValue>(cmd);
}

void SimpleVM::Impl::VisitGreaterOrEqualDouble(
    const GreaterOrEqualDoubleCmd &cmd) {
  VisitGreaterOrEqual<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitGreaterOrEqual(
    const GreaterOrEqualCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(BoolValue(lhs >= rhs));
}

void SimpleVM::Impl::VisitLessChar(const LessCharCmd &cmd) {
  VisitLess<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLessInt(const LessIntCmd &cmd) {
  VisitLess<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLessLong(const LessLongCmd &cmd) {
  VisitLess<LongValue>(cmd);
}

void SimpleVM::Impl::VisitLessDouble(const LessDoubleCmd &cmd) {
  VisitLess<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLess(const LessCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(BoolValue(lhs < rhs));
}

void SimpleVM::Impl::VisitLessOrEqualInt(const LessOrEqualIntCmd &cmd) {
  VisitLessOrEqual<IntValue>(cmd);
}

void SimpleVM::Impl::VisitLessOrEqualDouble(const LessOrEqualDoubleCmd &cmd) {
  VisitLessOrEqual<DoubleValue>(cmd);
}

void SimpleVM::Impl::VisitLessOrEqualChar(const LessOrEqualCharCmd &cmd) {
  VisitLessOrEqual<CharValue>(cmd);
}

void SimpleVM::Impl::VisitLessOrEqualLong(const LessOrEqualLongCmd &cmd) {
  VisitLessOrEqual<LongValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitLessOrEqual(
    const LessOrEqualCmd&) {
  const auto rhs = operands_.Pop<T>();
  const auto lhs = operands_.Pop<T>();
  operands_.Push(BoolValue(lhs <= rhs));
}

void SimpleVM::Impl::VisitLogicalNegateBool(const LogicalNegateBoolCmd&) {
  operands_.Push<BoolValue>(!operands_.Pop<BoolValue>());
}

void SimpleVM::Impl::VisitArithmeticNegateInt(
    const ArithmeticNegateIntCmd &cmd) {
  VisitArithmeticNegate<IntValue>(cmd);
}

void SimpleVM::Impl::VisitArithmeticNegateLong(
    const ArithmeticNegateLongCmd &cmd) {
  VisitArithmeticNegate<LongValue>(cmd);
}

void SimpleVM::Impl::VisitArithmeticNegateDouble(
    const ArithmeticNegateDoubleCmd &cmd) {
  VisitArithmeticNegate<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitArithmeticNegate(
    const ArithmeticNegateCmd&) {
  operands_.Push<T>(-operands_.Pop<T>());
}

void SimpleVM::Impl::VisitPreDecChar(const PreDecCharCmd &cmd) {
  VisitPreDec<CharValue>(cmd);
}

void SimpleVM::Impl::VisitPreDecInt(const PreDecIntCmd &cmd) {
  VisitPreDec<IntValue>(cmd);
}

void SimpleVM::Impl::VisitPreDecLong(const PreDecLongCmd &cmd) {
  VisitPreDec<LongValue>(cmd);
}

void SimpleVM::Impl::VisitPreDecDouble(const PreDecDoubleCmd &cmd) {
  VisitPreDec<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitPreDec(const PreDecCmd&) {
  auto var_address = operands_.Pop<VarAddressValue>();
  --(*static_cast<T*>(var_address));
}

void SimpleVM::Impl::VisitPreIncChar(const PreIncCharCmd &cmd) {
  VisitPreInc<CharValue>(cmd);
}

void SimpleVM::Impl::VisitPreIncInt(const PreIncIntCmd &cmd) {
  VisitPreInc<IntValue>(cmd);
}

void SimpleVM::Impl::VisitPreIncLong(const PreIncLongCmd &cmd) {
  VisitPreInc<LongValue>(cmd);
}

void SimpleVM::Impl::VisitPreIncDouble(const PreIncDoubleCmd &cmd) {
  VisitPreInc<DoubleValue>(cmd);
}

template<typename T> void SimpleVM::Impl::VisitPreInc(const PreIncCmd&) {
  auto var_address = operands_.Pop<VarAddressValue>();
  ++(*static_cast<T*>(var_address));
}

void SimpleVM::Impl::VisitAnd(const AndCmd&) {
  const auto rhs = operands_.Pop<BoolValue>();
  const auto lhs = operands_.Pop<BoolValue>();
  operands_.Push(BoolValue(lhs && rhs));
}

void SimpleVM::Impl::VisitOr(const OrCmd&) {
  const auto rhs = operands_.Pop<BoolValue>();
  const auto lhs = operands_.Pop<BoolValue>();
  operands_.Push(BoolValue(lhs || rhs));
}
}
}
