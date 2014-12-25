
#ifndef _REAL_TALK_SEMANTIC_DATA_TYPE_VISITOR_H_
#define _REAL_TALK_SEMANTIC_DATA_TYPE_VISITOR_H_

namespace real_talk {
namespace semantic {

class ArrayDataType;
class BoolDataType;
class FuncDataType;
class IntDataType;
class LongDataType;
class DoubleDataType;
class CharDataType;
class StringDataType;
class VoidDataType;

class DataTypeVisitor {
 public:
  virtual ~DataTypeVisitor() {}
  virtual void VisitArray(const ArrayDataType &data_type) = 0;
  virtual void VisitFunc(const FuncDataType &data_type) = 0;
  virtual void VisitBool(const BoolDataType &data_type) = 0;
  virtual void VisitInt(const IntDataType &data_type) = 0;
  virtual void VisitLong(const LongDataType &data_type) = 0;
  virtual void VisitDouble(const DoubleDataType &data_type) = 0;
  virtual void VisitChar(const CharDataType &data_type) = 0;
  virtual void VisitString(const StringDataType &data_type) = 0;
  virtual void VisitVoid(const VoidDataType &data_type) = 0;
};
}
}
#endif
