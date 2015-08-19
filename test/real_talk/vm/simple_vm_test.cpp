
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
using std::stringstream;
using testing::Test;
using real_talk::code::Code;
using real_talk::code::CmdId;
using real_talk::code::Exe;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace vm {
namespace {

typedef void (*DataStorageAsserter)(const DataStorage&, const DataStorage&);

template<typename T> string PrintArray(
    const ArrayValue<T> &array, uint8_t dimensions_count) {
  stringstream stream;
  array.Print(stream, dimensions_count);
  return stream.str();
}
}

class SimpleVMTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestExecute(
      unique_ptr<Code> cmds,
      uint32_t main_cmds_code_size,
      const SimpleVM::FuncFrames &expected_func_frames = {},
      const DataStorage &expected_global_vars = {},
      DataStorageAsserter global_vars_asserter = nullptr,
      const DataStorage &expected_local_vars = {},
      DataStorageAsserter local_vars_asserter = nullptr,
      const DataStorage &expected_operands = {},
      DataStorageAsserter operands_asserter = nullptr,
      const vector<NativeFuncValue> &native_funcs = {}) {
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

  void TestLoadValueCmd(unique_ptr<Code> cmds,
                        const DataStorage &expected_operands,
                        DataStorageAsserter operands_asserter,
                        vector<NativeFuncValue> native_funcs = {}) {
    uint32_t main_cmds_code_size = cmds->GetPosition();
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
                operands_asserter,
                native_funcs);
  }

  void TestCreateArrayCmd(unique_ptr<Code> cmds,
                          const DataStorage &expected_operands,
                          DataStorageAsserter operands_asserter) {
    uint32_t main_cmds_code_size = cmds->GetPosition();
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
    ASSERT_DOUBLE_EQ(expected_global_vars.GetDouble(var_index),
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
    const ArrayValue<IntValue> &expected_var =
      expected_global_vars.GetArray<IntValue>(var_index);
    const ArrayValue<IntValue> &actual_var =
      actual_global_vars.GetArray<IntValue>(var_index);
    uint8_t dimensions_count = UINT8_C(1);
    ASSERT_TRUE(actual_var.IsDeeplyEqual(expected_var, dimensions_count));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.CreateArray<IntValue>(var_index);
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
  DataStorage expected_operands;
  expected_operands.PushInt(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadLongValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetLong(operand_index),
              actual_operands.GetLong(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadLongValue);
  int64_t value = INT64_C(7);
  cmds->WriteInt64(value);
  DataStorage expected_operands;
  expected_operands.PushLong(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadDoubleValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_DOUBLE_EQ(expected_operands.GetDouble(operand_index),
                     actual_operands.GetDouble(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadDoubleValue);
  double value = 7.7;
  cmds->WriteDouble(value);
  DataStorage expected_operands;
  expected_operands.PushDouble(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadCharValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetChar(operand_index),
              actual_operands.GetChar(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadCharValue);
  char value = 'a';
  cmds->WriteChar(value);
  DataStorage expected_operands;
  expected_operands.PushChar(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadBoolValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetBool(operand_index),
              actual_operands.GetBool(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadBoolValue);
  bool value = true;
  cmds->WriteBool(value);
  DataStorage expected_operands;
  expected_operands.PushBool(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
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
  DataStorage expected_operands;
  expected_operands.PushString(StringValue(value));
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadFuncValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetFunc(operand_index),
              actual_operands.GetFunc(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadFuncValue);
  uint32_t func_address = UINT32_C(7);
  cmds->WriteUint32(func_address);
  DataStorage expected_operands;
  expected_operands.PushFunc(func_address);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadNativeFuncValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.GetNativeFunc(operand_index),
              actual_operands.GetNativeFunc(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadNativeFuncValue);
  uint32_t func_index = UINT32_C(5);
  cmds->WriteUint32(func_index);
  DataStorage expected_operands;
  NativeFuncValue value = []() {};
  expected_operands.PushNativeFunc(value);
  vector<NativeFuncValue> native_funcs(7);
  native_funcs[func_index] = value;
  TestLoadValueCmd(
      move(cmds), expected_operands, operands_asserter, native_funcs);
}

TEST_F(SimpleVMTest, CreateIntArrayCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    const ArrayValue<IntValue> &expected_array =
      expected_operands.GetArray<IntValue>(operand_index);
    const ArrayValue<IntValue> &actual_array =
      actual_operands.GetArray<IntValue>(operand_index);
    uint8_t dimensions_count = UINT8_C(2);
    ASSERT_TRUE(expected_array.IsDeeplyEqual(actual_array, dimensions_count))
      << "[expected]\n" << PrintArray(expected_array, dimensions_count)
      << "\n[actual]\n" << PrintArray(actual_array, dimensions_count);
  };
  unique_ptr<Code> cmds(new Code());
  cmds->WriteCmdId(CmdId::kLoadIntValue);
  int32_t inner_array_size = INT32_C(2);
  cmds->WriteInt32(inner_array_size);
  cmds->WriteCmdId(CmdId::kLoadIntValue);
  int32_t outer_array_size = INT32_C(1);
  cmds->WriteInt32(outer_array_size);
  cmds->WriteCmdId(CmdId::kCreateIntArray);
  uint8_t outer_array_dimensions_count = UINT8_C(2);
  cmds->WriteUint8(outer_array_dimensions_count);
  DataStorage expected_operands;
  vector<size_t> dimensions = {static_cast<size_t>(outer_array_size),
                               static_cast<size_t>(inner_array_size)};
  expected_operands.PushArray<IntValue>(
      ArrayValue<IntValue>::Multidimensional(dimensions));
  TestCreateArrayCmd(move(cmds), expected_operands, operands_asserter);
}
}
}
