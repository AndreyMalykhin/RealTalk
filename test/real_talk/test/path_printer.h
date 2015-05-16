
#ifndef _REAL_TALK_TEST_PATH_PRINTER_H_
#define _REAL_TALK_TEST_PATH_PRINTER_H_

#include <boost/filesystem.hpp>
#include <iostream>

namespace boost {
namespace filesystem {

void PrintTo(const path &path, std::ostream *stream);
}
}
#endif
