
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "real_talk/semantic/char_data_type.h"
#include "real_talk/semantic/int_data_type.h"
#include "real_talk/semantic/long_data_type.h"
#include "real_talk/semantic/double_data_type.h"
#include "real_talk/semantic/string_data_type.h"
#include "real_talk/semantic/cast_resolver.h"

using std::unique_ptr;
using testing::Test;

namespace real_talk {
namespace semantic {

class CastResolverTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}

  void TestResolve(const DataType &left_data_type,
                   const DataType &right_data_type,
                   const CastResolver::ResolvedCast &expected_resolved_cast) {
    CastResolver::ResolvedCast actual_resolved_cast = CastResolver().Resolve(
        left_data_type, right_data_type);
    ASSERT_EQ(expected_resolved_cast, actual_resolved_cast);
  }
};

TEST_F(CastResolverTest, ResolveCharAndInt) {
  CharDataType left_data_type;
  IntDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type = right_data_type.Clone();
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveCharAndLong) {
  CharDataType left_data_type;
  LongDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type = right_data_type.Clone();
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveCharAndString) {
  CharDataType left_data_type;
  StringDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type = right_data_type.Clone();
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveIntAndLong) {
  IntDataType left_data_type;
  LongDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type = right_data_type.Clone();
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveIntAndDouble) {
  IntDataType left_data_type;
  DoubleDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type = right_data_type.Clone();
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveLongAndDouble) {
  LongDataType left_data_type;
  DoubleDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type = right_data_type.Clone();
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveIntAndChar) {
  IntDataType left_data_type;
  CharDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type = left_data_type.Clone();
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveLongAndChar) {
  LongDataType left_data_type;
  CharDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type = left_data_type.Clone();
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveStringAndChar) {
  StringDataType left_data_type;
  CharDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type = left_data_type.Clone();
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveLongAndInt) {
  LongDataType left_data_type;
  IntDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type = left_data_type.Clone();
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveDoubleAndInt) {
  DoubleDataType left_data_type;
  IntDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type = left_data_type.Clone();
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveDoubleAndLong) {
  DoubleDataType left_data_type;
  LongDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type = left_data_type.Clone();
  bool is_success = true;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  TestResolve(left_data_type, right_data_type, expected_resolved_cast);
}

TEST_F(CastResolverTest, ResolveFails) {
  IntDataType left_data_type;
  StringDataType right_data_type;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type;
  bool is_success = false;
  CastResolver::ResolvedCast expected_resolved_cast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
  CastResolver::ResolvedCast actual_resolved_cast = CastResolver().Resolve(
      left_data_type, right_data_type);
  ASSERT_EQ(expected_resolved_cast, actual_resolved_cast);
}
}
}
