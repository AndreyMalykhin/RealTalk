
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/code/exe.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/simple_vm.h"

using std::vector;
using std::string;
using std::unique_ptr;
using testing::Test;
using real_talk::code::Code;
using real_talk::code::CmdId;
using real_talk::code::Exe;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace vm {

class SimpleVMTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleVMTest, CreateGlobalIntVarCmd) {
  auto *cmds = new Code();
  cmds->WriteCmdId(CmdId::kCreateGlobalIntVar);
  uint32_t var_index = UINT32_C(7);
  cmds->WriteUint32(var_index);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  cmds->SetPosition(UINT32_C(0));
  uint32_t global_vars_size = UINT32_C(77);
  uint32_t exe_version = UINT32_C(1);
  vector<string> native_func_defs;
  vector<IdAddresses> native_func_refs;
  Exe exe(exe_version,
          unique_ptr<Code>(cmds),
          main_cmds_code_size,
          global_vars_size,
          native_func_defs,
          native_func_refs);
  DataStorage expected_global_vars(global_vars_size);
  expected_global_vars.GetInt(var_index) = IntValue();
  DataStorage expected_local_vars;
  DataStorage expected_operands;
  SimpleVM::FuncFrames expected_func_frames;
  size_t local_vars_start_index = 0;
  uint32_t return_address = UINT32_C(0);
  FuncFrame func_frame(local_vars_start_index, return_address);
  expected_func_frames.push_back(func_frame);
  uint32_t expected_cmds_position = main_cmds_code_size;
  vector<NativeFunc> native_funcs;
  SimpleVM vm(&exe, native_funcs);
  vm.Execute();
  uint32_t actual_cmds_position = cmds->GetPosition();
  ASSERT_EQ(expected_cmds_position, actual_cmds_position);
  const DataStorage &actual_global_vars = vm.GetGlobalVars();
  ASSERT_EQ(expected_global_vars, actual_global_vars);
  const DataStorage &actual_local_vars = vm.GetLocalVars();
  ASSERT_EQ(expected_local_vars, actual_local_vars);
  const DataStorage &actual_operands = vm.GetOperands();
  ASSERT_EQ(expected_operands, actual_operands);
  const SimpleVM::FuncFrames &actual_func_frames = vm.GetFuncFrames();
  ASSERT_EQ(expected_func_frames, actual_func_frames);
}
}
}
