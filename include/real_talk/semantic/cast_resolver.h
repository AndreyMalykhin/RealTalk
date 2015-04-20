
#ifndef _REAL_TALK_SEMANTIC_CAST_RESOLVER_H_
#define _REAL_TALK_SEMANTIC_CAST_RESOLVER_H_

#include <memory>
#include <iostream>
#include "real_talk/semantic/cast_handler.h"

namespace real_talk {
namespace semantic {

class DataType;

class CastResolver: private CastHandler {
 public:
  class ResolvedCast {
   public:
    ResolvedCast(bool is_success,
                 std::unique_ptr<DataType> left_data_type,
                 std::unique_ptr<DataType> right_data_type);

    /**
     * @return False if cast can't be performed
     */
    bool IsSuccess() const;

    void SetSuccess(bool is_success);

    /**
     * @return Null if left data type doesn't need cast
     */
    const DataType *GetLeftDataType() const;

    /**
     * @param data_type Null if left data type doesn't need cast
     */
    void SetLeftDataType(std::unique_ptr<DataType> data_type);

    /**
     * @return Null if right data type doesn't need cast
     */
    const DataType *GetRightDataType() const;

    /**
     * @param data_type Null if right data type doesn't need cast
     */
    void SetRightDataType(std::unique_ptr<DataType> data_type);

    /**
     * @return Destination data type, to which cast was performed
     */
    const DataType *GetFinalDataType() const;

    friend bool operator==(const ResolvedCast &lhs, const ResolvedCast &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream, const ResolvedCast &resolved_cast);

   private:
    bool is_success_;
    std::unique_ptr<DataType> left_data_type_;
    std::unique_ptr<DataType> right_data_type_;
  };

  CastResolver();
  ResolvedCast Resolve(
      const DataType &left_data_type, const DataType &right_data_type) const;

 private:
  void DoResolve() const;
  void HandleCharToInt() const override;
  void HandleCharToLong() const override;
  void HandleCharToDouble() const override;
  void HandleCharToString() const override;
  void HandleIntToLong() const override;
  void HandleIntToDouble() const override;
  void HandleLongToDouble() const override;
  void HandleFail() const override;

  mutable const DataType *left_data_type_;
  mutable const DataType *right_data_type_;
  mutable Direction direction_;
  mutable ResolvedCast resolved_cast_;
};
}
}
#endif
