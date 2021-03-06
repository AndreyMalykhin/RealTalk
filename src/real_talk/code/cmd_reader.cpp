
#include <cassert>
#include <string>
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/create_local_var_cmd.h"
#include "real_talk/code/destroy_local_var_cmd.h"
#include "real_talk/code/unload_cmd.h"
#include "real_talk/code/load_value_cmd.h"
#include "real_talk/code/create_and_init_global_var_cmd.h"
#include "real_talk/code/create_and_init_local_var_cmd.h"
#include "real_talk/code/create_array_cmd.h"
#include "real_talk/code/create_and_init_array_cmd.h"
#include "real_talk/code/jump_cmd.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/return_cmd.h"
#include "real_talk/code/load_global_var_value_cmd.h"
#include "real_talk/code/load_local_var_value_cmd.h"
#include "real_talk/code/load_global_var_address_cmd.h"
#include "real_talk/code/load_local_var_address_cmd.h"
#include "real_talk/code/load_array_element_value_cmd.h"
#include "real_talk/code/load_array_element_address_cmd.h"
#include "real_talk/code/store_cmd.h"
#include "real_talk/code/cast_cmd.h"
#include "real_talk/code/call_cmd.h"
#include "real_talk/code/call_native_cmd.h"
#include "real_talk/code/and_cmd.h"
#include "real_talk/code/or_cmd.h"
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
#include "real_talk/code/code.h"

using std::string;

