
#ifndef _REAL_TALK_SEMANTIC_FUNC_DATA_TYPE_H_
#define _REAL_TALK_SEMANTIC_FUNC_DATA_TYPE_H_

#include <vector>
#include <string>
#include "real_talk/semantic/data_type.h"

namespace real_talk {
namespace semantic {

class FuncDataType: public DataType {
 public:
  FuncDataType(std::unique_ptr<DataType> return_data_type,
               std::vector< std::unique_ptr<DataType> > arg_data_types,
               bool is_native);
  virtual DataTypeId GetId() const override;
  virtual std::string GetName() const override;
  virtual std::unique_ptr<DataType> Clone() const override;
  virtual void Accept(DataTypeVisitor &visitor) const override;
  const DataType &GetReturnDataType() const;
  const std::vector< std::unique_ptr<DataType> > &GetArgDataTypes() const;
  bool IsNative() const;

 private:
  virtual bool IsEqual(const DataType &rhs) const override;
  virtual void Print(std::ostream &stream) const override;

  std::unique_ptr<DataType> return_data_type_;
  std::vector< std::unique_ptr<DataType> > arg_data_types_;
  bool is_native_;
};
}
}
#endif
