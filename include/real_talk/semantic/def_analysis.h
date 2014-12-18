
#ifndef _REAL_TALK_SEMANTIC_DEF_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_DEF_ANALYSIS_H_

#include <iostream>

namespace real_talk {
namespace semantic {

class DataType;

class DefAnalysis {
 public:
  virtual ~DefAnalysis() {}
  virtual const DataType &GetDataType() const = 0;
  friend bool operator==(const DefAnalysis &lhs,
                         const DefAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const DefAnalysis &analysis);

 private:
  virtual bool IsEqual(const DefAnalysis &rhs) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}
#endif
