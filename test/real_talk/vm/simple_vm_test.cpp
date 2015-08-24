
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
    cmds->Write<CmdId>(cmd_id);
    cmds->Write<uint32_t>(var_index);
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

  template<typename T> void TestCreateArrayCmd(CmdId cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint32_t operand_index = UINT32_C(0);
      const auto &expected_array =
        expected_operands.Get< ArrayValue<T> >(operand_index);
      const auto &actual_array =
        actual_operands.Get< ArrayValue<T> >(operand_index);
      uint8_t dimensions_count = UINT8_C(2);
      ASSERT_TRUE(expected_array.IsDeeplyEqual(actual_array, dimensions_count))
        << "[expected]\n" << PrintArray(expected_array, dimensions_count)
        << "\n[actual]\n" << PrintArray(actual_array, dimensions_count);
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

  template<typename T> void TestCreateAndInitArrayCmd(
      const vector<Code> &value_codes, const vector<T> &values, CmdId cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint32_t operand_index = UINT32_C(0);
      const auto &expected_array =
        expected_operands.Get< ArrayValue<T> >(operand_index);
      const auto &actual_array =
        actual_operands.Get< ArrayValue<T> >(operand_index);
      uint8_t dimensions_count = UINT8_C(2);
      ASSERT_TRUE(expected_array.IsDeeplyEqual(actual_array, dimensions_count))
        << "[expected]\n" << PrintArray(expected_array, dimensions_count)
        << "\n[actual]\n" << PrintArray(actual_array, dimensions_count);
    };

    unique_ptr<Code> cmds(new Code());
    cmds->Write(value_codes.at(0).GetData(), value_codes.at(0).GetSize());
    cmds->Write(value_codes.at(1).GetData(), value_codes.at(1).GetSize());
    cmds->Write<CmdId>(cmd_id);
    uint8_t inner_array_dimensions_count = UINT8_C(1);
    cmds->Write<uint8_t>(inner_array_dimensions_count);
    int32_t inner_array_values_count = INT32_C(2);
    cmds->Write<int32_t>(inner_array_values_count);

    cmds->Write(value_codes.at(2).GetData(), value_codes.at(2).GetSize());
    cmds->Write<CmdId>(cmd_id);
    cmds->Write<uint8_t>(inner_array_dimensions_count);
    int32_t inner_array_values_count2 = INT32_C(1);
    cmds->Write<int32_t>(inner_array_values_count2);

    cmds->Write<CmdId>(cmd_id);
    uint8_t outer_array_dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    int32_t outer_array_values_count = INT32_C(2);
    cmds->Write<int32_t>(outer_array_values_count);

    DataStorage expected_operands;
    vector< ArrayValue<T> > outer_array_values;
    vector<T> inner_array_values = {values.at(2)};
    auto inner_array = ArrayValue<T>::Unidimensional(inner_array_values);
    outer_array_values.push_back(move(inner_array));
    vector<T> inner_array_values2 = {values.at(1), values.at(0)};
    auto inner_array2 = ArrayValue<T>::Unidimensional(inner_array_values2);
    outer_array_values.push_back(move(inner_array2));
    auto outer_array = ArrayValue<T>::Multidimensional(
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
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create<IntValue>(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalIntVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalLongVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<LongValue>(var_index),
              actual_global_vars.Get<LongValue>(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create<LongValue>(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalLongVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalDoubleVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_DOUBLE_EQ(expected_global_vars.Get<DoubleValue>(var_index),
                     actual_global_vars.Get<DoubleValue>(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create<DoubleValue>(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalDoubleVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalBoolVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<BoolValue>(var_index),
              actual_global_vars.Get<BoolValue>(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create<BoolValue>(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalBoolVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalCharVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<CharValue>(var_index),
              actual_global_vars.Get<CharValue>(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create<CharValue>(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalCharVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, CreateGlobalStringVarCmd) {
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    uint32_t var_index = UINT32_C(7);
    ASSERT_EQ(expected_global_vars.Get<StringValue>(var_index),
              actual_global_vars.Get<StringValue>(var_index));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create<StringValue>(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalStringVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
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
    ASSERT_TRUE(actual_var.IsDeeplyEqual(expected_var, dimensions_count));
  };
  size_t global_vars_size = 77;
  DataStorage expected_global_vars(global_vars_size);
  uint32_t var_index = UINT32_C(7);
  expected_global_vars.Create< ArrayValue<IntValue> >(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalArrayVar,
                         var_index,
                         expected_global_vars,
                         global_vars_asserter);
}

TEST_F(SimpleVMTest, LoadIntValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<IntValue>(operand_index),
              actual_operands.Get<IntValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value = INT32_C(7);
  cmds->Write<int32_t>(value);
  DataStorage expected_operands;
  expected_operands.Push(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadLongValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<LongValue>(operand_index),
              actual_operands.Get<LongValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadLongValue);
  int64_t value = INT64_C(7);
  cmds->Write<int64_t>(value);
  DataStorage expected_operands;
  expected_operands.Push(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadDoubleValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_DOUBLE_EQ(expected_operands.Get<DoubleValue>(operand_index),
                     actual_operands.Get<DoubleValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadDoubleValue);
  double value = 7.7;
  cmds->Write<double>(value);
  DataStorage expected_operands;
  expected_operands.Push(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadCharValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<CharValue>(operand_index),
              actual_operands.Get<CharValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadCharValue);
  char value = 'a';
  cmds->Write<char>(value);
  DataStorage expected_operands;
  expected_operands.Push(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadBoolValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<BoolValue>(operand_index),
              actual_operands.Get<BoolValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  bool value = true;
  cmds->Write<bool>(value);
  DataStorage expected_operands;
  expected_operands.Push(value);
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadStringValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<StringValue>(operand_index),
              actual_operands.Get<StringValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadStringValue);
  const string value = "abc";
  cmds->Write<string>(value);
  DataStorage expected_operands;
  expected_operands.Push(StringValue(value));
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadFuncValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<FuncValue>(operand_index),
              actual_operands.Get<FuncValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadFuncValue);
  uint32_t func_address = UINT32_C(7);
  cmds->Write<uint32_t>(func_address);
  DataStorage expected_operands;
  expected_operands.Push(FuncValue(func_address));
  TestLoadValueCmd(move(cmds), expected_operands, operands_asserter);
}

TEST_F(SimpleVMTest, LoadNativeFuncValueCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    uint32_t operand_index = UINT32_C(0);
    ASSERT_EQ(expected_operands.Get<NativeFuncValue>(operand_index),
              actual_operands.Get<NativeFuncValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadNativeFuncValue);
  uint32_t func_index = UINT32_C(5);
  cmds->Write<uint32_t>(func_index);
  DataStorage expected_operands;
  NativeFuncValue value = []() {};
  expected_operands.Push(value);
  vector<NativeFuncValue> native_funcs(7);
  native_funcs[func_index] = value;
  TestLoadValueCmd(
      move(cmds), expected_operands, operands_asserter, native_funcs);
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
  vector<Code> value_codes;
  Code value_code;
  value_code.Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value = INT32_C(1);
  value_code.Write<int32_t>(value);
  value_codes.push_back(move(value_code));
  Code value_code2;
  value_code2.Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value2 = INT32_C(2);
  value_code2.Write<int32_t>(value2);
  value_codes.push_back(move(value_code2));
  Code value_code3;
  value_code3.Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value3 = INT32_C(3);
  value_code3.Write<int32_t>(value3);
  value_codes.push_back(move(value_code3));
  vector<IntValue> values = {value, value2, value3};
  TestCreateAndInitArrayCmd<IntValue>(
      value_codes, values, CmdId::kCreateAndInitIntArray);
}

TEST_F(SimpleVMTest, CreateAndInitLongArrayCmd) {
  vector<Code> value_codes;
  Code value_code;
  value_code.Write<CmdId>(CmdId::kLoadLongValue);
  int64_t value = INT64_C(1);
  value_code.Write<int64_t>(value);
  value_codes.push_back(move(value_code));
  Code value_code2;
  value_code2.Write<CmdId>(CmdId::kLoadLongValue);
  int64_t value2 = INT64_C(2);
  value_code2.Write<int64_t>(value2);
  value_codes.push_back(move(value_code2));
  Code value_code3;
  value_code3.Write<CmdId>(CmdId::kLoadLongValue);
  int64_t value3 = INT64_C(3);
  value_code3.Write<int64_t>(value3);
  value_codes.push_back(move(value_code3));
  vector<LongValue> values = {value, value2, value3};
  TestCreateAndInitArrayCmd<LongValue>(
      value_codes, values, CmdId::kCreateAndInitLongArray);
}

TEST_F(SimpleVMTest, CreateAndInitDoubleArrayCmd) {
  vector<Code> value_codes;
  Code value_code;
  value_code.Write<CmdId>(CmdId::kLoadDoubleValue);
  double value = 1.1;
  value_code.Write<double>(value);
  value_codes.push_back(move(value_code));
  Code value_code2;
  value_code2.Write<CmdId>(CmdId::kLoadDoubleValue);
  double value2 = 2.2;
  value_code2.Write<double>(value2);
  value_codes.push_back(move(value_code2));
  Code value_code3;
  value_code3.Write<CmdId>(CmdId::kLoadDoubleValue);
  double value3 = 3.3;
  value_code3.Write<double>(value3);
  value_codes.push_back(move(value_code3));
  vector<DoubleValue> values = {value, value2, value3};
  TestCreateAndInitArrayCmd<DoubleValue>(
      value_codes, values, CmdId::kCreateAndInitDoubleArray);
}

TEST_F(SimpleVMTest, CreateAndInitCharArrayCmd) {
  vector<Code> value_codes;
  Code value_code;
  value_code.Write<CmdId>(CmdId::kLoadCharValue);
  char value = 'a';
  value_code.Write<char>(value);
  value_codes.push_back(move(value_code));
  Code value_code2;
  value_code2.Write<CmdId>(CmdId::kLoadCharValue);
  char value2 = 'b';
  value_code2.Write<char>(value2);
  value_codes.push_back(move(value_code2));
  Code value_code3;
  value_code3.Write<CmdId>(CmdId::kLoadCharValue);
  char value3 = 'c';
  value_code3.Write<char>(value3);
  value_codes.push_back(move(value_code3));
  vector<CharValue> values = {value, value2, value3};
  TestCreateAndInitArrayCmd<CharValue>(
      value_codes, values, CmdId::kCreateAndInitCharArray);
}

TEST_F(SimpleVMTest, CreateAndInitBoolArrayCmd) {
  vector<Code> value_codes;
  Code value_code;
  value_code.Write<CmdId>(CmdId::kLoadBoolValue);
  bool value = true;
  value_code.Write<bool>(value);
  value_codes.push_back(move(value_code));
  Code value_code2;
  value_code2.Write<CmdId>(CmdId::kLoadBoolValue);
  bool value2 = false;
  value_code2.Write<bool>(value2);
  value_codes.push_back(move(value_code2));
  Code value_code3;
  value_code3.Write<CmdId>(CmdId::kLoadBoolValue);
  bool value3 = true;
  value_code3.Write<bool>(value3);
  value_codes.push_back(move(value_code3));
  vector<BoolValue> values = {value, value2, value3};
  TestCreateAndInitArrayCmd<BoolValue>(
      value_codes, values, CmdId::kCreateAndInitBoolArray);
}

TEST_F(SimpleVMTest, CreateAndInitStringArrayCmd) {
  vector<Code> value_codes;
  Code value_code;
  value_code.Write<CmdId>(CmdId::kLoadStringValue);
  string value = "ab";
  value_code.Write<string>(value);
  value_codes.push_back(move(value_code));
  Code value_code2;
  value_code2.Write<CmdId>(CmdId::kLoadStringValue);
  string value2 = "cd";
  value_code2.Write<string>(value2);
  value_codes.push_back(move(value_code2));
  Code value_code3;
  value_code3.Write<CmdId>(CmdId::kLoadStringValue);
  string value3 = "ef";
  value_code3.Write<string>(value3);
  value_codes.push_back(move(value_code3));
  vector<StringValue> values =
      {StringValue(value), StringValue(value2), StringValue(value3)};
  TestCreateAndInitArrayCmd<StringValue>(
      value_codes, values, CmdId::kCreateAndInitStringArray);
}
}
}
