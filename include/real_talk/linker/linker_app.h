
#ifndef _REAL_TALK_LINKER_LINKER_APP_H_
#define _REAL_TALK_LINKER_LINKER_APP_H_

#include <functional>
#include <iostream>

namespace real_talk {
namespace util {

class FileSearcher;
class DirCreator;
class File;
}

namespace code {

class ModuleReader;
class Code;
class CodeContainerWriter;
}

namespace linker {

class LinkerConfigParser;
class MsgPrinter;
class LinkerFactory;
class LinkerConfig;

class LinkerApp {
 public:
  LinkerApp(const LinkerConfigParser &config_parser,
            const MsgPrinter &msg_printer,
            const real_talk::util::FileSearcher &file_searcher,
            const real_talk::code::ModuleReader &module_reader,
            const LinkerFactory &linker_factory,
            const real_talk::code::CodeContainerWriter &code_container_writer,
            const real_talk::util::DirCreator &dir_creator,
            const real_talk::util::File &file,
            LinkerConfig *config,
            real_talk::code::Code *output_code);
  void Run(int argc, const char *argv[]) const;

 private:
  typedef std::function<void (std::ostream *stream)> LogDataWriter;
  void Log(LogDataWriter data_provider) const;

  const LinkerConfigParser &config_parser_;
  const MsgPrinter &msg_printer_;
  const real_talk::util::FileSearcher &file_searcher_;
  const real_talk::code::ModuleReader &module_reader_;
  const LinkerFactory &linker_factory_;
  const real_talk::code::CodeContainerWriter &code_container_writer_;
  const real_talk::util::DirCreator &dir_creator_;
  const real_talk::util::File &file_;
  LinkerConfig *config_;
  real_talk::code::Code *output_code_;
};
}
}
#endif
