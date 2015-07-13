
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
#include "real_talk/code/simple_exe_reader.h"
#include "real_talk/code/simple_code_container_writer.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/id_addresses.h"
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

struct TestExe {
  Exe expected_exe;
};

class StreambufMock: public stringbuf {
 public:
  MOCK_METHOD3(seekoff,
               streampos(streamoff, ios_base::seekdir, ios_base::openmode));
};
}

class SimpleExeReaderTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  vector<TestExe> GetDataForReadFromTest() {
    vector<TestExe> test_data_suits;

    {
      unique_ptr<Code> cmds_code(new Code());
      cmds_code->WriteCmdId(CmdId::kCreateGlobalIntVar);
      cmds_code->WriteCmdId(CmdId::kCreateGlobalLongVar);
      uint32_t main_cmds_code_size = cmds_code->GetPosition();
      cmds_code->WriteCmdId(CmdId::kCreateLocalDoubleVar);
      cmds_code->SetPosition(UINT32_C(0));
      vector<string> native_func_defs = {"native_func", "native_func2"};
      vector<IdAddresses> native_func_refs =
          {{"func3", {UINT32_C(1), UINT32_C(2)}},
           {"func4", {UINT32_C(3), UINT32_C(4)}}};
      uint32_t version = UINT32_C(1);
      uint32_t global_vars_size = UINT32_C(7);
      Exe expected_exe(version,
                       move(cmds_code),
                       main_cmds_code_size,
                       global_vars_size,
                       native_func_defs,
                       native_func_refs);
      TestExe test_data = {move(expected_exe)};
      test_data_suits.push_back(move(test_data));
    }

    {
      unique_ptr<Code> cmds_code(new Code());
      vector<string> native_func_defs;
      vector<IdAddresses> native_func_refs;
      uint32_t version = UINT32_C(1);
      uint32_t main_cmds_code_size = UINT32_C(0);
      uint32_t global_vars_size = UINT32_C(0);
      Exe expected_exe(version,
                       move(cmds_code),
                       main_cmds_code_size,
                       global_vars_size,
                       native_func_defs,
                       native_func_refs);
      TestExe test_data = {move(expected_exe)};
      test_data_suits.push_back(move(test_data));
    }

    return test_data_suits;
  }
};

TEST_F(SimpleExeReaderTest, ReadFromCode) {
  for (const TestExe &test_data: GetDataForReadFromTest()) {
    Code code;
    SimpleCodeContainerWriter().Write(test_data.expected_exe, &code);
    code.SetPosition(UINT32_C(0));
    unique_ptr<Exe> actual_exe = SimpleExeReader().ReadFromCode(&code);
    ASSERT_TRUE(actual_exe.get());
    ASSERT_EQ(test_data.expected_exe, *actual_exe);
  }
}

TEST_F(SimpleExeReaderTest, ReadFromStream) {
  for (const TestExe &test_data: GetDataForReadFromTest()) {
    Code code;
    SimpleCodeContainerWriter().Write(test_data.expected_exe, &code);
    stringstream stream;
    stream.exceptions(ios::failbit | ios::badbit);
    stream.write(
        reinterpret_cast<char*>(code.GetData()), code.GetSize());
    unique_ptr<Exe> actual_exe = SimpleExeReader().ReadFromStream(&stream);
    ASSERT_TRUE(actual_exe.get());
    ASSERT_EQ(test_data.expected_exe, *actual_exe);
  }
}

TEST_F(SimpleExeReaderTest, ReadFromStreamWithIOError) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, seekoff(_, _, _))
      .Times(1)
      .WillOnce(Throw(ios::failure("seekoff() error")));
  iostream stream(&stream_buffer);
  SimpleExeReader reader;

  try {
    reader.ReadFromStream(&stream);
    FAIL();
  } catch (const IOError&) {}
}

TEST_F(SimpleExeReaderTest, ReadFromStreamWithCodeSizeOverflowError) {
  StreambufMock stream_buffer;
  EXPECT_CALL(stream_buffer, seekoff(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(numeric_limits<int64_t>::max()));
  iostream stream(&stream_buffer);
  SimpleExeReader reader;

  try {
    reader.ReadFromStream(&stream);
    FAIL();
  } catch (const Code::CodeSizeOverflowError&) {}
}
}
}
