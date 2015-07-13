
#include <limits>
#include <vector>
#include <string>
#include "real_talk/code/exe.h"
#include "real_talk/linker/linker_test.h"
#include "real_talk/linker/exe_linker.h"

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
using real_talk::code::Exe;
using real_talk::code::Module;

namespace real_talk {
namespace linker {

class ExeLinkerTest: public LinkerTest {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  virtual unique_ptr<Linker> CreateLinker() const override {
    return unique_ptr<Linker>(new ExeLinker());
  }

  void TestMissingDefError(
      const Linker::Modules &modules, const string &expected_id) {
    uint32_t exe_version = UINT32_C(1);

    try {
      CreateLinker()->Link(modules, exe_version);
      FAIL();
    } catch (const Linker::MissingDefError &error) {
      std::string actual_id = error.GetId();
      ASSERT_EQ(expected_id, actual_id);
    }
  }
};

TEST_F(ExeLinkerTest, Link) {
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
  uint32_t var_index = UINT32_C(0);
  cmds_code->WriteUint32(var_index);
  cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
  uint32_t var_index2 = UINT32_C(2);
  cmds_code->WriteUint32(var_index2);
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  vector<uint32_t> func_address_placeholders2;
  func_address_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  vector<uint32_t> native_func_index_placeholders2;
  native_func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
  cmds_code->WriteUint32(var_index2);
  cmds_code->WriteCmdId(CmdId::kLoadGlobalDoubleVarValue);
  cmds_code->WriteUint32(var_index);
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  vector<uint32_t> func_address_placeholders;
  func_address_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  vector<uint32_t> native_func_index_placeholders;
  native_func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  uint32_t main_cmds_code_size = cmds_code->GetPosition();
  uint32_t func_def_address = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kLoadGlobalLongVarValue);
  cmds_code->WriteUint32(var_index2);
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  func_address_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  native_func_index_placeholders2.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kReturn);
  uint32_t func_def_address2 = cmds_code->GetPosition();
  cmds_code->WriteCmdId(CmdId::kLoadGlobalDoubleVarValue);
  cmds_code->WriteUint32(var_index);
  cmds_code->WriteCmdId(CmdId::kLoadFuncValue);
  func_address_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->Skip(sizeof(uint32_t));
  cmds_code->WriteCmdId(CmdId::kLoadNativeFuncValue);
  native_func_index_placeholders.push_back(cmds_code->GetPosition());
  cmds_code->WriteUint32(numeric_limits<uint32_t>::max());
  cmds_code->WriteCmdId(CmdId::kReturn);
  cmds_code->SetPosition(func_address_placeholders.at(0));
  cmds_code->WriteUint32(func_def_address);
  cmds_code->SetPosition(func_address_placeholders.at(1));
  cmds_code->WriteUint32(func_def_address);
  cmds_code->SetPosition(func_address_placeholders2.at(0));
  cmds_code->WriteUint32(func_def_address2);
  cmds_code->SetPosition(func_address_placeholders2.at(1));
  cmds_code->WriteUint32(func_def_address2);
  cmds_code->SetPosition(UINT32_C(0));
  vector<string> native_func_defs = {"native_func", "native_func2"};
  vector<IdAddresses> native_func_refs =
      {{"native_func2", native_func_index_placeholders2},
       {"native_func", native_func_index_placeholders}};
  uint32_t exe_version = UINT32_C(1);
  uint32_t global_vars_size = UINT32_C(4);
  Exe expected_exe(exe_version,
                   move(cmds_code),
                   main_cmds_code_size,
                   global_vars_size,
                   native_func_defs,
                   native_func_refs);
  unique_ptr<CodeContainer> actual_exe =
      ExeLinker().Link(modules, expected_exe.GetVersion());
  ASSERT_TRUE(actual_exe.get());
  ASSERT_EQ(expected_exe, *actual_exe)
      << "[expected]\n" << PrintCodeContainer(&expected_exe)
      << "\n[actual]\n" << PrintCodeContainer(actual_exe.get());
}

TEST_F(ExeLinkerTest, GlobalVarMissingDefError) {
  Linker::Modules modules;
  std::string expected_id = "var";

  {
    std::unique_ptr<real_talk::code::Code> cmds_code(
        new real_talk::code::Code());
    std::vector<real_talk::code::IdSize> global_var_defs;
    std::vector<real_talk::code::IdAddress> func_defs;
    std::vector<std::string> native_func_defs;
    std::vector<real_talk::code::IdAddresses> global_var_refs =
        {{expected_id, {UINT32_C(0)}}};
    std::vector<real_talk::code::IdAddresses> func_refs;
    std::vector<real_talk::code::IdAddresses> native_func_refs;
    uint32_t module_version = UINT32_C(1);
    uint32_t main_cmds_code_size = UINT32_C(0);
    std::unique_ptr<real_talk::code::Module> module(
        new real_talk::code::Module(module_version,
                                    std::move(cmds_code),
                                    main_cmds_code_size,
                                    func_defs,
                                    global_var_defs,
                                    native_func_defs,
                                    func_refs,
                                    native_func_refs,
                                    global_var_refs));
    modules.push_back(std::move(module));
  }

  {
    std::unique_ptr<real_talk::code::Code> cmds_code(
        new real_talk::code::Code());
    std::vector<real_talk::code::IdSize> global_var_defs;
    std::vector<real_talk::code::IdAddress> func_defs;
    std::vector<std::string> native_func_defs;
    std::vector<real_talk::code::IdAddresses> global_var_refs =
        {{"var2", {UINT32_C(1)}}};
    std::vector<real_talk::code::IdAddresses> func_refs;
    std::vector<real_talk::code::IdAddresses> native_func_refs;
    uint32_t module_version = UINT32_C(1);
    uint32_t main_cmds_code_size = UINT32_C(0);
    std::unique_ptr<real_talk::code::Module> module(
        new real_talk::code::Module(module_version,
                                    std::move(cmds_code),
                                    main_cmds_code_size,
                                    func_defs,
                                    global_var_defs,
                                    native_func_defs,
                                    func_refs,
                                    native_func_refs,
                                    global_var_refs));
    modules.push_back(std::move(module));
  }

  TestMissingDefError(modules, expected_id);
}

