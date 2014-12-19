
#include "real_talk/semantic/simple_import_file_searcher.h"

using boost::filesystem::path;
using boost::filesystem::canonical;

namespace real_talk {
namespace semantic {

path SimpleImportFileSearcher::Search(const path &relative_file_path) const {
  return canonical(relative_file_path);
}
}
}
