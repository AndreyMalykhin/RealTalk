
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <limits>
#include "real_talk/code/exe.h"
#include "real_talk/vm/simple_native_func_linker.h"

using std::unique_ptr;
using std::string;
using std::vector;
using std::numeric_limits;
using testing::Test;
using real_talk::code::Code;
using real_talk::code::Exe;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace vm {

class SimpleNativeFuncLinkerTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleNativeFuncLinkerTest, Link) {
  unique_ptr<Code> actual_cmds_code(new Code());
  auto unlinked_func_index = numeric_limits<uint32_t>::max();
  actual_cmds_code->Write(unlinked_func_index);
  actual_cmds_code->Write(unlinked_func_index);
  actual_cmds_code->Write(unlinked_func_index);
  actual_cmds_code->Write(unlinked_func_index);
  uint32_t main_cmds_code_size = actual_cmds_code->GetPosition();
  vector<string> native_func_defs = {"func3", "func2", "func"};
  vector<IdAddresses> native_func_refs = {
    {"func", {UINT32_C(0), UINT32_C(4)}},
    {"func2", {UINT32_C(8), UINT32_C(12)}}};
  NativeFuncValue func = [](VM*) {};
  NativeFuncValue func2 = [](VM*) {};
  NativeFuncValue func3 = [](VM*) {};
  NativeFuncStorage::NativeFuncsMap available_funcs = {
    {"func", func}, {"func3", func3}, {"func2", func2}};
  vector<NativeFuncValue> actual_used_funcs;
  auto exe_version = UINT32_C(1);
  auto global_vars_size = UINT32_C(0);
  Exe exe(exe_version,
          move(actual_cmds_code),
          main_cmds_code_size,
          global_vars_size,
          native_func_defs,
          native_func_refs);

  vector<NativeFuncValue> expected_used_funcs = {func, func2};
  Code expected_cmds_code;
  auto linked_func_index = UINT32_C(0);
  expected_cmds_code.Write(linked_func_index);
  expected_cmds_code.Write(linked_func_index);
  auto linked_func_index2 = UINT32_C(1);
  expected_cmds_code.Write(linked_func_index2);
  expected_cmds_code.Write(linked_func_index2);

  SimpleNativeFuncLinker().Link(available_funcs, &actual_used_funcs, &exe);
  ASSERT_EQ(expected_used_funcs, actual_used_funcs);
  ASSERT_EQ(expected_cmds_code, exe.GetCmdsCode());
}
}
}
