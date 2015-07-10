
#include "real_talk/vm/vm_config.h"

using boost::filesystem::path;

namespace real_talk {
namespace vm {

void VMConfig::SetInputFilePath(const path &file_path) {
  input_file_path_ = file_path;
}

const path &VMConfig::GetInputFilePath() const {
  return input_file_path_;
}
}
}
