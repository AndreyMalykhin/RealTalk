
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <limits>
#include <sstream>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/util/errors.h"
#include "real_talk/util/simple_file.h"

using std::string;
using std::ios;
using std::istream;
using std::istringstream;
using std::streampos;
using std::streamsize;
using std::unique_ptr;
using std::numeric_limits;
using boost::filesystem::path;
using boost::filesystem::ifstream;
using boost::filesystem::ofstream;
using boost::format;
using real_talk::code::Code;

namespace real_talk {
namespace util {

void SimpleFile::Write(const path &file_path, const Code &code) const {
  try {
    ofstream stream;
    stream.exceptions(ios::failbit | ios::badbit);
    stream.open(file_path, ios::binary | ios::trunc);

    if (code.GetSize() > numeric_limits<streamsize>::max()) {
      const string msg = (format("Code size exceeds max writable size; max=%1%")
                          % numeric_limits<streamsize>::max()).str();
      throw IOError(msg);
    }

    stream.write(reinterpret_cast<const char*>(code.GetData()), code.GetSize());
  } catch (const ios::failure &e) {
    throw IOError(e.what());
  }
}

unique_ptr<istream> SimpleFile::Read(const path &file_path) const {
  try {
    ifstream file_stream;
    file_stream.exceptions(ios::failbit | ios::badbit);
    file_stream.open(file_path, ios::binary);
    file_stream.seekg(0, ios::end);
    streampos content_size = file_stream.tellg();
    assert(content_size >= 0);
    file_stream.seekg(0, ios::beg);
    string content(static_cast<size_t>(content_size), ' ');
    file_stream.read(&content[0], content_size);
    unique_ptr<istream> string_stream(new istringstream(content));
    string_stream->exceptions(ios::failbit | ios::badbit);
    return string_stream;
  } catch (const ios::failure &e) {
    throw IOError(e.what());
  }
}
}
}
