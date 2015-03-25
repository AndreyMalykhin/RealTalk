
#include <cassert>
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/create_local_var_cmd.h"
#include "real_talk/code/unload_cmd.h"
#include "real_talk/code/load_value_cmd.h"
#include "real_talk/code/create_and_init_global_var_cmd.h"
#include "real_talk/code/create_and_init_local_var_cmd.h"
#include "real_talk/code/create_array_cmd.h"
#include "real_talk/code/jump_cmd.h"
#include "real_talk/code/destroy_local_vars_and_jump_cmd.h"
#include "real_talk/code/cmd_reader.h"
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
}

const CmdReader::Readers CmdReader::kReaders = CmdReader::InitReaders();

void CmdReader::SetCode(Code *code) {
  assert(code);
  code_ = code;
}

Code *CmdReader::GetCode() const {
  return code_;
}

const Cmd &CmdReader::GetNextCmd() {
  const uint8_t cmd_id = static_cast<uint8_t>(code_->ReadCmdId());
  assert(cmd_id < kReaders.size());
  const Reader reader = kReaders[cmd_id];
  assert(reader);
  return (this->*reader)();
}

const CmdReader::Readers CmdReader::InitReaders() {
  Readers readers = {};
  readers[static_cast<uint8_t>(CmdId::kEndMain)] = &CmdReader::ReadEndMainCmd;
  readers[static_cast<uint8_t>(CmdId::kEndFuncs)] = &CmdReader::ReadEndFuncsCmd;

  readers[static_cast<uint8_t>(CmdId::kCreateGlobalIntVar)] =
      &CmdReader::ReadCreateGlobalIntVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalArrayVar)] =
      &CmdReader::ReadCreateGlobalArrayVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalLongVar)] =
      &CmdReader::ReadCreateGlobalLongVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalDoubleVar)] =
      &CmdReader::ReadCreateGlobalDoubleVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalCharVar)] =
      &CmdReader::ReadCreateGlobalCharVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalStringVar)] =
      &CmdReader::ReadCreateGlobalStringVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalBoolVar)] =
      &CmdReader::ReadCreateGlobalBoolVarCmd;

  readers[static_cast<uint8_t>(CmdId::kCreateLocalIntVar)] =
      &CmdReader::ReadCreateLocalIntVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLocalArrayVar)] =
      &CmdReader::ReadCreateLocalArrayVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLocalLongVar)] =
      &CmdReader::ReadCreateLocalLongVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLocalDoubleVar)] =
      &CmdReader::ReadCreateLocalDoubleVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLocalCharVar)] =
      &CmdReader::ReadCreateLocalCharVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLocalStringVar)] =
      &CmdReader::ReadCreateLocalStringVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLocalBoolVar)] =
      &CmdReader::ReadCreateLocalBoolVarCmd;

  readers[static_cast<uint8_t>(CmdId::kUnload)] = &CmdReader::ReadUnloadCmd;

  readers[static_cast<uint8_t>(CmdId::kLoadIntValue)] =
      &CmdReader::ReadLoadIntValueCmd;
  readers[static_cast<uint8_t>(CmdId::kLoadLongValue)] =
      &CmdReader::ReadLoadLongValueCmd;
  readers[static_cast<uint8_t>(CmdId::kLoadBoolValue)] =
      &CmdReader::ReadLoadBoolValueCmd;
  readers[static_cast<uint8_t>(CmdId::kLoadCharValue)] =
      &CmdReader::ReadLoadCharValueCmd;
  readers[static_cast<uint8_t>(CmdId::kLoadStringValue)] =
      &CmdReader::ReadLoadStringValueCmd;
  readers[static_cast<uint8_t>(CmdId::kLoadDoubleValue)] =
      &CmdReader::ReadLoadDoubleValueCmd;

  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalIntVar)] =
      &CmdReader::ReadCreateAndInitGlobalIntVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalArrayVar)] =
      &CmdReader::ReadCreateAndInitGlobalArrayVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalLongVar)] =
      &CmdReader::ReadCreateAndInitGlobalLongVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalDoubleVar)] =
      &CmdReader::ReadCreateAndInitGlobalDoubleVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalCharVar)] =
      &CmdReader::ReadCreateAndInitGlobalCharVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalStringVar)] =
      &CmdReader::ReadCreateAndInitGlobalStringVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitGlobalBoolVar)] =
      &CmdReader::ReadCreateAndInitGlobalBoolVarCmd;

  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalIntVar)] =
      &CmdReader::ReadCreateAndInitLocalIntVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalArrayVar)] =
      &CmdReader::ReadCreateAndInitLocalArrayVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalLongVar)] =
      &CmdReader::ReadCreateAndInitLocalLongVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalDoubleVar)] =
      &CmdReader::ReadCreateAndInitLocalDoubleVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalCharVar)] =
      &CmdReader::ReadCreateAndInitLocalCharVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalStringVar)] =
      &CmdReader::ReadCreateAndInitLocalStringVarCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateAndInitLocalBoolVar)] =
      &CmdReader::ReadCreateAndInitLocalBoolVarCmd;

  readers[static_cast<uint8_t>(CmdId::kCreateIntArray)] =
      &CmdReader::ReadCreateIntArrayCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateLongArray)] =
      &CmdReader::ReadCreateLongArrayCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateDoubleArray)] =
      &CmdReader::ReadCreateDoubleArrayCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateBoolArray)] =
      &CmdReader::ReadCreateBoolArrayCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateCharArray)] =
      &CmdReader::ReadCreateCharArrayCmd;
  readers[static_cast<uint8_t>(CmdId::kCreateStringArray)] =
      &CmdReader::ReadCreateStringArrayCmd;

  readers[static_cast<uint8_t>(CmdId::kJumpIfNot)] =
      &CmdReader::ReadJumpIfNotCmd;
  readers[static_cast<uint8_t>(CmdId::kDirectJump)] =
      &CmdReader::ReadDirectJumpCmd;

  readers[static_cast<uint8_t>(CmdId::kDestroyLocalVarsAndJumpCmd)] =
      &CmdReader::ReadDestroyLocalVarsAndJumpCmd;

  return readers;
}

