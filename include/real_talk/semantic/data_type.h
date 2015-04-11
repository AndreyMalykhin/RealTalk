
#ifndef _REAL_TALK_SEMANTIC_DATA_TYPE_H_
#define _REAL_TALK_SEMANTIC_DATA_TYPE_H_

#include <cstdint>
#include <string>
#include <memory>
#include <iostream>

namespace real_talk {
namespace semantic {

class DataTypeVisitor;

enum class DataTypeId: uint8_t {
  kInt = UINT8_C(1),
  kLong,
  kDouble,
  kBool,
  kChar,
  kString,
  kVoid,
  kArray,
  kFunc
};

class DataType {
 public:
  virtual ~DataType() {}
  virtual DataTypeId GetId() const = 0;
  virtual std::string GetName() const = 0;
  virtual std::unique_ptr<DataType> Clone() const = 0;
  virtual void Accept(DataTypeVisitor &visitor) const = 0;
  friend bool operator==(const DataType &lhs, const DataType &rhs);
  friend bool operator!=(const DataType &lhs, const DataType &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const DataType &data_type);

 private:
  virtual bool IsEqual(const DataType &rhs) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}
#endif
