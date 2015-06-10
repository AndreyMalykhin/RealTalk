
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "real_talk/test/test_config.h"
#include "real_talk/util/errors.h"
#include "real_talk/util/simple_file_searcher.h"

using std::vector;
using boost::filesystem::path;
using boost::filesystem::canonical;
using boost::filesystem::current_path;
using testing::Test;
using real_talk::test::TestConfig;

namespace real_talk {
namespace util {

class SimpleFileSearcherTest: public Test {
 protected:
  virtual void SetUp() override {
    old_current_path_ = current_path();
    current_path(TestConfig::GetResourceDir() / path("myapp"));
  }

  virtual void TearDown() override {
    current_path(old_current_path_);
  }

 private:
  path old_current_path_;
};

TEST_F(SimpleFileSearcherTest, SearchInSrcDir) {
  path search_file_path("myapp/mycomponent.rts");
  path src_dir_path("src");
  path vendor_dir_path("vendor");
  vector<path> import_dir_paths = {"../mylib2"};
  path expected_found_file_path(
      canonical(path("src/myapp/mycomponent.rts")));
  SimpleFileSearcher searcher;
  path actual_found_file_path = searcher.Search(
      search_file_path, src_dir_path, vendor_dir_path, import_dir_paths);
  ASSERT_EQ(expected_found_file_path.string(),
            actual_found_file_path.string());
}

TEST_F(SimpleFileSearcherTest, SearcInVendorDir) {
  path search_file_path("mylib/mylib.rts");
  path src_dir_path("src");
  path vendor_dir_path("vendor");
  vector<path> import_dir_paths = {"../mylib2"};
  path expected_found_file_path(
      canonical(path("vendor/myvendor/mylib/src/mylib/mylib.rts")));
  SimpleFileSearcher searcher;
  path actual_found_file_path = searcher.Search(
      search_file_path, src_dir_path, vendor_dir_path, import_dir_paths);
  ASSERT_EQ(expected_found_file_path.string(),
            actual_found_file_path.string());
}

TEST_F(SimpleFileSearcherTest, SearcInImportDir) {
  path search_file_path("mylib2/mylib2.rts");
  path src_dir_path("src");
  path vendor_dir_path("vendor");
  vector<path> import_dir_paths = {"../mylib2"};
  path expected_found_file_path(
      canonical(path("../mylib2/src/mylib2/mylib2.rts")));
  SimpleFileSearcher searcher;
  path actual_found_file_path = searcher.Search(
      search_file_path, src_dir_path, vendor_dir_path, import_dir_paths);
  ASSERT_EQ(expected_found_file_path.string(),
            actual_found_file_path.string());
}

TEST_F(SimpleFileSearcherTest, SearchCantFind) {
  path search_file_path("i_am_not_here.rts");
  path src_dir_path("src");
  path vendor_dir_path("vendor");
  vector<path> import_dir_paths = {"../mylib2"};
  path expected_found_file_path;
  SimpleFileSearcher searcher;
  path actual_found_file_path = searcher.Search(
      search_file_path, src_dir_path, vendor_dir_path, import_dir_paths);
  ASSERT_EQ(expected_found_file_path.string(),
            actual_found_file_path.string());
}
}
}