const Cmd &CmdReader::ReadEndMainCmd() {
  return kEndMainCmd;
}

const Cmd &CmdReader::ReadEndFuncsCmd() {
  return kEndFuncsCmd;
}

inline const Cmd &CmdReader::ReadCreateGlobalVarCmd(CreateGlobalVarCmd &cmd) {
  cmd.SetVarIndex(code_->ReadUint32());
  return cmd;
}

const Cmd &CmdReader::ReadCreateGlobalIntVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalIntVarCmd);
}

const Cmd &CmdReader::ReadCreateGlobalArrayVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalArrayVarCmd);
}

const Cmd &CmdReader::ReadCreateGlobalLongVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalLongVarCmd);
}

const Cmd &CmdReader::ReadCreateGlobalDoubleVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalDoubleVarCmd);
}

const Cmd &CmdReader::ReadCreateGlobalCharVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalCharVarCmd);
}

const Cmd &CmdReader::ReadCreateGlobalStringVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalStringVarCmd);
}

const Cmd &CmdReader::ReadCreateGlobalBoolVarCmd() {
  return ReadCreateGlobalVarCmd(kCreateGlobalBoolVarCmd);
}

const Cmd &CmdReader::ReadCreateLocalIntVarCmd() {
  return kCreateLocalIntVarCmd;
}

const Cmd &CmdReader::ReadCreateLocalArrayVarCmd() {
  return kCreateLocalArrayVarCmd;
}

const Cmd &CmdReader::ReadCreateLocalLongVarCmd() {
  return kCreateLocalLongVarCmd;
}

const Cmd &CmdReader::ReadCreateLocalDoubleVarCmd() {
  return kCreateLocalDoubleVarCmd;
}

const Cmd &CmdReader::ReadCreateLocalCharVarCmd() {
  return kCreateLocalCharVarCmd;
}

const Cmd &CmdReader::ReadCreateLocalStringVarCmd() {
  return kCreateLocalStringVarCmd;
}

const Cmd &CmdReader::ReadCreateLocalBoolVarCmd() {
  return kCreateLocalBoolVarCmd;
}

const Cmd &CmdReader::ReadUnloadCmd() {
  return kUnloadCmd;
}

const Cmd &CmdReader::ReadLoadIntValueCmd() {
  kLoadIntValueCmd.SetValue(code_->ReadInt32());
  return kLoadIntValueCmd;
}

const Cmd &CmdReader::ReadLoadLongValueCmd() {
  kLoadLongValueCmd.SetValue(code_->ReadInt64());
  return kLoadLongValueCmd;
}

