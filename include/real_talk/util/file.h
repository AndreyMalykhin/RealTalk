
#ifndef _REAL_TALK_UTIL_FILE_H_
#define _REAL_TALK_UTIL_FILE_H_

#include <boost/filesystem.hpp>

namespace real_talk {
namespace code {

class Code;
}

namespace util {

class File {
 public:
  virtual ~File() {}
  virtual void Open(const boost::filesystem::path &file_path) = 0;
  virtual void Write(const real_talk::code::Code &code) = 0;
};
}
}
#endif
