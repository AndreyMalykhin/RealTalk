
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
#include "real_talk/code/jump_cmd.h"
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

TEST_F(CmdReaderTest, JumpIfNotCmd) {
  uint32_t address = UINT32_C(7);
  JumpIfNotCmd expected_cmd(address);
  Code code;
  code.WriteCmdId(CmdId::kJumpIfNot);
  code.WriteUint32(address);
  TestGetNextCmd(code, expected_cmd);
}
}
}
