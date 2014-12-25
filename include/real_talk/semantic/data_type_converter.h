
#ifndef _REAL_TALK_SEMANTIC_DATA_TYPE_CONVERTER_H_
#define _REAL_TALK_SEMANTIC_DATA_TYPE_CONVERTER_H_

namespace real_talk {
namespace semantic {

class DataType;

class DataTypeConverter {
 public:
  virtual ~DataTypeConverter() {}
  virtual bool IsConvertable(const DataType &dest,
                             const DataType &src) const = 0;
};
}
}
#endif
