
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/simple_code_container_writer.h"

using std::vector;
using std::string;
using std::unique_ptr;
using std::ostringstream;
using testing::Test;

namespace real_talk {
namespace code {

class SimpleCodeContainerWriterTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleCodeContainerWriterTest, WriteWithModule) {
  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalIntVar);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
  cmds_code->WriteCmdId(CmdId::kCreateGlobalDoubleVar);
  cmds_code->SetPosition(UINT32_C(0));
  vector<string> global_var_defs = {"var", "var2"};
  vector<IdAddress> func_defs =
      {{"func", UINT32_C(1)}, {"func2", UINT32_C(2)}};
  vector<string> native_func_defs = {"native_func", "native_func2"};
  vector<IdAddresses> global_var_refs =
      {{"var", {UINT32_C(3), UINT32_C(4)}},
       {"var2", {UINT32_C(5), UINT32_C(6)}}};
  vector<IdAddresses> func_refs =
      {{"func", {UINT32_C(7), UINT32_C(8)}},
       {"func2", {UINT32_C(9), UINT32_C(10)}}};
  vector<IdAddresses> native_func_refs =
      {{"func3", {UINT32_C(11), UINT32_C(12)}},
       {"func4", {UINT32_C(13), UINT32_C(14)}}};
  uint32_t version = UINT32_C(1);
  Module module(version,
                move(cmds_code),
                main_cmds_code_size,
                func_defs,
                global_var_defs,
                native_func_defs,
                func_refs,
                native_func_refs,
                global_var_refs);
  Code expected_code;
  expected_code.WriteUint32(module.GetVersion());
  uint32_t segments_metadata_address = expected_code.GetPosition();
  expected_code.Skip(15 * sizeof(uint32_t));
  uint32_t cmds_address = expected_code.GetPosition();
  expected_code.WriteBytes(
      module.GetCmdsCode().GetData(), module.GetCmdsCode().GetSize());
  uint32_t global_var_defs_metadata_address = expected_code.GetPosition();
  expected_code.WriteString("var");
  expected_code.WriteString("var2");
  uint32_t global_var_defs_metadata_size =
      expected_code.GetPosition() - global_var_defs_metadata_address;
  uint32_t func_defs_metadata_address = expected_code.GetPosition();
  expected_code.WriteIdAddress(IdAddress("func", UINT32_C(1)));
  expected_code.WriteIdAddress(IdAddress("func2", UINT32_C(2)));
  uint32_t func_defs_metadata_size =
      expected_code.GetPosition() - func_defs_metadata_address;
  uint32_t native_func_defs_metadata_address = expected_code.GetPosition();
  expected_code.WriteString("native_func");
  expected_code.WriteString("native_func2");
  uint32_t native_func_defs_metadata_size =
      expected_code.GetPosition() - native_func_defs_metadata_address;
  uint32_t global_var_refs_metadata_address = expected_code.GetPosition();
  expected_code.WriteIdAddresses(
      IdAddresses("var", vector<uint32_t>({UINT32_C(3), UINT32_C(4)})));
  expected_code.WriteIdAddresses(
      IdAddresses("var2", vector<uint32_t>({UINT32_C(5), UINT32_C(6)})));
  uint32_t global_var_refs_metadata_size =
      expected_code.GetPosition() - global_var_refs_metadata_address;
  uint32_t func_refs_metadata_address = expected_code.GetPosition();
  expected_code.WriteIdAddresses(
      IdAddresses("func", vector<uint32_t>({UINT32_C(7), UINT32_C(8)})));
  expected_code.WriteIdAddresses(
      IdAddresses("func2", vector<uint32_t>({UINT32_C(9), UINT32_C(10)})));
  uint32_t func_refs_metadata_size =
      expected_code.GetPosition() - func_refs_metadata_address;
  uint32_t native_func_refs_metadata_address = expected_code.GetPosition();
  expected_code.WriteIdAddresses(
      IdAddresses("func3", vector<uint32_t>({UINT32_C(11), UINT32_C(12)})));
  expected_code.WriteIdAddresses(
      IdAddresses("func4", vector<uint32_t>({UINT32_C(13), UINT32_C(14)})));
  uint32_t native_func_refs_metadata_size =
      expected_code.GetPosition() - native_func_refs_metadata_address;
  expected_code.SetPosition(segments_metadata_address);
  expected_code.WriteUint32(cmds_address);
  expected_code.WriteUint32(module.GetMainCmdsCodeSize());
  expected_code.WriteUint32(module.GetFuncCmdsCodeSize());
  expected_code.WriteUint32(global_var_defs_metadata_address);
  expected_code.WriteUint32(global_var_defs_metadata_size);
  expected_code.WriteUint32(func_defs_metadata_address);
  expected_code.WriteUint32(func_defs_metadata_size);
  expected_code.WriteUint32(native_func_defs_metadata_address);
  expected_code.WriteUint32(native_func_defs_metadata_size);
  expected_code.WriteUint32(global_var_refs_metadata_address);
  expected_code.WriteUint32(global_var_refs_metadata_size);
  expected_code.WriteUint32(func_refs_metadata_address);
  expected_code.WriteUint32(func_refs_metadata_size);
  expected_code.WriteUint32(native_func_refs_metadata_address);
  expected_code.WriteUint32(native_func_refs_metadata_size);
  Code actual_code;
  SimpleCodeContainerWriter().Write(module, &actual_code);
  ASSERT_EQ(expected_code, actual_code);
}

