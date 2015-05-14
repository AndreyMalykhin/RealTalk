
#ifndef _REAL_TALK_COMPILER_COMPILER_H_
#define _REAL_TALK_COMPILER_COMPILER_H_

namespace real_talk {
namespace code {

class Code;
class CodeGenerator;
}

namespace semantic {

class SemanticAnalyzer;
}

namespace util {

class DirCreator;
class File;
}

namespace compiler {

class FileParser;
class MsgPrinter;
class CompilerConfig;
class CompilerConfigParser;

class Compiler {
 public:
  Compiler(const CompilerConfigParser &compiler_config_parser,
           const FileParser &file_parser,
           real_talk::semantic::SemanticAnalyzer *semantic_analyzer,
           real_talk::code::CodeGenerator *code_generator,
           const MsgPrinter &msg_printer,
           const real_talk::util::DirCreator &dir_creator,
           CompilerConfig *compiler_config,
           real_talk::util::File *file,
           real_talk::code::Code *code);
  void Compile(int argc, const char *argv[]) const;
};
}
}
#endif
