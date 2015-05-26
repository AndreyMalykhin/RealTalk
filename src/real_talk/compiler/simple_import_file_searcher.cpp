
#include <boost/format.hpp>
#include <vector>
#include "real_talk/util/errors.h"
#include "real_talk/compiler/simple_import_file_searcher.h"

using std::vector;
using boost::format;
using boost::filesystem::path;
using boost::filesystem::canonical;
using boost::filesystem::exists;
using boost::filesystem::filesystem_error;
using real_talk::util::IOError;

namespace real_talk {
namespace compiler {

path SimpleImportFileSearcher::Search(
    const path &file_path,
    const path &src_dir_path,
    const path &vendor_dir_path,
    const vector<path> &import_dir_paths) const {
  path search_file_path(src_dir_path / file_path);

  if (!exists(search_file_path)) {
    return path();
  }

  return canonical(search_file_path);
}
}
}
