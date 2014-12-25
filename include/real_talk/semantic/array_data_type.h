
#ifndef _REAL_TALK_SEMANTIC_ARRAY_DATA_TYPE_H_
#define _REAL_TALK_SEMANTIC_ARRAY_DATA_TYPE_H_

#include <string>
#include "real_talk/semantic/data_type.h"

namespace real_talk {
namespace semantic {

class ArrayDataType: public DataType {
 public:
  explicit ArrayDataType(std::unique_ptr<DataType> element_data_type);
  virtual std::string GetName() const override;
  virtual std::unique_ptr<DataType> Clone() const override;
  virtual void Accept(DataTypeVisitor &visitor) const override;
  const DataType &GetElementDataType() const;

 private:
  virtual bool IsEqual(const DataType &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<DataType> element_data_type_;
};
}
}
#endif
