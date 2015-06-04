
#include "real_talk/util/errors.h"
#include "real_talk/util/simple_dir_creator.h"

using boost::filesystem::create_directories;
using boost::filesystem::path;
using boost::filesystem::filesystem_error;

namespace real_talk {
namespace util {

void SimpleDirCreator::Create(const path &dir_path) const {
  try {
    create_directories(dir_path);
  } catch (const filesystem_error &e) {
    throw IOError(e.what());
  }
}
}
}
