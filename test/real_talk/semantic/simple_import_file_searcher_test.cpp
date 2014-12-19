
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/test/test_config.h"
#include "real_talk/semantic/simple_import_file_searcher.h"

using boost::filesystem::path;
using boost::filesystem::current_path;
using boost::filesystem::filesystem_error;
using testing::Test;
using real_talk::test::TestConfig;

namespace real_talk {
namespace semantic {

class SimpleImportFileSearcherTest: public Test {
 protected:
  virtual void SetUp() override {
  }

  virtual void TearDown() override {
  }
};

TEST_F(SimpleImportFileSearcherTest, Search) {
  SimpleImportFileSearcher searcher;
  path relative_file_path(TestConfig::GetResourceDir() / "." / "program.rt");
  path expected_absolute_file_path(
      current_path() / TestConfig::GetResourceDir() / "program.rt");

  path actual_absolute_file_path = searcher.Search(relative_file_path);
  ASSERT_EQ(expected_absolute_file_path, actual_absolute_file_path);
}

TEST_F(SimpleImportFileSearcherTest, SearchFailsIfFileNotExists) {
  SimpleImportFileSearcher searcher;
  path relative_file_path("i_am_not_here.rt");

  try {
    searcher.Search(relative_file_path);
  } catch (const filesystem_error&) {
  }
}
}
}
