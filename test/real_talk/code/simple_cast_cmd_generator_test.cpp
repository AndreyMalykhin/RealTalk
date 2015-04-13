
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/simple_cast_cmd_generator.h"

using std::unique_ptr;
using testing::Test;
using real_talk::semantic::DataType;
using real_talk::semantic::IntDataType;
using real_talk::semantic::LongDataType;
using real_talk::semantic::CharDataType;
using real_talk::semantic::StringDataType;
using real_talk::semantic::DoubleDataType;

namespace real_talk {
namespace code {

class SimpleCastCmdGeneratorTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestGenerate(const DataType &dest_data_type,
                    const DataType &src_data_type,
                    CmdId expected_cmd_id) {
    SimpleCastCmdGenerator generator;
    CmdId actual_cmd_id = generator.Generate(dest_data_type, src_data_type);
    ASSERT_EQ(expected_cmd_id, actual_cmd_id);
  }
};

TEST_F(SimpleCastCmdGeneratorTest, CastCharToIntCmd) {
  IntDataType dest_data_type;
  CharDataType src_data_type;
  CmdId expected_cmd_id = CmdId::kCastCharToInt;
  TestGenerate(dest_data_type, src_data_type, expected_cmd_id);
}

TEST_F(SimpleCastCmdGeneratorTest, CastCharToLongCmd) {
  LongDataType dest_data_type;
  CharDataType src_data_type;
  CmdId expected_cmd_id = CmdId::kCastCharToLong;
  TestGenerate(dest_data_type, src_data_type, expected_cmd_id);
}

TEST_F(SimpleCastCmdGeneratorTest, CastCharToStringCmd) {
  StringDataType dest_data_type;
  CharDataType src_data_type;
  CmdId expected_cmd_id = CmdId::kCastCharToString;
  TestGenerate(dest_data_type, src_data_type, expected_cmd_id);
}

TEST_F(SimpleCastCmdGeneratorTest, CastIntToLongCmd) {
  LongDataType dest_data_type;
  IntDataType src_data_type;
  CmdId expected_cmd_id = CmdId::kCastIntToLong;
  TestGenerate(dest_data_type, src_data_type, expected_cmd_id);
}

TEST_F(SimpleCastCmdGeneratorTest, CastIntToDoubleCmd) {
  DoubleDataType dest_data_type;
  IntDataType src_data_type;
  CmdId expected_cmd_id = CmdId::kCastIntToDouble;
  TestGenerate(dest_data_type, src_data_type, expected_cmd_id);
}

TEST_F(SimpleCastCmdGeneratorTest, CastLongToDoubleCmd) {
  DoubleDataType dest_data_type;
  LongDataType src_data_type;
  CmdId expected_cmd_id = CmdId::kCastLongToDouble;
  TestGenerate(dest_data_type, src_data_type, expected_cmd_id);
}
}
}
