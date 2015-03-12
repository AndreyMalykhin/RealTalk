
#ifndef _REAL_TALK_TEST_CONFIG_H_
#define _REAL_TALK_TEST_CONFIG_H_

#include <boost/filesystem.hpp>

namespace real_talk {

class TestConfig {
 public:
  static const boost::filesystem::path &GetResourceDir() {
    static const boost::filesystem::path &kResourceDir =
        *(new boost::filesystem::path("test_resources"));
    return kResourceDir;
  }
};
}
#endif
