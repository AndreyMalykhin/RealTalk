
#ifndef _REAL_TALK_LINKER_LINKER_CONFIG_H_
#define _REAL_TALK_LINKER_LINKER_CONFIG_H_

#include <boost/filesystem.hpp>
#include <vector>
#include "real_talk/linker/output_file_type.h"

namespace real_talk {
namespace linker {

class LinkerConfig {
 public:
  LinkerConfig();
  void SetInputFilePaths(const std::vector<boost::filesystem::path> &paths);
  void SetOutputFilePath(const boost::filesystem::path &path);
  void SetOutputFileType(OutputFileType type);
  void SetBinDirPath(const boost::filesystem::path &path);
  void SetVendorDirPath(const boost::filesystem::path &path);
  void SetImportDirPaths(const std::vector<boost::filesystem::path> &paths);
  void SetDebug(bool is_debug);
  const std::vector<boost::filesystem::path> &GetInputFilePaths() const;
  const boost::filesystem::path &GetOutputFilePath() const;
  OutputFileType GetOutputFileType() const;
  const boost::filesystem::path &GetBinDirPath() const;
  const boost::filesystem::path &GetVendorDirPath() const;
  const std::vector<boost::filesystem::path> &GetImportDirPaths() const;
  bool IsDebug() const;
  friend bool operator==(const LinkerConfig &lhs, const LinkerConfig &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const LinkerConfig &config);

 private:
  std::vector<boost::filesystem::path> input_file_paths_;
  boost::filesystem::path output_file_path_;
  OutputFileType output_file_type_;
  boost::filesystem::path bin_dir_path_;
  boost::filesystem::path vendor_dir_path_;
  std::vector<boost::filesystem::path> import_dir_paths_;
  bool is_debug_;
};
}
}
#endif
