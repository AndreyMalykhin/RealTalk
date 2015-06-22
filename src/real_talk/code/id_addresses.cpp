
#include <vector>
#include <string>
#include "real_talk/code/id_addresses.h"

using std::vector;
using std::string;
using std::ostream;

namespace real_talk {
namespace code {

IdAddresses::IdAddresses(const string &id, const vector<uint32_t> &addresses)
    : id_(id), addresses_(addresses) {
}

const string &IdAddresses::GetId() const {
  return id_;
}

const vector<uint32_t> &IdAddresses::GetAddresses() const {
  return addresses_;
}

vector<uint32_t> &IdAddresses::GetAddresses() {
  return addresses_;
}

ostream &operator<<(ostream &stream, const IdAddresses &id_addresses) {
  stream << "id=" << id_addresses.id_ << "; addresses=";

  for (const uint32_t address: id_addresses.addresses_) {
    stream << address << ", ";
  }

  return stream;
}

bool operator==(const IdAddresses &lhs, const IdAddresses &rhs) {
  return lhs.id_ == rhs.id_ && lhs.addresses_ == rhs.addresses_;
}
}
}
