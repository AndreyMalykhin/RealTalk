
#ifndef _REAL_TALK_UTIL_FILE_SEARCHER_H_
#define _REAL_TALK_UTIL_FILE_SEARCHER_H_

#include <boost/filesystem.hpp>
#include <vector>

namespace real_talk {
namespace util {

class FileSearcher {
 public:
  virtual ~FileSearcher() {}

  /**
   * @throws real_talk::util::IOError
   * @return Empty path if file not found
   */
  virtual boost::filesystem::path Search(
      const boost::filesystem::path &file_path,
      const boost::filesystem::path &main_dir_path,
      const boost::filesystem::path &vendor_dir_path,
      const std::vector<boost::filesystem::path> &project_dir_paths) const = 0;
};
}
}
#endif
