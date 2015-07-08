
#include <string>
#include "real_talk/code/data_type_size.h"
#include "real_talk/code/id_size.h"

using std::ostream;
using std::string;

namespace real_talk {
namespace code {

IdSize::IdSize(const string &id, DataTypeSize size): id_(id), size_(size) {}

const string &IdSize::GetId() const {return id_;}

DataTypeSize IdSize::GetSize() const {return size_;}

bool operator==(const IdSize &lhs, const IdSize &rhs) {
  return lhs.id_ == rhs.id_ && lhs.size_ == rhs.size_;
}

ostream &operator<<(ostream &stream, const IdSize &id_size) {
  return stream << "id=" << id_size.id_
                << "; size=" << static_cast<uint32_t>(id_size.size_);
}
}
}
