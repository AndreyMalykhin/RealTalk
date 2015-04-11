
#include <cassert>
#include "real_talk/code/end_cmd.h"
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
#include "real_talk/code/store_cmd.h"
#include "real_talk/code/code.h"

namespace real_talk {
namespace code {
namespace {

const EndMainCmd &kEndMainCmd = *new EndMainCmd();
const EndFuncsCmd &kEndFuncsCmd = *new EndFuncsCmd();

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

const UnloadCmd &kUnloadCmd = *new UnloadCmd();

LoadIntValueCmd &kLoadIntValueCmd = *new LoadIntValueCmd(INT32_C(0));
LoadLongValueCmd &kLoadLongValueCmd = *new LoadLongValueCmd(INT64_C(0));
LoadBoolValueCmd &kLoadBoolValueCmd = *new LoadBoolValueCmd(false);
LoadCharValueCmd &kLoadCharValueCmd = *new LoadCharValueCmd('\0');
LoadStringValueCmd &kLoadStringValueCmd = *new LoadStringValueCmd("");
LoadDoubleValueCmd &kLoadDoubleValueCmd = *new LoadDoubleValueCmd(0.0);

CreateAndInitGlobalIntVarCmd &kCreateAndInitGlobalIntVarCmd =
    *new CreateAndInitGlobalIntVarCmd(UINT32_C(0));
CreateAndInitGlobalArrayVarCmd &kCreateAndInitGlobalArrayVarCmd =
    *new CreateAndInitGlobalArrayVarCmd(UINT32_C(0));
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

JumpIfNotCmd &kJumpIfNotCmd = *new JumpIfNotCmd(UINT32_C(0));
DirectJumpCmd &kDirectJumpCmd = *new DirectJumpCmd(UINT32_C(0));

DestroyLocalVarsAndJumpCmd &kDestroyLocalVarsAndJumpCmd =
    *new DestroyLocalVarsAndJumpCmd(UINT32_C(1), UINT32_C(0));
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

const StoreIntCmd &kStoreIntCmd = *new StoreIntCmd();
const StoreLongCmd &kStoreLongCmd = *new StoreLongCmd();
const StoreDoubleCmd &kStoreDoubleCmd = *new StoreDoubleCmd();
const StoreBoolCmd &kStoreBoolCmd = *new StoreBoolCmd();
const StoreCharCmd &kStoreCharCmd = *new StoreCharCmd();
const StoreStringCmd &kStoreStringCmd = *new StoreStringCmd();
const StoreArrayCmd &kStoreArrayCmd = *new StoreArrayCmd();
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
    case CmdId::kEndMain:
      cmd = &kEndMainCmd;
      break;
    case CmdId::kEndFuncs:
      cmd = &kEndFuncsCmd;
      break;
    case CmdId::kCreateGlobalIntVar:
      ReadCreateGlobalVarCmd(kCreateGlobalIntVarCmd);
      cmd = &kCreateGlobalIntVarCmd;
      break;
    case CmdId::kCreateGlobalArrayVar:
      ReadCreateGlobalVarCmd(kCreateGlobalArrayVarCmd);
      cmd = &kCreateGlobalArrayVarCmd;
      break;
    case CmdId::kCreateGlobalLongVar:
      ReadCreateGlobalVarCmd(kCreateGlobalLongVarCmd);
      cmd = &kCreateGlobalLongVarCmd;
      break;
    case CmdId::kCreateGlobalDoubleVar:
      ReadCreateGlobalVarCmd(kCreateGlobalDoubleVarCmd);
      cmd = &kCreateGlobalDoubleVarCmd;
      break;
    case CmdId::kCreateGlobalCharVar:
      ReadCreateGlobalVarCmd(kCreateGlobalCharVarCmd);
      cmd = &kCreateGlobalCharVarCmd;
      break;
    case CmdId::kCreateGlobalStringVar:
      ReadCreateGlobalVarCmd(kCreateGlobalStringVarCmd);
      cmd = &kCreateGlobalStringVarCmd;
      break;
    case CmdId::kCreateGlobalBoolVar:
      ReadCreateGlobalVarCmd(kCreateGlobalBoolVarCmd);
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
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalIntVarCmd);
      cmd = &kCreateAndInitGlobalIntVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalArrayVar:
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalArrayVarCmd);
      cmd = &kCreateAndInitGlobalArrayVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalLongVar:
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalLongVarCmd);
      cmd = &kCreateAndInitGlobalLongVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalDoubleVar:
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalDoubleVarCmd);
      cmd = &kCreateAndInitGlobalDoubleVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalCharVar:
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalCharVarCmd);
      cmd = &kCreateAndInitGlobalCharVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalStringVar:
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalStringVarCmd);
      cmd = &kCreateAndInitGlobalStringVarCmd;
      break;
    case CmdId::kCreateAndInitGlobalBoolVar:
      ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalBoolVarCmd);
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
    case CmdId::kDirectJump:
      ReadJumpCmd(kDirectJumpCmd);
      cmd = &kDirectJumpCmd;
      break;
    case CmdId::kDestroyLocalVarsAndJump:
      kDestroyLocalVarsAndJumpCmd.SetVarsCount(code_->ReadUint32());
      kDestroyLocalVarsAndJumpCmd.SetAddress(code_->ReadUint32());
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
  }

  assert(cmd != nullptr);
  return *cmd;
}

inline void CmdReader::ReadCreateGlobalVarCmd(CreateGlobalVarCmd &cmd) {
  cmd.SetVarIndex(code_->ReadUint32());
}

inline void CmdReader::ReadCreateAndInitGlobalVarCmd(
    CreateAndInitGlobalVarCmd &cmd) {
  cmd.SetVarIndex(code_->ReadUint32());
}

inline void CmdReader::ReadCreateArrayCmd(CreateArrayCmd &cmd) {
  cmd.SetDimensionsCount(code_->ReadUint8());
}

inline void CmdReader::ReadCreateAndInitArrayCmd(
    CreateAndInitArrayCmd &cmd) {
  cmd.SetDimensionsCount(code_->ReadUint8());
  cmd.SetValuesCount(code_->ReadUint32());
}

inline void CmdReader::ReadJumpCmd(JumpCmd &cmd) {
  cmd.SetAddress(code_->ReadUint32());
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
