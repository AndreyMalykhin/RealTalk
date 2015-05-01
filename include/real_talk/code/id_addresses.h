
#ifndef _REAL_TALK_CODE_ID_ADDRESSES_H_
#define _REAL_TALK_CODE_ID_ADDRESSES_H_

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

namespace real_talk {
namespace code {

class IdAddresses {
 public:
  IdAddresses(const std::string &id, const std::vector<uint32_t> &addresses);
  const std::string &GetId() const;
  const std::vector<uint32_t> &GetAddresses() const;
  friend bool operator==(const IdAddresses &lhs, const IdAddresses &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const IdAddresses &id_addresses);

 private:
  std::string id_;
  std::vector<uint32_t> addresses_;
};
}
}
#endif
