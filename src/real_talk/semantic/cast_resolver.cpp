
#include <cassert>
#include "real_talk/semantic/cast_resolver.h"

using std::move;
using std::unique_ptr;
using std::ostream;

namespace real_talk {
namespace semantic {

CastResolver::CastResolver(): resolved_cast_(false, nullptr, nullptr) {}

CastResolver::ResolvedCast CastResolver::Resolve(
    const DataType &left_data_type, const DataType &right_data_type) const {
  resolved_cast_.SetSuccess(false);
  resolved_cast_.SetLeftDataType(nullptr);
  resolved_cast_.SetRightDataType(nullptr);
  left_data_type_ = &left_data_type;
  right_data_type_ = &right_data_type;
  direction_ = Direction::kLeftToRight;
  Handle(left_data_type, right_data_type, direction_);

  if (!resolved_cast_.IsSuccess()) {
    direction_ = Direction::kRightToLeft;
    Handle(left_data_type, right_data_type, direction_);
  }

  return move(resolved_cast_);
}

void CastResolver::HandleCharToInt() const {DoResolve();}

void CastResolver::HandleCharToLong() const {DoResolve();}

void CastResolver::HandleCharToDouble() const {DoResolve();}

void CastResolver::HandleCharToString() const {DoResolve();}

void CastResolver::HandleIntToLong() const {DoResolve();}

void CastResolver::HandleIntToDouble() const {DoResolve();}

void CastResolver::HandleLongToDouble() const {DoResolve();}

void CastResolver::HandleFail() const {}

void CastResolver::DoResolve() const {
  resolved_cast_.SetSuccess(true);

  if (direction_ == Direction::kLeftToRight) {
    resolved_cast_.SetLeftDataType(right_data_type_->Clone());
  } else {
    resolved_cast_.SetRightDataType(left_data_type_->Clone());
  }
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

void CastResolver::ResolvedCast::SetSuccess(bool is_success) {
  is_success_ = is_success;
}

const DataType *CastResolver::ResolvedCast::GetLeftDataType() const {
  assert(is_success_);
  return left_data_type_.get();
}

void CastResolver::ResolvedCast::SetLeftDataType(
    unique_ptr<DataType> data_type) {
  left_data_type_ = move(data_type);
}

const DataType *CastResolver::ResolvedCast::GetRightDataType() const {
  assert(is_success_);
  return right_data_type_.get();
}

void CastResolver::ResolvedCast::SetRightDataType(
    unique_ptr<DataType> data_type) {
  right_data_type_ = move(data_type);
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
