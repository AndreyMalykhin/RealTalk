
#ifndef _REAL_TALK_CODE_ID_ADDRESS_H_
#define _REAL_TALK_CODE_ID_ADDRESS_H_

#include <cstdint>
#include <string>
#include <iostream>

namespace real_talk {
namespace code {

class IdAddress {
 public:
  IdAddress(const std::string &id, uint32_t address);
  const std::string &GetId() const;
  uint32_t GetAddress() const;
  friend bool operator==(const IdAddress &lhs, const IdAddress &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const IdAddress &id_address);

 private:
  std::string id_;
  uint32_t address_;
};
}
}
#endif
