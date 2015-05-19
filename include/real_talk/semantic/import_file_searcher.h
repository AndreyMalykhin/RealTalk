
#ifndef _REAL_TALK_SEMANTIC_IMPORT_FILE_SEARCHER_H_
#define _REAL_TALK_SEMANTIC_IMPORT_FILE_SEARCHER_H_

#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <stdexcept>

namespace real_talk {
namespace semantic {

class ImportFileSearcher {
 public:
  virtual ~ImportFileSearcher() {}

  /**
   * @throws real_talk::util::FileNotFoundError
   * @throws real_talk::util::IOError
   */
  virtual boost::filesystem::path Search(
      const boost::filesystem::path &file_path,
      const boost::filesystem::path &src_dir_path,
      const boost::filesystem::path &vendor_dir_path,
      const std::vector<boost::filesystem::path> &import_dir_paths) const = 0;
};
}
}
#endif
