
#ifndef _REAL_TALK_VM_VM_CONFIG_H_
#define _REAL_TALK_VM_VM_CONFIG_H_

#include <boost/filesystem.hpp>

namespace real_talk {
namespace vm {

class VMConfig {
 public:
  void SetInputFilePath(const boost::filesystem::path &file_path);
  const boost::filesystem::path &GetInputFilePath() const noexcept;
  void SetDebug(bool is_debug) noexcept;
  bool IsDebug() const noexcept;
  friend bool operator==(const VMConfig &lhs, const VMConfig &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, const VMConfig &config);

 private:
  boost::filesystem::path input_file_path_;
  bool is_debug_ = false;
};
}
}
#endif
