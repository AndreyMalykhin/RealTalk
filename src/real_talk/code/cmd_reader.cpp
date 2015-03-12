
#include <cassert>
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/create_global_cmd.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/code.h"

namespace real_talk {
namespace code {
namespace {

const EndMainCmd &kEndMainCmd = *new EndMainCmd();
const EndFuncsCmd &kEndFuncsCmd = *new EndFuncsCmd();
CreateGlobalIntCmd &kCreateGlobalIntCmd =
    *new CreateGlobalIntCmd(UINT32_C(0));
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
  readers[static_cast<uint8_t>(CmdId::kEndMain)] = &CmdReader::ReadEndMain;
  readers[static_cast<uint8_t>(CmdId::kEndFuncs)] = &CmdReader::ReadEndFuncs;
  readers[static_cast<uint8_t>(CmdId::kCreateGlobalInt)] =
      &CmdReader::ReadCreateGlobalInt;
  return readers;
}

const Cmd &CmdReader::ReadEndMain() {
  return kEndMainCmd;
}

const Cmd &CmdReader::ReadEndFuncs() {
  return kEndFuncsCmd;
}

const Cmd &CmdReader::ReadCreateGlobalInt() {
  kCreateGlobalIntCmd.SetVarIndex(code_->ReadUint32());
  return kCreateGlobalIntCmd;
}
}
}
