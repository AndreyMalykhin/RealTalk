
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
#include "real_talk/code/store_cmd.h"
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
    code.WriteUint32(expected_cmd.GetAddress());
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
  uint32_t var_index = UINT32_C(1);
  CreateGlobalIntVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalIntVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalArrayVarCmd) {
  uint32_t var_index = UINT32_C(2);
  CreateGlobalArrayVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalLongVarCmd) {
  uint32_t var_index = UINT32_C(3);
  CreateGlobalLongVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalLongVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalDoubleVarCmd) {
  uint32_t var_index = UINT32_C(4);
  CreateGlobalDoubleVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalDoubleVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalCharVarCmd) {
  uint32_t var_index = UINT32_C(5);
  CreateGlobalCharVarCmd expected_cmd(var_index);
  TestCreateGlobalVarCmd(CmdId::kCreateGlobalCharVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalStringVarCmd) {
  uint32_t var_index = UINT32_C(6);
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
  uint32_t var_index = UINT32_C(1);
  CreateAndInitGlobalIntVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalIntVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalArrayVarCmd) {
  uint32_t var_index = UINT32_C(2);
  CreateAndInitGlobalArrayVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalArrayVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalLongVarCmd) {
  uint32_t var_index = UINT32_C(3);
  CreateAndInitGlobalLongVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalLongVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalDoubleVarCmd) {
  uint32_t var_index = UINT32_C(4);
  CreateAndInitGlobalDoubleVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalDoubleVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalCharVarCmd) {
  uint32_t var_index = UINT32_C(5);
  CreateAndInitGlobalCharVarCmd expected_cmd(var_index);
  TestCreateAndInitGlobalVarCmd(
      CmdId::kCreateAndInitGlobalCharVar, expected_cmd);
}

TEST_F(CmdReaderTest, CreateAndInitGlobalStringVarCmd) {
  uint32_t var_index = UINT32_C(6);
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
  uint32_t address = UINT32_C(7);
  JumpIfNotCmd expected_cmd(address);
  TestJumpCmd(CmdId::kJumpIfNot, expected_cmd);
}

TEST_F(CmdReaderTest, DirectJumpCmd) {
  uint32_t address = UINT32_C(2);
  DirectJumpCmd expected_cmd(address);
  TestJumpCmd(CmdId::kDirectJump, expected_cmd);
}

TEST_F(CmdReaderTest, DestroyLocalVarsAndJumpCmd) {
  uint32_t vars_count = UINT32_C(1);
  uint32_t address = UINT32_C(2);
  DestroyLocalVarsAndJumpCmd expected_cmd(vars_count, address);
  Code code;
  code.WriteCmdId(CmdId::kDestroyLocalVarsAndJump);
  code.WriteUint32(vars_count);
  code.WriteUint32(address);
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
}
}
