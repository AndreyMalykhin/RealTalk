
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/test/test_config.h"
#include "real_talk/util/simple_file.h"

using std::streampos;
using std::unique_ptr;
using std::istream;
using std::ios;
using testing::Test;
using boost::filesystem::path;
using real_talk::test::TestConfig;

namespace real_talk {
namespace util {

class SimpleFileTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleFileTest, Read) {
  path file_path(TestConfig::GetResourceDir() / path("file.txt"));
  char expected_content[] = "abc\n";
  streampos expected_content_size = 4;
  streampos expected_stream_position = 0;
  unique_ptr<istream> stream = SimpleFile().Read(file_path);
  ios::iostate expected_exceptions_mask = ios::failbit | ios::badbit;
  ASSERT_EQ(expected_exceptions_mask, stream->exceptions());
  ASSERT_EQ(expected_stream_position, stream->tellg());
  stream->seekg(0, ios::end);
  streampos actual_content_size = stream->tellg();
  stream->seekg(0, ios::beg);
  ASSERT_EQ(expected_content_size, actual_content_size);
  unique_ptr<char[]> actual_content(new char[actual_content_size]);
  stream->read(actual_content.get(), actual_content_size);
  ASSERT_STREQ(expected_content, actual_content.get());
}
}
}