namespace real_talk {
namespace code {
namespace {

CreateGlobalIntVarCmd &kCreateGlobalIntVarCmd =
    *new CreateGlobalIntVarCmd(UINT32_C(0));
CreateGlobalArrayVarCmd &kCreateGlobalArrayVarCmd =
    *new CreateGlobalArrayVarCmd(UINT32_C(0));
CreateGlobalLongVarCmd &kCreateGlobalLongVarCmd =
    *new CreateGlobalLongVarCmd(UINT32_C(0));
CreateGlobalDoubleVarCmd &kCreateGlobalDoubleVarCmd =
    *new CreateGlobalDoubleVarCmd(UINT32_C(0));
CreateGlobalCharVarCmd &kCreateGlobalCharVarCmd =
    *new CreateGlobalCharVarCmd(UINT32_C(0));
CreateGlobalStringVarCmd &kCreateGlobalStringVarCmd =
    *new CreateGlobalStringVarCmd(UINT32_C(0));
CreateGlobalBoolVarCmd &kCreateGlobalBoolVarCmd =
    *new CreateGlobalBoolVarCmd(UINT32_C(0));

const CreateLocalIntVarCmd &kCreateLocalIntVarCmd =
    *new CreateLocalIntVarCmd();
const CreateLocalArrayVarCmd &kCreateLocalArrayVarCmd =
    *new CreateLocalArrayVarCmd();
const CreateLocalLongVarCmd &kCreateLocalLongVarCmd =
    *new CreateLocalLongVarCmd();
const CreateLocalDoubleVarCmd &kCreateLocalDoubleVarCmd =
    *new CreateLocalDoubleVarCmd();
const CreateLocalCharVarCmd &kCreateLocalCharVarCmd =
    *new CreateLocalCharVarCmd();
const CreateLocalStringVarCmd &kCreateLocalStringVarCmd =
    *new CreateLocalStringVarCmd();
const CreateLocalBoolVarCmd &kCreateLocalBoolVarCmd =
    *new CreateLocalBoolVarCmd();

const DestroyLocalIntVarCmd &kDestroyLocalIntVarCmd =
    *new DestroyLocalIntVarCmd();
const DestroyLocalLongVarCmd &kDestroyLocalLongVarCmd =
    *new DestroyLocalLongVarCmd();
const DestroyLocalDoubleVarCmd &kDestroyLocalDoubleVarCmd =
    *new DestroyLocalDoubleVarCmd();
const DestroyLocalCharVarCmd &kDestroyLocalCharVarCmd =
    *new DestroyLocalCharVarCmd();
const DestroyLocalStringVarCmd &kDestroyLocalStringVarCmd =
    *new DestroyLocalStringVarCmd();
const DestroyLocalBoolVarCmd &kDestroyLocalBoolVarCmd =
    *new DestroyLocalBoolVarCmd();

DestroyLocalIntArrayVarCmd &kDestroyLocalIntArrayVarCmd =
    *new DestroyLocalIntArrayVarCmd(UINT8_C(1));
DestroyLocalLongArrayVarCmd &kDestroyLocalLongArrayVarCmd =
    *new DestroyLocalLongArrayVarCmd(UINT8_C(1));
DestroyLocalDoubleArrayVarCmd &kDestroyLocalDoubleArrayVarCmd =
    *new DestroyLocalDoubleArrayVarCmd(UINT8_C(1));
DestroyLocalCharArrayVarCmd &kDestroyLocalCharArrayVarCmd =
    *new DestroyLocalCharArrayVarCmd(UINT8_C(1));
DestroyLocalStringArrayVarCmd &kDestroyLocalStringArrayVarCmd =
    *new DestroyLocalStringArrayVarCmd(UINT8_C(1));
DestroyLocalBoolArrayVarCmd &kDestroyLocalBoolArrayVarCmd =
    *new DestroyLocalBoolArrayVarCmd(UINT8_C(1));

const UnloadIntCmd &kUnloadIntCmd = *new UnloadIntCmd();
const UnloadLongCmd &kUnloadLongCmd = *new UnloadLongCmd();
const UnloadDoubleCmd &kUnloadDoubleCmd = *new UnloadDoubleCmd();
const UnloadCharCmd &kUnloadCharCmd = *new UnloadCharCmd();
const UnloadStringCmd &kUnloadStringCmd = *new UnloadStringCmd();
const UnloadBoolCmd &kUnloadBoolCmd = *new UnloadBoolCmd();

UnloadIntArrayCmd &kUnloadIntArrayCmd = *new UnloadIntArrayCmd(UINT8_C(1));
UnloadLongArrayCmd &kUnloadLongArrayCmd = *new UnloadLongArrayCmd(UINT8_C(1));
UnloadDoubleArrayCmd &kUnloadDoubleArrayCmd =
    *new UnloadDoubleArrayCmd(UINT8_C(1));
UnloadCharArrayCmd &kUnloadCharArrayCmd = *new UnloadCharArrayCmd(UINT8_C(1));
UnloadStringArrayCmd &kUnloadStringArrayCmd =
    *new UnloadStringArrayCmd(UINT8_C(1));
UnloadBoolArrayCmd &kUnloadBoolArrayCmd = *new UnloadBoolArrayCmd(UINT8_C(1));

LoadIntValueCmd &kLoadIntValueCmd = *new LoadIntValueCmd(INT32_C(0));
LoadLongValueCmd &kLoadLongValueCmd = *new LoadLongValueCmd(INT64_C(0));
LoadBoolValueCmd &kLoadBoolValueCmd = *new LoadBoolValueCmd(false);
LoadCharValueCmd &kLoadCharValueCmd = *new LoadCharValueCmd('\0');
LoadStringValueCmd &kLoadStringValueCmd = *new LoadStringValueCmd("");
LoadDoubleValueCmd &kLoadDoubleValueCmd = *new LoadDoubleValueCmd(0.0);
LoadFuncValueCmd &kLoadFuncValueCmd = *new LoadFuncValueCmd(UINT32_C(0));
LoadNativeFuncValueCmd &kLoadNativeFuncValueCmd =
    *new LoadNativeFuncValueCmd(UINT32_C(0));

CreateAndInitGlobalIntVarCmd &kCreateAndInitGlobalIntVarCmd =
    *new CreateAndInitGlobalIntVarCmd(UINT32_C(0));
CreateAndInitGlobalLongVarCmd &kCreateAndInitGlobalLongVarCmd =
    *new CreateAndInitGlobalLongVarCmd(UINT32_C(0));
CreateAndInitGlobalDoubleVarCmd &kCreateAndInitGlobalDoubleVarCmd =
    *new CreateAndInitGlobalDoubleVarCmd(UINT32_C(0));
CreateAndInitGlobalCharVarCmd &kCreateAndInitGlobalCharVarCmd =
    *new CreateAndInitGlobalCharVarCmd(UINT32_C(0));
CreateAndInitGlobalStringVarCmd &kCreateAndInitGlobalStringVarCmd =
    *new CreateAndInitGlobalStringVarCmd(UINT32_C(0));
CreateAndInitGlobalBoolVarCmd &kCreateAndInitGlobalBoolVarCmd =
    *new CreateAndInitGlobalBoolVarCmd(UINT32_C(0));

CreateAndInitGlobalIntArrayVarCmd &kCreateAndInitGlobalIntArrayVarCmd =
    *new CreateAndInitGlobalIntArrayVarCmd(UINT32_C(0), UINT8_C(1));
CreateAndInitGlobalLongArrayVarCmd &kCreateAndInitGlobalLongArrayVarCmd =
    *new CreateAndInitGlobalLongArrayVarCmd(UINT32_C(0), UINT8_C(1));
CreateAndInitGlobalDoubleArrayVarCmd &kCreateAndInitGlobalDoubleArrayVarCmd =
    *new CreateAndInitGlobalDoubleArrayVarCmd(UINT32_C(0), UINT8_C(1));
CreateAndInitGlobalCharArrayVarCmd &kCreateAndInitGlobalCharArrayVarCmd =
    *new CreateAndInitGlobalCharArrayVarCmd(UINT32_C(0), UINT8_C(1));
CreateAndInitGlobalStringArrayVarCmd &kCreateAndInitGlobalStringArrayVarCmd =
    *new CreateAndInitGlobalStringArrayVarCmd(UINT32_C(0), UINT8_C(1));
CreateAndInitGlobalBoolArrayVarCmd &kCreateAndInitGlobalBoolArrayVarCmd =
    *new CreateAndInitGlobalBoolArrayVarCmd(UINT32_C(0), UINT8_C(1));

CreateIntArrayCmd &kCreateIntArrayCmd = *new CreateIntArrayCmd(UINT8_C(1));
CreateLongArrayCmd &kCreateLongArrayCmd = *new CreateLongArrayCmd(UINT8_C(1));
CreateDoubleArrayCmd &kCreateDoubleArrayCmd =
    *new CreateDoubleArrayCmd(UINT8_C(1));
CreateBoolArrayCmd &kCreateBoolArrayCmd = *new CreateBoolArrayCmd(UINT8_C(1));
CreateCharArrayCmd &kCreateCharArrayCmd = *new CreateCharArrayCmd(UINT8_C(1));
CreateStringArrayCmd &kCreateStringArrayCmd =
    *new CreateStringArrayCmd(UINT8_C(1));

CreateAndInitIntArrayCmd &kCreateAndInitIntArrayCmd =
    *new CreateAndInitIntArrayCmd(UINT8_C(1), UINT32_C(0));
CreateAndInitLongArrayCmd &kCreateAndInitLongArrayCmd =
    *new CreateAndInitLongArrayCmd(UINT8_C(1), UINT32_C(0));
CreateAndInitDoubleArrayCmd &kCreateAndInitDoubleArrayCmd =
    *new CreateAndInitDoubleArrayCmd(UINT8_C(1), UINT32_C(0));
CreateAndInitBoolArrayCmd &kCreateAndInitBoolArrayCmd =
    *new CreateAndInitBoolArrayCmd(UINT8_C(1), UINT32_C(0));
CreateAndInitCharArrayCmd &kCreateAndInitCharArrayCmd =
    *new CreateAndInitCharArrayCmd(UINT8_C(1), UINT32_C(0));
CreateAndInitStringArrayCmd &kCreateAndInitStringArrayCmd =
    *new CreateAndInitStringArrayCmd(UINT8_C(1), UINT32_C(0));

const CreateAndInitLocalIntVarCmd &kCreateAndInitLocalIntVarCmd =
    *new CreateAndInitLocalIntVarCmd();
const CreateAndInitLocalLongVarCmd &kCreateAndInitLocalLongVarCmd =
    *new CreateAndInitLocalLongVarCmd();
const CreateAndInitLocalDoubleVarCmd &kCreateAndInitLocalDoubleVarCmd =
    *new CreateAndInitLocalDoubleVarCmd();
const CreateAndInitLocalCharVarCmd &kCreateAndInitLocalCharVarCmd =
    *new CreateAndInitLocalCharVarCmd();
const CreateAndInitLocalStringVarCmd &kCreateAndInitLocalStringVarCmd =
    *new CreateAndInitLocalStringVarCmd();
const CreateAndInitLocalBoolVarCmd &kCreateAndInitLocalBoolVarCmd =
    *new CreateAndInitLocalBoolVarCmd();

CreateAndInitLocalIntArrayVarCmd &kCreateAndInitLocalIntArrayVarCmd =
    *new CreateAndInitLocalIntArrayVarCmd(UINT8_C(1));
CreateAndInitLocalLongArrayVarCmd &kCreateAndInitLocalLongArrayVarCmd =
    *new CreateAndInitLocalLongArrayVarCmd(UINT8_C(1));
CreateAndInitLocalDoubleArrayVarCmd &kCreateAndInitLocalDoubleArrayVarCmd =
    *new CreateAndInitLocalDoubleArrayVarCmd(UINT8_C(1));
CreateAndInitLocalCharArrayVarCmd &kCreateAndInitLocalCharArrayVarCmd =
    *new CreateAndInitLocalCharArrayVarCmd(UINT8_C(1));
CreateAndInitLocalStringArrayVarCmd &kCreateAndInitLocalStringArrayVarCmd =
    *new CreateAndInitLocalStringArrayVarCmd(UINT8_C(1));
CreateAndInitLocalBoolArrayVarCmd &kCreateAndInitLocalBoolArrayVarCmd =
    *new CreateAndInitLocalBoolArrayVarCmd(UINT8_C(1));

JumpIfNotCmd &kJumpIfNotCmd = *new JumpIfNotCmd(INT32_C(0));
ImplicitJumpIfNotCmd &kImplicitJumpIfNotCmd =
    *new ImplicitJumpIfNotCmd(INT32_C(0));
ImplicitJumpIfCmd &kImplicitJumpIfCmd =
    *new ImplicitJumpIfCmd(INT32_C(0));
DirectJumpCmd &kDirectJumpCmd = *new DirectJumpCmd(INT32_C(0));

const ReturnCmd &kReturnCmd = *new ReturnCmd();

LoadGlobalIntVarValueCmd &kLoadGlobalIntVarValueCmd =
    *new LoadGlobalIntVarValueCmd(UINT32_C(0));
LoadGlobalLongVarValueCmd &kLoadGlobalLongVarValueCmd =
    *new LoadGlobalLongVarValueCmd(UINT32_C(0));
LoadGlobalDoubleVarValueCmd &kLoadGlobalDoubleVarValueCmd =
    *new LoadGlobalDoubleVarValueCmd(UINT32_C(0));
LoadGlobalCharVarValueCmd &kLoadGlobalCharVarValueCmd =
    *new LoadGlobalCharVarValueCmd(UINT32_C(0));
LoadGlobalStringVarValueCmd &kLoadGlobalStringVarValueCmd =
    *new LoadGlobalStringVarValueCmd(UINT32_C(0));
LoadGlobalBoolVarValueCmd &kLoadGlobalBoolVarValueCmd =
    *new LoadGlobalBoolVarValueCmd(UINT32_C(0));

LoadGlobalIntArrayVarValueCmd &kLoadGlobalIntArrayVarValueCmd =
    *new LoadGlobalIntArrayVarValueCmd(UINT32_C(0));
LoadGlobalLongArrayVarValueCmd &kLoadGlobalLongArrayVarValueCmd =
    *new LoadGlobalLongArrayVarValueCmd(UINT32_C(0));
LoadGlobalDoubleArrayVarValueCmd &kLoadGlobalDoubleArrayVarValueCmd =
    *new LoadGlobalDoubleArrayVarValueCmd(UINT32_C(0));
LoadGlobalCharArrayVarValueCmd &kLoadGlobalCharArrayVarValueCmd =
    *new LoadGlobalCharArrayVarValueCmd(UINT32_C(0));
LoadGlobalStringArrayVarValueCmd &kLoadGlobalStringArrayVarValueCmd =
    *new LoadGlobalStringArrayVarValueCmd(UINT32_C(0));
LoadGlobalBoolArrayVarValueCmd &kLoadGlobalBoolArrayVarValueCmd =
    *new LoadGlobalBoolArrayVarValueCmd(UINT32_C(0));

LoadLocalIntVarValueCmd &kLoadLocalIntVarValueCmd =
    *new LoadLocalIntVarValueCmd(UINT32_C(0));
LoadLocalLongVarValueCmd &kLoadLocalLongVarValueCmd =
    *new LoadLocalLongVarValueCmd(UINT32_C(0));
LoadLocalDoubleVarValueCmd &kLoadLocalDoubleVarValueCmd =
    *new LoadLocalDoubleVarValueCmd(UINT32_C(0));
LoadLocalCharVarValueCmd &kLoadLocalCharVarValueCmd =
    *new LoadLocalCharVarValueCmd(UINT32_C(0));
LoadLocalStringVarValueCmd &kLoadLocalStringVarValueCmd =
    *new LoadLocalStringVarValueCmd(UINT32_C(0));
LoadLocalBoolVarValueCmd &kLoadLocalBoolVarValueCmd =
    *new LoadLocalBoolVarValueCmd(UINT32_C(0));

LoadLocalIntArrayVarValueCmd &kLoadLocalIntArrayVarValueCmd =
    *new LoadLocalIntArrayVarValueCmd(UINT32_C(0));
LoadLocalLongArrayVarValueCmd &kLoadLocalLongArrayVarValueCmd =
    *new LoadLocalLongArrayVarValueCmd(UINT32_C(0));
LoadLocalDoubleArrayVarValueCmd &kLoadLocalDoubleArrayVarValueCmd =
    *new LoadLocalDoubleArrayVarValueCmd(UINT32_C(0));
LoadLocalCharArrayVarValueCmd &kLoadLocalCharArrayVarValueCmd =
    *new LoadLocalCharArrayVarValueCmd(UINT32_C(0));
LoadLocalStringArrayVarValueCmd &kLoadLocalStringArrayVarValueCmd =
    *new LoadLocalStringArrayVarValueCmd(UINT32_C(0));
LoadLocalBoolArrayVarValueCmd &kLoadLocalBoolArrayVarValueCmd =
    *new LoadLocalBoolArrayVarValueCmd(UINT32_C(0));

LoadGlobalVarAddressCmd &kLoadGlobalVarAddressCmd =
    *new LoadGlobalVarAddressCmd(UINT32_C(0));
LoadLocalVarAddressCmd &kLoadLocalVarAddressCmd =
    *new LoadLocalVarAddressCmd(UINT32_C(0));

const StoreIntCmd &kStoreIntCmd = *new StoreIntCmd();
const StoreLongCmd &kStoreLongCmd = *new StoreLongCmd();
const StoreDoubleCmd &kStoreDoubleCmd = *new StoreDoubleCmd();
const StoreBoolCmd &kStoreBoolCmd = *new StoreBoolCmd();
const StoreCharCmd &kStoreCharCmd = *new StoreCharCmd();
const StoreStringCmd &kStoreStringCmd = *new StoreStringCmd();

StoreIntArrayCmd &kStoreIntArrayCmd = *new StoreIntArrayCmd(UINT8_C(1));
StoreLongArrayCmd &kStoreLongArrayCmd = *new StoreLongArrayCmd(UINT8_C(1));
StoreDoubleArrayCmd &kStoreDoubleArrayCmd =
    *new StoreDoubleArrayCmd(UINT8_C(1));
StoreBoolArrayCmd &kStoreBoolArrayCmd = *new StoreBoolArrayCmd(UINT8_C(1));
StoreCharArrayCmd &kStoreCharArrayCmd = *new StoreCharArrayCmd(UINT8_C(1));
StoreStringArrayCmd &kStoreStringArrayCmd =
    *new StoreStringArrayCmd(UINT8_C(1));

const CastCharToIntCmd &kCastCharToIntCmd = *new CastCharToIntCmd();
const CastCharToLongCmd &kCastCharToLongCmd = *new CastCharToLongCmd();
const CastCharToDoubleCmd &kCastCharToDoubleCmd = *new CastCharToDoubleCmd();
const CastCharToStringCmd &kCastCharToStringCmd = *new CastCharToStringCmd();
const CastIntToLongCmd &kCastIntToLongCmd = *new CastIntToLongCmd();
const CastIntToDoubleCmd &kCastIntToDoubleCmd = *new CastIntToDoubleCmd();
const CastLongToDoubleCmd &kCastLongToDoubleCmd = *new CastLongToDoubleCmd();

const CallCmd &kCallCmd = *new CallCmd();
const CallNativeCmd &kCallNativeCmd = *new CallNativeCmd();

LoadIntArrayElementValueCmd &kLoadIntArrayElementValueCmd =
    *new LoadIntArrayElementValueCmd(UINT8_C(1));
LoadLongArrayElementValueCmd &kLoadLongArrayElementValueCmd =
    *new LoadLongArrayElementValueCmd(UINT8_C(1));
LoadDoubleArrayElementValueCmd &kLoadDoubleArrayElementValueCmd =
    *new LoadDoubleArrayElementValueCmd(UINT8_C(1));
LoadBoolArrayElementValueCmd &kLoadBoolArrayElementValueCmd =
    *new LoadBoolArrayElementValueCmd(UINT8_C(1));
LoadCharArrayElementValueCmd &kLoadCharArrayElementValueCmd =
    *new LoadCharArrayElementValueCmd(UINT8_C(1));
LoadStringArrayElementValueCmd &kLoadStringArrayElementValueCmd =
    *new LoadStringArrayElementValueCmd(UINT8_C(1));

LoadIntArrayElementAddressCmd &kLoadIntArrayElementAddressCmd =
    *new LoadIntArrayElementAddressCmd(UINT8_C(1));
LoadLongArrayElementAddressCmd &kLoadLongArrayElementAddressCmd =
    *new LoadLongArrayElementAddressCmd(UINT8_C(1));
LoadDoubleArrayElementAddressCmd &kLoadDoubleArrayElementAddressCmd =
    *new LoadDoubleArrayElementAddressCmd(UINT8_C(1));
LoadBoolArrayElementAddressCmd &kLoadBoolArrayElementAddressCmd =
    *new LoadBoolArrayElementAddressCmd(UINT8_C(1));
LoadCharArrayElementAddressCmd &kLoadCharArrayElementAddressCmd =
    *new LoadCharArrayElementAddressCmd(UINT8_C(1));
LoadStringArrayElementAddressCmd &kLoadStringArrayElementAddressCmd =
    *new LoadStringArrayElementAddressCmd(UINT8_C(1));

const AndCmd &kAndCmd = *new AndCmd();
const OrCmd &kOrCmd = *new OrCmd();

const MulCharCmd &kMulCharCmd = *new MulCharCmd();
const MulIntCmd &kMulIntCmd = *new MulIntCmd();
const MulLongCmd &kMulLongCmd = *new MulLongCmd();
const MulDoubleCmd &kMulDoubleCmd = *new MulDoubleCmd();

const DivCharCmd &kDivCharCmd = *new DivCharCmd();
const DivIntCmd &kDivIntCmd = *new DivIntCmd();
const DivLongCmd &kDivLongCmd = *new DivLongCmd();
const DivDoubleCmd &kDivDoubleCmd = *new DivDoubleCmd();

const SumCharCmd &kSumCharCmd = *new SumCharCmd();
const SumIntCmd &kSumIntCmd = *new SumIntCmd();
const SumLongCmd &kSumLongCmd = *new SumLongCmd();
const SumDoubleCmd &kSumDoubleCmd = *new SumDoubleCmd();
const SumStringCmd &kSumStringCmd = *new SumStringCmd();

const SubCharCmd &kSubCharCmd = *new SubCharCmd();
const SubIntCmd &kSubIntCmd = *new SubIntCmd();
const SubLongCmd &kSubLongCmd = *new SubLongCmd();
const SubDoubleCmd &kSubDoubleCmd = *new SubDoubleCmd();

const EqualIntCmd &kEqualIntCmd = *new EqualIntCmd();
const EqualLongCmd &kEqualLongCmd = *new EqualLongCmd();
const EqualDoubleCmd &kEqualDoubleCmd = *new EqualDoubleCmd();
const EqualBoolCmd &kEqualBoolCmd = *new EqualBoolCmd();
const EqualCharCmd &kEqualCharCmd = *new EqualCharCmd();
const EqualStringCmd &kEqualStringCmd = *new EqualStringCmd();

EqualIntArrayCmd &kEqualIntArrayCmd = *new EqualIntArrayCmd(UINT8_C(1));
EqualLongArrayCmd &kEqualLongArrayCmd = *new EqualLongArrayCmd(UINT8_C(1));
EqualDoubleArrayCmd &kEqualDoubleArrayCmd =
    *new EqualDoubleArrayCmd(UINT8_C(1));
EqualBoolArrayCmd &kEqualBoolArrayCmd = *new EqualBoolArrayCmd(UINT8_C(1));
EqualCharArrayCmd &kEqualCharArrayCmd = *new EqualCharArrayCmd(UINT8_C(1));
EqualStringArrayCmd &kEqualStringArrayCmd =
    *new EqualStringArrayCmd(UINT8_C(1));

const NotEqualIntCmd &kNotEqualIntCmd = *new NotEqualIntCmd();
const NotEqualLongCmd &kNotEqualLongCmd = *new NotEqualLongCmd();
const NotEqualDoubleCmd &kNotEqualDoubleCmd = *new NotEqualDoubleCmd();
const NotEqualBoolCmd &kNotEqualBoolCmd = *new NotEqualBoolCmd();
const NotEqualCharCmd &kNotEqualCharCmd = *new NotEqualCharCmd();
const NotEqualStringCmd &kNotEqualStringCmd = *new NotEqualStringCmd();

NotEqualIntArrayCmd &kNotEqualIntArrayCmd =
    *new NotEqualIntArrayCmd(UINT8_C(1));
NotEqualLongArrayCmd &kNotEqualLongArrayCmd =
    *new NotEqualLongArrayCmd(UINT8_C(1));
NotEqualDoubleArrayCmd &kNotEqualDoubleArrayCmd =
    *new NotEqualDoubleArrayCmd(UINT8_C(1));
NotEqualBoolArrayCmd &kNotEqualBoolArrayCmd =
    *new NotEqualBoolArrayCmd(UINT8_C(1));
NotEqualCharArrayCmd &kNotEqualCharArrayCmd =
    *new NotEqualCharArrayCmd(UINT8_C(1));
NotEqualStringArrayCmd &kNotEqualStringArrayCmd =
    *new NotEqualStringArrayCmd(UINT8_C(1));

const GreaterCharCmd &kGreaterCharCmd = *new GreaterCharCmd();
const GreaterIntCmd &kGreaterIntCmd = *new GreaterIntCmd();
const GreaterLongCmd &kGreaterLongCmd = *new GreaterLongCmd();
const GreaterDoubleCmd &kGreaterDoubleCmd = *new GreaterDoubleCmd();

const GreaterOrEqualCharCmd &kGreaterOrEqualCharCmd =
    *new GreaterOrEqualCharCmd();
const GreaterOrEqualIntCmd &kGreaterOrEqualIntCmd = *new GreaterOrEqualIntCmd();
const GreaterOrEqualLongCmd &kGreaterOrEqualLongCmd =
    *new GreaterOrEqualLongCmd();
const GreaterOrEqualDoubleCmd &kGreaterOrEqualDoubleCmd =
    *new GreaterOrEqualDoubleCmd();

const LessCharCmd &kLessCharCmd = *new LessCharCmd();
const LessIntCmd &kLessIntCmd = *new LessIntCmd();
const LessLongCmd &kLessLongCmd = *new LessLongCmd();
const LessDoubleCmd &kLessDoubleCmd = *new LessDoubleCmd();

const LessOrEqualCharCmd &kLessOrEqualCharCmd =
    *new LessOrEqualCharCmd();
const LessOrEqualIntCmd &kLessOrEqualIntCmd = *new LessOrEqualIntCmd();
const LessOrEqualLongCmd &kLessOrEqualLongCmd =
    *new LessOrEqualLongCmd();
const LessOrEqualDoubleCmd &kLessOrEqualDoubleCmd =
    *new LessOrEqualDoubleCmd();

const LogicalNegateBoolCmd &kLogicalNegateBoolCmd = *new LogicalNegateBoolCmd();

const PreDecCharCmd &kPreDecCharCmd = *new PreDecCharCmd();
const PreDecIntCmd &kPreDecIntCmd = *new PreDecIntCmd();
const PreDecLongCmd &kPreDecLongCmd = *new PreDecLongCmd();
const PreDecDoubleCmd &kPreDecDoubleCmd = *new PreDecDoubleCmd();

const PreIncCharCmd &kPreIncCharCmd = *new PreIncCharCmd();
const PreIncIntCmd &kPreIncIntCmd = *new PreIncIntCmd();
const PreIncLongCmd &kPreIncLongCmd = *new PreIncLongCmd();
const PreIncDoubleCmd &kPreIncDoubleCmd = *new PreIncDoubleCmd();

const ArithmeticNegateIntCmd &kArithmeticNegateIntCmd =
    *new ArithmeticNegateIntCmd();
const ArithmeticNegateLongCmd &kArithmeticNegateLongCmd =
    *new ArithmeticNegateLongCmd();
const ArithmeticNegateDoubleCmd &kArithmeticNegateDoubleCmd =
    *new ArithmeticNegateDoubleCmd();
}

void CmdReader::SetCode(Code *code) noexcept {
  assert(code);
  code_ = code;
}

Code *CmdReader::GetCode() const noexcept {
  return code_;
}

const Cmd &CmdReader::GetNextCmd() noexcept {
  const Cmd *cmd = nullptr;

  switch (code_->Read<CmdId>()) {
    case CmdId::kCreateGlobalIntVar:
      kCreateGlobalIntVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalIntVarCmd;
      break;
    case CmdId::kCreateGlobalArrayVar:
      kCreateGlobalArrayVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalArrayVarCmd;
      break;
    case CmdId::kCreateGlobalLongVar:
      kCreateGlobalLongVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalLongVarCmd;
      break;
    case CmdId::kCreateGlobalDoubleVar:
      kCreateGlobalDoubleVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalDoubleVarCmd;
      break;
    case CmdId::kCreateGlobalCharVar:
      kCreateGlobalCharVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalCharVarCmd;
      break;
    case CmdId::kCreateGlobalStringVar:
      kCreateGlobalStringVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalStringVarCmd;
      break;
    case CmdId::kCreateGlobalBoolVar:
      kCreateGlobalBoolVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateGlobalBoolVarCmd;
      break;
    case CmdId::kCreateLocalIntVar:
      cmd = &kCreateLocalIntVarCmd;
      break;
    case CmdId::kCreateLocalArrayVar:
      cmd = &kCreateLocalArrayVarCmd;
      break;
    case CmdId::kCreateLocalLongVar:
      cmd = &kCreateLocalLongVarCmd;
      break;
    case CmdId::kCreateLocalDoubleVar:
      cmd = &kCreateLocalDoubleVarCmd;
      break;
    case CmdId::kCreateLocalCharVar:
      cmd = &kCreateLocalCharVarCmd;
      break;
    case CmdId::kCreateLocalStringVar:
      cmd = &kCreateLocalStringVarCmd;
      break;
    case CmdId::kCreateLocalBoolVar:
      cmd = &kCreateLocalBoolVarCmd;
      break;
    case CmdId::kDestroyLocalIntVar:
      cmd = &kDestroyLocalIntVarCmd;
      break;
    case CmdId::kDestroyLocalLongVar:
      cmd = &kDestroyLocalLongVarCmd;
      break;
    case CmdId::kDestroyLocalDoubleVar:
      cmd = &kDestroyLocalDoubleVarCmd;
      break;
    case CmdId::kDestroyLocalCharVar:
      cmd = &kDestroyLocalCharVarCmd;
      break;
    case CmdId::kDestroyLocalStringVar:
      cmd = &kDestroyLocalStringVarCmd;
      break;
    case CmdId::kDestroyLocalBoolVar:
      cmd = &kDestroyLocalBoolVarCmd;
      break;
    case CmdId::kDestroyLocalIntArrayVar:
      kDestroyLocalIntArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kDestroyLocalIntArrayVarCmd;
      break;
    case CmdId::kDestroyLocalLongArrayVar:
      kDestroyLocalLongArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kDestroyLocalLongArrayVarCmd;
      break;
    case CmdId::kDestroyLocalDoubleArrayVar:
      kDestroyLocalDoubleArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kDestroyLocalDoubleArrayVarCmd;
      break;
    case CmdId::kDestroyLocalCharArrayVar:
      kDestroyLocalCharArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kDestroyLocalCharArrayVarCmd;
      break;
    case CmdId::kDestroyLocalStringArrayVar:
      kDestroyLocalStringArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kDestroyLocalStringArrayVarCmd;
      break;
    case CmdId::kDestroyLocalBoolArrayVar:
      kDestroyLocalBoolArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kDestroyLocalBoolArrayVarCmd;
      break;
    case CmdId::kUnloadInt:
      cmd = &kUnloadIntCmd;
      break;
    case CmdId::kUnloadLong:
      cmd = &kUnloadLongCmd;
      break;
    case CmdId::kUnloadDouble:
      cmd = &kUnloadDoubleCmd;
      break;
    case CmdId::kUnloadChar:
      cmd = &kUnloadCharCmd;
      break;
    case CmdId::kUnloadString:
      cmd = &kUnloadStringCmd;
      break;
    case CmdId::kUnloadBool:
      cmd = &kUnloadBoolCmd;
      break;
    case CmdId::kUnloadIntArray:
      kUnloadIntArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kUnloadIntArrayCmd;
      break;
    case CmdId::kUnloadLongArray:
      kUnloadLongArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kUnloadLongArrayCmd;
      break;
    case CmdId::kUnloadDoubleArray:
      kUnloadDoubleArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kUnloadDoubleArrayCmd;
      break;
    case CmdId::kUnloadCharArray:
      kUnloadCharArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kUnloadCharArrayCmd;
      break;
    case CmdId::kUnloadStringArray:
      kUnloadStringArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kUnloadStringArrayCmd;
      break;
    case CmdId::kUnloadBoolArray:
      kUnloadBoolArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kUnloadBoolArrayCmd;
      break;
    case CmdId::kLoadIntValue:
      kLoadIntValueCmd.SetValue(code_->Read<int32_t>());
      cmd = &kLoadIntValueCmd;
      break;
    case CmdId::kLoadLongValue:
      kLoadLongValueCmd.SetValue(code_->Read<int64_t>());
      cmd = &kLoadLongValueCmd;
      break;
    case CmdId::kLoadBoolValue:
      kLoadBoolValueCmd.SetValue(code_->Read<bool>());
      cmd = &kLoadBoolValueCmd;
      break;
    case CmdId::kLoadCharValue:
      kLoadCharValueCmd.SetValue(code_->Read<char>());
      cmd = &kLoadCharValueCmd;
      break;
    case CmdId::kLoadStringValue:
      kLoadStringValueCmd.SetValue(code_->Read<string>());
      cmd = &kLoadStringValueCmd;
      break;
    case CmdId::kLoadDoubleValue:
      kLoadDoubleValueCmd.SetValue(code_->Read<double>());
      cmd = &kLoadDoubleValueCmd;
      break;
    case CmdId::kCreateAndInitGlobalIntVar:
      kCreateAndInitGlobalIntVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateAndInitGlobalIntVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalLongVar:
      kCreateAndInitGlobalLongVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateAndInitGlobalLongVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalDoubleVar:
      kCreateAndInitGlobalDoubleVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateAndInitGlobalDoubleVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalCharVar:
      kCreateAndInitGlobalCharVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateAndInitGlobalCharVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalStringVar:
      kCreateAndInitGlobalStringVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateAndInitGlobalStringVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalBoolVar:
      kCreateAndInitGlobalBoolVarCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kCreateAndInitGlobalBoolVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalIntArrayVar:
      ReadCreateAndInitGlobalArrayVarCmd(kCreateAndInitGlobalIntArrayVarCmd);
      cmd = &kCreateAndInitGlobalIntArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalLongArrayVar:
      ReadCreateAndInitGlobalArrayVarCmd(kCreateAndInitGlobalLongArrayVarCmd);
      cmd = &kCreateAndInitGlobalLongArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalDoubleArrayVar:
      ReadCreateAndInitGlobalArrayVarCmd(kCreateAndInitGlobalDoubleArrayVarCmd);
      cmd = &kCreateAndInitGlobalDoubleArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalCharArrayVar:
      ReadCreateAndInitGlobalArrayVarCmd(kCreateAndInitGlobalCharArrayVarCmd);
      cmd = &kCreateAndInitGlobalCharArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalStringArrayVar:
      ReadCreateAndInitGlobalArrayVarCmd(kCreateAndInitGlobalStringArrayVarCmd);
      cmd = &kCreateAndInitGlobalStringArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalBoolArrayVar:
      ReadCreateAndInitGlobalArrayVarCmd(kCreateAndInitGlobalBoolArrayVarCmd);
      cmd = &kCreateAndInitGlobalBoolArrayVarCmd;
      break;
    case CmdId::kCreateAndInitLocalIntVar:
      cmd = &kCreateAndInitLocalIntVarCmd;
      break;
    case CmdId::kCreateAndInitLocalLongVar:
      cmd = &kCreateAndInitLocalLongVarCmd;
      break;
    case CmdId::kCreateAndInitLocalDoubleVar:
      cmd = &kCreateAndInitLocalDoubleVarCmd;
      break;
    case CmdId::kCreateAndInitLocalCharVar:
      cmd = &kCreateAndInitLocalCharVarCmd;
      break;
    case CmdId::kCreateAndInitLocalStringVar:
      cmd = &kCreateAndInitLocalStringVarCmd;
      break;
    case CmdId::kCreateAndInitLocalBoolVar:
      cmd = &kCreateAndInitLocalBoolVarCmd;
      break;
    case CmdId::kCreateAndInitLocalIntArrayVar:
      kCreateAndInitLocalIntArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kCreateAndInitLocalIntArrayVarCmd;
      break;
    case CmdId::kCreateAndInitLocalLongArrayVar:
      kCreateAndInitLocalLongArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kCreateAndInitLocalLongArrayVarCmd;
      break;
    case CmdId::kCreateAndInitLocalDoubleArrayVar:
      kCreateAndInitLocalDoubleArrayVarCmd.SetDimensionsCount(
          code_->Read<uint8_t>());
      cmd = &kCreateAndInitLocalDoubleArrayVarCmd;
      break;
    case CmdId::kCreateAndInitLocalCharArrayVar:
      kCreateAndInitLocalCharArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kCreateAndInitLocalCharArrayVarCmd;
      break;
    case CmdId::kCreateAndInitLocalStringArrayVar:
      kCreateAndInitLocalStringArrayVarCmd.SetDimensionsCount(
          code_->Read<uint8_t>());
      cmd = &kCreateAndInitLocalStringArrayVarCmd;
      break;
    case CmdId::kCreateAndInitLocalBoolArrayVar:
      kCreateAndInitLocalBoolArrayVarCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kCreateAndInitLocalBoolArrayVarCmd;
      break;
    case CmdId::kCreateIntArray:
      ReadCreateArrayCmd(kCreateIntArrayCmd);
      cmd = &kCreateIntArrayCmd;
      break;
    case CmdId::kCreateLongArray:
      ReadCreateArrayCmd(kCreateLongArrayCmd);
      cmd = &kCreateLongArrayCmd;
      break;
    case CmdId::kCreateDoubleArray:
      ReadCreateArrayCmd(kCreateDoubleArrayCmd);
      cmd = &kCreateDoubleArrayCmd;
      break;
    case CmdId::kCreateBoolArray:
      ReadCreateArrayCmd(kCreateBoolArrayCmd);
      cmd = &kCreateBoolArrayCmd;
      break;
    case CmdId::kCreateCharArray:
      ReadCreateArrayCmd(kCreateCharArrayCmd);
      cmd = &kCreateCharArrayCmd;
      break;
    case CmdId::kCreateStringArray:
      ReadCreateArrayCmd(kCreateStringArrayCmd);
      cmd = &kCreateStringArrayCmd;
      break;
    case CmdId::kCreateAndInitIntArray:
      ReadCreateAndInitArrayCmd(kCreateAndInitIntArrayCmd);
      cmd = &kCreateAndInitIntArrayCmd;
      break;
    case CmdId::kCreateAndInitLongArray:
      ReadCreateAndInitArrayCmd(kCreateAndInitLongArrayCmd);
      cmd = &kCreateAndInitLongArrayCmd;
      break;
    case CmdId::kCreateAndInitDoubleArray:
      ReadCreateAndInitArrayCmd(kCreateAndInitDoubleArrayCmd);
      cmd = &kCreateAndInitDoubleArrayCmd;
      break;
    case CmdId::kCreateAndInitBoolArray:
      ReadCreateAndInitArrayCmd(kCreateAndInitBoolArrayCmd);
      cmd = &kCreateAndInitBoolArrayCmd;
      break;
    case CmdId::kCreateAndInitCharArray:
      ReadCreateAndInitArrayCmd(kCreateAndInitCharArrayCmd);
      cmd = &kCreateAndInitCharArrayCmd;
      break;
    case CmdId::kCreateAndInitStringArray:
      ReadCreateAndInitArrayCmd(kCreateAndInitStringArrayCmd);
      cmd = &kCreateAndInitStringArrayCmd;
      break;
    case CmdId::kJumpIfNot:
      ReadJumpCmd(kJumpIfNotCmd);
      cmd = &kJumpIfNotCmd;
      break;
    case CmdId::kImplicitJumpIfNot:
      ReadJumpCmd(kImplicitJumpIfNotCmd);
      cmd = &kImplicitJumpIfNotCmd;
      break;
    case CmdId::kImplicitJumpIf:
      ReadJumpCmd(kImplicitJumpIfCmd);
      cmd = &kImplicitJumpIfCmd;
      break;
    case CmdId::kDirectJump:
      ReadJumpCmd(kDirectJumpCmd);
      cmd = &kDirectJumpCmd;
      break;
    case CmdId::kReturn:
      cmd = &kReturnCmd;
      break;
    case CmdId::kLoadGlobalIntVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalIntVarValueCmd);
      cmd = &kLoadGlobalIntVarValueCmd;
      break;
    case CmdId::kLoadGlobalLongVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalLongVarValueCmd);
      cmd = &kLoadGlobalLongVarValueCmd;
      break;
    case CmdId::kLoadGlobalDoubleVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalDoubleVarValueCmd);
      cmd = &kLoadGlobalDoubleVarValueCmd;
      break;
    case CmdId::kLoadGlobalCharVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalCharVarValueCmd);
      cmd = &kLoadGlobalCharVarValueCmd;
      break;
    case CmdId::kLoadGlobalStringVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalStringVarValueCmd);
      cmd = &kLoadGlobalStringVarValueCmd;
      break;
    case CmdId::kLoadGlobalBoolVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalBoolVarValueCmd);
      cmd = &kLoadGlobalBoolVarValueCmd;
      break;
    case CmdId::kLoadGlobalIntArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalIntArrayVarValueCmd);
      cmd = &kLoadGlobalIntArrayVarValueCmd;
      break;
    case CmdId::kLoadGlobalLongArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalLongArrayVarValueCmd);
      cmd = &kLoadGlobalLongArrayVarValueCmd;
      break;
    case CmdId::kLoadGlobalDoubleArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalDoubleArrayVarValueCmd);
      cmd = &kLoadGlobalDoubleArrayVarValueCmd;
      break;
    case CmdId::kLoadGlobalCharArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalCharArrayVarValueCmd);
      cmd = &kLoadGlobalCharArrayVarValueCmd;
      break;
    case CmdId::kLoadGlobalStringArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalStringArrayVarValueCmd);
      cmd = &kLoadGlobalStringArrayVarValueCmd;
      break;
    case CmdId::kLoadGlobalBoolArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalBoolArrayVarValueCmd);
      cmd = &kLoadGlobalBoolArrayVarValueCmd;
      break;
    case CmdId::kLoadGlobalVarAddress:
      kLoadGlobalVarAddressCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kLoadGlobalVarAddressCmd;
      break;
    case CmdId::kLoadLocalVarAddress:
      kLoadLocalVarAddressCmd.SetVarIndex(code_->Read<uint32_t>());
      cmd = &kLoadLocalVarAddressCmd;
      break;
    case CmdId::kLoadFuncValue:
      kLoadFuncValueCmd.SetAddress(code_->Read<uint32_t>());
      cmd = &kLoadFuncValueCmd;
      break;
    case CmdId::kLoadNativeFuncValue:
      kLoadNativeFuncValueCmd.SetFuncIndex(code_->Read<uint32_t>());
      cmd = &kLoadNativeFuncValueCmd;
      break;
    case CmdId::kLoadLocalIntVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalIntVarValueCmd);
      cmd = &kLoadLocalIntVarValueCmd;
      break;
    case CmdId::kLoadLocalLongVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalLongVarValueCmd);
      cmd = &kLoadLocalLongVarValueCmd;
      break;
    case CmdId::kLoadLocalDoubleVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalDoubleVarValueCmd);
      cmd = &kLoadLocalDoubleVarValueCmd;
      break;
    case CmdId::kLoadLocalCharVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalCharVarValueCmd);
      cmd = &kLoadLocalCharVarValueCmd;
      break;
    case CmdId::kLoadLocalStringVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalStringVarValueCmd);
      cmd = &kLoadLocalStringVarValueCmd;
      break;
    case CmdId::kLoadLocalBoolVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalBoolVarValueCmd);
      cmd = &kLoadLocalBoolVarValueCmd;
      break;
    case CmdId::kLoadLocalIntArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalIntArrayVarValueCmd);
      cmd = &kLoadLocalIntArrayVarValueCmd;
      break;
    case CmdId::kLoadLocalLongArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalLongArrayVarValueCmd);
      cmd = &kLoadLocalLongArrayVarValueCmd;
      break;
    case CmdId::kLoadLocalDoubleArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalDoubleArrayVarValueCmd);
      cmd = &kLoadLocalDoubleArrayVarValueCmd;
      break;
    case CmdId::kLoadLocalCharArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalCharArrayVarValueCmd);
      cmd = &kLoadLocalCharArrayVarValueCmd;
      break;
    case CmdId::kLoadLocalStringArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalStringArrayVarValueCmd);
      cmd = &kLoadLocalStringArrayVarValueCmd;
      break;
    case CmdId::kLoadLocalBoolArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalBoolArrayVarValueCmd);
      cmd = &kLoadLocalBoolArrayVarValueCmd;
      break;
    case CmdId::kStoreInt:
      cmd = &kStoreIntCmd;
      break;
    case CmdId::kStoreLong:
      cmd = &kStoreLongCmd;
      break;
    case CmdId::kStoreDouble:
      cmd = &kStoreDoubleCmd;
      break;
    case CmdId::kStoreBool:
      cmd = &kStoreBoolCmd;
      break;
    case CmdId::kStoreChar:
      cmd = &kStoreCharCmd;
      break;
    case CmdId::kStoreString:
      cmd = &kStoreStringCmd;
      break;
    case CmdId::kStoreIntArray:
      kStoreIntArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kStoreIntArrayCmd;
      break;
    case CmdId::kStoreLongArray:
      kStoreLongArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kStoreLongArrayCmd;
      break;
    case CmdId::kStoreDoubleArray:
      kStoreDoubleArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kStoreDoubleArrayCmd;
      break;
    case CmdId::kStoreBoolArray:
      kStoreBoolArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kStoreBoolArrayCmd;
      break;
    case CmdId::kStoreCharArray:
      kStoreCharArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kStoreCharArrayCmd;
      break;
    case CmdId::kStoreStringArray:
      kStoreStringArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kStoreStringArrayCmd;
      break;
    case CmdId::kCastCharToInt:
      cmd = &kCastCharToIntCmd;
      break;
    case CmdId::kCastCharToLong:
      cmd = &kCastCharToLongCmd;
      break;
    case CmdId::kCastCharToDouble:
      cmd = &kCastCharToDoubleCmd;
      break;
    case CmdId::kCastCharToString:
      cmd = &kCastCharToStringCmd;
      break;
    case CmdId::kCastIntToLong:
      cmd = &kCastIntToLongCmd;
      break;
    case CmdId::kCastIntToDouble:
      cmd = &kCastIntToDoubleCmd;
      break;
    case CmdId::kCastLongToDouble:
      cmd = &kCastLongToDoubleCmd;
      break;
    case CmdId::kCall:
      cmd = &kCallCmd;
      break;
    case CmdId::kCallNative:
      cmd = &kCallNativeCmd;
      break;
    case CmdId::kLoadIntArrayElementValue:
      kLoadIntArrayElementValueCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadIntArrayElementValueCmd;
      break;
    case CmdId::kLoadLongArrayElementValue:
      kLoadLongArrayElementValueCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadLongArrayElementValueCmd;
      break;
    case CmdId::kLoadDoubleArrayElementValue:
      kLoadDoubleArrayElementValueCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadDoubleArrayElementValueCmd;
      break;
    case CmdId::kLoadBoolArrayElementValue:
      kLoadBoolArrayElementValueCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadBoolArrayElementValueCmd;
      break;
    case CmdId::kLoadCharArrayElementValue:
      kLoadCharArrayElementValueCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadCharArrayElementValueCmd;
      break;
    case CmdId::kLoadStringArrayElementValue:
      kLoadStringArrayElementValueCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadStringArrayElementValueCmd;
      break;
    case CmdId::kLoadIntArrayElementAddress:
      kLoadIntArrayElementAddressCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadIntArrayElementAddressCmd;
      break;
    case CmdId::kLoadLongArrayElementAddress:
      kLoadLongArrayElementAddressCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadLongArrayElementAddressCmd;
      break;
    case CmdId::kLoadDoubleArrayElementAddress:
      kLoadDoubleArrayElementAddressCmd.SetDimensionsCount(
          code_->Read<uint8_t>());
      cmd = &kLoadDoubleArrayElementAddressCmd;
      break;
    case CmdId::kLoadBoolArrayElementAddress:
      kLoadBoolArrayElementAddressCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadBoolArrayElementAddressCmd;
      break;
    case CmdId::kLoadCharArrayElementAddress:
      kLoadCharArrayElementAddressCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kLoadCharArrayElementAddressCmd;
      break;
    case CmdId::kLoadStringArrayElementAddress:
      kLoadStringArrayElementAddressCmd.SetDimensionsCount(
          code_->Read<uint8_t>());
      cmd = &kLoadStringArrayElementAddressCmd;
      break;
    case CmdId::kAnd:
      cmd = &kAndCmd;
      break;
    case CmdId::kOr:
      cmd = &kOrCmd;
      break;
    case CmdId::kMulChar:
      cmd = &kMulCharCmd;
      break;
    case CmdId::kMulInt:
      cmd = &kMulIntCmd;
      break;
    case CmdId::kMulLong:
      cmd = &kMulLongCmd;
      break;
    case CmdId::kMulDouble:
      cmd = &kMulDoubleCmd;
      break;
    case CmdId::kDivChar:
      cmd = &kDivCharCmd;
      break;
    case CmdId::kDivInt:
      cmd = &kDivIntCmd;
      break;
    case CmdId::kDivLong:
      cmd = &kDivLongCmd;
      break;
    case CmdId::kDivDouble:
      cmd = &kDivDoubleCmd;
      break;
    case CmdId::kSumChar:
      cmd = &kSumCharCmd;
      break;
    case CmdId::kSumInt:
      cmd = &kSumIntCmd;
      break;
    case CmdId::kSumLong:
      cmd = &kSumLongCmd;
      break;
    case CmdId::kSumDouble:
      cmd = &kSumDoubleCmd;
      break;
    case CmdId::kSumString:
      cmd = &kSumStringCmd;
      break;
    case CmdId::kSubChar:
      cmd = &kSubCharCmd;
      break;
    case CmdId::kSubInt:
      cmd = &kSubIntCmd;
      break;
    case CmdId::kSubLong:
      cmd = &kSubLongCmd;
      break;
    case CmdId::kSubDouble:
      cmd = &kSubDoubleCmd;
      break;
    case CmdId::kEqualInt:
      cmd = &kEqualIntCmd;
      break;
    case CmdId::kEqualLong:
      cmd = &kEqualLongCmd;
      break;
    case CmdId::kEqualDouble:
      cmd = &kEqualDoubleCmd;
      break;
    case CmdId::kEqualBool:
      cmd = &kEqualBoolCmd;
      break;
    case CmdId::kEqualChar:
      cmd = &kEqualCharCmd;
      break;
    case CmdId::kEqualString:
      cmd = &kEqualStringCmd;
      break;
    case CmdId::kEqualIntArray:
      kEqualIntArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kEqualIntArrayCmd;
      break;
    case CmdId::kEqualLongArray:
      kEqualLongArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kEqualLongArrayCmd;
      break;
    case CmdId::kEqualDoubleArray:
      kEqualDoubleArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kEqualDoubleArrayCmd;
      break;
    case CmdId::kEqualBoolArray:
      kEqualBoolArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kEqualBoolArrayCmd;
      break;
    case CmdId::kEqualCharArray:
      kEqualCharArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kEqualCharArrayCmd;
      break;
    case CmdId::kEqualStringArray:
      kEqualStringArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kEqualStringArrayCmd;
      break;
    case CmdId::kNotEqualInt:
      cmd = &kNotEqualIntCmd;
      break;
    case CmdId::kNotEqualLong:
      cmd = &kNotEqualLongCmd;
      break;
    case CmdId::kNotEqualDouble:
      cmd = &kNotEqualDoubleCmd;
      break;
    case CmdId::kNotEqualBool:
      cmd = &kNotEqualBoolCmd;
      break;
    case CmdId::kNotEqualChar:
      cmd = &kNotEqualCharCmd;
      break;
    case CmdId::kNotEqualString:
      cmd = &kNotEqualStringCmd;
      break;
    case CmdId::kNotEqualIntArray:
      kNotEqualIntArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kNotEqualIntArrayCmd;
      break;
    case CmdId::kNotEqualLongArray:
      kNotEqualLongArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kNotEqualLongArrayCmd;
      break;
    case CmdId::kNotEqualDoubleArray:
      kNotEqualDoubleArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kNotEqualDoubleArrayCmd;
      break;
    case CmdId::kNotEqualBoolArray:
      kNotEqualBoolArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kNotEqualBoolArrayCmd;
      break;
    case CmdId::kNotEqualCharArray:
      kNotEqualCharArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kNotEqualCharArrayCmd;
      break;
    case CmdId::kNotEqualStringArray:
      kNotEqualStringArrayCmd.SetDimensionsCount(code_->Read<uint8_t>());
      cmd = &kNotEqualStringArrayCmd;
      break;
    case CmdId::kGreaterChar:
      cmd = &kGreaterCharCmd;
      break;
    case CmdId::kGreaterInt:
      cmd = &kGreaterIntCmd;
      break;
    case CmdId::kGreaterLong:
      cmd = &kGreaterLongCmd;
      break;
    case CmdId::kGreaterDouble:
      cmd = &kGreaterDoubleCmd;
      break;
    case CmdId::kGreaterOrEqualChar:
      cmd = &kGreaterOrEqualCharCmd;
      break;
    case CmdId::kGreaterOrEqualInt:
      cmd = &kGreaterOrEqualIntCmd;
      break;
    case CmdId::kGreaterOrEqualLong:
      cmd = &kGreaterOrEqualLongCmd;
      break;
    case CmdId::kGreaterOrEqualDouble:
      cmd = &kGreaterOrEqualDoubleCmd;
      break;
    case CmdId::kLessChar:
      cmd = &kLessCharCmd;
      break;
    case CmdId::kLessInt:
      cmd = &kLessIntCmd;
      break;
    case CmdId::kLessLong:
      cmd = &kLessLongCmd;
      break;
    case CmdId::kLessDouble:
      cmd = &kLessDoubleCmd;
      break;
    case CmdId::kLessOrEqualChar:
      cmd = &kLessOrEqualCharCmd;
      break;
    case CmdId::kLessOrEqualInt:
      cmd = &kLessOrEqualIntCmd;
      break;
    case CmdId::kLessOrEqualLong:
      cmd = &kLessOrEqualLongCmd;
      break;
    case CmdId::kLessOrEqualDouble:
      cmd = &kLessOrEqualDoubleCmd;
      break;
    case CmdId::kLogicalNegateBool:
      cmd = &kLogicalNegateBoolCmd;
      break;
    case CmdId::kPreDecChar:
      cmd = &kPreDecCharCmd;
      break;
    case CmdId::kPreDecInt:
      cmd = &kPreDecIntCmd;
      break;
    case CmdId::kPreDecLong:
      cmd = &kPreDecLongCmd;
      break;
    case CmdId::kPreDecDouble:
      cmd = &kPreDecDoubleCmd;
      break;
    case CmdId::kPreIncChar:
      cmd = &kPreIncCharCmd;
      break;
    case CmdId::kPreIncInt:
      cmd = &kPreIncIntCmd;
      break;
    case CmdId::kPreIncLong:
      cmd = &kPreIncLongCmd;
      break;
    case CmdId::kPreIncDouble:
      cmd = &kPreIncDoubleCmd;
      break;
    case CmdId::kArithmeticNegateInt:
      cmd = &kArithmeticNegateIntCmd;
      break;
    case CmdId::kArithmeticNegateLong:
      cmd = &kArithmeticNegateLongCmd;
      break;
    case CmdId::kArithmeticNegateDouble:
      cmd = &kArithmeticNegateDoubleCmd;
      break;
  }

  assert(cmd != nullptr);
  return *cmd;
}

