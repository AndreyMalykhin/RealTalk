
#ifndef _REAL_TALK_TEST_TEST_CONFIG_H_
#define _REAL_TALK_TEST_TEST_CONFIG_H_

#include <boost/filesystem.hpp>

namespace real_talk {
namespace test {

class TestConfig {
 public:
  static const boost::filesystem::path &GetFixturesDir() {
    static const boost::filesystem::path &kFixturesDir =
        *(new boost::filesystem::path("fixtures"));
    return kFixturesDir;
  }
};
}
}
#endif
