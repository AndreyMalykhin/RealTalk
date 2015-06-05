
#include <boost/filesystem/fstream.hpp>
#include "real_talk/util/simple_file.h"

using std::ios;
using std::istream;
using std::unique_ptr;
using boost::filesystem::path;
using boost::filesystem::ifstream;
using real_talk::code::Code;

namespace real_talk {
namespace util {

void SimpleFile::Write(const path &file_path, const Code &code) const {
  assert(false);
}

unique_ptr<istream> SimpleFile::Read(const path &file_path) const {
  auto *fstream = new ifstream();
  unique_ptr<istream> stream(fstream);
  fstream->exceptions(ios::failbit | ios::badbit);
  fstream->open(file_path, ios::binary);
  return stream;
}
}
}
