
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "real_talk/test_config.h"
#include "real_talk/util/errors.h"
#include "real_talk/semantic/simple_import_file_searcher.h"

using std::vector;
using boost::filesystem::path;
using boost::filesystem::current_path;
using testing::Test;
using real_talk::TestConfig;
using real_talk::util::FileNotFoundError;

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
  path relative_file_path("./program.rt");
  path expected_absolute_file_path(
      current_path() / TestConfig::GetResourceDir() / "program.rt");
  vector<path> dirs = {TestConfig::GetResourceDir()};
  SimpleImportFileSearcher searcher(dirs);

  path actual_absolute_file_path = searcher.Search(relative_file_path);
  ASSERT_EQ(expected_absolute_file_path, actual_absolute_file_path);
}

TEST_F(SimpleImportFileSearcherTest, SearchFailsIfFileNotExists) {
  vector<path> dirs = {path(".")};
  SimpleImportFileSearcher searcher(dirs);
  path relative_file_path("i_am_not_here.rt");

  try {
    searcher.Search(relative_file_path);
  } catch (const FileNotFoundError&) {
  }
}
}
}
