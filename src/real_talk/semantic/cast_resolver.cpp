
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <cassert>
#include <utility>
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/cast_resolver.h"

using std::unordered_map;
using std::make_pair;
using std::pair;
using boost::hash;

namespace real_talk {
namespace semantic {
namespace {

typedef pair<DataTypeId, DataTypeId> CastableDataTypesKey;
typedef unordered_map< CastableDataTypesKey,
                       bool,
                       hash<CastableDataTypesKey> > CastableDataTypes;

const CastableDataTypes &kCastableDataTypes = *new CastableDataTypes({
    {make_pair(DataTypeId::kInt, DataTypeId::kChar), true},
    {make_pair(DataTypeId::kLong, DataTypeId::kChar), true},
    {make_pair(DataTypeId::kString, DataTypeId::kChar), true},
    {make_pair(DataTypeId::kLong, DataTypeId::kInt), true},
    {make_pair(DataTypeId::kDouble, DataTypeId::kInt), true},
    {make_pair(DataTypeId::kDouble, DataTypeId::kLong), true}
  });
}

bool CastResolver::CanCastTo(const DataType &dest_data_type,
                             const DataType &src_data_type) const {
  assert(dest_data_type != src_data_type);
  CastableDataTypes::const_iterator castable_data_types_it =
      kCastableDataTypes.find(
          make_pair(dest_data_type.GetId(), src_data_type.GetId()));
  return castable_data_types_it != kCastableDataTypes.cend()
      && castable_data_types_it->second;
}
}
}
