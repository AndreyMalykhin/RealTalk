
#include <string>
#include "real_talk/code/id_address.h"

using std::string;
using std::ostream;

namespace real_talk {
namespace code {

IdAddress::IdAddress(const string &id, uint32_t address)
    : id_(id), address_(address) {
}

const string &IdAddress::GetId() const {
  return id_;
}

uint32_t IdAddress::GetAddress() const {
  return address_;
}

ostream &operator<<(ostream &stream, const IdAddress &id_address) {
  return stream << "id=" << id_address.id_ << "; address="
                << id_address.address_;
}

bool operator==(const IdAddress &lhs, const IdAddress &rhs) {
  return lhs.id_ == rhs.id_ && lhs.address_ == rhs.address_;
}
}
}
