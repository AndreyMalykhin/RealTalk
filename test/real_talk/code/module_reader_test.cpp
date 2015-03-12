
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/filesystem.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include "real_talk/util/errors.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/module.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/test_utils.h"
#include "real_talk/code/code.h"

using std::numeric_limits;
using std::ios;
using std::ios_base;
using std::move;
using std::vector;
using std::string;
using std::stringstream;
using std::iostream;
using std::streambuf;
using std::stringbuf;
using std::streampos;
using std::streamoff;
using std::istream;
using std::unique_ptr;
using testing::Return;
using testing::AnyNumber;
using testing::Test;
using testing::Throw;
using testing::_;
using boost::filesystem::path;
using real_talk::util::IOError;

namespace real_talk {
namespace code {

class StreambufMock: public stringbuf {
 public:
  MOCK_METHOD3(seekoff,
               streampos(streamoff, ios_base::seekdir, ios_base::openmode));
};

class ModuleReaderTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }
};

TEST_F(ModuleReaderTest, Read) {
  struct TestData {
    Module expected_module;
  };
  vector<TestData> test_data_suits;

  {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kCreateGlobalInt);
    cmds_code->WriteUint32(UINT32_C(7));
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);
    cmds_code->SetPosition(UINT32_C(0));
    vector<path> import_file_paths = {"src/class.rt", "src/class2.rt"};
    vector<string> ids_of_global_var_defs = {"var", "var2"};
    vector<IdAddress> id_addresses_of_func_defs = {{"func", UINT32_C(1)},
                                                   {"func2", UINT32_C(2)}};
    vector<string> ids_of_native_func_defs = {"native_func", "native_func2"};
    vector<IdAddress> id_addresses_of_global_var_refs = {{"var", UINT32_C(3)},
                                                         {"var2", UINT32_C(4)}};
    vector<IdAddress> id_addresses_of_func_refs = {{"func", UINT32_C(5)},
                                                   {"func2", UINT32_C(6)}};
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           id_addresses_of_func_defs,
                           ids_of_global_var_defs,
                           ids_of_native_func_defs,
                           id_addresses_of_func_refs,
                           id_addresses_of_global_var_refs,
                           import_file_paths);
    TestData test_data = {move(expected_module)};
    test_data_suits.push_back(move(test_data));
  }

  {
    unique_ptr<Code> cmds_code(new Code());
    cmds_code->WriteCmdId(CmdId::kEndMain);
    cmds_code->WriteCmdId(CmdId::kEndFuncs);
    cmds_code->SetPosition(UINT32_C(0));
    vector<path> import_file_paths;
    vector<string> ids_of_global_var_defs;
    vector<IdAddress> id_addresses_of_func_defs;
    vector<string> ids_of_native_func_defs;
    vector<IdAddress> id_addresses_of_global_var_refs;
    vector<IdAddress> id_addresses_of_func_refs;
    uint32_t version = UINT32_C(1);
    Module expected_module(version,
                           move(cmds_code),
                           id_addresses_of_func_defs,
                           ids_of_global_var_defs,
                           ids_of_native_func_defs,
                           id_addresses_of_func_refs,
                           id_addresses_of_global_var_refs,
                           import_file_paths);
    TestData test_data = {move(expected_module)};
    test_data_suits.push_back(move(test_data));
  }

  for (const TestData &test_data: test_data_suits) {
    Code module_code;
    WriteModule(test_data.expected_module, module_code);
    stringstream module_code_stream;
    module_code_stream.exceptions(ios::failbit | ios::badbit);
    module_code_stream.write(
        reinterpret_cast<char*>(module_code.GetData()), module_code.GetSize());

    unique_ptr<Module> actual_module = ModuleReader().Read(module_code_stream);
    ASSERT_EQ(test_data.expected_module, *actual_module);
  }
}

TEST_F(ModuleReaderTest, ReadFailsOnIOError) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, seekoff(_, _, _))
      .Times(1)
      .WillOnce(Throw(ios::failure("seekoff() error")));
  iostream module_code_stream(&stream_buffer);
  ModuleReader reader;

  try {
    reader.Read(module_code_stream);
    FAIL();
  } catch (const IOError&) {}
}

TEST_F(ModuleReaderTest, ReadFailsIfCodeSizeExceeds32Bits) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, seekoff(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(numeric_limits<long long>::max()));
  iostream module_code_stream(&stream_buffer);
  ModuleReader reader;

  try {
    reader.Read(module_code_stream);
    FAIL();
  } catch (const Code::CodeSizeOverflowError&) {}
}
}
}
