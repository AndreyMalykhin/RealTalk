
#include <vector>
#include "real_talk/compiler/compiler_config.h"

using std::vector;
using std::ostream;
using boost::filesystem::path;

namespace real_talk {
namespace compiler {

CompilerConfig::CompilerConfig()
    : src_dir_path_("src"),
      bin_dir_path_("bin"),
      vendor_dir_path_("vendor"),
      module_file_extension_("rtm") {}

void CompilerConfig::SetInputFilePath(const path &path) {
  input_file_path_ = path;
}

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

bool operator==(const CompilerConfig &lhs, const CompilerConfig &rhs) {
  return lhs.input_file_path_ == rhs.input_file_path_
      && lhs.src_dir_path_ == rhs.src_dir_path_
      && lhs.bin_dir_path_ == rhs.bin_dir_path_
      && lhs.vendor_dir_path_ == rhs.vendor_dir_path_
      && lhs.module_file_extension_ == rhs.module_file_extension_
      && lhs.import_dir_paths_ == rhs.import_dir_paths_;
}

ostream &operator<<(ostream &stream, const CompilerConfig &config) {
  stream << "input_file=" << config.input_file_path_ << "; src_dir="
         << config.src_dir_path_ << "; bin_dir=" << config.bin_dir_path_
         << "; vendor_dir=" << config.vendor_dir_path_
         << "; module_file_extension=" << config.module_file_extension_
         << "; import_dirs=\n";

  for (const path &import_dir_path: config.import_dir_paths_) {
    stream << import_dir_path << '\n';
  }

  return stream;
}
}
}
