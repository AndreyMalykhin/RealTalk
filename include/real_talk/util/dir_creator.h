
#ifndef _REAL_TALK_UTIL_DIR_CREATOR_H_
#define _REAL_TALK_UTIL_DIR_CREATOR_H_

#include <boost/filesystem.hpp>

namespace real_talk {
namespace util {

class DirCreator {
 public:
  virtual ~DirCreator() {}
  virtual void Create(const boost::filesystem::path &dir_path) const = 0;
};
}
}
#endif
