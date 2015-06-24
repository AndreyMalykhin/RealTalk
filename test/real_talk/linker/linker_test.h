
#ifndef _REAL_TALK_LINKER_LINKER_TEST_H_
#define _REAL_TALK_LINKER_LINKER_TEST_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <limits>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/linker/linker.h"

namespace real_talk {
namespace linker {

class LinkerTest: public testing::Test {
 protected:
  virtual std::unique_ptr<Linker> CreateLinker() const = 0;

  std::string PrintCodeContainer(real_talk::code::CodeContainer *container) {
    container->GetCmdsCode().SetPosition(UINT32_C(0));
    std::ostringstream stream;
    stream << *container;
    return stream.str();
  }

  void TestDuplicateDefError(
      const Linker::Modules &modules, const std::string &expected_id) {
    uint32_t exe_version = UINT32_C(1);

    try {
      CreateLinker()->Link(modules, exe_version);
      FAIL();
    } catch (const Linker::DuplicateDefError &error) {
      std::string actual_id = error.GetId();
      ASSERT_EQ(expected_id, actual_id);
    }
  }

  void TestGlobalVarDuplicateDefError() {
    Linker::Modules modules;
    std::string expected_id = "var";

    {
      std::unique_ptr<real_talk::code::Code> cmds_code(
          new real_talk::code::Code());
      std::vector<std::string> ids_of_global_var_defs = {expected_id};
      std::vector<real_talk::code::IdAddress> id_addresses_of_func_defs;
      std::vector<std::string> ids_of_native_func_defs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_global_var_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_func_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_native_func_refs;
      uint32_t module_version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      std::unique_ptr<real_talk::code::Module> module(
          new real_talk::code::Module(module_version,
                                      std::move(cmds_code),
                                      main_cmds_code_size,
                                      id_addresses_of_func_defs,
                                      ids_of_global_var_defs,
                                      ids_of_native_func_defs,
                                      id_addresses_of_func_refs,
                                      id_addresses_of_native_func_refs,
                                      id_addresses_of_global_var_refs));
      modules.push_back(std::move(module));
    }

    {
      std::unique_ptr<real_talk::code::Code> cmds_code(
          new real_talk::code::Code());
      std::vector<std::string> ids_of_global_var_defs = {expected_id};
      std::vector<real_talk::code::IdAddress> id_addresses_of_func_defs;
      std::vector<std::string> ids_of_native_func_defs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_global_var_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_func_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_native_func_refs;
      uint32_t module_version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      std::unique_ptr<real_talk::code::Module> module(
          new real_talk::code::Module(module_version,
                                      std::move(cmds_code),
                                      main_cmds_code_size,
                                      id_addresses_of_func_defs,
                                      ids_of_global_var_defs,
                                      ids_of_native_func_defs,
                                      id_addresses_of_func_refs,
                                      id_addresses_of_native_func_refs,
                                      id_addresses_of_global_var_refs));
      modules.push_back(std::move(module));
    }

    TestDuplicateDefError(modules, expected_id);
  }

  void TestFuncDuplicateDefError() {
    Linker::Modules modules;
    std::string expected_id = "func";

    {
      std::unique_ptr<real_talk::code::Code> cmds_code(
          new real_talk::code::Code());
      std::vector<std::string> ids_of_global_var_defs;
      std::vector<real_talk::code::IdAddress> id_addresses_of_func_defs =
          {{expected_id, UINT32_C(0)}};
      std::vector<std::string> ids_of_native_func_defs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_global_var_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_func_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_native_func_refs;
      uint32_t module_version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      std::unique_ptr<real_talk::code::Module> module(
          new real_talk::code::Module(module_version,
                                      std::move(cmds_code),
                                      main_cmds_code_size,
                                      id_addresses_of_func_defs,
                                      ids_of_global_var_defs,
                                      ids_of_native_func_defs,
                                      id_addresses_of_func_refs,
                                      id_addresses_of_native_func_refs,
                                      id_addresses_of_global_var_refs));
      modules.push_back(std::move(module));
    }

    {
      std::unique_ptr<real_talk::code::Code> cmds_code(
          new real_talk::code::Code());
      std::vector<std::string> ids_of_global_var_defs;
      std::vector<real_talk::code::IdAddress> id_addresses_of_func_defs =
          {{expected_id, UINT32_C(1)}};
      std::vector<std::string> ids_of_native_func_defs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_global_var_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_func_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_native_func_refs;
      uint32_t module_version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      std::unique_ptr<real_talk::code::Module> module(
          new real_talk::code::Module(module_version,
                                      std::move(cmds_code),
                                      main_cmds_code_size,
                                      id_addresses_of_func_defs,
                                      ids_of_global_var_defs,
                                      ids_of_native_func_defs,
                                      id_addresses_of_func_refs,
                                      id_addresses_of_native_func_refs,
                                      id_addresses_of_global_var_refs));
      modules.push_back(std::move(module));
    }

    TestDuplicateDefError(modules, expected_id);
  }

  void TestNativeFuncDuplicateDefError() {
    Linker::Modules modules;
    std::string expected_id = "native_func";

    {
      std::unique_ptr<real_talk::code::Code> cmds_code(
          new real_talk::code::Code());
      std::vector<std::string> ids_of_global_var_defs;
      std::vector<real_talk::code::IdAddress> id_addresses_of_func_defs;
      std::vector<std::string> ids_of_native_func_defs = {expected_id};
      std::vector<real_talk::code::IdAddresses> id_addresses_of_global_var_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_func_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_native_func_refs;
      uint32_t module_version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      std::unique_ptr<real_talk::code::Module> module(
          new real_talk::code::Module(module_version,
                                      std::move(cmds_code),
                                      main_cmds_code_size,
                                      id_addresses_of_func_defs,
                                      ids_of_global_var_defs,
                                      ids_of_native_func_defs,
                                      id_addresses_of_func_refs,
                                      id_addresses_of_native_func_refs,
                                      id_addresses_of_global_var_refs));
      modules.push_back(std::move(module));
    }

    {
      std::unique_ptr<real_talk::code::Code> cmds_code(
          new real_talk::code::Code());
      std::vector<std::string> ids_of_global_var_defs;
      std::vector<real_talk::code::IdAddress> id_addresses_of_func_defs;
      std::vector<std::string> ids_of_native_func_defs = {expected_id};
      std::vector<real_talk::code::IdAddresses> id_addresses_of_global_var_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_func_refs;
      std::vector<real_talk::code::IdAddresses> id_addresses_of_native_func_refs;
      uint32_t module_version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      std::unique_ptr<real_talk::code::Module> module(
          new real_talk::code::Module(module_version,
                                      std::move(cmds_code),
                                      main_cmds_code_size,
                                      id_addresses_of_func_defs,
                                      ids_of_global_var_defs,
                                      ids_of_native_func_defs,
                                      id_addresses_of_func_refs,
                                      id_addresses_of_native_func_refs,
                                      id_addresses_of_global_var_refs));
      modules.push_back(std::move(module));
    }

    TestDuplicateDefError(modules, expected_id);
  }
};
}
}
#endif
