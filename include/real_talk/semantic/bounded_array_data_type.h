
#ifndef _REAL_TALK_SEMANTIC_BOUNDED_ARRAY_DATA_TYPE_H_
#define _REAL_TALK_SEMANTIC_BOUNDED_ARRAY_DATA_TYPE_H_

#include <cstdint>
#include <string>
#include "real_talk/semantic/data_type.h"

namespace real_talk {
namespace semantic {

class IntLit;

class BoundedArrayDataType: public DataType {
 public:
  BoundedArrayDataType(std::unique_ptr<DataType> element_data_type,
                       std::unique_ptr<IntLit> size);
  virtual std::string GetName() const override;
  virtual std::unique_ptr<DataType> Clone() const override;

 private:
  virtual bool IsEqual(const DataType &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<DataType> element_data_type_;
  std::unique_ptr<IntLit> size_;
};
}
}
#endif
