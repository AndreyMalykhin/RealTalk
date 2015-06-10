
#include <boost/format.hpp>
#include <vector>
#include "real_talk/util/errors.h"
#include "real_talk/util/simple_file_searcher.h"

using std::vector;
using boost::format;
using boost::filesystem::path;
using boost::filesystem::directory_iterator;
using boost::filesystem::is_directory;
using boost::filesystem::canonical;
using boost::filesystem::exists;
using boost::filesystem::filesystem_error;

namespace real_talk {
namespace util {

path SimpleFileSearcher::Search(
    const path &file_path,
    const path &main_dir_path,
    const path &vendor_dir_path,
    const vector<path> &project_dir_paths) const {
  try {
    const path search_file_path(main_dir_path / file_path);

    if (exists(search_file_path)) {
      return canonical(search_file_path);
    }

    if (exists(vendor_dir_path)) {
      const directory_iterator end_dir_it;

      for (directory_iterator vendor_dir_it(vendor_dir_path);
           vendor_dir_it != end_dir_it;
           ++vendor_dir_it) {
        if (!is_directory(vendor_dir_it->status())) {
          continue;
        }

        for (directory_iterator vendor_lib_dir_it(vendor_dir_it->path());
             vendor_lib_dir_it != end_dir_it;
             ++vendor_lib_dir_it) {
          const path vendor_search_file_path(
              vendor_lib_dir_it->path() / search_file_path);

          if (exists(vendor_search_file_path)) {
            return canonical(vendor_search_file_path);
          }
        }
      }
    }

    for (const path &project_dir_path: project_dir_paths) {
      const path project_search_file_path(project_dir_path / search_file_path);

      if (exists(project_search_file_path)) {
        return canonical(project_search_file_path);
      }
    }

    return path();
  } catch (const filesystem_error &error) {
    throw IOError(error.what());
  }
}
}
}
