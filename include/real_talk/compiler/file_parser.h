
#ifndef _REAL_TALK_COMPILER_FILE_PARSER_H_
#define _REAL_TALK_COMPILER_FILE_PARSER_H_

#include <boost/filesystem.hpp>
#include <vector>
#include <memory>

namespace real_talk {
namespace parser {

class ProgramNode;
}

namespace compiler {

class FileParser {
 public:
  typedef
  std::vector< std::unique_ptr<real_talk::parser::ProgramNode> > ImportPrograms;

  class Programs {
   public:
    Programs(std::unique_ptr<real_talk::parser::ProgramNode> main,
             ImportPrograms import);
    const real_talk::parser::ProgramNode &GetMain() const;
    const ImportPrograms &GetImport() const;

   private:
    std::unique_ptr<real_talk::parser::ProgramNode> main_;
    ImportPrograms import_;
  };

  virtual ~FileParser() {}
  virtual std::unique_ptr<Programs> Parse(
      const boost::filesystem::path &file_path,
      const boost::filesystem::path &src_dir_path,
      const boost::filesystem::path &vendor_dir_path,
      const std::vector<boost::filesystem::path> &import_dir_paths) const = 0;
};
}
}
#endif
