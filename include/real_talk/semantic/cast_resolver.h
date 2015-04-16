
#ifndef _REAL_TALK_SEMANTIC_CAST_RESOLVER_H_
#define _REAL_TALK_SEMANTIC_CAST_RESOLVER_H_

#include <iostream>

namespace real_talk {
namespace semantic {

class DataType;

class CastResolver {
 public:
  class ResolvedCast {
   public:
    ResolvedCast(bool is_success,
                 const DataType *left_data_type,
                 const DataType *right_data_type);

    /**
     * @return False if cast can't be performed
     */
    bool IsSuccess() const;

    /**
     * @return Null if left data type doesn't need cast
     */
    const DataType *GetLeftDataType() const;

    /**
     * @return Null if right data type doesn't need cast
     */
    const DataType *GetRightDataType() const;

    /**
     * @return Destination data type, to which cast was performed
     */
    const DataType *GetFinalDataType() const;

    friend bool operator==(const ResolvedCast &lhs, const ResolvedCast &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream, const ResolvedCast &resolved_cast);

   private:
    bool is_success_;
    const DataType *left_data_type_;
    const DataType *right_data_type_;
  };

  bool CanCastTo(
      const DataType &dest_data_type, const DataType &src_data_type) const;
  ResolvedCast Resolve(
      const DataType &left_data_type, const DataType &right_data_type) const;
};
}
}
#endif
