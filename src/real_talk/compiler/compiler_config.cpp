
#include <vector>
#include "real_talk/compiler/compiler_config.h"

using std::vector;
using boost::filesystem::path;

namespace real_talk {
namespace compiler {

CompilerConfig::CompilerConfig(const path &input_file_path)
    : input_file_path_(input_file_path),
      src_dir_path_("src"),
      bin_dir_path_("bin"),
      vendor_dir_path_("vendor"),
      module_file_extension_("rtm") {}

void CompilerConfig::SetSrcDirPath(const path &path) {
  src_dir_path_ = path;
}

void CompilerConfig::SetBinDirPath(const path &path) {
  bin_dir_path_ = path;
}

void CompilerConfig::SetVendorDirPath(const path &path) {
  vendor_dir_path_ = path;
}

void CompilerConfig::SetModuleFileExtension(const path &extension) {
  module_file_extension_ = extension;
}

void CompilerConfig::SetImportDirPaths(const std::vector<path> &paths) {
  import_dir_paths_ = paths;
}

const path &CompilerConfig::GetInputFilePath() const {
  return input_file_path_;
}

const path &CompilerConfig::GetSrcDirPath() const {
  return src_dir_path_;
}

const path &CompilerConfig::GetBinDirPath() const {
  return bin_dir_path_;
}

const path &CompilerConfig::GetVendorDirPath() const {
  return vendor_dir_path_;
}

const path &CompilerConfig::GetModuleFileExtension() const {
  return module_file_extension_;
}

const vector<path> &CompilerConfig::GetImportDirPaths() const {
  return import_dir_paths_;
}
}
}
