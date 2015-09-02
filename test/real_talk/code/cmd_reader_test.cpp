
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/create_local_var_cmd.h"
#include "real_talk/code/destroy_local_var_cmd.h"
#include "real_talk/code/load_value_cmd.h"
#include "real_talk/code/unload_cmd.h"
#include "real_talk/code/create_and_init_global_var_cmd.h"
#include "real_talk/code/create_and_init_local_var_cmd.h"
#include "real_talk/code/create_array_cmd.h"
#include "real_talk/code/create_and_init_array_cmd.h"
#include "real_talk/code/jump_cmd.h"
#include "real_talk/code/return_cmd.h"
#include "real_talk/code/load_global_var_value_cmd.h"
#include "real_talk/code/load_local_var_value_cmd.h"
#include "real_talk/code/load_global_var_address_cmd.h"
#include "real_talk/code/load_local_var_address_cmd.h"
#include "real_talk/code/load_array_element_value_cmd.h"
#include "real_talk/code/load_array_element_address_cmd.h"
#include "real_talk/code/store_cmd.h"
#include "real_talk/code/cast_cmd.h"
#include "real_talk/code/call_cmd.h"
#include "real_talk/code/call_native_cmd.h"
#include "real_talk/code/and_cmd.h"
#include "real_talk/code/or_cmd.h"
#include "real_talk/code/mul_cmd.h"
#include "real_talk/code/div_cmd.h"
#include "real_talk/code/sum_cmd.h"
#include "real_talk/code/sub_cmd.h"
#include "real_talk/code/equal_cmd.h"
#include "real_talk/code/not_equal_cmd.h"
#include "real_talk/code/greater_cmd.h"
#include "real_talk/code/greater_or_equal_cmd.h"
#include "real_talk/code/less_cmd.h"
#include "real_talk/code/less_or_equal_cmd.h"
#include "real_talk/code/logical_negate_cmd.h"
#include "real_talk/code/arithmetic_negate_cmd.h"
#include "real_talk/code/pre_dec_cmd.h"
#include "real_talk/code/pre_inc_cmd.h"
#include "real_talk/code/code.h"

using std::stringstream;
using std::string;
using testing::Test;

namespace real_talk {
namespace code {

class CmdReaderTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestGetNextCmd(Code &code, const Cmd &expected_cmd) {
    code.SetPosition(UINT32_C(0));
    CmdReader cmd_reader;
    cmd_reader.SetCode(&code);
    const Cmd &actual_cmd = cmd_reader.GetNextCmd();
    ASSERT_EQ(expected_cmd, actual_cmd);
  }

