
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
    SimpleVM::FuncFrames expected_func_frames;
    size_t local_vars_start_index = 0;
    uint32_t return_address = UINT32_C(0);
    FuncFrame func_frame(local_vars_start_index, return_address);
    expected_func_frames.push_back(func_frame);
    uint32_t expected_cmds_position = main_cmds_code_size;
    SimpleVM vm(&exe, native_funcs);
    vm.Execute();
    ASSERT_EQ(expected_cmds_position, exe.GetCmdsCode().GetPosition());
    ASSERT_EQ(expected_global_vars.GetSize(), vm.GetGlobalVars().GetSize());
    ASSERT_EQ(expected_local_vars.GetSize(), vm.GetLocalVars().GetSize());
    ASSERT_EQ(expected_operands.GetSize(), vm.GetOperands().GetSize());
    ASSERT_EQ(expected_func_frames, vm.GetFuncFrames());

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
    size_t global_vars_size = 10;
    DataStorage expected_global_vars(global_vars_size, global_vars_size);
    expected_global_vars.Create<T>(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter);
  }

  template<typename T> void TestCreateLocalVarCmd(
      CmdId cmd_id, DataStorageAsserter local_vars_asserter) {
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(cmd_id);
    DataStorage expected_local_vars;
    expected_local_vars.Push<T>(T());
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter);
  }

  template<typename TType, typename TSerializableType> void TestLoadValueCmd(
      CmdId cmd_id,
      TType value,
      TSerializableType serializable_value,
      vector<NativeFuncValue> native_funcs = {}) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      ASSERT_EQ(expected_operands.GetTop<TType>(),
                actual_operands.GetTop<TType>());
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
    TestExecute(move(cmds),
                main_cmds_code_size,
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
    TestExecute(move(cmds),
                main_cmds_code_size,
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
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter,
                expected_operands,
                operands_asserter);
  }

  template<typename TType, typename TSerializableType>
  void TestCreateAndInitGlobalVarCmd(
      CmdId load_value_cmd_id,
      CmdId create_var_cmd_id,
      TSerializableType value) {
    auto global_vars_asserter = [](const DataStorage &expected_vars,
                                   const DataStorage &actual_vars) {
      uint32_t var_index = UINT32_C(7);
      ASSERT_EQ(expected_vars.Get<TType>(var_index),
                actual_vars.Get<TType>(var_index));
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(value);
    cmds->Write<CmdId>(create_var_cmd_id);
    uint32_t var_index = UINT32_C(7);
    cmds->Write<uint32_t>(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    size_t global_vars_size = 10;
    DataStorage expected_global_vars(global_vars_size, global_vars_size);
    expected_global_vars.Create(var_index, TType(value));
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter);
  }

  template<typename TType, typename TSerializableType>
  void TestCreateAndInitLocalVarCmd(
      CmdId load_value_cmd_id,
      CmdId create_var_cmd_id,
      TSerializableType value) {
    auto local_vars_asserter = [](const DataStorage &expected_vars,
                                  const DataStorage &actual_vars) {
      ASSERT_EQ(expected_vars.GetTop<TType>(), actual_vars.GetTop<TType>());
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(value);
    cmds->Write<CmdId>(create_var_cmd_id);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_local_vars;
    expected_local_vars.Push(TType(value));
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter);
  }

  template<typename TType, typename TSerializableType>
  void TestCreateAndInitGlobalArrayVarCmd(CmdId load_value_cmd_id,
                                          CmdId create_array_cmd_id,
                                          CmdId create_var_cmd_id,
                                          TSerializableType array_item) {
    auto global_vars_asserter = [](const DataStorage &expected_vars,
                                   const DataStorage &actual_vars) {
      uint32_t var_index = UINT32_C(7);
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(expected_vars.Get< ArrayValue<TType> >(var_index),
                        actual_vars.Get< ArrayValue<TType> >(var_index),
                        dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(array_item);
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(1);
    cmds->Write<uint8_t>(dimensions_count);
    int32_t values_count = INT32_C(1);
    cmds->Write<int32_t>(values_count);
    cmds->Write<CmdId>(create_var_cmd_id);
    uint32_t var_index = UINT32_C(7);
    cmds->Write<uint32_t>(var_index);
    cmds->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    size_t global_vars_size = 10;
    DataStorage expected_global_vars(global_vars_size, global_vars_size);
    vector<TType> array_items = {TType(array_item)};
    auto var_value = ArrayValue<TType>::Unidimensional(array_items);
    expected_global_vars.Create(var_index, move(var_value));
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter);
  }

  template<typename TType, typename TSerializableType>
  void TestCreateAndInitLocalArrayVarCmd(CmdId load_value_cmd_id,
                                         CmdId create_array_cmd_id,
                                         CmdId create_var_cmd_id,
                                         TSerializableType array_item) {
    auto local_vars_asserter = [](const DataStorage &expected_vars,
                                  const DataStorage &actual_vars) {
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(expected_vars.GetTop< ArrayValue<TType> >(),
                        actual_vars.GetTop< ArrayValue<TType> >(),
                        dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(load_value_cmd_id);
    cmds->Write(array_item);
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(1);
    cmds->Write<uint8_t>(dimensions_count);
    int32_t values_count = INT32_C(1);
    cmds->Write<int32_t>(values_count);
    cmds->Write<CmdId>(create_var_cmd_id);
    cmds->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_local_vars;
    vector<TType> array_items = {TType(array_item)};
    auto var_value = ArrayValue<TType>::Unidimensional(array_items);
    expected_local_vars.Push(move(var_value));
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter);
  }

  void TestDestroyLocalVarCmd(
      CmdId create_var_cmd_id, CmdId destroy_var_cmd_id) {
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(create_var_cmd_id);
    cmds->Write<CmdId>(destroy_var_cmd_id);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    TestExecute(move(cmds), main_cmds_code_size);
  }

  void TestDestroyLocalArrayVarCmd(CmdId create_array_cmd_id,
                                   CmdId create_var_cmd_id,
                                   CmdId destroy_var_cmd_id) {
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t array_size = INT32_C(2);
    cmds->Write<int32_t>(array_size);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t array_size2 = INT32_C(3);
    cmds->Write<int32_t>(array_size2);
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(dimensions_count);
    cmds->Write<CmdId>(create_var_cmd_id);
    cmds->Write<uint8_t>(dimensions_count);
    cmds->Write<CmdId>(destroy_var_cmd_id);
    cmds->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    TestExecute(move(cmds), main_cmds_code_size);
  }

  template<typename T> void TestLoadLocalVarValueCmd(
      CmdId create_var_cmd_id,
      CmdId destroy_var_cmd_id,
      CmdId load_var_value_cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      ASSERT_EQ(expected_operands.GetTop<T>(), actual_operands.GetTop<T>());
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(create_var_cmd_id);
    cmds->Write<CmdId>(load_var_value_cmd_id);
    uint32_t var_index = UINT32_C(0);
    cmds->Write<uint32_t>(var_index);
    cmds->Write<CmdId>(destroy_var_cmd_id);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_operands;
    expected_operands.Push(T());
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    DataStorage expected_local_vars;
    DataStorageAsserter local_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter,
                expected_operands,
                operands_asserter);
  }

  template<typename T> void TestLoadLocalArrayVarValueCmd(
      CmdId create_var_cmd_id,
      CmdId destroy_var_cmd_id,
      CmdId load_var_value_cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(expected_operands.GetTop< ArrayValue<T> >(),
                        actual_operands.GetTop< ArrayValue<T> >(),
                        dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(create_var_cmd_id);
    cmds->Write<CmdId>(load_var_value_cmd_id);
    uint32_t var_index = UINT32_C(0);
    cmds->Write<uint32_t>(var_index);
    cmds->Write<CmdId>(destroy_var_cmd_id);
    uint8_t dimensions_count = UINT8_C(1);
    cmds->Write<uint8_t>(dimensions_count);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_operands;
    expected_operands.Push(ArrayValue<T>());
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    DataStorage expected_local_vars;
    DataStorageAsserter local_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter,
                expected_operands,
                operands_asserter);
  }

  void TestUnloadCmd(unique_ptr<Code> cmds) {
    uint32_t main_cmds_code_size = cmds->GetPosition();
    TestExecute(move(cmds), main_cmds_code_size);
  }

  void TestUnloadArrayCmd(
      CmdId create_array_cmd_id, CmdId unload_array_cmd_id) {
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t array_size = INT32_C(2);
    cmds->Write<int32_t>(array_size);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t array_size2 = INT32_C(3);
    cmds->Write<int32_t>(array_size2);
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(dimensions_count);
    cmds->Write<CmdId>(unload_array_cmd_id);
    cmds->Write<uint8_t>(dimensions_count);
    TestUnloadCmd(move(cmds));
  }

  template<typename T> void TestLoadGlobalVarValueCmd(
      CmdId create_var_cmd_id, CmdId load_var_value_cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      ASSERT_EQ(expected_operands.GetTop<T>(), actual_operands.GetTop<T>());
    };
    auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                   const DataStorage &actual_global_vars) {
      size_t var_index = 5;
      ASSERT_EQ(expected_global_vars.Get<T>(var_index),
                actual_global_vars.Get<T>(var_index));
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(create_var_cmd_id);
    uint32_t var_index = UINT32_C(5);
    cmds->Write<uint32_t>(var_index);
    cmds->Write<CmdId>(load_var_value_cmd_id);
    cmds->Write<uint32_t>(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    size_t global_vars_size = 7;
    DataStorage expected_global_vars(global_vars_size, global_vars_size);
    expected_global_vars.Create(var_index, T());
    DataStorage expected_operands;
    expected_operands.Push(expected_global_vars.Get<T>(var_index));
    DataStorage expected_local_vars;
    DataStorageAsserter local_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter,
                expected_operands,
                operands_asserter);
  }

  template<typename T> void TestLoadGlobalArrayVarValueCmd(
      CmdId load_var_value_cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(expected_operands.GetTop< ArrayValue<T> >(),
                        actual_operands.GetTop< ArrayValue<T> >(),
                        dimensions_count);
    };
    auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                   const DataStorage &actual_global_vars) {
      size_t var_index = 5;
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(expected_global_vars.Get< ArrayValue<T> >(var_index),
                        actual_global_vars.Get< ArrayValue<T> >(var_index),
                        dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(CmdId::kCreateGlobalArrayVar);
    uint32_t var_index = UINT32_C(5);
    cmds->Write<uint32_t>(var_index);
    cmds->Write<CmdId>(load_var_value_cmd_id);
    cmds->Write<uint32_t>(var_index);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    size_t global_vars_size = 7;
    DataStorage expected_global_vars(global_vars_size, global_vars_size);
    expected_global_vars.Create(var_index, ArrayValue<T>());
    DataStorage expected_operands;
    expected_operands.Push(
        expected_global_vars.Get< ArrayValue<T> >(var_index).Clone());
    DataStorage expected_local_vars;
    DataStorageAsserter local_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter,
                expected_operands,
                operands_asserter);
  }

  template<typename T> void TestLoadArrayElementValueCmd(
      CmdId create_array_cmd_id, CmdId load_element_cmd_id) {
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(expected_operands.GetTop< ArrayValue<T> >(),
                        actual_operands.GetTop< ArrayValue<T> >(),
                        dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t item_index = INT32_C(0);
    cmds->Write<int32_t>(item_index);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t inner_array_size = INT32_C(2);
    cmds->Write<int32_t>(inner_array_size);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t outer_array_size = INT32_C(1);
    cmds->Write<int32_t>(outer_array_size);
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t outer_array_dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    cmds->Write<CmdId>(load_element_cmd_id);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    DataStorage expected_operands;
    expected_operands.Push(ArrayValue<T>::Unidimensional(
        static_cast<size_t>(inner_array_size)));
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    DataStorage expected_local_vars;
    DataStorageAsserter local_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
                expected_global_vars,
                global_vars_asserter,
                expected_local_vars,
                local_vars_asserter,
                expected_operands,
                operands_asserter);
  }

  template<typename T> void TestLoadArrayElementAddressCmd(
      CmdId create_array_cmd_id,
      CmdId create_var_cmd_id,
      CmdId load_array_cmd_id,
      CmdId load_address_cmd_id) {
    auto local_vars_asserter = [](const DataStorage &expected_vars,
                                  const DataStorage &actual_vars) {
      uint8_t dimensions_count = UINT8_C(2);
      AssertArraysEqual(expected_vars.GetTop< ArrayValue<T> >(),
                        actual_vars.GetTop< ArrayValue<T> >(),
                        dimensions_count);
    };
    auto operands_asserter = [](const DataStorage &expected_operands,
                                const DataStorage &actual_operands) {
      auto expected_operand = expected_operands.GetTop<VarAddressValue>();
      auto actual_operand = actual_operands.GetTop<VarAddressValue>();
      ASSERT_TRUE(expected_operand);
      ASSERT_TRUE(actual_operand);
      uint8_t dimensions_count = UINT8_C(1);
      AssertArraysEqual(
          *(static_cast<ArrayValue<T>*>(expected_operand)),
          *(static_cast<ArrayValue<T>*>(actual_operand)),
          dimensions_count);
    };
    unique_ptr<Code> cmds(new Code());
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t inner_array_size = INT32_C(2);
    cmds->Write<int32_t>(inner_array_size);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t outer_array_size = INT32_C(1);
    cmds->Write<int32_t>(outer_array_size);
    cmds->Write<CmdId>(create_array_cmd_id);
    uint8_t outer_array_dimensions_count = UINT8_C(2);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    cmds->Write<CmdId>(create_var_cmd_id);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    cmds->Write<CmdId>(CmdId::kLoadIntValue);
    int32_t item_index = INT32_C(0);
    cmds->Write<int32_t>(item_index);
    cmds->Write<CmdId>(load_array_cmd_id);
    uint32_t var_index = UINT32_C(0);
    cmds->Write<uint32_t>(var_index);
    cmds->Write<CmdId>(load_address_cmd_id);
    cmds->Write<uint8_t>(outer_array_dimensions_count);
    vector<size_t> outer_array_dimensions = {
      static_cast<size_t>(outer_array_size),
      static_cast<size_t>(inner_array_size)};
    auto outer_array = ArrayValue<T>::Multidimensional(
        outer_array_dimensions.begin(), outer_array_dimensions.end());
    ArrayValue<T> &outer_array_item =
        outer_array.GetItemArray(static_cast<size_t>(item_index));
    DataStorage expected_local_vars;
    expected_local_vars.Push(move(outer_array));
    DataStorage expected_operands;
    expected_operands.Push<VarAddressValue>(&outer_array_item);
    uint32_t main_cmds_code_size = cmds->GetPosition();
    DataStorage expected_global_vars;
    DataStorageAsserter global_vars_asserter = nullptr;
    TestExecute(move(cmds),
                main_cmds_code_size,
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
  NativeFuncValue func_value = [](VM*) {};
  vector<NativeFuncValue> native_funcs(7);
  native_funcs[func_index] = func_value;
  TestLoadValueCmd(
      CmdId::kLoadNativeFuncValue, func_value, func_index, native_funcs);
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
  int32_t value = INT32_C(777);
  TestCreateAndInitGlobalVarCmd<IntValue>(
      CmdId::kLoadIntValue, CmdId::kCreateAndInitGlobalIntVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalLongVarCmd) {
  int64_t value = INT64_C(777);
  TestCreateAndInitGlobalVarCmd<LongValue>(
      CmdId::kLoadLongValue, CmdId::kCreateAndInitGlobalLongVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalDoubleVarCmd) {
  double value = 7.7;
  TestCreateAndInitGlobalVarCmd<DoubleValue>(
      CmdId::kLoadDoubleValue, CmdId::kCreateAndInitGlobalDoubleVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalCharVarCmd) {
  char value = 'a';
  TestCreateAndInitGlobalVarCmd<CharValue>(
      CmdId::kLoadCharValue, CmdId::kCreateAndInitGlobalCharVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalBoolVarCmd) {
  bool value = true;
  TestCreateAndInitGlobalVarCmd<BoolValue>(
      CmdId::kLoadBoolValue, CmdId::kCreateAndInitGlobalBoolVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalStringVarCmd) {
  string value = "swag";
  TestCreateAndInitGlobalVarCmd<StringValue>(
      CmdId::kLoadStringValue, CmdId::kCreateAndInitGlobalStringVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalIntArrayVarCmd) {
  int32_t array_item = INT32_C(777);
  TestCreateAndInitGlobalArrayVarCmd<IntValue>(
      CmdId::kLoadIntValue,
      CmdId::kCreateAndInitIntArray,
      CmdId::kCreateAndInitGlobalIntArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalLongArrayVarCmd) {
  int64_t array_item = INT64_C(777);
  TestCreateAndInitGlobalArrayVarCmd<LongValue>(
      CmdId::kLoadLongValue,
      CmdId::kCreateAndInitLongArray,
      CmdId::kCreateAndInitGlobalLongArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalDoubleArrayVarCmd) {
  double array_item = 7.7;
  TestCreateAndInitGlobalArrayVarCmd<DoubleValue>(
      CmdId::kLoadDoubleValue,
      CmdId::kCreateAndInitDoubleArray,
      CmdId::kCreateAndInitGlobalDoubleArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalCharArrayVarCmd) {
  char array_item = 'a';
  TestCreateAndInitGlobalArrayVarCmd<CharValue>(
      CmdId::kLoadCharValue,
      CmdId::kCreateAndInitCharArray,
      CmdId::kCreateAndInitGlobalCharArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalBoolArrayVarCmd) {
  bool array_item = true;
  TestCreateAndInitGlobalArrayVarCmd<BoolValue>(
      CmdId::kLoadBoolValue,
      CmdId::kCreateAndInitBoolArray,
      CmdId::kCreateAndInitGlobalBoolArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitGlobalStringArrayVarCmd) {
  string array_item = "swag";
  TestCreateAndInitGlobalArrayVarCmd<StringValue>(
      CmdId::kLoadStringValue,
      CmdId::kCreateAndInitStringArray,
      CmdId::kCreateAndInitGlobalStringArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateLocalIntVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<IntValue>(),
              actual_local_vars.GetTop<IntValue>());
  };
  TestCreateLocalVarCmd<IntValue>(
      CmdId::kCreateLocalIntVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateLocalLongVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<LongValue>(),
              actual_local_vars.GetTop<LongValue>());
  };
  TestCreateLocalVarCmd<LongValue>(
      CmdId::kCreateLocalLongVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateLocalDoubleVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<DoubleValue>(),
              actual_local_vars.GetTop<DoubleValue>());
  };
  TestCreateLocalVarCmd<DoubleValue>(
      CmdId::kCreateLocalDoubleVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateLocalCharVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<CharValue>(),
              actual_local_vars.GetTop<CharValue>());
  };
  TestCreateLocalVarCmd<CharValue>(
      CmdId::kCreateLocalCharVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateLocalBoolVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<BoolValue>(),
              actual_local_vars.GetTop<BoolValue>());
  };
  TestCreateLocalVarCmd<BoolValue>(
      CmdId::kCreateLocalBoolVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateLocalStringVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<StringValue>(),
              actual_local_vars.GetTop<StringValue>());
  };
  TestCreateLocalVarCmd<StringValue>(
      CmdId::kCreateLocalStringVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateLocalArrayVarCmd) {
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    uint8_t dimensions_count = UINT8_C(1);
    AssertArraysEqual(
        expected_local_vars.GetTop< ArrayValue<IntValue> >(),
        actual_local_vars.GetTop< ArrayValue<IntValue> >(),
        dimensions_count);
  };
  TestCreateLocalVarCmd< ArrayValue<IntValue> >(
      CmdId::kCreateLocalArrayVar, local_vars_asserter);
}

TEST_F(SimpleVMTest, CreateAndInitLocalIntVarCmd) {
  int32_t value = INT32_C(777);
  TestCreateAndInitLocalVarCmd<IntValue>(
      CmdId::kLoadIntValue, CmdId::kCreateAndInitLocalIntVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitLocalLongVarCmd) {
  int64_t value = INT64_C(777);
  TestCreateAndInitLocalVarCmd<LongValue>(
      CmdId::kLoadLongValue, CmdId::kCreateAndInitLocalLongVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitLocalDoubleVarCmd) {
  double value = 7.7;
  TestCreateAndInitLocalVarCmd<DoubleValue>(
      CmdId::kLoadDoubleValue, CmdId::kCreateAndInitLocalDoubleVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitLocalCharVarCmd) {
  char value = 'a';
  TestCreateAndInitLocalVarCmd<CharValue>(
      CmdId::kLoadCharValue, CmdId::kCreateAndInitLocalCharVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitLocalBoolVarCmd) {
  bool value = true;
  TestCreateAndInitLocalVarCmd<BoolValue>(
      CmdId::kLoadBoolValue, CmdId::kCreateAndInitLocalBoolVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitLocalStringVarCmd) {
  string value = "swag";
  TestCreateAndInitLocalVarCmd<StringValue>(
      CmdId::kLoadStringValue, CmdId::kCreateAndInitLocalStringVar, value);
}

TEST_F(SimpleVMTest, CreateAndInitLocalIntArrayVarCmd) {
  int32_t array_item = INT32_C(777);
  TestCreateAndInitLocalArrayVarCmd<IntValue>(
      CmdId::kLoadIntValue,
      CmdId::kCreateAndInitIntArray,
      CmdId::kCreateAndInitLocalIntArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitLocalLongArrayVarCmd) {
  int64_t array_item = INT64_C(777);
  TestCreateAndInitLocalArrayVarCmd<LongValue>(
      CmdId::kLoadLongValue,
      CmdId::kCreateAndInitLongArray,
      CmdId::kCreateAndInitLocalLongArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitLocalDoubleArrayVarCmd) {
  double array_item = 7.7;
  TestCreateAndInitLocalArrayVarCmd<DoubleValue>(
      CmdId::kLoadDoubleValue,
      CmdId::kCreateAndInitDoubleArray,
      CmdId::kCreateAndInitLocalDoubleArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitLocalCharArrayVarCmd) {
  char array_item = 'a';
  TestCreateAndInitLocalArrayVarCmd<CharValue>(
      CmdId::kLoadCharValue,
      CmdId::kCreateAndInitCharArray,
      CmdId::kCreateAndInitLocalCharArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitLocalBoolArrayVarCmd) {
  bool array_item = true;
  TestCreateAndInitLocalArrayVarCmd<BoolValue>(
      CmdId::kLoadBoolValue,
      CmdId::kCreateAndInitBoolArray,
      CmdId::kCreateAndInitLocalBoolArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, CreateAndInitLocalStringArrayVarCmd) {
  string array_item = "swag";
  TestCreateAndInitLocalArrayVarCmd<StringValue>(
      CmdId::kLoadStringValue,
      CmdId::kCreateAndInitStringArray,
      CmdId::kCreateAndInitLocalStringArrayVar,
      array_item);
}

TEST_F(SimpleVMTest, DestroyLocalIntVarCmd) {
  TestDestroyLocalVarCmd(
      CmdId::kCreateLocalIntVar, CmdId::kDestroyLocalIntVar);
}

TEST_F(SimpleVMTest, DestroyLocalLongVarCmd) {
  TestDestroyLocalVarCmd(
      CmdId::kCreateLocalLongVar, CmdId::kDestroyLocalLongVar);
}

TEST_F(SimpleVMTest, DestroyLocalDoubleVarCmd) {
  TestDestroyLocalVarCmd(
      CmdId::kCreateLocalDoubleVar, CmdId::kDestroyLocalDoubleVar);
}

TEST_F(SimpleVMTest, DestroyLocalCharVarCmd) {
  TestDestroyLocalVarCmd(
      CmdId::kCreateLocalCharVar, CmdId::kDestroyLocalCharVar);
}

TEST_F(SimpleVMTest, DestroyLocalBoolVarCmd) {
  TestDestroyLocalVarCmd(
      CmdId::kCreateLocalBoolVar, CmdId::kDestroyLocalBoolVar);
}

TEST_F(SimpleVMTest, DestroyLocalStringVarCmd) {
  TestDestroyLocalVarCmd(
      CmdId::kCreateLocalStringVar, CmdId::kDestroyLocalStringVar);
}

TEST_F(SimpleVMTest, DestroyLocalIntArrayVarCmd) {
  TestDestroyLocalArrayVarCmd(CmdId::kCreateIntArray,
                              CmdId::kCreateAndInitLocalIntArrayVar,
                              CmdId::kDestroyLocalIntArrayVar);
}

TEST_F(SimpleVMTest, DestroyLocalLongArrayVarCmd) {
  TestDestroyLocalArrayVarCmd(CmdId::kCreateLongArray,
                              CmdId::kCreateAndInitLocalLongArrayVar,
                              CmdId::kDestroyLocalLongArrayVar);
}

TEST_F(SimpleVMTest, DestroyLocalDoubleArrayVarCmd) {
  TestDestroyLocalArrayVarCmd(CmdId::kCreateDoubleArray,
                              CmdId::kCreateAndInitLocalDoubleArrayVar,
                              CmdId::kDestroyLocalDoubleArrayVar);
}

TEST_F(SimpleVMTest, DestroyLocalCharArrayVarCmd) {
  TestDestroyLocalArrayVarCmd(CmdId::kCreateCharArray,
                              CmdId::kCreateAndInitLocalCharArrayVar,
                              CmdId::kDestroyLocalCharArrayVar);
}

TEST_F(SimpleVMTest, DestroyLocalBoolArrayVarCmd) {
  TestDestroyLocalArrayVarCmd(CmdId::kCreateBoolArray,
                              CmdId::kCreateAndInitLocalBoolArrayVar,
                              CmdId::kDestroyLocalBoolArrayVar);
}

TEST_F(SimpleVMTest, DestroyLocalStringArrayVarCmd) {
  TestDestroyLocalArrayVarCmd(CmdId::kCreateStringArray,
                              CmdId::kCreateAndInitLocalStringArrayVar,
                              CmdId::kDestroyLocalStringArrayVar);
}

TEST_F(SimpleVMTest, JumpIfNotCmdWithFalseCondition) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds->Write<bool>(false);
  cmds->Write<CmdId>(CmdId::kJumpIfNot);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  cmds->Write<int32_t>(INT32_C(7));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  TestExecute(move(cmds), main_cmds_code_size);
}

TEST_F(SimpleVMTest, JumpIfNotCmdWithTrueCondition) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    ASSERT_EQ(expected_operands.GetTop<IntValue>(),
              actual_operands.GetTop<IntValue>());
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds->Write<bool>(true);
  cmds->Write<CmdId>(CmdId::kJumpIfNot);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value = INT32_C(7);
  cmds->Write<int32_t>(value);
  DataStorage expected_operands;
  expected_operands.Push(IntValue(value));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, ImplicitJumpIfNotCmdWithFalseCondition) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    ASSERT_EQ(expected_operands.GetTop<BoolValue>(),
              actual_operands.GetTop<BoolValue>());
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  bool condition = false;
  cmds->Write<bool>(condition);
  cmds->Write<CmdId>(CmdId::kImplicitJumpIfNot);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  cmds->Write<int32_t>(INT32_C(7));
  DataStorage expected_operands;
  expected_operands.Push(BoolValue(condition));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, ImplicitJumpIfNotCmdWithTrueCondition) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    size_t operand_index = 0;
    ASSERT_EQ(expected_operands.Get<BoolValue>(operand_index),
              actual_operands.Get<BoolValue>(operand_index));
    operand_index = 1;
    ASSERT_EQ(expected_operands.Get<IntValue>(operand_index),
              actual_operands.Get<IntValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  bool condition = true;
  cmds->Write<bool>(condition);
  cmds->Write<CmdId>(CmdId::kImplicitJumpIfNot);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value = INT32_C(7);
  cmds->Write<int32_t>(value);
  DataStorage expected_operands;
  expected_operands.Push(BoolValue(condition));
  expected_operands.Push(IntValue(value));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, ImplicitJumpIfCmdWithTrueCondition) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    ASSERT_EQ(expected_operands.GetTop<BoolValue>(),
              actual_operands.GetTop<BoolValue>());
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  bool condition = true;
  cmds->Write<bool>(condition);
  cmds->Write<CmdId>(CmdId::kImplicitJumpIf);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  cmds->Write<int32_t>(INT32_C(7));
  DataStorage expected_operands;
  expected_operands.Push(BoolValue(condition));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, ImplicitJumpIfCmdWithFalseCondition) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    size_t operand_index = 0;
    ASSERT_EQ(expected_operands.Get<BoolValue>(operand_index),
              actual_operands.Get<BoolValue>(operand_index));
    operand_index = 1;
    ASSERT_EQ(expected_operands.Get<IntValue>(operand_index),
              actual_operands.Get<IntValue>(operand_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  bool condition = false;
  cmds->Write<bool>(condition);
  cmds->Write<CmdId>(CmdId::kImplicitJumpIf);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  int32_t value = INT32_C(7);
  cmds->Write<int32_t>(value);
  DataStorage expected_operands;
  expected_operands.Push(BoolValue(condition));
  expected_operands.Push(IntValue(value));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, DirectJumpCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kDirectJump);
  int32_t offset = INT32_C(5);
  cmds->Write<int32_t>(offset);
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  cmds->Write<int32_t>(INT32_C(7));
  uint32_t main_cmds_code_size = cmds->GetPosition();
  TestExecute(move(cmds), main_cmds_code_size);
}

TEST_F(SimpleVMTest, LoadLocalIntVarValueCmd) {
  TestLoadLocalVarValueCmd<IntValue>(CmdId::kCreateLocalIntVar,
                                     CmdId::kDestroyLocalIntVar,
                                     CmdId::kLoadLocalIntVarValue);
}

TEST_F(SimpleVMTest, LoadLocalLongVarValueCmd) {
  TestLoadLocalVarValueCmd<LongValue>(CmdId::kCreateLocalLongVar,
                                      CmdId::kDestroyLocalLongVar,
                                      CmdId::kLoadLocalLongVarValue);
}

TEST_F(SimpleVMTest, LoadLocalDoubleVarValueCmd) {
  TestLoadLocalVarValueCmd<DoubleValue>(CmdId::kCreateLocalDoubleVar,
                                        CmdId::kDestroyLocalDoubleVar,
                                        CmdId::kLoadLocalDoubleVarValue);
}

TEST_F(SimpleVMTest, LoadLocalCharVarValueCmd) {
  TestLoadLocalVarValueCmd<CharValue>(CmdId::kCreateLocalCharVar,
                                      CmdId::kDestroyLocalCharVar,
                                      CmdId::kLoadLocalCharVarValue);
}

TEST_F(SimpleVMTest, LoadLocalBoolVarValueCmd) {
  TestLoadLocalVarValueCmd<BoolValue>(CmdId::kCreateLocalBoolVar,
                                      CmdId::kDestroyLocalBoolVar,
                                      CmdId::kLoadLocalBoolVarValue);
}

TEST_F(SimpleVMTest, LoadLocalStringVarValueCmd) {
  TestLoadLocalVarValueCmd<StringValue>(CmdId::kCreateLocalStringVar,
                                        CmdId::kDestroyLocalStringVar,
                                        CmdId::kLoadLocalStringVarValue);
}

TEST_F(SimpleVMTest, LoadLocalIntArrayVarValueCmd) {
  TestLoadLocalArrayVarValueCmd<IntValue>(CmdId::kCreateLocalArrayVar,
                                          CmdId::kDestroyLocalIntArrayVar,
                                          CmdId::kLoadLocalIntArrayVarValue);
}

TEST_F(SimpleVMTest, LoadLocalLongArrayVarValueCmd) {
  TestLoadLocalArrayVarValueCmd<LongValue>(CmdId::kCreateLocalArrayVar,
                                           CmdId::kDestroyLocalLongArrayVar,
                                           CmdId::kLoadLocalLongArrayVarValue);
}

TEST_F(SimpleVMTest, LoadLocalDoubleArrayVarValueCmd) {
  TestLoadLocalArrayVarValueCmd<DoubleValue>(
      CmdId::kCreateLocalArrayVar,
      CmdId::kDestroyLocalDoubleArrayVar,
      CmdId::kLoadLocalDoubleArrayVarValue);
}

TEST_F(SimpleVMTest, LoadLocalCharArrayVarValueCmd) {
  TestLoadLocalArrayVarValueCmd<CharValue>(CmdId::kCreateLocalArrayVar,
                                           CmdId::kDestroyLocalCharArrayVar,
                                           CmdId::kLoadLocalCharArrayVarValue);
}

TEST_F(SimpleVMTest, LoadLocalBoolArrayVarValueCmd) {
  TestLoadLocalArrayVarValueCmd<BoolValue>(CmdId::kCreateLocalArrayVar,
                                           CmdId::kDestroyLocalBoolArrayVar,
                                           CmdId::kLoadLocalBoolArrayVarValue);
}

TEST_F(SimpleVMTest, LoadLocalStringArrayVarValueCmd) {
  TestLoadLocalArrayVarValueCmd<StringValue>(
      CmdId::kCreateLocalArrayVar,
      CmdId::kDestroyLocalStringArrayVar,
      CmdId::kLoadLocalStringArrayVarValue);
}

TEST_F(SimpleVMTest, CallAndReturnCmds) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    ASSERT_EQ(expected_operands.GetTop<StringValue>(),
              actual_operands.GetTop<StringValue>());
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds->Write<CmdId>(CmdId::kLoadFuncValue);
  uint32_t func_address_placeholder = cmds->GetPosition();
  cmds->Skip(sizeof(uint32_t));
  cmds->Write<CmdId>(CmdId::kCall);
  cmds->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  cmds->Write<CmdId>(CmdId::kReturn);
  uint32_t func_address = cmds->GetPosition();
  cmds->Write<CmdId>(CmdId::kCreateLocalStringVar);
  cmds->Write<CmdId>(CmdId::kLoadLocalStringVarValue);
  uint32_t var_index = UINT32_C(0);
  cmds->Write<uint32_t>(var_index);
  cmds->Write<CmdId>(CmdId::kDestroyLocalStringVar);
  cmds->Write<CmdId>(CmdId::kReturn);
  cmds->SetPosition(func_address_placeholder);
  cmds->Write<uint32_t>(func_address);
  DataStorage expected_operands;
  expected_operands.Push(StringValue());
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, CallNativeAndReturnCmds) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kCreateLocalIntVar);
  cmds->Write<CmdId>(CmdId::kLoadNativeFuncValue);
  uint32_t func_index = UINT32_C(5);
  cmds->Write<uint32_t>(func_index);
  cmds->Write<CmdId>(CmdId::kCallNative);
  cmds->Write<CmdId>(CmdId::kDestroyLocalIntVar);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  vector<NativeFuncValue> native_funcs(7);
  NativeFuncValue func_value = [](VM *vm) {
    ASSERT_TRUE(vm);
    ASSERT_EQ(2, vm->GetFuncFrames().size());
    ASSERT_EQ(1, vm->GetFuncFrames().back().GetLocalVarsStartIndex());
  };
  native_funcs[func_index] = func_value;
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  DataStorage expected_operands;
  DataStorageAsserter operands_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter,
              native_funcs);
}

TEST_F(SimpleVMTest, UnloadIntCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadIntValue);
  cmds->Write<int32_t>(INT32_C(7));
  cmds->Write<CmdId>(CmdId::kUnloadInt);
  TestUnloadCmd(move(cmds));
}

TEST_F(SimpleVMTest, UnloadLongCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadLongValue);
  cmds->Write<int64_t>(INT64_C(7));
  cmds->Write<CmdId>(CmdId::kUnloadLong);
  TestUnloadCmd(move(cmds));
}

TEST_F(SimpleVMTest, UnloadDoubleCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadDoubleValue);
  cmds->Write<double>(7.7);
  cmds->Write<CmdId>(CmdId::kUnloadDouble);
  TestUnloadCmd(move(cmds));
}

TEST_F(SimpleVMTest, UnloadCharCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadCharValue);
  cmds->Write<char>('a');
  cmds->Write<CmdId>(CmdId::kUnloadChar);
  TestUnloadCmd(move(cmds));
}

TEST_F(SimpleVMTest, UnloadBoolCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadBoolValue);
  cmds->Write<bool>(true);
  cmds->Write<CmdId>(CmdId::kUnloadBool);
  TestUnloadCmd(move(cmds));
}

TEST_F(SimpleVMTest, UnloadStringCmd) {
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kLoadStringValue);
  cmds->Write<string>("swag");
  cmds->Write<CmdId>(CmdId::kUnloadString);
  TestUnloadCmd(move(cmds));
}

TEST_F(SimpleVMTest, UnloadIntArrayCmd) {
  TestUnloadArrayCmd(CmdId::kCreateIntArray, CmdId::kUnloadIntArray);
}

TEST_F(SimpleVMTest, UnloadLongArrayCmd) {
  TestUnloadArrayCmd(CmdId::kCreateLongArray, CmdId::kUnloadLongArray);
}

TEST_F(SimpleVMTest, UnloadDoubleArrayCmd) {
  TestUnloadArrayCmd(CmdId::kCreateDoubleArray, CmdId::kUnloadDoubleArray);
}

TEST_F(SimpleVMTest, UnloadCharArrayCmd) {
  TestUnloadArrayCmd(CmdId::kCreateCharArray, CmdId::kUnloadCharArray);
}

TEST_F(SimpleVMTest, UnloadBoolArrayCmd) {
  TestUnloadArrayCmd(CmdId::kCreateBoolArray, CmdId::kUnloadBoolArray);
}

TEST_F(SimpleVMTest, UnloadStringArrayCmd) {
  TestUnloadArrayCmd(CmdId::kCreateStringArray, CmdId::kUnloadStringArray);
}

TEST_F(SimpleVMTest, LoadGlobalIntVarValueCmd) {
  TestLoadGlobalVarValueCmd<IntValue>(
      CmdId::kCreateGlobalIntVar, CmdId::kLoadGlobalIntVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalLongVarValueCmd) {
  TestLoadGlobalVarValueCmd<LongValue>(
      CmdId::kCreateGlobalLongVar, CmdId::kLoadGlobalLongVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalDoubleVarValueCmd) {
  TestLoadGlobalVarValueCmd<DoubleValue>(
      CmdId::kCreateGlobalDoubleVar, CmdId::kLoadGlobalDoubleVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalCharVarValueCmd) {
  TestLoadGlobalVarValueCmd<CharValue>(
      CmdId::kCreateGlobalCharVar, CmdId::kLoadGlobalCharVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalBoolVarValueCmd) {
  TestLoadGlobalVarValueCmd<BoolValue>(
      CmdId::kCreateGlobalBoolVar, CmdId::kLoadGlobalBoolVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalStringVarValueCmd) {
  TestLoadGlobalVarValueCmd<StringValue>(
      CmdId::kCreateGlobalStringVar, CmdId::kLoadGlobalStringVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalIntArrayVarValueCmd) {
  TestLoadGlobalArrayVarValueCmd<IntValue>(CmdId::kLoadGlobalIntArrayVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalLongArrayVarValueCmd) {
  TestLoadGlobalArrayVarValueCmd<LongValue>(
      CmdId::kLoadGlobalLongArrayVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalDoubleArrayVarValueCmd) {
  TestLoadGlobalArrayVarValueCmd<DoubleValue>(
      CmdId::kLoadGlobalDoubleArrayVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalCharArrayVarValueCmd) {
  TestLoadGlobalArrayVarValueCmd<CharValue>(
      CmdId::kLoadGlobalCharArrayVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalBoolArrayVarValueCmd) {
  TestLoadGlobalArrayVarValueCmd<BoolValue>(
      CmdId::kLoadGlobalBoolArrayVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalStringArrayVarValueCmd) {
  TestLoadGlobalArrayVarValueCmd<StringValue>(
      CmdId::kLoadGlobalStringArrayVarValue);
}

TEST_F(SimpleVMTest, LoadGlobalVarAddressCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    auto expected_operand = expected_operands.GetTop<VarAddressValue>();
    auto actual_operand = actual_operands.GetTop<VarAddressValue>();
    ASSERT_TRUE(expected_operand);
    ASSERT_TRUE(actual_operand);
    ASSERT_EQ(*(static_cast<StringValue*>(expected_operand)),
              *(static_cast<StringValue*>(actual_operand)));
  };
  auto global_vars_asserter = [](const DataStorage &expected_global_vars,
                                 const DataStorage &actual_global_vars) {
    size_t var_index = 5;
    ASSERT_EQ(expected_global_vars.Get<StringValue>(var_index),
              actual_global_vars.Get<StringValue>(var_index));
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kCreateGlobalStringVar);
  uint32_t var_index = UINT32_C(5);
  cmds->Write<uint32_t>(var_index);
  cmds->Write<CmdId>(CmdId::kLoadGlobalVarAddress);
  cmds->Write<uint32_t>(var_index);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  size_t global_vars_size = 7;
  DataStorage expected_global_vars(global_vars_size, global_vars_size);
  expected_global_vars.Create(var_index, StringValue());
  DataStorage expected_operands;
  expected_operands.Push(expected_global_vars.GetAddress(var_index));
  DataStorage expected_local_vars;
  DataStorageAsserter local_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, LoadLocalVarAddressCmd) {
  auto operands_asserter = [](const DataStorage &expected_operands,
                              const DataStorage &actual_operands) {
    auto expected_operand = expected_operands.GetTop<VarAddressValue>();
    auto actual_operand = actual_operands.GetTop<VarAddressValue>();
    ASSERT_TRUE(expected_operand);
    ASSERT_TRUE(actual_operand);
    ASSERT_EQ(*(static_cast<StringValue*>(expected_operand)),
              *(static_cast<StringValue*>(actual_operand)));
  };
  auto local_vars_asserter = [](const DataStorage &expected_local_vars,
                                const DataStorage &actual_local_vars) {
    ASSERT_EQ(expected_local_vars.GetTop<StringValue>(),
              actual_local_vars.GetTop<StringValue>());
  };
  unique_ptr<Code> cmds(new Code());
  cmds->Write<CmdId>(CmdId::kCreateLocalStringVar);
  uint32_t var_index = UINT32_C(0);
  cmds->Write<CmdId>(CmdId::kLoadLocalVarAddress);
  cmds->Write<uint32_t>(var_index);
  uint32_t main_cmds_code_size = cmds->GetPosition();
  DataStorage expected_local_vars;
  expected_local_vars.Push(StringValue());
  DataStorage expected_operands;
  expected_operands.Push(expected_local_vars.GetAddress(var_index));
  DataStorage expected_global_vars;
  DataStorageAsserter global_vars_asserter = nullptr;
  TestExecute(move(cmds),
              main_cmds_code_size,
              expected_global_vars,
              global_vars_asserter,
              expected_local_vars,
              local_vars_asserter,
              expected_operands,
              operands_asserter);
}

TEST_F(SimpleVMTest, LoadIntArrayElementValueCmd) {
  TestLoadArrayElementValueCmd<IntValue>(
      CmdId::kCreateIntArray, CmdId::kLoadIntArrayElementValue);
}

TEST_F(SimpleVMTest, LoadLongArrayElementValueCmd) {
  TestLoadArrayElementValueCmd<LongValue>(
      CmdId::kCreateLongArray, CmdId::kLoadLongArrayElementValue);
}

TEST_F(SimpleVMTest, LoadStringArrayElementValueCmd) {
  TestLoadArrayElementValueCmd<StringValue>(
      CmdId::kCreateStringArray, CmdId::kLoadStringArrayElementValue);
}

TEST_F(SimpleVMTest, LoadDoubleArrayElementValueCmd) {
  TestLoadArrayElementValueCmd<DoubleValue>(
      CmdId::kCreateDoubleArray, CmdId::kLoadDoubleArrayElementValue);
}

TEST_F(SimpleVMTest, LoadCharArrayElementValueCmd) {
  TestLoadArrayElementValueCmd<CharValue>(
      CmdId::kCreateCharArray, CmdId::kLoadCharArrayElementValue);
}

TEST_F(SimpleVMTest, LoadBoolArrayElementValueCmd) {
  TestLoadArrayElementValueCmd<BoolValue>(
      CmdId::kCreateBoolArray, CmdId::kLoadBoolArrayElementValue);
}

TEST_F(SimpleVMTest, LoadIntArrayElementAddressCmd) {
  TestLoadArrayElementAddressCmd<IntValue>(
      CmdId::kCreateIntArray,
      CmdId::kCreateAndInitLocalIntArrayVar,
      CmdId::kLoadLocalIntArrayVarValue,
      CmdId::kLoadIntArrayElementAddress);
}

TEST_F(SimpleVMTest, LoadLongArrayElementAddressCmd) {
  TestLoadArrayElementAddressCmd<LongValue>(
      CmdId::kCreateLongArray,
      CmdId::kCreateAndInitLocalLongArrayVar,
      CmdId::kLoadLocalLongArrayVarValue,
      CmdId::kLoadLongArrayElementAddress);
}

TEST_F(SimpleVMTest, LoadDoubleArrayElementAddressCmd) {
  TestLoadArrayElementAddressCmd<DoubleValue>(
      CmdId::kCreateDoubleArray,
      CmdId::kCreateAndInitLocalDoubleArrayVar,
      CmdId::kLoadLocalDoubleArrayVarValue,
      CmdId::kLoadDoubleArrayElementAddress);
}

TEST_F(SimpleVMTest, LoadCharArrayElementAddressCmd) {
  TestLoadArrayElementAddressCmd<CharValue>(
      CmdId::kCreateCharArray,
      CmdId::kCreateAndInitLocalCharArrayVar,
      CmdId::kLoadLocalCharArrayVarValue,
      CmdId::kLoadCharArrayElementAddress);
}

TEST_F(SimpleVMTest, LoadBoolArrayElementAddressCmd) {
  TestLoadArrayElementAddressCmd<BoolValue>(
      CmdId::kCreateBoolArray,
      CmdId::kCreateAndInitLocalBoolArrayVar,
      CmdId::kLoadLocalBoolArrayVarValue,
      CmdId::kLoadBoolArrayElementAddress);
}

TEST_F(SimpleVMTest, LoadStringArrayElementAddressCmd) {
  TestLoadArrayElementAddressCmd<StringValue>(
      CmdId::kCreateStringArray,
      CmdId::kCreateAndInitLocalStringArrayVar,
      CmdId::kLoadLocalStringArrayVarValue,
      CmdId::kLoadStringArrayElementAddress);
}
}
}
