
#include <cassert>
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/create_global_var_cmd.h"
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
}

const CmdReader::Readers CmdReader::kReaders = CmdReader::InitReaders();

void CmdReader::SetCode(Code *code) {
  assert(code);
  code_ = code;
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
}
}
