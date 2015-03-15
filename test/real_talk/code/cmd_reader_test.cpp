
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/create_global_var_cmd.h"
#include "real_talk/code/code.h"

using std::stringstream;
using testing::Test;

namespace real_talk {
namespace code {

class CmdReaderTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }

  void TestGetNextToken(Code &code, const Cmd &expected_cmd) {
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
    TestGetNextToken(code, expected_cmd);
  }
};

TEST_F(CmdReaderTest, EndMainCmd) {
  EndMainCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEndMain);
  TestGetNextToken(code, expected_cmd);
}

TEST_F(CmdReaderTest, EndFuncsCmd) {
  EndFuncsCmd expected_cmd;
  Code code;
  code.WriteCmdId(CmdId::kEndFuncs);
  TestGetNextToken(code, expected_cmd);
}

TEST_F(CmdReaderTest, CreateGlobalIntVarCmd) {
  uint32_t var_index = UINT32_C(1);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalIntVar, CreateGlobalIntVarCmd(var_index));
}

TEST_F(CmdReaderTest, CreateGlobalArrayVarCmd) {
  uint32_t var_index = UINT32_C(2);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalArrayVar, CreateGlobalArrayVarCmd(var_index));
}

TEST_F(CmdReaderTest, CreateGlobalLongVarCmd) {
  uint32_t var_index = UINT32_C(3);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalLongVar, CreateGlobalLongVarCmd(var_index));
}

TEST_F(CmdReaderTest, CreateGlobalDoubleVarCmd) {
  uint32_t var_index = UINT32_C(4);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalDoubleVar, CreateGlobalDoubleVarCmd(var_index));
}

TEST_F(CmdReaderTest, CreateGlobalCharVarCmd) {
  uint32_t var_index = UINT32_C(5);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalCharVar, CreateGlobalCharVarCmd(var_index));
}

TEST_F(CmdReaderTest, CreateGlobalStringVarCmd) {
  uint32_t var_index = UINT32_C(6);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalStringVar, CreateGlobalStringVarCmd(var_index));
}

TEST_F(CmdReaderTest, CreateGlobalBoolVarCmd) {
  uint32_t var_index = UINT32_C(7);
  TestCreateGlobalVarCmd(
      CmdId::kCreateGlobalBoolVar, CreateGlobalBoolVarCmd(var_index));
}
}
}
