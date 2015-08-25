
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <deque>
#include <string>
#include "real_talk/code/exe.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/simple_vm.h"

using std::vector;
using std::deque;
using std::string;
using std::unique_ptr;
using std::stringstream;
using std::move;
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

template<typename T> void AssertArraysEqual(const ArrayValue<T> &expected,
                                            const ArrayValue<T> &actual,
                                            uint8_t dimensions_count) {
  ASSERT_TRUE(expected.IsDeeplyEqual(actual, dimensions_count))
        << "[expected]\n" << PrintArray(expected, dimensions_count)
        << "\n[actual]\n" << PrintArray(actual, dimensions_count);
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

  template<typename T> void TestCreateGlobalVarCmd(
      CmdId cmd_id, DataStorageAsserter global_vars_asserter) {
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(cmd_id);
    uint32_t var_index = UINT32_C(7);
    cmds->Write<uint32_t>(var_index);
    size_t global_vars_size = 77;
    DataStorage expected_global_vars(global_vars_size);
    expected_global_vars.Create<T>(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    SimpleVM::FuncFrames expected_func_frames;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_func_frames,
                expected_global_vars,
                global_vars_asserter);
  }

  template<typename TType, typename TSerializableType> void TestLoadValueCmd(
      CmdId cmd_id,
      TType value,
      TSerializableType serializable_value,
      vector<NativeFuncValue> native_funcs = {}) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint32_t operand_index = UINT32_C(0);
      ASSERT_EQ(expected_operands.Get<TType>(operand_index),
                actual_operands.Get<TType>(operand_index));
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(cmd_id);
    cmds->Write<TSerializableType>(serializable_value);
    DataStorage expected_operands;
    expected_operands.Push(value);
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

  template<typename T> void TestCreateArrayCmd(CmdId cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint32_t operand_index = UINT32_C(0);
      const auto &expected_array =
        expected_operands.Get< ArrayValue<T> >(operand_index);
      const auto &actual_array =
        actual_operands.Get< ArrayValue<T> >(operand_index);
      uint8_t dimensions_count = UINT8_C(2);
      AssertArraysEqual(expected_array, actual_array, dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t inner_array_size = INT32_C(2);
    cmds->Write<int32_t>(inner_array_size);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t outer_array_size = INT32_C(1);
    cmds->Write<int32_t>(outer_array_size);
    cmds->Write<CmdId>(cmd_id);
    uint8_t outer_array_dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    DataStorage expected_operands;
    vector<size_t> dimensions = {static_cast<size_t>(outer_array_size),
                                 static_cast<size_t>(inner_array_size)};
    expected_operands.Push(ArrayValue<T>::Multidimensional(
        dimensions.begin(), dimensions.end()));
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

  template<typename TType, typename TSerializableType>
  void TestCreateAndInitArrayCmd(const deque<TSerializableType> &values,
                                 CmdId load_value_cmd_id,
                                 CmdId create_array_cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint32_t operand_index = UINT32_C(0);
      const auto &expected_array =
        expected_operands.Get< ArrayValue<TType> >(operand_index);
      const auto &actual_array =
        actual_operands.Get< ArrayValue<TType> >(operand_index);
      uint8_t dimensions_count = UINT8_C(2);
      AssertArraysEqual(expected_array, actual_array, dimensions_count);
    };

    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(values.at(0));
    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(values.at(1));
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t inner_array_dimensions_count = UINT8_C(1);
    cmds->Write<uint8_t>(inner_array_dimensions_count);
    int32_t inner_array_values_count = INT32_C(2);
    cmds->Write<int32_t>(inner_array_values_count);

    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(values.at(2));
    cmds->Write<CmdId>(create_array_cmd_id);
    cmds->Write<uint8_t>(inner_array_dimensions_count);
    int32_t inner_array_values_count2 = INT32_C(1);
    cmds->Write<int32_t>(inner_array_values_count2);

    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t outer_array_dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    int32_t outer_array_values_count = INT32_C(2);
    cmds->Write<int32_t>(outer_array_values_count);

    DataStorage expected_operands;
    vector< ArrayValue<TType> > outer_array_values;
    vector<TType> inner_array_values = {TType(values.at(2))};
    auto inner_array = ArrayValue<TType>::Unidimensional(inner_array_values);
    outer_array_values.push_back(move(inner_array));
    vector<TType> inner_array_values2 =
        {TType(values.at(1)), TType(values.at(0))};
    auto inner_array2 = ArrayValue<TType>::Unidimensional(inner_array_values2);
    outer_array_values.push_back(move(inner_array2));
    auto outer_array = ArrayValue<TType>::Multidimensional(
        move(outer_array_values));
    expected_operands.Push(move(outer_array));

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
    ASSERT_EQ(expected_global_vars.Get<IntValue>(var_index),
              actual_global_vars.Get<IntValue>(var_index));
  };
  TestCreateGlobalVarCmd<IntValue>(
      CmdId::kCreateGlobalIntVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalLongVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<LongValue>(var_index),
              actual_global_vars.Get<LongValue>(var_index));
  };
  TestCreateGlobalVarCmd<LongValue>(
      CmdId::kCreateGlobalLongVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalDoubleVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_DOUBLE_EQ(expected_global_vars.Get<DoubleValue>(var_index),
                     actual_global_vars.Get<DoubleValue>(var_index));
  };
  TestCreateGlobalVarCmd<DoubleValue>(
      CmdId::kCreateGlobalDoubleVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalBoolVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<BoolValue>(var_index),
              actual_global_vars.Get<BoolValue>(var_index));
  };
  TestCreateGlobalVarCmd<BoolValue>(
      CmdId::kCreateGlobalBoolVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalCharVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<CharValue>(var_index),
              actual_global_vars.Get<CharValue>(var_index));
  };
  TestCreateGlobalVarCmd<CharValue>(
      CmdId::kCreateGlobalCharVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalStringVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<StringValue>(var_index),
              actual_global_vars.Get<StringValue>(var_index));
  };
  TestCreateGlobalVarCmd<StringValue>(
      CmdId::kCreateGlobalStringVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalArrayVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    const auto &expected_var =
      expected_global_vars.Get< ArrayValue<IntValue> >(var_index);
    const auto &actual_var =
      actual_global_vars.Get< ArrayValue<IntValue> >(var_index);
    uint8_t dimensions_count = UINT8_C(1);
    AssertArraysEqual(expected_var, actual_var, dimensions_count);
  };
  TestCreateGlobalVarCmd< ArrayValue<IntValue> >(
      CmdId::kCreateGlobalArrayVar, global_vars_asserter);
}

