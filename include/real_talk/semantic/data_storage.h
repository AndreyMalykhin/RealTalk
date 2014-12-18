
#ifndef _REAL_TALK_SEMANTIC_DATA_STORAGE_H_
#define _REAL_TALK_SEMANTIC_DATA_STORAGE_H_

#include <iostream>

namespace real_talk {
namespace semantic {

class DataStorage {
 public:
  virtual ~DataStorage() {}
  friend bool operator==(const DataStorage &lhs, const DataStorage &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const DataStorage &storage);

 private:
  virtual bool IsEqual(const DataStorage &rhs) const = 0;
  virtual void Print(std::ostream &stream) const = 0;
};
}
}
#endif
