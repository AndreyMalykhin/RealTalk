
#ifndef _REAL_TALK_UTIL_SIMPLE_FILE_H_
#define _REAL_TALK_UTIL_SIMPLE_FILE_H_

#include "real_talk/util/file.h"

namespace real_talk {
namespace util {

class SimpleFile: public File {
 public:
  virtual void Write(const boost::filesystem::path &file_path,
                     const real_talk::code::Code &code) const override;
  virtual std::unique_ptr<std::istream> Read(
      const boost::filesystem::path &file_path) const override;
};
}
}
#endif
