
#ifndef _REAL_TALK_VM_VM_CONFIG_H_
#define _REAL_TALK_VM_VM_CONFIG_H_

#include <boost/filesystem.hpp>

namespace real_talk {
namespace vm {

class VMConfig {
 public:
  void SetInputFilePath(const boost::filesystem::path &file_path);
  const boost::filesystem::path &GetInputFilePath() const;

 private:
  boost::filesystem::path input_file_path_;
};
}
}
#endif
