
#ifndef _REAL_TALK_SEMANTIC_LOCAL_DATA_STORAGE_H_
#define _REAL_TALK_SEMANTIC_LOCAL_DATA_STORAGE_H_

#include <iostream>
#include "real_talk/semantic/data_storage.h"

namespace real_talk {
namespace semantic {

class LocalDataStorage: public DataStorage {
 private:
  virtual bool IsEqual(const DataStorage &rhs) const override;
  virtual void Print(std::ostream &stream) const override;
};
}
}
#endif