const Cmd &CmdReader::ReadLoadBoolValueCmd() {
  kLoadBoolValueCmd.SetValue(code_->ReadBool());
  return kLoadBoolValueCmd;
}

const Cmd &CmdReader::ReadLoadCharValueCmd() {
  kLoadCharValueCmd.SetValue(code_->ReadChar());
  return kLoadCharValueCmd;
}

const Cmd &CmdReader::ReadLoadStringValueCmd() {
  kLoadStringValueCmd.SetValue(code_->ReadString());
  return kLoadStringValueCmd;
}

const Cmd &CmdReader::ReadLoadDoubleValueCmd() {
  kLoadDoubleValueCmd.SetValue(code_->ReadDouble());
  return kLoadDoubleValueCmd;
}

inline const Cmd &CmdReader::ReadCreateAndInitGlobalVarCmd(
    CreateAndInitGlobalVarCmd &cmd) {
  cmd.SetVarIndex(code_->ReadUint32());
  return cmd;
}

const Cmd &CmdReader::ReadCreateAndInitGlobalIntVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalIntVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitGlobalArrayVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalArrayVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitGlobalLongVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalLongVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitGlobalDoubleVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalDoubleVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitGlobalCharVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalCharVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitGlobalStringVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalStringVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitGlobalBoolVarCmd() {
  return ReadCreateAndInitGlobalVarCmd(kCreateAndInitGlobalBoolVarCmd);
}

const Cmd &CmdReader::ReadCreateAndInitLocalIntVarCmd() {
  return kCreateAndInitLocalIntVarCmd;
}

const Cmd &CmdReader::ReadCreateAndInitLocalArrayVarCmd() {
  return kCreateAndInitLocalArrayVarCmd;
}

const Cmd &CmdReader::ReadCreateAndInitLocalLongVarCmd() {
  return kCreateAndInitLocalLongVarCmd;
}

const Cmd &CmdReader::ReadCreateAndInitLocalDoubleVarCmd() {
  return kCreateAndInitLocalDoubleVarCmd;
}

const Cmd &CmdReader::ReadCreateAndInitLocalCharVarCmd() {
  return kCreateAndInitLocalCharVarCmd;
}

const Cmd &CmdReader::ReadCreateAndInitLocalStringVarCmd() {
  return kCreateAndInitLocalStringVarCmd;
}

const Cmd &CmdReader::ReadCreateAndInitLocalBoolVarCmd() {
  return kCreateAndInitLocalBoolVarCmd;
}

inline const Cmd &CmdReader::ReadCreateArrayCmd(CreateArrayCmd &cmd) {
  cmd.SetDimensionsCount(code_->ReadUint8());
  return cmd;
}

const Cmd &CmdReader::ReadCreateIntArrayCmd() {
  return ReadCreateArrayCmd(kCreateIntArrayCmd);
}

const Cmd &CmdReader::ReadCreateLongArrayCmd() {
  return ReadCreateArrayCmd(kCreateLongArrayCmd);
}

const Cmd &CmdReader::ReadCreateDoubleArrayCmd() {
  return ReadCreateArrayCmd(kCreateDoubleArrayCmd);
}

const Cmd &CmdReader::ReadCreateBoolArrayCmd() {
  return ReadCreateArrayCmd(kCreateBoolArrayCmd);
}

const Cmd &CmdReader::ReadCreateCharArrayCmd() {
  return ReadCreateArrayCmd(kCreateCharArrayCmd);
}

const Cmd &CmdReader::ReadCreateStringArrayCmd() {
  return ReadCreateArrayCmd(kCreateStringArrayCmd);
}

inline const Cmd &CmdReader::ReadJumpCmd(JumpCmd &cmd) {
  cmd.SetAddress(code_->ReadUint32());
  return cmd;
}

const Cmd &CmdReader::ReadJumpIfNotCmd() {
  return ReadJumpCmd(kJumpIfNotCmd);
}

const Cmd &CmdReader::ReadDirectJumpCmd() {
  return ReadJumpCmd(kDirectJumpCmd);
}

const Cmd &CmdReader::ReadDestroyLocalVarsAndJumpCmd() {
  kDestroyLocalVarsAndJumpCmd.SetVarsCount(code_->ReadUint32());
  kDestroyLocalVarsAndJumpCmd.SetAddress(code_->ReadUint32());
  return kDestroyLocalVarsAndJumpCmd;
}
}
}
