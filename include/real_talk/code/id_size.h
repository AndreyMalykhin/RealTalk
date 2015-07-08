
#ifndef _REAL_TALK_CODE_ID_SIZE_H_
#define _REAL_TALK_CODE_ID_SIZE_H_

#include <cstdint>
#include <iostream>
#include <string>

namespace real_talk {
namespace code {

enum class DataTypeSize: uint8_t;

class IdSize {
 public:
  IdSize(const std::string &id, DataTypeSize size);
  const std::string &GetId() const;
  DataTypeSize GetSize() const;
  friend bool operator==(const IdSize &lhs, const IdSize &rhs);
  friend std::ostream &operator<<(std::ostream &stream, const IdSize &id_size);

 private:
  std::string id_;
  DataTypeSize size_;
};
}
}
#endif
