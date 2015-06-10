
#ifndef _REAL_TALK_UTIL_SIMPLE_FILE_SEARCHER_H_
#define _REAL_TALK_UTIL_SIMPLE_FILE_SEARCHER_H_

#include <vector>
#include "real_talk/util/file_searcher.h"

namespace real_talk {
namespace util {

class SimpleFileSearcher: public FileSearcher {
 public:
  virtual boost::filesystem::path Search(
      const boost::filesystem::path &file_path,
      const boost::filesystem::path &main_dir_path,
      const boost::filesystem::path &vendor_dir_path,
      const std::vector<boost::filesystem::path> &project_dir_paths)
      const override;
};
}
}
#endif
