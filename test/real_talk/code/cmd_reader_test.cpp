
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/code/cmd_reader.h"
#include "real_talk/code/end_cmd.h"
#include "real_talk/code/create_global_cmd.h"
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

TEST_F(CmdReaderTest, CreateGlobalIntCmd) {
  uint32_t var_index = UINT32_C(7);
  CreateGlobalIntCmd expected_cmd(var_index);
  Code code;
  code.WriteCmdId(CmdId::kCreateGlobalInt);
  code.WriteUint32(var_index);
  TestGetNextToken(code, expected_cmd);
}
}
}
