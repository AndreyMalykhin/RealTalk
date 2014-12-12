
#ifndef _REAL_TALK_SEMANTIC_DATA_TYPE_H_
#define _REAL_TALK_SEMANTIC_DATA_TYPE_H_

#include <string>
#include <memory>
#include <iostream>

namespace real_talk {
namespace semantic {

class DataType {
 public:
  virtual ~DataType() {}
  virtual std::string GetName() const = 0;
  virtual std::unique_ptr<DataType> Clone() const = 0;
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
