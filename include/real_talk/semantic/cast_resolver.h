
#ifndef _REAL_TALK_SEMANTIC_CAST_RESOLVER_H_
#define _REAL_TALK_SEMANTIC_CAST_RESOLVER_H_

namespace real_talk {
namespace semantic {

class DataType;

class CastResolver {
 public:
  bool CanCastTo(const DataType &dest_data_type,
                 const DataType &src_data_type) const;
};
}
}
#endif
