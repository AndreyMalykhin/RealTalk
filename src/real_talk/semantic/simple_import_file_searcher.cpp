
#include <boost/format.hpp>
#include <vector>
#include "real_talk/util/errors.h"
#include "real_talk/semantic/simple_import_file_searcher.h"

using std::vector;
using boost::format;
using boost::filesystem::path;
using boost::filesystem::canonical;
using boost::filesystem::exists;
using boost::filesystem::filesystem_error;
using real_talk::util::IOError;
using real_talk::util::FileNotFoundError;

namespace real_talk {
namespace semantic {

SimpleImportFileSearcher::SimpleImportFileSearcher(const vector<path> &dirs)
    : dirs_(dirs) {
  assert(!dirs_.empty());
}

path SimpleImportFileSearcher::Search(const path &relative_file_path) const {
  try {
    for (const path &dir: dirs_) {
      const path search_file_path(dir / relative_file_path);

      if (!exists(search_file_path)) {
        continue;
      }

      return canonical(search_file_path);
    }
  } catch (const filesystem_error &e) {
    throw IOError(e.what());
  }

  throw FileNotFoundError(
      (format("File not found: %1%") % relative_file_path.string()).str());
}
}
}
