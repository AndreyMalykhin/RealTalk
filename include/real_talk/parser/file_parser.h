
#ifndef _REAL_TALK_PARSER_FILE_PARSER_H_
#define _REAL_TALK_PARSER_FILE_PARSER_H_

#include <boost/filesystem.hpp>
#include <memory>

namespace real_talk {
namespace parser {

class ProgramNode;

class FileParser {
 public:
  virtual ~FileParser() {}
  virtual std::shared_ptr<ProgramNode> Parse(
      const boost::filesystem::path &file_path) const = 0;
};
}
}
#endif
