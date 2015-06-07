
#ifndef _REAL_TALK_COMPILER_COMPILER_CONFIG_H_
#define _REAL_TALK_COMPILER_COMPILER_CONFIG_H_

#include <boost/filesystem.hpp>
#include <vector>

namespace real_talk {
namespace compiler {

class CompilerConfig {
 public:
  CompilerConfig();
  void SetInputFilePath(const boost::filesystem::path &path);
  void SetSrcDirPath(const boost::filesystem::path &path);
  void SetBinDirPath(const boost::filesystem::path &path);
  void SetVendorDirPath(const boost::filesystem::path &path);
  void SetModuleFileExtension(const boost::filesystem::path &path);
  void SetImportDirPaths(const std::vector<boost::filesystem::path> &paths);
  const boost::filesystem::path &GetInputFilePath() const;
  const boost::filesystem::path &GetSrcDirPath() const;
  const boost::filesystem::path &GetBinDirPath() const;
  const boost::filesystem::path &GetVendorDirPath() const;
  const boost::filesystem::path &GetModuleFileExtension() const;
  const std::vector<boost::filesystem::path> &GetImportDirPaths() const;
  friend bool operator==(const CompilerConfig &lhs, const CompilerConfig &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const CompilerConfig &config);

 private:
  boost::filesystem::path input_file_path_;
  boost::filesystem::path src_dir_path_;
  boost::filesystem::path bin_dir_path_;
  boost::filesystem::path vendor_dir_path_;
  boost::filesystem::path module_file_extension_;
  std::vector<boost::filesystem::path> import_dir_paths_;
};
}
}
#endif
