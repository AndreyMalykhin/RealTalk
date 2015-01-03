
#include <boost/format.hpp>
#include "real_talk/util/errors.h"
#include "real_talk/semantic/simple_import_file_searcher.h"

using boost::format;
using boost::filesystem::path;
using boost::filesystem::canonical;
using boost::filesystem::exists;
using boost::filesystem::filesystem_error;
using real_talk::util::IOError;
using real_talk::util::FileNotFoundError;

namespace real_talk {
namespace semantic {

path SimpleImportFileSearcher::Search(const path &relative_file_path) const {
  try {
    if (!exists(relative_file_path)) {
      throw FileNotFoundError(
          (format("File not found: %1%") % relative_file_path.string()).str());
    }

    return canonical(relative_file_path);
  } catch (const filesystem_error &e) {
    throw IOError(e.what());
  }
}
}
}
