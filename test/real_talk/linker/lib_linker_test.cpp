
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <limits>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/linker/lib_linker.h"

using std::ostringstream;
using std::unique_ptr;
using std::vector;
using std::string;
using std::numeric_limits;
using testing::Test;
using real_talk::code::Code;
using real_talk::code::CodeContainer;
using real_talk::code::CmdId;
using real_talk::code::IdAddress;
using real_talk::code::IdAddresses;
using real_talk::code::Module;

namespace real_talk {
namespace linker {

class LibLinkerTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  string PrintModule(CodeContainer *module) {
    module->GetCmdsCode().SetPosition(UINT32_C(0));
    ostringstream stream;
    stream << *module;
    return stream.str();
  }
};

TEST_F(LibLinkerTest, Link) {
  LibLinker::Modules modules;

  {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kCreateGlobalIntVar);
    vector<uint32_t> var_index_placeholders;
    var_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadGlobalIntVarValue);
    vector<uint32_t> var_index_placeholders2;
    var_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    vector<uint32_t> func_index_placeholders2;
    func_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    vector<uint32_t> native_func_index_placeholders2;
    native_func_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    uint32_t func_def_address = cmds_code->GetPosition();
    cmds_code->WriteCmdId(CmdId::kLoadGlobalIntVarValue);
    var_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    func_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    native_func_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kReturn);
    vector<string> ids_of_global_var_defs = {"var"};
    vector<IdAddress> id_addresses_of_func_defs = {{"func", func_def_address}};
    vector<string> ids_of_native_func_defs = {"native_func"};
    vector<IdAddresses> id_addresses_of_global_var_refs =
        {{"var", var_index_placeholders}, {"var2", var_index_placeholders2}};
    vector<IdAddresses> id_addresses_of_func_refs =
        {{"func2", func_index_placeholders2}};
    vector<IdAddresses> id_addresses_of_native_func_refs =
        {{"native_func2", native_func_index_placeholders2}};
    uint32_t module_version = UINT32_C(1);
    unique_ptr<Module> module(new Module(module_version,
                                         move(cmds_code),
                                         main_cmds_code_size,
                                         id_addresses_of_func_defs,
                                         ids_of_global_var_defs,
                                         ids_of_native_func_defs,
                                         id_addresses_of_func_refs,
                                         id_addresses_of_native_func_refs,
                                         id_addresses_of_global_var_refs));
    modules.push_back(move(module));
  }

  {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
    vector<uint32_t> var_index_placeholders2;
    var_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
    vector<uint32_t> var_index_placeholders;
    var_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    vector<uint32_t> func_index_placeholders;
    func_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    vector<uint32_t> native_func_index_placeholders;
    native_func_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    uint32_t func_def_address2 = cmds_code->GetPosition();
    cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
    var_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    func_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    native_func_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kReturn);
    vector<string> ids_of_global_var_defs = {"var2"};
    vector<IdAddress> id_addresses_of_func_defs =
        {{"func2", func_def_address2}};
    vector<string> ids_of_native_func_defs = {"native_func2"};
    vector<IdAddresses> id_addresses_of_global_var_refs =
        {{"var2", var_index_placeholders2}, {"var", var_index_placeholders}};
    vector<IdAddresses> id_addresses_of_func_refs =
        {{"func", func_index_placeholders}};
    vector<IdAddresses> id_addresses_of_native_func_refs =
        {{"native_func", native_func_index_placeholders}};
    uint32_t module_version = UINT32_C(1);
    unique_ptr<Module> module(new Module(module_version,
                                         move(cmds_code),
                                         main_cmds_code_size,
                                         id_addresses_of_func_defs,
                                         ids_of_global_var_defs,
                                         ids_of_native_func_defs,
                                         id_addresses_of_func_refs,
                                         id_addresses_of_native_func_refs,
                                         id_addresses_of_global_var_refs));
    modules.push_back(move(module));
  }

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalIntVar);
  vector<uint32_t> var_index_placeholders;
  var_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadGlobalIntVarValue);
  vector<uint32_t> var_index_placeholders2;
  var_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  vector<uint32_t> func_index_placeholders2;
  func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  vector<uint32_t> native_func_index_placeholders2;
  native_func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
  var_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
  var_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  vector<uint32_t> func_index_placeholders;
  func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  vector<uint32_t> native_func_index_placeholders;
  native_func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  uint32_t func_def_address = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kLoadGlobalIntVarValue);
  var_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  native_func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kReturn);
  uint32_t func_def_address2 = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
  var_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  native_func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kReturn);
  vector<string> ids_of_global_var_defs = {"var", "var2"};
  vector<IdAddress> id_addresses_of_func_defs =
      {{"func", func_def_address}, {"func2", func_def_address2}};
  vector<string> ids_of_native_func_defs = {"native_func", "native_func2"};
  vector<IdAddresses> id_addresses_of_global_var_refs =
      {{"var", var_index_placeholders}, {"var2", var_index_placeholders2}};
  vector<IdAddresses> id_addresses_of_func_refs =
      {{"func", func_index_placeholders}, {"func2", func_index_placeholders2}};
  vector<IdAddresses> id_addresses_of_native_func_refs =
      {{"native_func", native_func_index_placeholders},
       {"native_func2", native_func_index_placeholders2}};
  uint32_t module_version = UINT32_C(1);
  Module expected_module(module_version,
                         move(cmds_code),
                         main_cmds_code_size,
                         id_addresses_of_func_defs,
                         ids_of_global_var_defs,
                         ids_of_native_func_defs,
                         id_addresses_of_func_refs,
                         id_addresses_of_native_func_refs,
                         id_addresses_of_global_var_refs);
  unique_ptr<CodeContainer> actual_module =
      LibLinker().Link(modules, expected_module.GetVersion());
  ASSERT_TRUE(actual_module.get());
  ASSERT_EQ(expected_module, *actual_module)
      << "[expected]\n" << PrintModule(&expected_module)
      << "\n[actual]\n" << PrintModule(actual_module.get());
}
}
}
