
#include <vector>
#include "real_talk/linker/linker_config.h"

using std::vector;
using std::ostream;
using boost::filesystem::path;

namespace real_talk {
namespace linker {

LinkerConfig::LinkerConfig()
    : output_file_type_(OutputFileType::kExe),
      bin_dir_path_("build/bin"),
      vendor_dir_path_("vendor"),
      is_debug_(false) {}

void LinkerConfig::SetDebug(bool is_debug) {
  is_debug_ = is_debug;
}

void LinkerConfig::SetInputFilePaths(const vector<path> &paths) {
  input_file_paths_ = paths;
}

void LinkerConfig::SetOutputFilePath(const boost::filesystem::path &path) {
  output_file_path_ = path;
}

void LinkerConfig::SetOutputFileType(OutputFileType type) {
  output_file_type_ = type;
}

void LinkerConfig::SetBinDirPath(const path &path) {
  bin_dir_path_ = path;
}

void LinkerConfig::SetVendorDirPath(const path &path) {
  vendor_dir_path_ = path;
}

void LinkerConfig::SetImportDirPaths(const std::vector<path> &paths) {
  import_dir_paths_ = paths;
}

const path &LinkerConfig::GetOutputFilePath() const {
  return output_file_path_;
}

OutputFileType LinkerConfig::GetOutputFileType() const {
  return output_file_type_;
}

const vector<path> &LinkerConfig::GetInputFilePaths() const {
  return input_file_paths_;
}

const path &LinkerConfig::GetBinDirPath() const {
  return bin_dir_path_;
}

const path &LinkerConfig::GetVendorDirPath() const {
  return vendor_dir_path_;
}

const vector<path> &LinkerConfig::GetImportDirPaths() const {
  return import_dir_paths_;
}

bool LinkerConfig::IsDebug() const {
  return is_debug_;
}

bool operator==(const LinkerConfig &lhs, const LinkerConfig &rhs) {
  return lhs.input_file_paths_ == rhs.input_file_paths_
      && lhs.output_file_path_ == rhs.output_file_path_
      && lhs.output_file_type_ == rhs.output_file_type_
      && lhs.bin_dir_path_ == rhs.bin_dir_path_
      && lhs.vendor_dir_path_ == rhs.vendor_dir_path_
      && lhs.import_dir_paths_ == rhs.import_dir_paths_
      && lhs.is_debug_ == rhs.is_debug_;
}

ostream &operator<<(ostream &stream, const LinkerConfig &config) {
  stream << "debug=" << config.is_debug_
         << "\noutput_file=" << config.output_file_path_
         << "\noutput_file_type="
         << static_cast<uint32_t>(config.output_file_type_)
         << "\nbin_dir=" << config.bin_dir_path_
         << "\nvendor_dir=" << config.vendor_dir_path_
         << "\nimport_dirs=\n";

  for (const path &import_dir_path: config.import_dir_paths_) {
    stream << import_dir_path << '\n';
  }

  stream << "input_files=\n";

  for (const path &input_file_path: config.input_file_paths_) {
    stream << input_file_path << '\n';
  }

  return stream;
}
}
}
