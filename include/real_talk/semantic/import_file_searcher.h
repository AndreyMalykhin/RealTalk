
#ifndef _REAL_TALK_SEMANTIC_IMPORT_FILE_SEARCHER_H_
#define _REAL_TALK_SEMANTIC_IMPORT_FILE_SEARCHER_H_

#include <boost/filesystem.hpp>

namespace real_talk {
namespace semantic {

class ImportFileSearcher {
 public:
  virtual ~ImportFileSearcher() {}
  virtual boost::filesystem::path Search(
      const boost::filesystem::path &relative_path) const = 0;
};
}
}
#endif
