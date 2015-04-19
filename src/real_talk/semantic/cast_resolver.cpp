
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <cstdint>
#include <cassert>
#include <utility>
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/cast_resolver.h"

using std::unique_ptr;
using std::ostream;
using std::unordered_map;
using std::make_pair;
using std::pair;
using boost::hash;

namespace real_talk {
namespace semantic {
namespace {

enum class Direction: uint8_t {
  kLeftToRight = UINT8_C(1),
  kRightToLeft = UINT8_C(2)
};

typedef pair<DataTypeId, DataTypeId> CastsKey;
typedef unordered_map< CastsKey, Direction, hash<CastsKey> > Casts;

const Casts &kCasts = *new Casts({
    {make_pair(DataTypeId::kChar, DataTypeId::kInt),
          Direction::kLeftToRight},
    {make_pair(DataTypeId::kChar, DataTypeId::kLong),
          Direction::kLeftToRight},
    {make_pair(DataTypeId::kChar, DataTypeId::kString),
          Direction::kLeftToRight},
    {make_pair(DataTypeId::kInt, DataTypeId::kLong),
          Direction::kLeftToRight},
    {make_pair(DataTypeId::kInt, DataTypeId::kDouble),
          Direction::kLeftToRight},
    {make_pair(DataTypeId::kLong, DataTypeId::kDouble),
          Direction::kLeftToRight},

    {make_pair(DataTypeId::kInt, DataTypeId::kChar),
          Direction::kRightToLeft},
    {make_pair(DataTypeId::kLong, DataTypeId::kChar),
          Direction::kRightToLeft},
    {make_pair(DataTypeId::kString, DataTypeId::kChar),
          Direction::kRightToLeft},
    {make_pair(DataTypeId::kLong, DataTypeId::kInt),
          Direction::kRightToLeft},
    {make_pair(DataTypeId::kDouble, DataTypeId::kInt),
          Direction::kRightToLeft},
    {make_pair(DataTypeId::kDouble, DataTypeId::kLong),
          Direction::kRightToLeft}
  });
}

CastResolver::ResolvedCast CastResolver::Resolve(
    const DataType &left_data_type, const DataType &right_data_type) const {
  assert(left_data_type != right_data_type);
  bool is_success = false;
  unique_ptr<DataType> casted_left_data_type;
  unique_ptr<DataType> casted_right_data_type;
  Casts::const_iterator casts_it =
      kCasts.find(make_pair(left_data_type.GetId(), right_data_type.GetId()));

  if (casts_it != kCasts.cend()) {
    const Direction direction = casts_it->second;

    if (direction == Direction::kLeftToRight) {
      casted_left_data_type = right_data_type.Clone();
    } else {
      casted_right_data_type = left_data_type.Clone();
    }

    is_success = true;
  }

  return ResolvedCast(
      is_success, move(casted_left_data_type), move(casted_right_data_type));
}

CastResolver::ResolvedCast::ResolvedCast(
    bool is_success,
    unique_ptr<DataType> left_data_type,
    unique_ptr<DataType> right_data_type)
    : is_success_(is_success),
      left_data_type_(move(left_data_type)),
      right_data_type_(move(right_data_type)) {}

bool CastResolver::ResolvedCast::IsSuccess() const {
  return is_success_;
}

const DataType *CastResolver::ResolvedCast::GetLeftDataType() const {
  assert(is_success_);
  return left_data_type_.get();
}

const DataType *CastResolver::ResolvedCast::GetRightDataType() const {
  assert(is_success_);
  return right_data_type_.get();
}

const DataType *CastResolver::ResolvedCast::GetFinalDataType() const {
  assert(is_success_);
  assert(left_data_type_ || right_data_type_);
  return left_data_type_ ? left_data_type_.get() : right_data_type_.get();
}

bool operator==(const CastResolver::ResolvedCast &lhs,
                const CastResolver::ResolvedCast &rhs) {
  if (lhs.is_success_ != rhs.is_success_) {
    return false;
  } else if (lhs.left_data_type_
             && rhs.left_data_type_
             && *(lhs.left_data_type_) != *(rhs.left_data_type_)) {
    return false;
  } else if ((!lhs.left_data_type_ && rhs.left_data_type_)
             || (lhs.left_data_type_ && !rhs.left_data_type_)) {
    return false;
  }

  return true;
}

ostream &operator<<(
    ostream &stream, const CastResolver::ResolvedCast &resolved_cast) {
  stream << "is_success=" << resolved_cast.is_success_ << "; left_data_type=";

  if (resolved_cast.left_data_type_) {
    stream << *(resolved_cast.left_data_type_);
  }

  stream << "; right_data_type=";

  if (resolved_cast.right_data_type_) {
    stream << *(resolved_cast.right_data_type_);
  }

  return stream;
}
}
}
