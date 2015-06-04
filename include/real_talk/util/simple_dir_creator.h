
#ifndef _REAL_TALK_UTIL_SIMPLE_DIR_CREATOR_H_
#define _REAL_TALK_UTIL_SIMPLE_DIR_CREATOR_H_

#include "real_talk/util/dir_creator.h"

namespace real_talk {
namespace util {

class SimpleDirCreator: public DirCreator {
 public:
  virtual void Create(const boost::filesystem::path &dir_path) const override;
};
}
}
#endif