TEST_F(SimpleVMTest, LoadIntValueCmd) {
  int32_t serializable_value = INT32_C(7);
  TestLoadValueCmd(CmdId::kLoadIntValue,
                   IntValue(serializable_value),
                   serializable_value);
}

TEST_F(SimpleVMTest, LoadLongValueCmd) {
  int64_t serializable_value = INT64_C(7);
  TestLoadValueCmd(CmdId::kLoadLongValue,
                   LongValue(serializable_value),
                   serializable_value);
}

TEST_F(SimpleVMTest, LoadDoubleValueCmd) {
  double serializable_value = 7.7;
  TestLoadValueCmd(CmdId::kLoadDoubleValue,
                   DoubleValue(serializable_value),
                   serializable_value);
}

TEST_F(SimpleVMTest, LoadCharValueCmd) {
  char serializable_value = 'a';
  TestLoadValueCmd(CmdId::kLoadCharValue,
                   CharValue(serializable_value),
                   serializable_value);
}

TEST_F(SimpleVMTest, LoadBoolValueCmd) {
  bool serializable_value = true;
  TestLoadValueCmd(CmdId::kLoadBoolValue,
                   BoolValue(serializable_value),
                   serializable_value);
}

TEST_F(SimpleVMTest, LoadStringValueCmd) {
  string serializable_value = "abc";
  TestLoadValueCmd(CmdId::kLoadStringValue,
                   StringValue(serializable_value),
                   serializable_value);
}

TEST_F(SimpleVMTest, LoadFuncValueCmd) {
  uint32_t func_address = UINT32_C(7);
  TestLoadValueCmd(
      CmdId::kLoadFuncValue, FuncValue(func_address), func_address);
}

TEST_F(SimpleVMTest, LoadNativeFuncValueCmd) {
  uint32_t func_index = UINT32_C(5);
  NativeFuncValue value = []() {};
  vector<NativeFuncValue> native_funcs(7);
  native_funcs[func_index] = value;
  TestLoadValueCmd(
      CmdId::kLoadNativeFuncValue, value, func_index, native_funcs);
}

TEST_F(SimpleVMTest, CreateIntArrayCmd) {
  TestCreateArrayCmd<IntValue>(CmdId::kCreateIntArray);
}

TEST_F(SimpleVMTest, CreateLongArrayCmd) {
  TestCreateArrayCmd<LongValue>(CmdId::kCreateLongArray);
}

