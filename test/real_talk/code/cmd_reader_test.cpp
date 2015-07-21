
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
    code.WriteCmdId(cmd_id);
    code.WriteUint32(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitGlobalVarCmd(
      CmdId cmd_id, const CreateAndInitGlobalVarCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint32(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitGlobalArrayVarCmd(
      CmdId cmd_id, const CreateAndInitGlobalArrayVarCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint32(expected_cmd.GetVarIndex());
    code.WriteUint8(expected_cmd.GetDimensionsCount());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateArrayCmd(CmdId cmd_id, const CreateArrayCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint8(expected_cmd.GetDimensionsCount());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitArrayCmd(
      CmdId cmd_id, const CreateAndInitArrayCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint8(expected_cmd.GetDimensionsCount());
    code.WriteInt32(expected_cmd.GetValuesCount());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestJumpCmd(CmdId cmd_id, const JumpCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteInt32(expected_cmd.GetOffset());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestLoadGlobalVarValueCmd(
      CmdId cmd_id, const LoadGlobalVarValueCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint32(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestLoadLocalVarValueCmd(
      CmdId cmd_id, const LoadLocalVarValueCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint32(expected_cmd.GetVarIndex());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestLoadArrayElementValueCmd(
      CmdId cmd_id, const LoadArrayElementValueCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint8(expected_cmd.GetDimensionsCount());
    TestGetNextCmd(code, expected_cmd);
  }

  void TestLoadArrayElementAddressCmd(
      CmdId cmd_id, const LoadArrayElementAddressCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
    code.WriteUint8(expected_cmd.GetDimensionsCount());
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
  code.WriteCmdId(CmdId::kCreateLocalIntVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalLongVarCmd) {
  CreateLocalLongVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateLocalLongVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalDoubleVarCmd) {
  CreateLocalDoubleVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateLocalDoubleVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalBoolVarCmd) {
  CreateLocalBoolVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateLocalBoolVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalCharVarCmd) {
  CreateLocalCharVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateLocalCharVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalStringVarCmd) {
  CreateLocalStringVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateLocalStringVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLocalArrayVarCmd) {
  CreateLocalArrayVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateLocalArrayVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalIntVarCmd) {
  DestroyLocalIntVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalIntVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalLongVarCmd) {
  DestroyLocalLongVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalLongVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalDoubleVarCmd) {
  DestroyLocalDoubleVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalDoubleVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalBoolVarCmd) {
  DestroyLocalBoolVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalBoolVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalCharVarCmd) {
  DestroyLocalCharVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalCharVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalStringVarCmd) {
  DestroyLocalStringVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalStringVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalArrayVarCmd) {
  DestroyLocalArrayVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalArrayVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadBoolCmd) {
  UnloadBoolCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadCharCmd) {
  UnloadCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadIntCmd) {
  UnloadIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadLongCmd) {
  UnloadLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadDoubleCmd) {
  UnloadDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadStringCmd) {
  UnloadStringCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, UnloadArrayCmd) {
  UnloadArrayCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnloadArray);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadIntValueCmd) {
  int32_t value = INT32_C(-7);
  LoadIntValueCmd expected_cmd(value);
  Code code;
  code.WriteCmdId(CmdId::kLoadIntValue);
  code.WriteInt32(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLongValueCmd) {
  int64_t value = INT64_C(-77);
  LoadLongValueCmd expected_cmd(value);
  Code code;
  code.WriteCmdId(CmdId::kLoadLongValue);
  code.WriteInt64(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadBoolValueCmd) {
  bool value = true;
  LoadBoolValueCmd expected_cmd(value);
  Code code;
  code.WriteCmdId(CmdId::kLoadBoolValue);
  code.WriteBool(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadCharValueCmd) {
  char value = 'a';
  LoadCharValueCmd expected_cmd(value);
  Code code;
  code.WriteCmdId(CmdId::kLoadCharValue);
  code.WriteChar(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadStringValueCmd) {
  string value("swag");
  LoadStringValueCmd expected_cmd(value);
  Code code;
  code.WriteCmdId(CmdId::kLoadStringValue);
  code.WriteString(value);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadDoubleValueCmd) {
  double value = -7.77777777777777777777;
  LoadDoubleValueCmd expected_cmd(value);
  Code code;
  code.WriteCmdId(CmdId::kLoadDoubleValue);
  code.WriteDouble(value);
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
  code.WriteCmdId(CmdId::kCreateAndInitLocalIntVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalLongVarCmd) {
  CreateAndInitLocalLongVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateAndInitLocalLongVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalDoubleVarCmd) {
  CreateAndInitLocalDoubleVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateAndInitLocalDoubleVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalBoolVarCmd) {
  CreateAndInitLocalBoolVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateAndInitLocalBoolVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalCharVarCmd) {
  CreateAndInitLocalCharVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateAndInitLocalCharVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLocalStringVarCmd) {
  CreateAndInitLocalStringVarCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCreateAndInitLocalStringVar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateIntArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateIntArrayCmd expected_cmd(dimensions_count);
  TestCreateArrayCmd(CmdId::kCreateIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateLongArrayCmd expected_cmd(dimensions_count);
  TestCreateArrayCmd(CmdId::kCreateLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateDoubleArrayCmd expected_cmd(dimensions_count);
  TestCreateArrayCmd(CmdId::kCreateDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateBoolArrayCmd expected_cmd(dimensions_count);
  TestCreateArrayCmd(CmdId::kCreateBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateCharArrayCmd expected_cmd(dimensions_count);
  TestCreateArrayCmd(CmdId::kCreateCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  CreateStringArrayCmd expected_cmd(dimensions_count);
  TestCreateArrayCmd(CmdId::kCreateStringArray, expected_cmd);
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
  code.WriteCmdId(CmdId::kReturn);
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

TEST_F(CmdReaderTest, LoadGlobalArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalArrayVarValueCmd expected_cmd(var_index);
  TestLoadGlobalVarValueCmd(CmdId::kLoadGlobalArrayVarValue, expected_cmd);
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

TEST_F(CmdReaderTest, LoadLocalArrayVarValueCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalArrayVarValueCmd expected_cmd(var_index);
  TestLoadLocalVarValueCmd(CmdId::kLoadLocalArrayVarValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadGlobalVarAddressCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadGlobalVarAddressCmd expected_cmd(var_index);
  Code code;
  code.WriteCmdId(CmdId::kLoadGlobalVarAddress);
  code.WriteUint32(var_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadLocalVarAddressCmd) {
  uint32_t var_index = UINT32_C(7);
  LoadLocalVarAddressCmd expected_cmd(var_index);
  Code code;
  code.WriteCmdId(CmdId::kLoadLocalVarAddress);
  code.WriteUint32(var_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreIntCmd) {
  StoreIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreLongCmd) {
  StoreLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreDoubleCmd) {
  StoreDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreBoolCmd) {
  StoreBoolCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreCharCmd) {
  StoreCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreStringCmd) {
  StoreStringCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, StoreArrayCmd) {
  StoreArrayCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kStoreArray);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToIntCmd) {
  CastCharToIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastCharToInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToLongCmd) {
  CastCharToLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastCharToLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToDoubleCmd) {
  CastCharToDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastCharToDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastCharToStringCmd) {
  CastCharToStringCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastCharToString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastIntToLongCmd) {
  CastIntToLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastIntToLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastIntToDoubleCmd) {
  CastIntToDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastIntToDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CastLongToDoubleCmd) {
  CastLongToDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCastLongToDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadFuncValueCmd) {
  uint32_t address = UINT32_C(7);
  LoadFuncValueCmd expected_cmd(address);
  Code code;
  code.WriteCmdId(CmdId::kLoadFuncValue);
  code.WriteUint32(address);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadNativeFuncValueCmd) {
  uint32_t func_index = UINT32_C(7);
  LoadNativeFuncValueCmd expected_cmd(func_index);
  Code code;
  code.WriteCmdId(CmdId::kLoadNativeFuncValue);
  code.WriteUint32(func_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CallCmd) {
  CallCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCall);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CallNativeCmd) {
  CallNativeCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kCallNative);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfIntsElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfIntsElementValueCmd expected_cmd(dimensions_count);
  TestLoadArrayElementValueCmd(
      CmdId::kLoadArrayOfIntsElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfLongsElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfLongsElementValueCmd expected_cmd(dimensions_count);
  TestLoadArrayElementValueCmd(
      CmdId::kLoadArrayOfLongsElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfDoublesElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfDoublesElementValueCmd expected_cmd(dimensions_count);
  TestLoadArrayElementValueCmd(
      CmdId::kLoadArrayOfDoublesElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfCharsElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfCharsElementValueCmd expected_cmd(dimensions_count);
  TestLoadArrayElementValueCmd(
      CmdId::kLoadArrayOfCharsElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfBoolsElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfBoolsElementValueCmd expected_cmd(dimensions_count);
  TestLoadArrayElementValueCmd(
      CmdId::kLoadArrayOfBoolsElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfStringsElementValueCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfStringsElementValueCmd expected_cmd(dimensions_count);
  TestLoadArrayElementValueCmd(
      CmdId::kLoadArrayOfStringsElementValue, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfIntsElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfIntsElementAddressCmd expected_cmd(dimensions_count);
  TestLoadArrayElementAddressCmd(
      CmdId::kLoadArrayOfIntsElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfLongsElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfLongsElementAddressCmd expected_cmd(dimensions_count);
  TestLoadArrayElementAddressCmd(
      CmdId::kLoadArrayOfLongsElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfDoublesElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfDoublesElementAddressCmd expected_cmd(dimensions_count);
  TestLoadArrayElementAddressCmd(
      CmdId::kLoadArrayOfDoublesElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfCharsElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfCharsElementAddressCmd expected_cmd(dimensions_count);
  TestLoadArrayElementAddressCmd(
      CmdId::kLoadArrayOfCharsElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfBoolsElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfBoolsElementAddressCmd expected_cmd(dimensions_count);
  TestLoadArrayElementAddressCmd(
      CmdId::kLoadArrayOfBoolsElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfStringsElementAddressCmd) {
  uint8_t dimensions_count = UINT8_C(7);
  LoadArrayOfStringsElementAddressCmd expected_cmd(dimensions_count);
  TestLoadArrayElementAddressCmd(
      CmdId::kLoadArrayOfStringsElementAddress, expected_cmd);
}

TEST_F(CmdReaderTest, AndCmd) {
  AndCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kAnd);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, OrCmd) {
  OrCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kOr);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulIntCmd) {
  MulIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kMulInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulLongCmd) {
  MulLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kMulLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulDoubleCmd) {
  MulDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kMulDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, MulCharCmd) {
  MulCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kMulChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivCharCmd) {
  DivCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDivChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivIntCmd) {
  DivIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDivInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivLongCmd) {
  DivLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDivLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DivDoubleCmd) {
  DivDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kDivDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumCharCmd) {
  SumCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSumChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumIntCmd) {
  SumIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSumInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumLongCmd) {
  SumLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSumLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumDoubleCmd) {
  SumDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSumDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SumStringCmd) {
  SumStringCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSumString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubCharCmd) {
  SubCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSubChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubIntCmd) {
  SubIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSubInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubLongCmd) {
  SubLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSubLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, SubDoubleCmd) {
  SubDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kSubDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualCharCmd) {
  EqualCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualIntCmd) {
  EqualIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualLongCmd) {
  EqualLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualDoubleCmd) {
  EqualDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualBoolCmd) {
  EqualBoolCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualStringCmd) {
  EqualStringCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EqualArrayCmd) {
  EqualArrayCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEqualArray);
  TestGetNextCmd(code, expected_cmd);
}
TEST_F(CmdReaderTest, NotEqualCharCmd) {
  NotEqualCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualIntCmd) {
  NotEqualIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualLongCmd) {
  NotEqualLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualDoubleCmd) {
  NotEqualDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualBoolCmd) {
  NotEqualBoolCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualStringCmd) {
  NotEqualStringCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualString);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, NotEqualArrayCmd) {
  NotEqualArrayCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kNotEqualArray);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterCharCmd) {
  GreaterCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterIntCmd) {
  GreaterIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterLongCmd) {
  GreaterLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterDoubleCmd) {
  GreaterDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualCharCmd) {
  GreaterOrEqualCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterOrEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualIntCmd) {
  GreaterOrEqualIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterOrEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualLongCmd) {
  GreaterOrEqualLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterOrEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, GreaterOrEqualDoubleCmd) {
  GreaterOrEqualDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kGreaterOrEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessCharCmd) {
  LessCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessIntCmd) {
  LessIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessLongCmd) {
  LessLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessDoubleCmd) {
  LessDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualCharCmd) {
  LessOrEqualCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessOrEqualChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualIntCmd) {
  LessOrEqualIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessOrEqualInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualLongCmd) {
  LessOrEqualLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessOrEqualLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LessOrEqualDoubleCmd) {
  LessOrEqualDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLessOrEqualDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LogicalNegateBoolCmd) {
  LogicalNegateBoolCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLogicalNegateBool);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecCharCmd) {
  PreDecCharCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kPreDecChar);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecIntCmd) {
  PreDecIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kPreDecInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecLongCmd) {
  PreDecLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kPreDecLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, PreDecDoubleCmd) {
  PreDecDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kPreDecDouble);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ArithmeticNegateIntCmd) {
  ArithmeticNegateIntCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kArithmeticNegateInt);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ArithmeticNegateLongCmd) {
  ArithmeticNegateLongCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kArithmeticNegateLong);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ArithmeticNegateDoubleCmd) {
  ArithmeticNegateDoubleCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kArithmeticNegateDouble);
  TestGetNextCmd(code, expected_cmd);
}
}
}
