
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

  void TestExecute(
      unique_ptr<Code> cmds,
      uint32_t main_cmds_code_size,
      uint32_t global_vars_size,
      const DataStorage &expected_global_vars = DataStorage(),
      const DataStorage &expected_local_vars = DataStorage(),
      const DataStorage &expected_operands = DataStorage(),
      const SimpleVM::FuncFrames &expected_func_frames = SimpleVM::FuncFrames()) {
    cmds->SetPosition(UINT32_C(0));
    uint32_t exe_version = UINT32_C(1);
    vector<string> native_func_defs;
    vector<IdAddresses> native_func_refs;
    Exe exe(exe_version,
            move(cmds),
            main_cmds_code_size,
            global_vars_size,
            native_func_defs,
            native_func_refs);
    SimpleVM::FuncFrames expected_all_func_frames;
    size_t local_vars_start_index = 0;
    uint32_t return_address = UINT32_C(0);
    FuncFrame func_frame(local_vars_start_index, return_address);
    expected_all_func_frames.push_back(func_frame);
    expected_all_func_frames.insert(expected_all_func_frames.cend(),
                                    expected_func_frames.cbegin(),
                                    expected_func_frames.cend());
    uint32_t expected_cmds_position = main_cmds_code_size;
    vector<NativeFunc> native_funcs;
    SimpleVM vm(&exe, native_funcs);
    vm.Execute();
    uint32_t actual_cmds_position = exe.GetCmdsCode().GetPosition();
    ASSERT_EQ(expected_cmds_position, actual_cmds_position);
    const DataStorage &actual_global_vars = vm.GetGlobalVars();
    ASSERT_EQ(expected_global_vars, actual_global_vars);
    const DataStorage &actual_local_vars = vm.GetLocalVars();
    ASSERT_EQ(expected_local_vars, actual_local_vars);
    const DataStorage &actual_operands = vm.GetOperands();
    ASSERT_EQ(expected_operands, actual_operands);
    const SimpleVM::FuncFrames &actual_func_frames = vm.GetFuncFrames();
    ASSERT_EQ(expected_all_func_frames, actual_func_frames);
  }

  void TestCreateGlobalVarCmd(CmdId cmd_id,
                              uint32_t var_index,
                              const DataStorage &expected_global_vars) {
    unique_ptr<Code> cmds(new Code());
    cmds->WriteCmdId(cmd_id);
    cmds->WriteUint32(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    uint32_t global_vars_size =
        static_cast<uint32_t>(expected_global_vars.GetSize());
    TestExecute(move(cmds),
                main_cmds_code_size,
                global_vars_size,
                expected_global_vars);
  }
};

TEST_F(SimpleVMTest, CreateGlobalIntVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateInt(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalIntVar, var_index, expected_global_vars);
}

TEST_F(SimpleVMTest, CreateGlobalLongVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateLong(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalLongVar, var_index, expected_global_vars);
}

TEST_F(SimpleVMTest, CreateGlobalDoubleVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateDouble(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalDoubleVar, var_index, expected_global_vars);
}

TEST_F(SimpleVMTest, CreateGlobalBoolVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateBool(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalBoolVar, var_index, expected_global_vars);
}

TEST_F(SimpleVMTest, CreateGlobalCharVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateChar(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalCharVar, var_index, expected_global_vars);
}

TEST_F(SimpleVMTest, CreateGlobalStringVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateString(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalStringVar, var_index, expected_global_vars);
}

TEST_F(SimpleVMTest, CreateGlobalArrayVarCmd) {
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateArray(var_index);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalArrayVar, var_index, expected_global_vars);
}
}
}
