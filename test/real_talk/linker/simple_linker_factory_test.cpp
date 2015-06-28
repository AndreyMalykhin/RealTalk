
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <typeinfo>
#include "real_talk/linker/output_file_type.h"
#include "real_talk/linker/lib_linker.h"
#include "real_talk/linker/exe_linker.h"
#include "real_talk/linker/simple_linker_factory.h"

using std::unique_ptr;
using testing::Test;

namespace real_talk {
namespace linker {

class SimpleLinkerFactoryTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(SimpleLinkerFactoryTest, LibLinker) {
  unique_ptr<Linker> linker =
      SimpleLinkerFactory().Create(OutputFileType::kLib);
  ASSERT_TRUE(linker.get());
  const auto &expected_type = typeid(LibLinker);
  const auto &actual_type = typeid(*linker);
  ASSERT_EQ(expected_type, actual_type);
}

TEST_F(SimpleLinkerFactoryTest, ExeLinker) {
  unique_ptr<Linker> linker =
      SimpleLinkerFactory().Create(OutputFileType::kExe);
  ASSERT_TRUE(linker.get());
  const auto &expected_type = typeid(ExeLinker);
  const auto &actual_type = typeid(*linker);
  ASSERT_EQ(expected_type, actual_type);
}
}
}
