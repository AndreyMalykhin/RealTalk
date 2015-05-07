
#include <cassert>
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/create_local_var_cmd.h"
#include "real_talk/code/unload_cmd.h"
#include "real_talk/code/load_value_cmd.h"
#include "real_talk/code/create_and_init_global_var_cmd.h"
#include "real_talk/code/create_and_init_local_var_cmd.h"
#include "real_talk/code/create_array_cmd.h"
#include "real_talk/code/create_and_init_array_cmd.h"
#include "real_talk/code/jump_cmd.h"
#include "real_talk/code/destroy_local_vars_and_jump_cmd.h"
#include "real_talk/code/destroy_local_vars_cmd.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/return_cmd.h"
#include "real_talk/code/load_global_var_value_cmd.h"
#include "real_talk/code/load_local_var_value_cmd.h"
#include "real_talk/code/load_global_var_address_cmd.h"
#include "real_talk/code/load_local_var_address_cmd.h"
#include "real_talk/code/load_element_value_cmd.h"
#include "real_talk/code/load_element_address_cmd.h"
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
#include "real_talk/code/code.h"

namespace real_talk {
namespace code {
namespace {

const CreateGlobalIntVarCmd &kCreateGlobalIntVarCmd =
    *new CreateGlobalIntVarCmd();
const CreateGlobalArrayVarCmd &kCreateGlobalArrayVarCmd =
    *new CreateGlobalArrayVarCmd();
const CreateGlobalLongVarCmd &kCreateGlobalLongVarCmd =
    *new CreateGlobalLongVarCmd();
const CreateGlobalDoubleVarCmd &kCreateGlobalDoubleVarCmd =
    *new CreateGlobalDoubleVarCmd();
const CreateGlobalCharVarCmd &kCreateGlobalCharVarCmd =
    *new CreateGlobalCharVarCmd();
const CreateGlobalStringVarCmd &kCreateGlobalStringVarCmd =
    *new CreateGlobalStringVarCmd();
const CreateGlobalBoolVarCmd &kCreateGlobalBoolVarCmd =
    *new CreateGlobalBoolVarCmd();

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

const UnloadCmd &kUnloadCmd = *new UnloadCmd();

LoadIntValueCmd &kLoadIntValueCmd = *new LoadIntValueCmd(INT32_C(0));
LoadLongValueCmd &kLoadLongValueCmd = *new LoadLongValueCmd(INT64_C(0));
LoadBoolValueCmd &kLoadBoolValueCmd = *new LoadBoolValueCmd(false);
LoadCharValueCmd &kLoadCharValueCmd = *new LoadCharValueCmd('\0');
LoadStringValueCmd &kLoadStringValueCmd = *new LoadStringValueCmd("");
LoadDoubleValueCmd &kLoadDoubleValueCmd = *new LoadDoubleValueCmd(0.0);
LoadFuncValueCmd &kLoadFuncValueCmd = *new LoadFuncValueCmd(UINT32_C(0));
LoadNativeFuncValueCmd &kLoadNativeFuncValueCmd =
    *new LoadNativeFuncValueCmd(UINT32_C(0));

const CreateAndInitGlobalIntVarCmd &kCreateAndInitGlobalIntVarCmd =
    *new CreateAndInitGlobalIntVarCmd();
const CreateAndInitGlobalArrayVarCmd &kCreateAndInitGlobalArrayVarCmd =
    *new CreateAndInitGlobalArrayVarCmd();
const CreateAndInitGlobalLongVarCmd &kCreateAndInitGlobalLongVarCmd =
    *new CreateAndInitGlobalLongVarCmd();
const CreateAndInitGlobalDoubleVarCmd &kCreateAndInitGlobalDoubleVarCmd =
    *new CreateAndInitGlobalDoubleVarCmd();
const CreateAndInitGlobalCharVarCmd &kCreateAndInitGlobalCharVarCmd =
    *new CreateAndInitGlobalCharVarCmd();
const CreateAndInitGlobalStringVarCmd &kCreateAndInitGlobalStringVarCmd =
    *new CreateAndInitGlobalStringVarCmd();
const CreateAndInitGlobalBoolVarCmd &kCreateAndInitGlobalBoolVarCmd =
    *new CreateAndInitGlobalBoolVarCmd();

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
const CreateAndInitLocalArrayVarCmd &kCreateAndInitLocalArrayVarCmd =
    *new CreateAndInitLocalArrayVarCmd();
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

JumpIfNotCmd &kJumpIfNotCmd = *new JumpIfNotCmd(INT32_C(0));
ImplicitJumpIfNotCmd &kImplicitJumpIfNotCmd =
    *new ImplicitJumpIfNotCmd(INT32_C(0));
ImplicitJumpIfCmd &kImplicitJumpIfCmd =
    *new ImplicitJumpIfCmd(INT32_C(0));
DirectJumpCmd &kDirectJumpCmd = *new DirectJumpCmd(INT32_C(0));

DestroyLocalVarsAndJumpCmd &kDestroyLocalVarsAndJumpCmd =
    *new DestroyLocalVarsAndJumpCmd(UINT32_C(1), INT32_C(0));
DestroyLocalVarsCmd &kDestroyLocalVarsCmd =
    *new DestroyLocalVarsCmd(UINT32_C(1));

const ReturnCmd &kReturnCmd = *new ReturnCmd();
const ReturnIntValueCmd &kReturnIntValueCmd = *new ReturnIntValueCmd();
const ReturnLongValueCmd &kReturnLongValueCmd = *new ReturnLongValueCmd();
const ReturnDoubleValueCmd &kReturnDoubleValueCmd = *new ReturnDoubleValueCmd();
const ReturnCharValueCmd &kReturnCharValueCmd = *new ReturnCharValueCmd();
const ReturnStringValueCmd &kReturnStringValueCmd = *new ReturnStringValueCmd();
const ReturnBoolValueCmd &kReturnBoolValueCmd = *new ReturnBoolValueCmd();
const ReturnArrayValueCmd &kReturnArrayValueCmd = *new ReturnArrayValueCmd();

LoadGlobalIntVarValueCmd &kLoadGlobalIntVarValueCmd =
    *new LoadGlobalIntVarValueCmd(UINT32_C(0));
LoadGlobalArrayVarValueCmd &kLoadGlobalArrayVarValueCmd =
    *new LoadGlobalArrayVarValueCmd(UINT32_C(0));
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

LoadLocalIntVarValueCmd &kLoadLocalIntVarValueCmd =
    *new LoadLocalIntVarValueCmd(UINT32_C(0));
LoadLocalArrayVarValueCmd &kLoadLocalArrayVarValueCmd =
    *new LoadLocalArrayVarValueCmd(UINT32_C(0));
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
const StoreArrayCmd &kStoreArrayCmd = *new StoreArrayCmd();

const CastCharToIntCmd &kCastCharToIntCmd = *new CastCharToIntCmd();
const CastCharToLongCmd &kCastCharToLongCmd = *new CastCharToLongCmd();
const CastCharToDoubleCmd &kCastCharToDoubleCmd = *new CastCharToDoubleCmd();
const CastCharToStringCmd &kCastCharToStringCmd = *new CastCharToStringCmd();
const CastIntToLongCmd &kCastIntToLongCmd = *new CastIntToLongCmd();
const CastIntToDoubleCmd &kCastIntToDoubleCmd = *new CastIntToDoubleCmd();
const CastLongToDoubleCmd &kCastLongToDoubleCmd = *new CastLongToDoubleCmd();

const CallCmd &kCallCmd = *new CallCmd();
const CallNativeCmd &kCallNativeCmd = *new CallNativeCmd();

const LoadArrayOfIntsElementValueCmd &kLoadArrayOfIntsElementValueCmd =
    *new LoadArrayOfIntsElementValueCmd();
const LoadArrayOfLongsElementValueCmd &kLoadArrayOfLongsElementValueCmd =
    *new LoadArrayOfLongsElementValueCmd();
const LoadArrayOfDoublesElementValueCmd &kLoadArrayOfDoublesElementValueCmd =
    *new LoadArrayOfDoublesElementValueCmd();
const LoadArrayOfBoolsElementValueCmd &kLoadArrayOfBoolsElementValueCmd =
    *new LoadArrayOfBoolsElementValueCmd();
const LoadArrayOfCharsElementValueCmd &kLoadArrayOfCharsElementValueCmd =
    *new LoadArrayOfCharsElementValueCmd();
const LoadArrayOfStringsElementValueCmd &kLoadArrayOfStringsElementValueCmd =
    *new LoadArrayOfStringsElementValueCmd();
const LoadArrayOfArraysElementValueCmd &kLoadArrayOfArraysElementValueCmd =
    *new LoadArrayOfArraysElementValueCmd();

const LoadArrayOfIntsElementAddressCmd &kLoadArrayOfIntsElementAddressCmd =
    *new LoadArrayOfIntsElementAddressCmd();
const LoadArrayOfLongsElementAddressCmd &kLoadArrayOfLongsElementAddressCmd =
    *new LoadArrayOfLongsElementAddressCmd();
const LoadArrayOfDoublesElementAddressCmd &kLoadArrayOfDoublesElementAddressCmd =
    *new LoadArrayOfDoublesElementAddressCmd();
const LoadArrayOfBoolsElementAddressCmd &kLoadArrayOfBoolsElementAddressCmd =
    *new LoadArrayOfBoolsElementAddressCmd();
const LoadArrayOfCharsElementAddressCmd &kLoadArrayOfCharsElementAddressCmd =
    *new LoadArrayOfCharsElementAddressCmd();
const LoadArrayOfStringsElementAddressCmd &kLoadArrayOfStringsElementAddressCmd =
    *new LoadArrayOfStringsElementAddressCmd();
const LoadArrayOfArraysElementAddressCmd &kLoadArrayOfArraysElementAddressCmd =
    *new LoadArrayOfArraysElementAddressCmd();

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
const EqualArrayCmd &kEqualArrayCmd = *new EqualArrayCmd();

const NotEqualIntCmd &kNotEqualIntCmd = *new NotEqualIntCmd();
const NotEqualLongCmd &kNotEqualLongCmd = *new NotEqualLongCmd();
const NotEqualDoubleCmd &kNotEqualDoubleCmd = *new NotEqualDoubleCmd();
const NotEqualBoolCmd &kNotEqualBoolCmd = *new NotEqualBoolCmd();
const NotEqualCharCmd &kNotEqualCharCmd = *new NotEqualCharCmd();
const NotEqualStringCmd &kNotEqualStringCmd = *new NotEqualStringCmd();
const NotEqualArrayCmd &kNotEqualArrayCmd = *new NotEqualArrayCmd();

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
}

void CmdReader::SetCode(Code *code) {
  assert(code);
  code_ = code;
}

Code *CmdReader::GetCode() const {
  return code_;
}

const Cmd &CmdReader::GetNextCmd() {
  const Cmd *cmd = nullptr;

  switch (code_->ReadCmdId()) {
    case CmdId::kCreateGlobalIntVar:
      cmd = &kCreateGlobalIntVarCmd;
      break;
    case CmdId::kCreateGlobalArrayVar:
      cmd = &kCreateGlobalArrayVarCmd;
      break;
    case CmdId::kCreateGlobalLongVar:
      cmd = &kCreateGlobalLongVarCmd;
      break;
    case CmdId::kCreateGlobalDoubleVar:
      cmd = &kCreateGlobalDoubleVarCmd;
      break;
    case CmdId::kCreateGlobalCharVar:
      cmd = &kCreateGlobalCharVarCmd;
      break;
    case CmdId::kCreateGlobalStringVar:
      cmd = &kCreateGlobalStringVarCmd;
      break;
    case CmdId::kCreateGlobalBoolVar:
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
    case CmdId::kUnload:
      cmd = &kUnloadCmd;
      break;
    case CmdId::kLoadIntValue:
      kLoadIntValueCmd.SetValue(code_->ReadInt32());
      cmd = &kLoadIntValueCmd;
      break;
    case CmdId::kLoadLongValue:
      kLoadLongValueCmd.SetValue(code_->ReadInt64());
      cmd = &kLoadLongValueCmd;
      break;
    case CmdId::kLoadBoolValue:
      kLoadBoolValueCmd.SetValue(code_->ReadBool());
      cmd = &kLoadBoolValueCmd;
      break;
    case CmdId::kLoadCharValue:
      kLoadCharValueCmd.SetValue(code_->ReadChar());
      cmd = &kLoadCharValueCmd;
      break;
    case CmdId::kLoadStringValue:
      kLoadStringValueCmd.SetValue(code_->ReadString());
      cmd = &kLoadStringValueCmd;
      break;
    case CmdId::kLoadDoubleValue:
      kLoadDoubleValueCmd.SetValue(code_->ReadDouble());
      cmd = &kLoadDoubleValueCmd;
      break;
    case CmdId::kCreateAndInitGlobalIntVar:
      cmd = &kCreateAndInitGlobalIntVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalArrayVar:
      cmd = &kCreateAndInitGlobalArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalLongVar:
      cmd = &kCreateAndInitGlobalLongVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalDoubleVar:
      cmd = &kCreateAndInitGlobalDoubleVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalCharVar:
      cmd = &kCreateAndInitGlobalCharVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalStringVar:
      cmd = &kCreateAndInitGlobalStringVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalBoolVar:
      cmd = &kCreateAndInitGlobalBoolVarCmd;
      break;
    case CmdId::kCreateAndInitLocalIntVar:
      cmd = &kCreateAndInitLocalIntVarCmd;
      break;
    case CmdId::kCreateAndInitLocalArrayVar:
      cmd = &kCreateAndInitLocalArrayVarCmd;
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
    case CmdId::kDestroyLocalVarsAndJump:
      kDestroyLocalVarsAndJumpCmd.SetVarsCount(code_->ReadUint32());
      kDestroyLocalVarsAndJumpCmd.SetOffset(code_->ReadInt32());
      cmd = &kDestroyLocalVarsAndJumpCmd;
      break;
    case CmdId::kDestroyLocalVars:
      kDestroyLocalVarsCmd.SetVarsCount(code_->ReadUint32());
      cmd = &kDestroyLocalVarsCmd;
      break;
    case CmdId::kReturn:
      cmd = &kReturnCmd;
      break;
    case CmdId::kReturnIntValue:
      cmd = &kReturnIntValueCmd;
      break;
    case CmdId::kReturnLongValue:
      cmd = &kReturnLongValueCmd;
      break;
    case CmdId::kReturnDoubleValue:
      cmd = &kReturnDoubleValueCmd;
      break;
    case CmdId::kReturnCharValue:
      cmd = &kReturnCharValueCmd;
      break;
    case CmdId::kReturnStringValue:
      cmd = &kReturnStringValueCmd;
      break;
    case CmdId::kReturnBoolValue:
      cmd = &kReturnBoolValueCmd;
      break;
    case CmdId::kReturnArrayValue:
      cmd = &kReturnArrayValueCmd;
      break;
    case CmdId::kLoadGlobalIntVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalIntVarValueCmd);
      cmd = &kLoadGlobalIntVarValueCmd;
      break;
    case CmdId::kLoadGlobalArrayVarValue:
      ReadLoadGlobalVarValueCmd(kLoadGlobalArrayVarValueCmd);
      cmd = &kLoadGlobalArrayVarValueCmd;
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
    case CmdId::kLoadGlobalVarAddress:
      kLoadGlobalVarAddressCmd.SetVarIndex(code_->ReadUint32());
      cmd = &kLoadGlobalVarAddressCmd;
      break;
    case CmdId::kLoadLocalVarAddress:
      kLoadLocalVarAddressCmd.SetVarIndex(code_->ReadUint32());
      cmd = &kLoadLocalVarAddressCmd;
      break;
    case CmdId::kLoadFuncValue:
      kLoadFuncValueCmd.SetAddress(code_->ReadUint32());
      cmd = &kLoadFuncValueCmd;
      break;
    case CmdId::kLoadNativeFuncValue:
      kLoadNativeFuncValueCmd.SetFuncIndex(code_->ReadUint32());
      cmd = &kLoadNativeFuncValueCmd;
      break;
    case CmdId::kLoadLocalIntVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalIntVarValueCmd);
      cmd = &kLoadLocalIntVarValueCmd;
      break;
    case CmdId::kLoadLocalArrayVarValue:
      ReadLoadLocalVarValueCmd(kLoadLocalArrayVarValueCmd);
      cmd = &kLoadLocalArrayVarValueCmd;
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
    case CmdId::kStoreArray:
      cmd = &kStoreArrayCmd;
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
    case CmdId::kLoadArrayOfIntsElementValue:
      cmd = &kLoadArrayOfIntsElementValueCmd;
      break;
    case CmdId::kLoadArrayOfLongsElementValue:
      cmd = &kLoadArrayOfLongsElementValueCmd;
      break;
    case CmdId::kLoadArrayOfDoublesElementValue:
      cmd = &kLoadArrayOfDoublesElementValueCmd;
      break;
    case CmdId::kLoadArrayOfBoolsElementValue:
      cmd = &kLoadArrayOfBoolsElementValueCmd;
      break;
    case CmdId::kLoadArrayOfCharsElementValue:
      cmd = &kLoadArrayOfCharsElementValueCmd;
      break;
    case CmdId::kLoadArrayOfStringsElementValue:
      cmd = &kLoadArrayOfStringsElementValueCmd;
      break;
    case CmdId::kLoadArrayOfArraysElementValue:
      cmd = &kLoadArrayOfArraysElementValueCmd;
      break;
    case CmdId::kLoadArrayOfIntsElementAddress:
      cmd = &kLoadArrayOfIntsElementAddressCmd;
      break;
    case CmdId::kLoadArrayOfLongsElementAddress:
      cmd = &kLoadArrayOfLongsElementAddressCmd;
      break;
    case CmdId::kLoadArrayOfDoublesElementAddress:
      cmd = &kLoadArrayOfDoublesElementAddressCmd;
      break;
    case CmdId::kLoadArrayOfBoolsElementAddress:
      cmd = &kLoadArrayOfBoolsElementAddressCmd;
      break;
    case CmdId::kLoadArrayOfCharsElementAddress:
      cmd = &kLoadArrayOfCharsElementAddressCmd;
      break;
    case CmdId::kLoadArrayOfStringsElementAddress:
      cmd = &kLoadArrayOfStringsElementAddressCmd;
      break;
    case CmdId::kLoadArrayOfArraysElementAddress:
      cmd = &kLoadArrayOfArraysElementAddressCmd;
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
    case CmdId::kEqualArray:
      cmd = &kEqualArrayCmd;
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
    case CmdId::kNotEqualArray:
      cmd = &kNotEqualArrayCmd;
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
  }

  assert(cmd != nullptr);
  return *cmd;
}

inline void CmdReader::ReadCreateArrayCmd(CreateArrayCmd &cmd) {
  cmd.SetDimensionsCount(code_->ReadUint8());
}

inline void CmdReader::ReadCreateAndInitArrayCmd(
    CreateAndInitArrayCmd &cmd) {
  cmd.SetDimensionsCount(code_->ReadUint8());
  cmd.SetValuesCount(code_->ReadInt32());
}

inline void CmdReader::ReadJumpCmd(JumpCmd &cmd) {
  cmd.SetOffset(code_->ReadInt32());
}

inline void CmdReader::ReadLoadGlobalVarValueCmd(
    LoadGlobalVarValueCmd &cmd) {
  cmd.SetVarIndex(code_->ReadUint32());
}

inline void CmdReader::ReadLoadLocalVarValueCmd(
    LoadLocalVarValueCmd &cmd) {
  cmd.SetVarIndex(code_->ReadUint32());
}
}
}
