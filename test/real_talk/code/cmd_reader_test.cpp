
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/create_local_var_cmd.h"
#include "real_talk/code/load_value_cmd.h"
#include "real_talk/code/unload_cmd.h"
#include "real_talk/code/create_and_init_global_var_cmd.h"
#include "real_talk/code/create_and_init_local_var_cmd.h"
#include "real_talk/code/create_array_cmd.h"
#include "real_talk/code/create_and_init_array_cmd.h"
#include "real_talk/code/jump_cmd.h"
#include "real_talk/code/destroy_local_vars_and_jump_cmd.h"
#include "real_talk/code/destroy_local_vars_cmd.h"
#include "real_talk/code/return_cmd.h"
#include "real_talk/code/load_global_var_value_cmd.h"
#include "real_talk/code/load_local_var_value_cmd.h"
#include "real_talk/code/load_global_var_address_cmd.h"
#include "real_talk/code/load_local_var_address_cmd.h"
#include "real_talk/code/load_func_address_cmd.h"
#include "real_talk/code/load_native_func_address_cmd.h"
#include "real_talk/code/load_element_value_cmd.h"
#include "real_talk/code/load_element_address_cmd.h"
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
#include "real_talk/code/code.h"

using std::stringstream;
using std::string;
using testing::Test;

namespace real_talk {
namespace code {

class CmdReaderTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

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
    TestGetNextCmd(code, expected_cmd);
  }

  void TestCreateAndInitGlobalVarCmd(
      CmdId cmd_id, const CreateAndInitGlobalVarCmd &expected_cmd) {
    Code code;
    code.WriteCmdId(cmd_id);
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
    code.WriteUint32(expected_cmd.GetValuesCount());
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
};

TEST_F(CmdReaderTest, EndMainCmd) {
  EndMainCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEndMain);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, EndFuncsCmd) {
  EndFuncsCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEndFuncs);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalIntVarCmd) {
  CreateGlobalIntVarCmd expected_cmd;
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalIntVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalArrayVarCmd) {
  CreateGlobalArrayVarCmd expected_cmd;
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalLongVarCmd) {
  CreateGlobalLongVarCmd expected_cmd;
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalLongVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalDoubleVarCmd) {
  CreateGlobalDoubleVarCmd expected_cmd;
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalDoubleVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalCharVarCmd) {
  CreateGlobalCharVarCmd expected_cmd;
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalCharVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalStringVarCmd) {
  CreateGlobalStringVarCmd expected_cmd;
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalStringVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalBoolVarCmd) {
  CreateGlobalBoolVarCmd expected_cmd;
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

TEST_F(CmdReaderTest, UnloadCmd) {
  UnloadCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kUnload);
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
  CreateAndInitGlobalIntVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalIntVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalArrayVarCmd) {
  CreateAndInitGlobalArrayVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalLongVarCmd) {
  CreateAndInitGlobalLongVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalLongVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalDoubleVarCmd) {
  CreateAndInitGlobalDoubleVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalDoubleVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalCharVarCmd) {
  CreateAndInitGlobalCharVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalCharVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalStringVarCmd) {
  CreateAndInitGlobalStringVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalStringVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalBoolVarCmd) {
  CreateAndInitGlobalBoolVarCmd expected_cmd;
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalBoolVar, expected_cmd);
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
  uint32_t values_count = UINT32_C(3);
  CreateAndInitIntArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitIntArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitLongArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  uint32_t values_count = UINT32_C(3);
  CreateAndInitLongArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitLongArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitDoubleArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  uint32_t values_count = UINT32_C(3);
  CreateAndInitDoubleArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitDoubleArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitBoolArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  uint32_t values_count = UINT32_C(3);
  CreateAndInitBoolArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitBoolArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitCharArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  uint32_t values_count = UINT32_C(3);
  CreateAndInitCharArrayCmd expected_cmd(dimensions_count, values_count);
  TestCreateAndInitArrayCmd(CmdId::kCreateAndInitCharArray, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitStringArrayCmd) {
  uint8_t dimensions_count = UINT8_C(2);
  uint32_t values_count = UINT32_C(3);
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

TEST_F(CmdReaderTest, DestroyLocalVarsAndJumpCmd) {
  uint32_t vars_count = UINT32_C(1);
  int32_t offset = INT32_C(2);
  DestroyLocalVarsAndJumpCmd expected_cmd(vars_count, offset);
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalVarsAndJump);
  code.WriteUint32(vars_count);
  code.WriteInt32(offset);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalVarsCmd) {
  uint32_t vars_count = UINT32_C(1);
  DestroyLocalVarsCmd expected_cmd(vars_count);
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalVars);
  code.WriteUint32(vars_count);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnCmd) {
  ReturnCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturn);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnIntValueCmd) {
  ReturnIntValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnIntValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnLongValueCmd) {
  ReturnLongValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnLongValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnDoubleValueCmd) {
  ReturnDoubleValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnDoubleValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnCharValueCmd) {
  ReturnCharValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnCharValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnStringValueCmd) {
  ReturnStringValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnStringValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnBoolValueCmd) {
  ReturnBoolValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnBoolValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, ReturnArrayValueCmd) {
  ReturnArrayValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kReturnArrayValue);
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

TEST_F(CmdReaderTest, LoadFuncAddressCmd) {
  uint32_t func_index = UINT32_C(7);
  LoadFuncAddressCmd expected_cmd(func_index);
  Code code;
  code.WriteCmdId(CmdId::kLoadFuncAddress);
  code.WriteUint32(func_index);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadNativeFuncAddressCmd) {
  uint32_t func_index = UINT32_C(7);
  LoadNativeFuncAddressCmd expected_cmd(func_index);
  Code code;
  code.WriteCmdId(CmdId::kLoadNativeFuncAddress);
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
  LoadArrayOfIntsElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfIntsElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfLongsElementValueCmd) {
  LoadArrayOfLongsElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfLongsElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfDoublesElementValueCmd) {
  LoadArrayOfDoublesElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfDoublesElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfCharsElementValueCmd) {
  LoadArrayOfCharsElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfCharsElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfBoolsElementValueCmd) {
  LoadArrayOfBoolsElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfBoolsElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfStringsElementValueCmd) {
  LoadArrayOfStringsElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfStringsElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfArraysElementValueCmd) {
  LoadArrayOfArraysElementValueCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfArraysElementValue);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfIntsElementAddressCmd) {
  LoadArrayOfIntsElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfIntsElementAddress);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfLongsElementAddressCmd) {
  LoadArrayOfLongsElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfLongsElementAddress);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfDoublesElementAddressCmd) {
  LoadArrayOfDoublesElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfDoublesElementAddress);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfCharsElementAddressCmd) {
  LoadArrayOfCharsElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfCharsElementAddress);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfBoolsElementAddressCmd) {
  LoadArrayOfBoolsElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfBoolsElementAddress);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfStringsElementAddressCmd) {
  LoadArrayOfStringsElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfStringsElementAddress);
  TestGetNextCmd(code, expected_cmd);
}

TEST_F(CmdReaderTest, LoadArrayOfArraysElementAddressCmd) {
  LoadArrayOfArraysElementAddressCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kLoadArrayOfArraysElementAddress);
  TestGetNextCmd(code, expected_cmd);
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
}
}
