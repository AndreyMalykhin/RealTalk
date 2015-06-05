
#ifndef _REAL_TALK_UTIL_FILE_H_
#define _REAL_TALK_UTIL_FILE_H_

#include <boost/filesystem.hpp>
#include <memory>
#include <iostream>

namespace real_talk {
namespace code {

class Code;
}

namespace util {

class File {
 public:
  virtual ~File() {}

  /**
   * @throws real_talk::util::IOError
   */
  virtual void Write(const boost::filesystem::path &file_path,
                     const real_talk::code::Code &code) const = 0;

  /**
   * @throws real_talk::util::IOError
   */
  virtual std::unique_ptr<std::istream> Read(
      const boost::filesystem::path &file_path) const = 0;
};
}
}
#endif
