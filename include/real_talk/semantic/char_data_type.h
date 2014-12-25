
#ifndef _REAL_TALK_SEMANTIC_CHAR_DATA_TYPE_H_
#define _REAL_TALK_SEMANTIC_CHAR_DATA_TYPE_H_

#include <string>
#include <memory>
#include "real_talk/semantic/data_type.h"

namespace real_talk {
namespace semantic {

class CharDataType: public DataType {
 public:
  virtual std::string GetName() const override;
  virtual std::unique_ptr<DataType> Clone() const override;
  virtual void Accept(DataTypeVisitor &visitor) const override;

 private:
  virtual bool IsEqual(const DataType &rhs) const override;
  virtual void Print(std::ostream &stream) const override;
};
}
}
#endif
