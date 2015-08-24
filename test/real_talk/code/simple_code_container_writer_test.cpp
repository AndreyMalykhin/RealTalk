
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/code/data_type_size.h"
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/simple_code_container_writer.h"

using std::vector;
using std::move;
using std::string;
using std::unique_ptr;
using std::ostringstream;
using testing::Test;

namespace real_talk {
namespace code {
namespace {

struct TestData {
  unique_ptr<CodeContainer> code_container;
  Code expected_code;
};
}

class SimpleCodeContainerWriterTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  TestData GetDataForModuleTest() {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kCreateGlobalIntVar);
    cmds_code->Write<CmdId>(CmdId::kCreateGlobalLongVar);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    cmds_code->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
    cmds_code->SetPosition(UINT32_C(0));
    vector<IdSize> global_var_defs =
        {{"var", DataTypeSize::kInt}, {"var2", DataTypeSize::kLong}};
    vector<IdAddress> func_defs =
        {{"func", UINT32_C(1)}, {"func2", UINT32_C(2)}};
    vector<string> native_func_defs = {"native_func", "native_func2"};
    vector<IdAddresses> global_var_refs =
        {{"var", {UINT32_C(3), UINT32_C(4)}},
         {"var2", {UINT32_C(5), UINT32_C(6)}}};
    vector<IdAddresses> func_refs =
        {{"func", {UINT32_C(7), UINT32_C(8)}},
         {"func2", {UINT32_C(9), UINT32_C(10)}}};
    vector<IdAddresses> native_func_refs =
        {{"func3", {UINT32_C(11), UINT32_C(12)}},
         {"func4", {UINT32_C(13), UINT32_C(14)}}};
    uint32_t version = UINT32_C(1);
    unique_ptr<Module> module(new Module(version,
                                         move(cmds_code),
                                         main_cmds_code_size,
                                         func_defs,
                                         global_var_defs,
                                         native_func_defs,
                                         func_refs,
                                         native_func_refs,
                                         global_var_refs));
    Code expected_code;
    expected_code.Write<uint32_t>(module->GetVersion());
    uint32_t segments_metadata_address = expected_code.GetPosition();
    expected_code.Skip(15 * sizeof(uint32_t));
    uint32_t cmds_address = expected_code.GetPosition();
    expected_code.Write(
        module->GetCmdsCode().GetData(), module->GetCmdsCode().GetSize());
    uint32_t global_var_defs_metadata_address = expected_code.GetPosition();
    expected_code.Write<IdSize>(IdSize("var", DataTypeSize::kInt));
    expected_code.Write<IdSize>(IdSize("var2", DataTypeSize::kLong));
    uint32_t global_var_defs_metadata_size =
        expected_code.GetPosition() - global_var_defs_metadata_address;
    uint32_t func_defs_metadata_address = expected_code.GetPosition();
    expected_code.Write<IdAddress>(IdAddress("func", UINT32_C(1)));
    expected_code.Write<IdAddress>(IdAddress("func2", UINT32_C(2)));
    uint32_t func_defs_metadata_size =
        expected_code.GetPosition() - func_defs_metadata_address;
    uint32_t native_func_defs_metadata_address = expected_code.GetPosition();
    expected_code.Write<string>("native_func");
    expected_code.Write<string>("native_func2");
    uint32_t native_func_defs_metadata_size =
        expected_code.GetPosition() - native_func_defs_metadata_address;
    uint32_t global_var_refs_metadata_address = expected_code.GetPosition();
    expected_code.Write<IdAddresses>(
        IdAddresses("var", vector<uint32_t>({UINT32_C(3), UINT32_C(4)})));
    expected_code.Write<IdAddresses>(
        IdAddresses("var2", vector<uint32_t>({UINT32_C(5), UINT32_C(6)})));
    uint32_t global_var_refs_metadata_size =
        expected_code.GetPosition() - global_var_refs_metadata_address;
    uint32_t func_refs_metadata_address = expected_code.GetPosition();
    expected_code.Write<IdAddresses>(
        IdAddresses("func", vector<uint32_t>({UINT32_C(7), UINT32_C(8)})));
    expected_code.Write<IdAddresses>(
        IdAddresses("func2", vector<uint32_t>({UINT32_C(9), UINT32_C(10)})));
    uint32_t func_refs_metadata_size =
        expected_code.GetPosition() - func_refs_metadata_address;
    uint32_t native_func_refs_metadata_address = expected_code.GetPosition();
    expected_code.Write<IdAddresses>(
        IdAddresses("func3", vector<uint32_t>({UINT32_C(11), UINT32_C(12)})));
    expected_code.Write<IdAddresses>(
        IdAddresses("func4", vector<uint32_t>({UINT32_C(13), UINT32_C(14)})));
    uint32_t native_func_refs_metadata_size =
        expected_code.GetPosition() - native_func_refs_metadata_address;
    expected_code.SetPosition(segments_metadata_address);
    expected_code.Write<uint32_t>(cmds_address);
    expected_code.Write<uint32_t>(module->GetMainCmdsCodeSize());
    expected_code.Write<uint32_t>(module->GetFuncCmdsCodeSize());
    expected_code.Write<uint32_t>(global_var_defs_metadata_address);
    expected_code.Write<uint32_t>(global_var_defs_metadata_size);
    expected_code.Write<uint32_t>(func_defs_metadata_address);
    expected_code.Write<uint32_t>(func_defs_metadata_size);
    expected_code.Write<uint32_t>(native_func_defs_metadata_address);
    expected_code.Write<uint32_t>(native_func_defs_metadata_size);
    expected_code.Write<uint32_t>(global_var_refs_metadata_address);
    expected_code.Write<uint32_t>(global_var_refs_metadata_size);
    expected_code.Write<uint32_t>(func_refs_metadata_address);
    expected_code.Write<uint32_t>(func_refs_metadata_size);
    expected_code.Write<uint32_t>(native_func_refs_metadata_address);
    expected_code.Write<uint32_t>(native_func_refs_metadata_size);
    return TestData{move(module), move(expected_code)};
  }

