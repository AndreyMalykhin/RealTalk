
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "real_talk/test/test_config.h"
#include "real_talk/util/errors.h"
#include "real_talk/compiler/simple_import_file_searcher.h"

using std::vector;
using boost::filesystem::path;
using boost::filesystem::current_path;
using testing::Test;
using real_talk::test::TestConfig;
using real_talk::util::FileNotFoundError;

namespace real_talk {
namespace compiler {

class SimpleImportFileSearcherTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleImportFileSearcherTest, Search) {

}

TEST_F(SimpleImportFileSearcherTest, SearchFailsIfFileNotExists) {

}
}
}
