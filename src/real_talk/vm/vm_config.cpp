
#include "real_talk/vm/vm_config.h"

using std::ostream;
using boost::filesystem::path;

namespace real_talk {
namespace vm {

void VMConfig::SetInputFilePath(const path &file_path) {
  input_file_path_ = file_path;
}

const path &VMConfig::GetInputFilePath() const noexcept {
  return input_file_path_;
}

void VMConfig::SetDebug(bool is_debug) noexcept {
  is_debug_ = is_debug;
}

bool VMConfig::IsDebug() const noexcept {
  return is_debug_;
}

bool operator==(const VMConfig &lhs, const VMConfig &rhs) {
  return lhs.input_file_path_ == rhs.input_file_path_
      && lhs.is_debug_ == rhs.is_debug_;
}

ostream &operator<<(ostream &stream, const VMConfig &config) {
  return stream << "debug=" << config.is_debug_
                << "\ninput_file=" << config.input_file_path_;
}
}
}
