
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/test/test_config.h"
#include "real_talk/util/simple_dir_creator.h"

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::remove_all;
using testing::Test;
using real_talk::test::TestConfig;

namespace real_talk {
namespace util {

class SimpleDirCreatorTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleDirCreatorTest, Create) {
  path parent_dir_path(TestConfig::GetFixturesDir() / path("parent"));
  path child_dir_path(parent_dir_path / path("child"));
  ASSERT_FALSE(exists(parent_dir_path));
  ASSERT_FALSE(exists(child_dir_path));
  SimpleDirCreator().Create(child_dir_path);
  ASSERT_TRUE(exists(parent_dir_path));
  ASSERT_TRUE(exists(child_dir_path));
  SimpleDirCreator().Create(child_dir_path);
  ASSERT_EQ(2, remove_all(parent_dir_path));
}
}
}
