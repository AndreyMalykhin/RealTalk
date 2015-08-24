
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
#include "real_talk/code/simple_module_reader.h"
#include "real_talk/code/simple_code_container_writer.h"
#include "real_talk/code/module.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/id_addresses.h"
#include "real_talk/code/data_type_size.h"
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
namespace {

struct TestModule {
  Module expected_module;
};

class StreambufMock: public stringbuf {
 public:
  MOCK_METHOD3(seekoff,
               streampos(streamoff, ios_base::seekdir, ios_base::openmode));
};
}

class SimpleModuleReaderTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  vector<TestModule> GetDataForReadFromTest() {
    vector<TestModule> test_data_suits;

    {
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
      Module expected_module(version,
                             move(cmds_code),
                             main_cmds_code_size,
                             func_defs,
                             global_var_defs,
                             native_func_defs,
                             func_refs,
                             native_func_refs,
                             global_var_refs);
      TestModule test_data = {move(expected_module)};
      test_data_suits.push_back(move(test_data));
    }

    {
      unique_ptr<Code> cmds_code(new Code());
      vector<IdSize> global_var_defs;
      vector<IdAddress> func_defs;
      vector<string> native_func_defs;
      vector<IdAddresses> global_var_refs;
      vector<IdAddresses> func_refs;
      vector<IdAddresses> native_func_refs;
      uint32_t version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      Module expected_module(version,
                             move(cmds_code),
                             main_cmds_code_size,
                             func_defs,
                             global_var_defs,
                             native_func_defs,
                             func_refs,
                             native_func_refs,
                             global_var_refs);
      TestModule test_data = {move(expected_module)};
      test_data_suits.push_back(move(test_data));
    }

    return test_data_suits;
  }
};

TEST_F(SimpleModuleReaderTest, ReadFromCode) {
  for (const TestModule &test_data: GetDataForReadFromTest()) {
    Code module_code;
    SimpleCodeContainerWriter().Write(test_data.expected_module, &module_code);
    module_code.SetPosition(UINT32_C(0));
    unique_ptr<Module> actual_module =
        SimpleModuleReader().ReadFromCode(&module_code);
    ASSERT_TRUE(actual_module.get());
    ASSERT_EQ(test_data.expected_module, *actual_module);
  }
}

TEST_F(SimpleModuleReaderTest, ReadFromStream) {
  for (const TestModule &test_data: GetDataForReadFromTest()) {
    Code module_code;
    SimpleCodeContainerWriter().Write(test_data.expected_module, &module_code);
    stringstream stream;
    stream.exceptions(ios::failbit | ios::badbit);
    stream.write(
        reinterpret_cast<char*>(module_code.GetData()), module_code.GetSize());
    unique_ptr<Module> actual_module =
        SimpleModuleReader().ReadFromStream(&stream);
    ASSERT_TRUE(actual_module.get());
    ASSERT_EQ(test_data.expected_module, *actual_module);
  }
}

TEST_F(SimpleModuleReaderTest, ReadFromStreamWithIOError) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, seekoff(_, _, _))
      .Times(1)
      .WillOnce(Throw(ios::failure("seekoff() error")));
  iostream stream(&stream_buffer);
  SimpleModuleReader reader;

  try {
    reader.ReadFromStream(&stream);
    FAIL();
  } catch (const IOError&) {}
}

TEST_F(SimpleModuleReaderTest, ReadFromStreamWithCodeSizeOverflowError) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, seekoff(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(numeric_limits<int64_t>::max()));
  iostream stream(&stream_buffer);
  SimpleModuleReader reader;

  try {
    reader.ReadFromStream(&stream);
    FAIL();
  } catch (const Code::CodeSizeOverflowError&) {}
}
}
}