  TestData GetDataForExeTest() {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->Write<CmdId>(CmdId::kCreateGlobalIntVar);
    cmds_code->Write<CmdId>(CmdId::kCreateGlobalLongVar);
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    cmds_code->Write<CmdId>(CmdId::kCreateLocalDoubleVar);
    cmds_code->SetPosition(UINT32_C(0));
    vector<string> native_func_defs = {"native_func", "native_func2"};
    vector<IdAddresses> native_func_refs =
        {{"func3", {UINT32_C(1), UINT32_C(2)}},
         {"func4", {UINT32_C(3), UINT32_C(4)}}};
    uint32_t version = UINT32_C(1);
    uint32_t global_vars_size = UINT32_C(7);
    unique_ptr<CodeContainer> exe(new Exe(version,
                                          move(cmds_code),
                                          main_cmds_code_size,
                                          global_vars_size,
                                          native_func_defs,
                                          native_func_refs));
    Code expected_code;
    expected_code.Write<uint32_t>(exe->GetVersion());
    expected_code.Write<uint32_t>(global_vars_size);
    uint32_t segments_metadata_address = expected_code.GetPosition();
    expected_code.Skip(7 * sizeof(uint32_t));
    uint32_t cmds_address = expected_code.GetPosition();
    expected_code.Write(
        exe->GetCmdsCode().GetData(), exe->GetCmdsCode().GetSize());
    uint32_t native_func_defs_metadata_address = expected_code.GetPosition();
    expected_code.Write<string>("native_func");
    expected_code.Write<string>("native_func2");
    uint32_t native_func_defs_metadata_size =
        expected_code.GetPosition() - native_func_defs_metadata_address;
    uint32_t native_func_refs_metadata_address = expected_code.GetPosition();
    expected_code.Write<IdAddresses>(
        IdAddresses("func3", vector<uint32_t>({UINT32_C(1), UINT32_C(2)})));
    expected_code.Write<IdAddresses>(
        IdAddresses("func4", vector<uint32_t>({UINT32_C(3), UINT32_C(4)})));
    uint32_t native_func_refs_metadata_size =
        expected_code.GetPosition() - native_func_refs_metadata_address;
    expected_code.SetPosition(segments_metadata_address);
    expected_code.Write<uint32_t>(cmds_address);
    expected_code.Write<uint32_t>(exe->GetMainCmdsCodeSize());
    expected_code.Write<uint32_t>(exe->GetFuncCmdsCodeSize());
    expected_code.Write<uint32_t>(native_func_defs_metadata_address);
    expected_code.Write<uint32_t>(native_func_defs_metadata_size);
    expected_code.Write<uint32_t>(native_func_refs_metadata_address);
    expected_code.Write<uint32_t>(native_func_refs_metadata_size);
    return TestData{move(exe), move(expected_code)};
  }
};

TEST_F(SimpleCodeContainerWriterTest, CodeContainer) {
  vector<TestData> test_data_suits;
  test_data_suits.push_back(GetDataForModuleTest());
  test_data_suits.push_back(GetDataForExeTest());

  for (const TestData &test_data: test_data_suits) {
    Code actual_code;
    SimpleCodeContainerWriter().Write(
        static_cast<const CodeContainer&>(*(test_data.code_container)),
        &actual_code);
    ASSERT_EQ(test_data.expected_code, actual_code);
  }
}

TEST_F(SimpleCodeContainerWriterTest, Module) {
  TestData test_data = GetDataForModuleTest();
  Code actual_code;
  SimpleCodeContainerWriter().Write(
      static_cast<const Module&>(*(test_data.code_container)), &actual_code);
  ASSERT_EQ(test_data.expected_code, actual_code);
}

TEST_F(SimpleCodeContainerWriterTest, Exe) {
  TestData test_data = GetDataForExeTest();
  Code actual_code;
  SimpleCodeContainerWriter().Write(
      static_cast<const Exe&>(*(test_data.code_container)), &actual_code);
  ASSERT_EQ(test_data.expected_code, actual_code);
}
}
}
