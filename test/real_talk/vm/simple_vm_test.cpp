
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
namespace {

typedef void (*DataStorageAsserter)(const DataStorage&, const DataStorage&);
}

class SimpleVMTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestExecute(
      unique_ptr<Code> cmds,
      uint32_t main_cmds_code_size,
      const SimpleVM::FuncFrames &expected_func_frames = SimpleVM::FuncFrames(),
      const DataStorage &expected_global_vars = DataStorage(),
      DataStorageAsserter global_vars_asserter = nullptr,
      const DataStorage &expected_local_vars = DataStorage(),
      DataStorageAsserter local_vars_asserter = nullptr,
      const DataStorage &expected_operands = DataStorage(),
      DataStorageAsserter operands_asserter = nullptr) {
    cmds->SetPosition(UINT32_C(0));
    uint32_t exe_version = UINT32_C(1);
    vector<string> native_func_defs;
    vector<IdAddresses> native_func_refs;
    uint32_t global_vars_size =
        static_cast<uint32_t>(expected_global_vars.GetSize());
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
    ASSERT_EQ(expected_global_vars.GetSize(), vm.GetGlobalVars().GetSize());
    ASSERT_EQ(expected_local_vars.GetSize(), vm.GetLocalVars().GetSize());
    ASSERT_EQ(expected_operands.GetSize(), vm.GetOperands().GetSize());
    ASSERT_EQ(expected_all_func_frames, vm.GetFuncFrames());

    if (global_vars_asserter) {
      global_vars_asserter(expected_global_vars, vm.GetGlobalVars());
    }

    if (local_vars_asserter) {
      local_vars_asserter(expected_local_vars, vm.GetLocalVars());
    }

    if (operands_asserter) {
      operands_asserter(expected_operands, vm.GetOperands());
    }
  }

  void TestCreateGlobalVarCmd(
      CmdId cmd_id,
      uint32_t var_index,
      const DataStorage &expected_global_vars,
      DataStorageAsserter global_vars_asserter) {
    unique_ptr<Code> cmds(new Code());
    cmds->WriteCmdId(cmd_id);
    cmds->WriteUint32(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    SimpleVM::FuncFrames expected_func_frames;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_func_frames,
                expected_global_vars,
                global_vars_asserter);
  }
};

TEST_F(SimpleVMTest, CreateGlobalIntVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetInt(var_index),
              actual_global_vars.GetInt(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateInt(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalIntVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalLongVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetLong(var_index),
              actual_global_vars.GetLong(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateLong(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalLongVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalDoubleVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetDouble(var_index),
              actual_global_vars.GetDouble(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateDouble(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalDoubleVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalBoolVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetBool(var_index),
              actual_global_vars.GetBool(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateBool(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalBoolVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalCharVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetChar(var_index),
              actual_global_vars.GetChar(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateChar(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalCharVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalStringVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetString(var_index),
              actual_global_vars.GetString(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateString(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalStringVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalArrayVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.GetArray(var_index),
              actual_global_vars.GetArray(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateArray(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalArrayVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, LoadIntValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetInt(operand_index),
              actual_operands.GetInt(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadIntValue);
  int32_t value = INT32_C(7);
  cmds->WriteInt32(value);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_operands;
  expected_operands.PushInt(value);
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  SimpleVM::FuncFrames expected_func_frames;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_func_frames,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, LoadStringValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetString(operand_index),
              actual_operands.GetString(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadStringValue);
  const string value = "abc";
  cmds->WriteString(value);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_operands;
  expected_operands.PushString(StringValue(value));
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  SimpleVM::FuncFrames expected_func_frames;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_func_frames,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}
}
}
