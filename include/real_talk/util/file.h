
#ifndef _REAL_TALK_UTIL_FILE_H_
#define _REAL_TALK_UTIL_FILE_H_

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
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
  virtual void Open(const boost::filesystem::path &file_path) = 0;

  /**
   * @throws real_talk::util::IOError
   */
  virtual void Write(const real_talk::code::Code &code) = 0;

  /**
   * @throws real_talk::util::IOError
   */
  virtual std::unique_ptr<std::istream> Read() = 0;

  /**
   * @throws real_talk::util::IOError
   */
  virtual void Close() = 0;

 private:
  boost::filesystem::fstream stream_;
};
}
}
#endif
