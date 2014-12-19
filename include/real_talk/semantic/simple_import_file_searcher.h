
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_IMPORT_FILE_SEARCHER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_IMPORT_FILE_SEARCHER_H_

#include "real_talk/semantic/import_file_searcher.h"

namespace real_talk {
namespace semantic {

class SimpleImportFileSearcher : public ImportFileSearcher {
 public:
  virtual boost::filesystem::path Search(
      const boost::filesystem::path &relative_file_path) const override;
};
}
}
#endif
