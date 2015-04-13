
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <utility>
#include "real_talk/semantic/data_type.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/simple_cast_cmd_generator.h"

using std::unordered_map;
using std::make_pair;
using std::pair;
using boost::hash;
using real_talk::semantic::DataType;
using real_talk::semantic::DataTypeId;

namespace real_talk {
namespace code {
namespace {

typedef pair<DataTypeId, DataTypeId> CmdsKey;
typedef unordered_map< CmdsKey, CmdId, hash<CmdsKey> > Cmds;
const Cmds &kCmds = *new Cmds({
    {make_pair(DataTypeId::kInt, DataTypeId::kChar),
          CmdId::kCastCharToInt},
    {make_pair(DataTypeId::kLong, DataTypeId::kChar),
          CmdId::kCastCharToLong},
    {make_pair(DataTypeId::kString, DataTypeId::kChar),
          CmdId::kCastCharToString},
    {make_pair(DataTypeId::kLong, DataTypeId::kInt),
          CmdId::kCastIntToLong},
    {make_pair(DataTypeId::kDouble, DataTypeId::kInt),
          CmdId::kCastIntToDouble},
    {make_pair(DataTypeId::kDouble, DataTypeId::kLong),
          CmdId::kCastLongToDouble}
  });
}

CmdId SimpleCastCmdGenerator::Generate(const DataType &dest_data_type,
                                       const DataType &src_data_type) const {
  Cmds::const_iterator cmds_it = kCmds.find(
      make_pair(dest_data_type.GetId(), src_data_type.GetId()));
  assert(cmds_it != kCmds.cend());
  return cmds_it->second;
}
}
}