TEST_F(SimpleVMTest, CreateDoubleArrayCmd) {
  TestCreateArrayCmd<DoubleValue>(CmdId::kCreateDoubleArray);
}

TEST_F(SimpleVMTest, CreateCharArrayCmd) {
  TestCreateArrayCmd<CharValue>(CmdId::kCreateCharArray);
}

TEST_F(SimpleVMTest, CreateBoolArrayCmd) {
  TestCreateArrayCmd<BoolValue>(CmdId::kCreateBoolArray);
}

TEST_F(SimpleVMTest, CreateStringArrayCmd) {
  TestCreateArrayCmd<StringValue>(CmdId::kCreateStringArray);
}

TEST_F(SimpleVMTest, CreateAndInitIntArrayCmd) {
  deque<int32_t> values = {INT32_C(1), INT32_C(2), INT32_C(3)};
  TestCreateAndInitArrayCmd<IntValue>(
      values, CmdId::kLoadIntValue, CmdId::kCreateAndInitIntArray);
}

TEST_F(SimpleVMTest, CreateAndInitLongArrayCmd) {
  deque<int64_t> values = {INT64_C(1), INT64_C(2), INT64_C(3)};
  TestCreateAndInitArrayCmd<LongValue>(
      values, CmdId::kLoadLongValue, CmdId::kCreateAndInitLongArray);
}

TEST_F(SimpleVMTest, CreateAndInitDoubleArrayCmd) {
  deque<double> values = {1.1, 2.2, 3.3};
  TestCreateAndInitArrayCmd<DoubleValue>(
      values, CmdId::kLoadDoubleValue, CmdId::kCreateAndInitDoubleArray);
}

TEST_F(SimpleVMTest, CreateAndInitCharArrayCmd) {
  deque<char> values = {'a', 'b', 'c'};
  TestCreateAndInitArrayCmd<CharValue>(
      values, CmdId::kLoadCharValue, CmdId::kCreateAndInitCharArray);
}

TEST_F(SimpleVMTest, CreateAndInitBoolArrayCmd) {
  deque<bool> values = {true, false, true};
  TestCreateAndInitArrayCmd<BoolValue>(
      values, CmdId::kLoadBoolValue, CmdId::kCreateAndInitBoolArray);
}

TEST_F(SimpleVMTest, CreateAndInitStringArrayCmd) {
  deque<string> values = {"ab", "cd", "ef"};
  TestCreateAndInitArrayCmd<StringValue>(
      values, CmdId::kLoadStringValue, CmdId::kCreateAndInitStringArray);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalIntVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_vars,
                                 const DataStorage &actual_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_vars.Get<IntValue>(var_index),
              actual_vars.Get<IntValue>(var_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value = INT32_C(777);
  cmds->Write<int32_t>(value);
  cmds->Write<CmdId>(CmdId::kCreateAndInitGlobalIntVar);
  uint32_t var_index = UINT32_C(7);
  cmds->Write<uint32_t>(var_index);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  expected_global_vars.Create<IntValue>(var_index, value);
  SimpleVM::FuncFrames expected_func_frames;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_func_frames,
              expected_global_vars,
              global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalIntArrayVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_vars,
                                 const DataStorage &actual_vars) {
    uint32_t var_index = UINT32_C(7);
    uint8_t dimensions_count = UINT8_C(1);
    AssertArraysEqual(expected_vars.Get< ArrayValue<IntValue> >(var_index),
                      actual_vars.Get< ArrayValue<IntValue> >(var_index),
                      dimensions_count);
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  int32_t array_item = INT32_C(777);
  cmds->Write<int32_t>(array_item);
  cmds->Write<CmdId>(CmdId::kCreateAndInitIntArray);
  uint8_t dimensions_count = UINT8_C(1);
  cmds->Write<uint8_t>(dimensions_count);
  int32_t values_count = INT32_C(1);
  cmds->Write<int32_t>(values_count);
  cmds->Write<CmdId>(CmdId::kCreateAndInitGlobalIntArrayVar);
  uint32_t var_index = UINT32_C(7);
  cmds->Write<uint32_t>(var_index);
  cmds->Write<uint8_t>(dimensions_count);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  vector<IntValue> array_items = {array_item};
  auto var_value = ArrayValue<IntValue>::Unidimensional(array_items);
  expected_global_vars.Create< ArrayValue<IntValue> >(
      var_index, move(var_value));
  SimpleVM::FuncFrames expected_func_frames;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_func_frames,
              expected_global_vars,
              global_vars_asserter);
}
}
}