inline void CmdReader::ReadCreateArrayCmd(CreateArrayCmd &cmd) noexcept {
  cmd.SetDimensionsCount(code_->Read<uint8_t>());
}

inline void CmdReader::ReadCreateAndInitArrayCmd(
    CreateAndInitArrayCmd &cmd) noexcept {
  cmd.SetDimensionsCount(code_->Read<uint8_t>());
  cmd.SetValuesCount(code_->Read<int32_t>());
}

inline void CmdReader::ReadJumpCmd(JumpCmd &cmd) noexcept {
  cmd.SetOffset(code_->Read<int32_t>());
}

inline void CmdReader::ReadLoadGlobalVarValueCmd(
    LoadGlobalVarValueCmd &cmd) noexcept {
  cmd.SetVarIndex(code_->Read<uint32_t>());
}

inline void CmdReader::ReadLoadLocalVarValueCmd(
    LoadLocalVarValueCmd &cmd) noexcept {
  cmd.SetVarIndex(code_->Read<uint32_t>());
}

inline void CmdReader::ReadCreateAndInitGlobalArrayVarCmd(
    CreateAndInitGlobalArrayVarCmd &cmd) noexcept {
  cmd.SetVarIndex(code_->Read<uint32_t>());
  cmd.SetDimensionsCount(code_->Read<uint8_t>());
}
}
}