TEST_F(SimpleCodeContainerWriterTest, WriteWithExe) {
  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalIntVar);
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
  cmds_code->WriteCmdId(CmdId::kCreateGlobalDoubleVar);
  cmds_code->SetPosition(UINT32_C(0));
  vector<string> native_func_defs = {"native_func", "native_func2"};
  vector<IdAddresses> native_func_refs =
      {{"func3", {UINT32_C(1), UINT32_C(2)}},
       {"func4", {UINT32_C(3), UINT32_C(4)}}};
  uint32_t version = UINT32_C(1);
  Exe exe(version,
          move(cmds_code),
          main_cmds_code_size,
          native_func_defs,
          native_func_refs);
  Code expected_code;
  expected_code.WriteUint32(exe.GetVersion());
  uint32_t segments_metadata_address = expected_code.GetPosition();
  expected_code.Skip(7 * sizeof(uint32_t));
  uint32_t cmds_address = expected_code.GetPosition();
  expected_code.WriteBytes(
      exe.GetCmdsCode().GetData(), exe.GetCmdsCode().GetSize());
  uint32_t native_func_defs_metadata_address = expected_code.GetPosition();
  expected_code.WriteString("native_func");
  expected_code.WriteString("native_func2");
  uint32_t native_func_defs_metadata_size =
      expected_code.GetPosition() - native_func_defs_metadata_address;
  uint32_t native_func_refs_metadata_address = expected_code.GetPosition();
  expected_code.WriteIdAddresses(
      IdAddresses("func3", vector<uint32_t>({UINT32_C(1), UINT32_C(2)})));
  expected_code.WriteIdAddresses(
      IdAddresses("func4", vector<uint32_t>({UINT32_C(3), UINT32_C(4)})));
  uint32_t native_func_refs_metadata_size =
      expected_code.GetPosition() - native_func_refs_metadata_address;
  expected_code.SetPosition(segments_metadata_address);
  expected_code.WriteUint32(cmds_address);
  expected_code.WriteUint32(exe.GetMainCmdsCodeSize());
  expected_code.WriteUint32(exe.GetFuncCmdsCodeSize());
  expected_code.WriteUint32(native_func_defs_metadata_address);
  expected_code.WriteUint32(native_func_defs_metadata_size);
  expected_code.WriteUint32(native_func_refs_metadata_address);
  expected_code.WriteUint32(native_func_refs_metadata_size);
  Code actual_code;
  SimpleCodeContainerWriter().Write(exe, &actual_code);
  ASSERT_EQ(expected_code, actual_code);
}
}
}
