
#include <boost/functional/hash.hpp>
#include <cassert>
#include "real_talk/semantic/cast_handler.h"

using std::unordered_map;
using std::make_pair;
using std::pair;
using boost::hash_combine;

namespace real_talk {
namespace semantic {

const CastHandler::Handlers &CastHandler::kHandlers = *new Handlers({
    {{DataTypeId::kChar, DataTypeId::kInt, Direction::kLeftToRight},
          &CastHandler::HandleCharToInt},
    {{DataTypeId::kChar, DataTypeId::kLong, Direction::kLeftToRight},
          &CastHandler::HandleCharToLong},
    {{DataTypeId::kChar, DataTypeId::kString, Direction::kLeftToRight},
          &CastHandler::HandleCharToString},
    {{DataTypeId::kInt, DataTypeId::kLong, Direction::kLeftToRight},
          &CastHandler::HandleIntToLong},
    {{DataTypeId::kInt, DataTypeId::kDouble, Direction::kLeftToRight},
          &CastHandler::HandleIntToDouble},
    {{DataTypeId::kLong, DataTypeId::kDouble, Direction::kLeftToRight},
          &CastHandler::HandleLongToDouble},

    {{DataTypeId::kInt, DataTypeId::kChar, Direction::kRightToLeft},
          &CastHandler::HandleCharToInt},
    {{DataTypeId::kLong, DataTypeId::kChar, Direction::kRightToLeft},
          &CastHandler::HandleCharToLong},
    {{DataTypeId::kString, DataTypeId::kChar, Direction::kRightToLeft},
          &CastHandler::HandleCharToString},
    {{DataTypeId::kLong, DataTypeId::kInt, Direction::kRightToLeft},
          &CastHandler::HandleIntToLong},
    {{DataTypeId::kDouble, DataTypeId::kInt, Direction::kRightToLeft},
          &CastHandler::HandleIntToDouble},
    {{DataTypeId::kDouble, DataTypeId::kLong, Direction::kRightToLeft},
          &CastHandler::HandleLongToDouble}
  });

void CastHandler::Handle(const DataType &left_data_type,
                         const DataType &right_data_type,
                         Direction direction) const {
  assert(left_data_type != right_data_type);
  Handlers::const_iterator handlers_it = kHandlers.find(
      {left_data_type.GetId(), right_data_type.GetId(), direction});

  if (handlers_it == kHandlers.cend()) {
    HandleFail();
    return;
  }

  const Handler handler = handlers_it->second;
  (this->*handler)();
}

bool operator==(const CastHandler::HandlersKey &lhs,
                const CastHandler::HandlersKey &rhs) {
  return lhs.left_data_type == rhs.left_data_type
      && lhs.right_data_type == rhs.right_data_type
      && lhs.direction == rhs.direction;
}

size_t CastHandler::HandlersKeyHasher::operator()(
    const CastHandler::HandlersKey &key) const {
  size_t hash = 0;
  hash_combine(hash, key.left_data_type);
  hash_combine(hash, key.right_data_type);
  hash_combine(hash, key.direction);
  return hash;
}
}
}
