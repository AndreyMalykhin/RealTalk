
#include <limits>
#include <vector>
#include <string>
#include "real_talk/linker/lib_linker.h"
#include "real_talk/linker/linker_test.h"

using std::ostringstream;
using std::unique_ptr;
using std::vector;
using std::string;
using std::numeric_limits;
using testing::Test;
using real_talk::code::Code;
using real_talk::code::CodeContainer;
using real_talk::code::CmdId;
using real_talk::code::DataTypeSize;
using real_talk::code::IdSize;
using real_talk::code::IdAddress;
using real_talk::code::IdAddresses;
using real_talk::code::Module;

namespace real_talk {
namespace linker {

class LibLinkerTest: public LinkerTest {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  virtual unique_ptr<Linker> CreateLinker() const override {
    return unique_ptr<Linker>(new LibLinker());
  }
};

TEST_F(LibLinkerTest, Link) {
  Linker::Modules modules;

  {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kCreateGlobalDoubleVar);
    vector<uint32_t> var_index_placeholders;
    var_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
    vector<uint32_t> var_index_placeholders2;
    var_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    vector<uint32_t> func_address_placeholders2;
    func_address_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    vector<uint32_t> native_func_index_placeholders2;
    native_func_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    uint32_t func_def_address = cmds_code->GetPosition();
    cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
    var_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    func_address_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    native_func_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kReturn);
    vector<IdSize> global_var_defs = {{"var", DataTypeSize::kDouble}};
    vector<IdAddress> func_defs = {{"func", func_def_address}};
    vector<string> native_func_defs = {"native_func"};
    vector<IdAddresses> global_var_refs =
        {{"var", var_index_placeholders}, {"var2", var_index_placeholders2}};
    vector<IdAddresses> func_refs =
        {{"func2", func_address_placeholders2}};
    vector<IdAddresses> native_func_refs =
        {{"native_func2", native_func_index_placeholders2}};
    uint32_t module_version = UINT32_C(1);
    unique_ptr<Module> module(new Module(module_version,
                                         move(cmds_code),
                                         main_cmds_code_size,
                                         func_defs,
                                         global_var_defs,
                                         native_func_defs,
                                         func_refs,
                                         native_func_refs,
                                         global_var_refs));
    modules.push_back(move(module));
  }

  {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
    vector<uint32_t> var_index_placeholders2;
    var_index_placeholders2.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadGlobalDoubleVarValue);
    vector<uint32_t> var_index_placeholders;
    var_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    vector<uint32_t> func_address_placeholders;
    func_address_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    vector<uint32_t> native_func_index_placeholders;
    native_func_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    uint32_t main_cmds_code_size = cmds_code->GetPosition();
    uint32_t func_def_address2 = cmds_code->GetPosition();
    cmds_code->WriteCmdId(CmdId::kLoadGlobalDoubleVarValue);
    var_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
    func_address_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
    native_func_index_placeholders.push_back(cmds_code->GetPosition());
    cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
    cmds_code->WriteCmdId(CmdId::kReturn);
    vector<IdSize> global_var_defs = {{"var2", DataTypeSize::kLong}};
    vector<IdAddress> func_defs =
        {{"func2", func_def_address2}};
    vector<string> native_func_defs = {"native_func2"};
    vector<IdAddresses> global_var_refs =
        {{"var2", var_index_placeholders2}, {"var", var_index_placeholders}};
    vector<IdAddresses> func_refs =
        {{"func", func_address_placeholders}};
    vector<IdAddresses> native_func_refs =
        {{"native_func", native_func_index_placeholders}};
    uint32_t module_version = UINT32_C(1);
    unique_ptr<Module> module(new Module(module_version,
                                         move(cmds_code),
                                         main_cmds_code_size,
                                         func_defs,
                                         global_var_defs,
                                         native_func_defs,
                                         func_refs,
                                         native_func_refs,
                                         global_var_refs));
    modules.push_back(move(module));
  }

  unique_ptr<Code> cmds_code(new Code());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalDoubleVar);
  vector<uint32_t> var_index_placeholders;
  var_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
  vector<uint32_t> var_index_placeholders2;
  var_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  vector<uint32_t> func_address_placeholders2;
  func_address_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  vector<uint32_t> native_func_index_placeholders2;
  native_func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
  var_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadGlobalDoubleVarValue);
  var_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  vector<uint32_t> func_address_placeholders;
  func_address_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  vector<uint32_t> native_func_index_placeholders;
  native_func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  uint32_t func_def_address = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
  var_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  func_address_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  native_func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kReturn);
  uint32_t func_def_address2 = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kLoadGlobalDoubleVarValue);
  var_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  func_address_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  native_func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kReturn);
  cmds_code->SetPosition(UINT32_C(0));
  vector<IdSize> global_var_defs =
      {{"var", DataTypeSize::kDouble}, {"var2", DataTypeSize::kLong}};
  vector<IdAddress> func_defs =
      {{"func", func_def_address}, {"func2", func_def_address2}};
  vector<string> native_func_defs = {"native_func", "native_func2"};
  vector<IdAddresses> global_var_refs =
      {{"var", var_index_placeholders}, {"var2", var_index_placeholders2}};
  vector<IdAddresses> func_refs =
      {{"func2", func_address_placeholders2},
       {"func", func_address_placeholders}};
  vector<IdAddresses> native_func_refs =
      {{"native_func2", native_func_index_placeholders2},
       {"native_func", native_func_index_placeholders}};
  uint32_t module_version = UINT32_C(1);
  Module expected_module(module_version,
                         move(cmds_code),
                         main_cmds_code_size,
                         func_defs,
                         global_var_defs,
                         native_func_defs,
                         func_refs,
                         native_func_refs,
                         global_var_refs);
  unique_ptr<CodeContainer> actual_module =
      LibLinker().Link(modules, expected_module.GetVersion());
  ASSERT_TRUE(actual_module.get());
  ASSERT_EQ(expected_module, *actual_module)
      << "[expected]\n" << PrintCodeContainer(&expected_module)
      << "\n[actual]\n" << PrintCodeContainer(actual_module.get());
}

TEST_F(LibLinkerTest, GlobalVarDuplicateDefError) {
  TestGlobalVarDuplicateDefError();
}

TEST_F(LibLinkerTest, FuncDuplicateDefError) {
  TestFuncDuplicateDefError();
}

TEST_F(LibLinkerTest, NativeFuncDuplicateDefError) {
  TestNativeFuncDuplicateDefError();
}
}
}
