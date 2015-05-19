
#ifndef _REAL_TALK_COMPILER_SIMPLE_IMPORT_FILE_SEARCHER_H_
#define _REAL_TALK_COMPILER_SIMPLE_IMPORT_FILE_SEARCHER_H_

#include <vector>
#include "real_talk/compiler/import_file_searcher.h"

namespace real_talk {
namespace compiler {

class SimpleImportFileSearcher: public ImportFileSearcher {
 public:
  virtual boost::filesystem::path Search(
      const boost::filesystem::path &file_path,
      const boost::filesystem::path &src_dir_path,
      const boost::filesystem::path &vendor_dir_path,
      const std::vector<boost::filesystem::path> &import_dir_paths)
      const override;
};
}
}
#endif