  void TestCreateGlobalVarCmd(
      CmdId cmd_id, const CreateGlobalVarCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint32_t>(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitGlobalVarCmd(
      CmdId cmd_id, const CreateAndInitGlobalVarCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint32_t>(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitGlobalArrayVarCmd(
      CmdId cmd_id, const CreateAndInitGlobalArrayVarCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint32_t>(expected_cmd.GetVarIndex());
    code.Write<uint8_t>(expected_cmd.GetDimensionsCount());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitArrayCmd(
      CmdId cmd_id, const CreateAndInitArrayCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint8_t>(expected_cmd.GetDimensionsCount());
    code.Write<int32_t>(expected_cmd.GetValuesCount());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestJumpCmd(CmdId cmd_id, const JumpCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<int32_t>(expected_cmd.GetOffset());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestLoadGlobalVarValueCmd(
      CmdId cmd_id, const LoadGlobalVarValueCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint32_t>(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestLoadLocalVarValueCmd(
      CmdId cmd_id, const LoadLocalVarValueCmd &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint32_t>(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  template<typename T> void TestArrayCmd(CmdId cmd_id, const T &expected_cmd) {
    Code code;
    code.Write<CmdId>(cmd_id);
    code.Write<uint8_t>(expected_cmd.GetDimensionsCount());
    TestGetNextCmd(code, expected_cmd);
  }
};

TEST_F(CmdReaderTest, CreateGlobalIntVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalIntVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalIntVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalArrayVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalArrayVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalLongVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalLongVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalLongVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalDoubleVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalDoubleVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalDoubleVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalCharVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalCharVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalCharVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalStringVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalStringVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalStringVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalBoolVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalBoolVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalBoolVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalIntVarCmd) {
  CreateLocalIntVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalIntVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalLongVarCmd) {
  CreateLocalLongVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalLongVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalDoubleVarCmd) {
  CreateLocalDoubleVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalDoubleVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalBoolVarCmd) {
  CreateLocalBoolVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalBoolVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalCharVarCmd) {
  CreateLocalCharVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalCharVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalStringVarCmd) {
  CreateLocalStringVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalStringVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalArrayVarCmd) {
  CreateLocalArrayVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateLocalArrayVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalIntVarCmd) {
  DestroyLocalIntVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDestroyLocalIntVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalLongVarCmd) {
  DestroyLocalLongVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDestroyLocalLongVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalDoubleVarCmd) {
  DestroyLocalDoubleVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDestroyLocalDoubleVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalBoolVarCmd) {
  DestroyLocalBoolVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDestroyLocalBoolVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalCharVarCmd) {
  DestroyLocalCharVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDestroyLocalCharVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalStringVarCmd) {
  DestroyLocalStringVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDestroyLocalStringVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalIntArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  DestroyLocalIntArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kDestroyLocalIntArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalLongArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  DestroyLocalLongArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kDestroyLocalLongArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalDoubleArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  DestroyLocalDoubleArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kDestroyLocalDoubleArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalCharArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  DestroyLocalCharArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kDestroyLocalCharArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalBoolArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  DestroyLocalBoolArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kDestroyLocalBoolArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalStringArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  DestroyLocalStringArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kDestroyLocalStringArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadBoolCmd) {
  UnloadBoolCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kUnloadBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadCharCmd) {
  UnloadCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kUnloadChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadIntCmd) {
  UnloadIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kUnloadInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadLongCmd) {
  UnloadLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kUnloadLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadDoubleCmd) {
  UnloadDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kUnloadDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadStringCmd) {
  UnloadStringCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kUnloadString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  UnloadIntArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kUnloadIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  UnloadLongArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kUnloadLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  UnloadDoubleArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kUnloadDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  UnloadCharArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kUnloadCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  UnloadBoolArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kUnloadBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  UnloadStringArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kUnloadStringArray, expected_cmd);
}

TEST_F(CmdReaderTest, LoadIntValueCmd) {
  int32_t value = INT32_C(-7);
  LoadIntValueCmd expected_cmd(value);
  Code code;
  code.Write<CmdId>(CmdId::kLoadIntValue);
  code.Write<int32_t>(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLongValueCmd) {
  int64_t value = INT64_C(-77);
  LoadLongValueCmd expected_cmd(value);
  Code code;
  code.Write<CmdId>(CmdId::kLoadLongValue);
  code.Write<int64_t>(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadBoolValueCmd) {
  bool value = true;
  LoadBoolValueCmd expected_cmd(value);
  Code code;
  code.Write<CmdId>(CmdId::kLoadBoolValue);
  code.Write<bool>(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadCharValueCmd) {
  char value = 'a';
  LoadCharValueCmd expected_cmd(value);
  Code code;
  code.Write<CmdId>(CmdId::kLoadCharValue);
  code.Write<char>(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadStringValueCmd) {
  string value("swag");
  LoadStringValueCmd expected_cmd(value);
  Code code;
  code.Write<CmdId>(CmdId::kLoadStringValue);
  code.Write<string>(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadDoubleValueCmd) {
  double value = -7.77777777777777777777;
  LoadDoubleValueCmd expected_cmd(value);
  Code code;
  code.Write<CmdId>(CmdId::kLoadDoubleValue);
  code.Write<double>(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalIntVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateAndInitGlobalIntVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalIntVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalLongVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateAndInitGlobalLongVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalLongVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalDoubleVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateAndInitGlobalDoubleVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalDoubleVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalCharVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateAndInitGlobalCharVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalCharVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalStringVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateAndInitGlobalStringVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalStringVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalBoolVarCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateAndInitGlobalBoolVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalBoolVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalIntArrayVarCmd) {
  uint32_t var_index = UINT32_C(1);
  uint8_t dimensions_count = UINT8_C(2);
  CreateAndInitGlobalIntArrayVarCmd expected_cmd(var_index, dimensions_count);
  TestCreateAndInitGlobalArrayVarCmd(
      CmdId::kCreateAndInitGlobalIntArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalLongArrayVarCmd) {
  uint32_t var_index = UINT32_C(1);
  uint8_t dimensions_count = UINT8_C(2);
  CreateAndInitGlobalLongArrayVarCmd expected_cmd(var_index, dimensions_count);
  TestCreateAndInitGlobalArrayVarCmd(
      CmdId::kCreateAndInitGlobalLongArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalDoubleArrayVarCmd) {
  uint32_t var_index = UINT32_C(1);
  uint8_t dimensions_count = UINT8_C(2);
  CreateAndInitGlobalDoubleArrayVarCmd expected_cmd(
      var_index, dimensions_count);
  TestCreateAndInitGlobalArrayVarCmd(
      CmdId::kCreateAndInitGlobalDoubleArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalCharArrayVarCmd) {
  uint32_t var_index = UINT32_C(1);
  uint8_t dimensions_count = UINT8_C(2);
  CreateAndInitGlobalCharArrayVarCmd expected_cmd(var_index, dimensions_count);
  TestCreateAndInitGlobalArrayVarCmd(
      CmdId::kCreateAndInitGlobalCharArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalStringArrayVarCmd) {
  uint32_t var_index = UINT32_C(1);
  uint8_t dimensions_count = UINT8_C(2);
  CreateAndInitGlobalStringArrayVarCmd expected_cmd(
      var_index, dimensions_count);
  TestCreateAndInitGlobalArrayVarCmd(
      CmdId::kCreateAndInitGlobalStringArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalBoolArrayVarCmd) {
  uint32_t var_index = UINT32_C(1);
  uint8_t dimensions_count = UINT8_C(2);
  CreateAndInitGlobalBoolArrayVarCmd expected_cmd(var_index, dimensions_count);
  TestCreateAndInitGlobalArrayVarCmd(
      CmdId::kCreateAndInitGlobalBoolArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalIntVarCmd) {
  CreateAndInitLocalIntVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateAndInitLocalIntVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalLongVarCmd) {
  CreateAndInitLocalLongVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateAndInitLocalLongVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalDoubleVarCmd) {
  CreateAndInitLocalDoubleVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateAndInitLocalDoubleVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalBoolVarCmd) {
  CreateAndInitLocalBoolVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateAndInitLocalBoolVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalCharVarCmd) {
  CreateAndInitLocalCharVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateAndInitLocalCharVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalStringVarCmd) {
  CreateAndInitLocalStringVarCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCreateAndInitLocalStringVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalIntArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(1);
  CreateAndInitLocalIntArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kCreateAndInitLocalIntArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalLongArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(1);
  CreateAndInitLocalLongArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kCreateAndInitLocalLongArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalDoubleArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(1);
  CreateAndInitLocalDoubleArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kCreateAndInitLocalDoubleArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalCharArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(1);
  CreateAndInitLocalCharArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kCreateAndInitLocalCharArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalBoolArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(1);
  CreateAndInitLocalBoolArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kCreateAndInitLocalBoolArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalStringArrayVarCmd) {
  uint8_t dimensions_count = UINT8_C(1);
  CreateAndInitLocalStringArrayVarCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kCreateAndInitLocalStringArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateIntArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kCreateIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateLongArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kCreateLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateDoubleArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kCreateDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateBoolArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kCreateBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateCharArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kCreateCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateStringArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kCreateStringArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  int32_t values_count = INT32_C(3);
  CreateAndInitIntArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  int32_t values_count = INT32_C(3);
  CreateAndInitLongArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  int32_t values_count = INT32_C(3);
  CreateAndInitDoubleArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  int32_t values_count = INT32_C(3);
  CreateAndInitBoolArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  int32_t values_count = INT32_C(3);
  CreateAndInitCharArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  int32_t values_count = INT32_C(3);
  CreateAndInitStringArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitStringArray, expected_cmd);
}

TEST_F(CmdReaderTest, JumpIfNotCmd) {
  int32_t offset = INT32_C(7);
  JumpIfNotCmd expected_cmd(offset);
  TestJumpCmd(CmdId::kJumpIfNot, expected_cmd);
}

TEST_F(CmdReaderTest, DirectJumpCmd) {
  int32_t offset = INT32_C(2);
  DirectJumpCmd expected_cmd(offset);
  TestJumpCmd(CmdId::kDirectJump, expected_cmd);
}

TEST_F(CmdReaderTest, ImplicitJumpIfNotCmd) {
  int32_t offset = INT32_C(7);
  ImplicitJumpIfNotCmd expected_cmd(offset);
  TestJumpCmd(CmdId::kImplicitJumpIfNot, expected_cmd);
}

TEST_F(CmdReaderTest, ImplicitJumpIfCmd) {
  int32_t offset = INT32_C(7);
  ImplicitJumpIfCmd expected_cmd(offset);
  TestJumpCmd(CmdId::kImplicitJumpIf, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnCmd) {
  ReturnCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kReturn);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalIntVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalIntVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalIntVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalLongVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalLongVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalLongVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalDoubleVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalDoubleVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalDoubleVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalBoolVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalBoolVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalBoolVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalCharVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalCharVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalCharVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalStringVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalStringVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalStringVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalIntArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalIntArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalIntArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalLongArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalLongArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalLongArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalDoubleArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalDoubleArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(
      CmdId::kLoadGlobalDoubleArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalCharArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalCharArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalCharArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalBoolArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalBoolArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalBoolArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalStringArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalStringArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(
      CmdId::kLoadGlobalStringArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalIntVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalIntVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalIntVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalLongVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalLongVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalLongVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalDoubleVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalDoubleVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalDoubleVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalBoolVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalBoolVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalBoolVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalCharVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalCharVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalCharVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalStringVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalStringVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalStringVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalIntArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalIntArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalIntArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalLongArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalLongArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalLongArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalDoubleArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalDoubleArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalDoubleArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalCharArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalCharArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalCharArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalBoolArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalBoolArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalBoolArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalStringArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalStringArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalStringArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalVarAddressCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalVarAddressCmd expected_cmd(var_index);
  Code code;
  code.Write<CmdId>(CmdId::kLoadGlobalVarAddress);
  code.Write<uint32_t>(var_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalVarAddressCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalVarAddressCmd expected_cmd(var_index);
  Code code;
  code.Write<CmdId>(CmdId::kLoadLocalVarAddress);
  code.Write<uint32_t>(var_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreIntCmd) {
  StoreIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kStoreInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreLongCmd) {
  StoreLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kStoreLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreDoubleCmd) {
  StoreDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kStoreDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreBoolCmd) {
  StoreBoolCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kStoreBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreCharCmd) {
  StoreCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kStoreChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreStringCmd) {
  StoreStringCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kStoreString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  StoreIntArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kStoreIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, StoreLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  StoreLongArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kStoreLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, StoreDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  StoreDoubleArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kStoreDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, StoreCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  StoreCharArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kStoreCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, StoreBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  StoreBoolArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kStoreBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, StoreStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  StoreStringArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kStoreStringArray, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToIntCmd) {
  CastCharToIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastCharToInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToLongCmd) {
  CastCharToLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastCharToLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToDoubleCmd) {
  CastCharToDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastCharToDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToStringCmd) {
  CastCharToStringCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastCharToString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastIntToLongCmd) {
  CastIntToLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastIntToLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastIntToDoubleCmd) {
  CastIntToDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastIntToDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastLongToDoubleCmd) {
  CastLongToDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCastLongToDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadFuncValueCmd) {
  uint32_t address = UINT32_C(7);
  LoadFuncValueCmd expected_cmd(address);
  Code code;
  code.Write<CmdId>(CmdId::kLoadFuncValue);
  code.Write<uint32_t>(address);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadNativeFuncValueCmd) {
  uint32_t func_index = UINT32_C(7);
  LoadNativeFuncValueCmd expected_cmd(func_index);
  Code code;
  code.Write<CmdId>(CmdId::kLoadNativeFuncValue);
  code.Write<uint32_t>(func_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CallCmd) {
  CallCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCall);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CallNativeCmd) {
  CallNativeCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kCallNative);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadIntArrayElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadIntArrayElementValueCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadIntArrayElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLongArrayElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadLongArrayElementValueCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadLongArrayElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadDoubleArrayElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadDoubleArrayElementValueCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadDoubleArrayElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadCharArrayElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadCharArrayElementValueCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadCharArrayElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadBoolArrayElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadBoolArrayElementValueCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadBoolArrayElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadStringArrayElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadStringArrayElementValueCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadStringArrayElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadIntArrayElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadIntArrayElementAddressCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadIntArrayElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLongArrayElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadLongArrayElementAddressCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadLongArrayElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadDoubleArrayElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadDoubleArrayElementAddressCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadDoubleArrayElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadCharArrayElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadCharArrayElementAddressCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadCharArrayElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadBoolArrayElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadBoolArrayElementAddressCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadBoolArrayElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadStringArrayElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadStringArrayElementAddressCmd expected_cmd(dimensions_count);
  TestArrayCmd(
      CmdId::kLoadStringArrayElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, AndCmd) {
  AndCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kAnd);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, OrCmd) {
  OrCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kOr);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulIntCmd) {
  MulIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kMulInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulLongCmd) {
  MulLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kMulLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulDoubleCmd) {
  MulDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kMulDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulCharCmd) {
  MulCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kMulChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivCharCmd) {
  DivCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDivChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivIntCmd) {
  DivIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDivInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivLongCmd) {
  DivLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDivLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivDoubleCmd) {
  DivDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kDivDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumCharCmd) {
  SumCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSumChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumIntCmd) {
  SumIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSumInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumLongCmd) {
  SumLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSumLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumDoubleCmd) {
  SumDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSumDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumStringCmd) {
  SumStringCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSumString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubCharCmd) {
  SubCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSubChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubIntCmd) {
  SubIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSubInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubLongCmd) {
  SubLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSubLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubDoubleCmd) {
  SubDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kSubDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualCharCmd) {
  EqualCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualIntCmd) {
  EqualIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualLongCmd) {
  EqualLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualDoubleCmd) {
  EqualDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualBoolCmd) {
  EqualBoolCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kEqualBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualStringCmd) {
  EqualStringCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kEqualString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  EqualIntArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kEqualIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, EqualLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  EqualLongArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kEqualLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, EqualDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  EqualDoubleArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kEqualDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, EqualCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  EqualCharArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kEqualCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, EqualBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  EqualBoolArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kEqualBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, EqualStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  EqualStringArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kEqualStringArray, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualCharCmd) {
  NotEqualCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kNotEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualIntCmd) {
  NotEqualIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kNotEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualLongCmd) {
  NotEqualLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kNotEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualDoubleCmd) {
  NotEqualDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kNotEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualBoolCmd) {
  NotEqualBoolCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kNotEqualBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualStringCmd) {
  NotEqualStringCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kNotEqualString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  NotEqualIntArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kNotEqualIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  NotEqualLongArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kNotEqualLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  NotEqualDoubleArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kNotEqualDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  NotEqualCharArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kNotEqualCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  NotEqualBoolArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kNotEqualBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  NotEqualStringArrayCmd expected_cmd(dimensions_count);
  TestArrayCmd(CmdId::kNotEqualStringArray, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterCharCmd) {
  GreaterCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterIntCmd) {
  GreaterIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterLongCmd) {
  GreaterLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterDoubleCmd) {
  GreaterDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualCharCmd) {
  GreaterOrEqualCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterOrEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualIntCmd) {
  GreaterOrEqualIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterOrEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualLongCmd) {
  GreaterOrEqualLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterOrEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualDoubleCmd) {
  GreaterOrEqualDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kGreaterOrEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessCharCmd) {
  LessCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessIntCmd) {
  LessIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessLongCmd) {
  LessLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessDoubleCmd) {
  LessDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualCharCmd) {
  LessOrEqualCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessOrEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualIntCmd) {
  LessOrEqualIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessOrEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualLongCmd) {
  LessOrEqualLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessOrEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualDoubleCmd) {
  LessOrEqualDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLessOrEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LogicalNegateBoolCmd) {
  LogicalNegateBoolCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kLogicalNegateBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecCharCmd) {
  PreDecCharCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kPreDecChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecIntCmd) {
  PreDecIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kPreDecInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecLongCmd) {
  PreDecLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kPreDecLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecDoubleCmd) {
  PreDecDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kPreDecDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ArithmeticNegateIntCmd) {
  ArithmeticNegateIntCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kArithmeticNegateInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ArithmeticNegateLongCmd) {
  ArithmeticNegateLongCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kArithmeticNegateLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ArithmeticNegateDoubleCmd) {
  ArithmeticNegateDoubleCmd expected_cmd;
  Code code;
  code.Write<CmdId>(CmdId::kArithmeticNegateDouble);
  TestGetNextCmd(code, expected_cmd);
}
}
}
