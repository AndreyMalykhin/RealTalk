
#include <boost/numeric/conversion/cast.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "real_talk/code/code.h"
#include "real_talk/test/test_config.h"
#include "real_talk/util/simple_file.h"

using std::vector;
using std::streampos;
using std::streamsize;
using std::unique_ptr;
using std::istream;
using std::ios;
using testing::Test;
using boost::numeric_cast;
using boost::filesystem::path;
using boost::filesystem::file_size;
using boost::filesystem::exists;
using boost::filesystem::remove;
using real_talk::test::TestConfig;
using real_talk::code::Code;

namespace real_talk {
namespace util {

class SimpleFileTest: public Test {
 protected:
  virtual void SetUp() override {}

  virtual void TearDown() override {
    while (!files_to_remove_.empty()) {
      remove(files_to_remove_.back());
      files_to_remove_.pop_back();
    }
  }

  void AssertFileContent(const char *expected_content,
                         size_t expected_size,
                         istream *stream,
                         const path &file_path) {
    uintmax_t actual_size = file_size(file_path);
    ASSERT_EQ(expected_size, actual_size);
    unique_ptr<char[]> actual_content(new char[actual_size]);
    stream->read(
        actual_content.get(), numeric_cast<streamsize>(actual_size));
    ASSERT_EQ(0, memcmp(expected_content, actual_content.get(), expected_size));
  }

  vector<path> files_to_remove_;
};

TEST_F(SimpleFileTest, Read) {
  path file_path(TestConfig::GetResourceDir() / path("file.txt"));
  char expected_content[] = "abc\n";
  size_t expected_content_size = 4;
  streampos expected_stream_position = 0;
  unique_ptr<istream> stream = SimpleFile().Read(file_path);
  ios::iostate expected_exceptions_mask = ios::failbit | ios::badbit;
  ASSERT_EQ(expected_exceptions_mask, stream->exceptions());
  ASSERT_EQ(expected_stream_position, stream->tellg());
  AssertFileContent(
      expected_content, expected_content_size, stream.get(), file_path);
}

TEST_F(SimpleFileTest, Write) {
  path file_path(TestConfig::GetResourceDir() / path("file2.txt"));
  files_to_remove_.push_back(file_path);
  Code code;
  code.Write<char>('a');
  code.Write<char>('b');
  size_t expected_content_size = 2;
  char expected_content[] = "ab";
  SimpleFile file;
  ASSERT_TRUE(!exists(file_path));
  file.Write(file_path, code);
  unique_ptr<istream> stream = file.Read(file_path);
  AssertFileContent(
      expected_content, expected_content_size, stream.get(), file_path);

  file.Write(file_path, code);
  stream = file.Read(file_path);
  AssertFileContent(
      expected_content, expected_content_size, stream.get(), file_path);
}
}
}