TEST_F(ExeLinkerTest, FuncMissingDefError) {
  Linker::Modules modules;
  std::string expected_id = "func";

  {
    std::unique_ptr<real_talk::code::Code> cmds_code(
        new real_talk::code::Code());
    std::vector<real_talk::code::IdSize> global_var_defs;
    std::vector<real_talk::code::IdAddress> func_defs;
    std::vector<std::string> native_func_defs;
    std::vector<real_talk::code::IdAddresses> global_var_refs;
    std::vector<real_talk::code::IdAddresses> func_refs =
        {{expected_id, {UINT32_C(0)}}};
    std::vector<real_talk::code::IdAddresses> native_func_refs;
    uint32_t module_version = UINT32_C(1);
    uint32_t main_cmds_code_size = UINT32_C(0);
    std::unique_ptr<real_talk::code::Module> module(
        new real_talk::code::Module(module_version,
                                    std::move(cmds_code),
                                    main_cmds_code_size,
                                    func_defs,
                                    global_var_defs,
                                    native_func_defs,
                                    func_refs,
                                    native_func_refs,
                                    global_var_refs));
    modules.push_back(std::move(module));
  }

  {
    std::unique_ptr<real_talk::code::Code> cmds_code(
        new real_talk::code::Code());
    std::vector<real_talk::code::IdSize> global_var_defs;
    std::vector<real_talk::code::IdAddress> func_defs;
    std::vector<std::string> native_func_defs;
    std::vector<real_talk::code::IdAddresses> global_var_refs;
    std::vector<real_talk::code::IdAddresses> func_refs =
        {{"func2", {UINT32_C(0)}}};
    std::vector<real_talk::code::IdAddresses> native_func_refs;
    uint32_t module_version = UINT32_C(1);
    uint32_t main_cmds_code_size = UINT32_C(0);
    std::unique_ptr<real_talk::code::Module> module(
        new real_talk::code::Module(module_version,
                                    std::move(cmds_code),
                                    main_cmds_code_size,
                                    func_defs,
                                    global_var_defs,
                                    native_func_defs,
                                    func_refs,
                                    native_func_refs,
                                    global_var_refs));
    modules.push_back(std::move(module));
  }

  TestMissingDefError(modules, expected_id);
}

TEST_F(ExeLinkerTest, NativeFuncMissingDefError) {
  Linker::Modules modules;
  std::string expected_id = "native_func";

  {
    std::unique_ptr<real_talk::code::Code> cmds_code(
        new real_talk::code::Code());
    std::vector<real_talk::code::IdSize> global_var_defs;
    std::vector<real_talk::code::IdAddress> func_defs;
    std::vector<std::string> native_func_defs;
    std::vector<real_talk::code::IdAddresses> global_var_refs;
    std::vector<real_talk::code::IdAddresses> func_refs;
    std::vector<real_talk::code::IdAddresses> native_func_refs =
        {{expected_id, {UINT32_C(0)}}};
    uint32_t module_version = UINT32_C(1);
    uint32_t main_cmds_code_size = UINT32_C(0);
    std::unique_ptr<real_talk::code::Module> module(
        new real_talk::code::Module(module_version,
                                    std::move(cmds_code),
                                    main_cmds_code_size,
                                    func_defs,
                                    global_var_defs,
                                    native_func_defs,
                                    func_refs,
                                    native_func_refs,
                                    global_var_refs));
    modules.push_back(std::move(module));
  }

  {
    std::unique_ptr<real_talk::code::Code> cmds_code(
        new real_talk::code::Code());
    std::vector<real_talk::code::IdSize> global_var_defs;
    std::vector<real_talk::code::IdAddress> func_defs;
    std::vector<std::string> native_func_defs;
    std::vector<real_talk::code::IdAddresses> global_var_refs;
    std::vector<real_talk::code::IdAddresses> func_refs;
    std::vector<real_talk::code::IdAddresses> native_func_refs =
        {{"native_func2", {UINT32_C(0)}}};
    uint32_t module_version = UINT32_C(1);
    uint32_t main_cmds_code_size = UINT32_C(0);
    std::unique_ptr<real_talk::code::Module> module(
        new real_talk::code::Module(module_version,
                                    std::move(cmds_code),
                                    main_cmds_code_size,
                                    func_defs,
                                    global_var_defs,
                                    native_func_defs,
                                    func_refs,
                                    native_func_refs,
                                    global_var_refs));
    modules.push_back(std::move(module));
  }

  TestMissingDefError(modules, expected_id);
}

TEST_F(ExeLinkerTest, GlobalVarDuplicateDefError) {
  TestGlobalVarDuplicateDefError();
}

TEST_F(ExeLinkerTest, FuncDuplicateDefError) {
  TestFuncDuplicateDefError();
}

TEST_F(ExeLinkerTest, NativeFuncDuplicateDefError) {
  TestNativeFuncDuplicateDefError();
}
}
}
