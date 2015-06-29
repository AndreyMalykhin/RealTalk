
#include "real_talk/util/simple_dir_creator.h"
#include "real_talk/util/simple_file.h"
#include "real_talk/util/simple_file_searcher.h"
#include "real_talk/lexer/simple_lexer_factory.h"
#include "real_talk/parser/simple_parser.h"
#include "real_talk/semantic/simple_lit_parser.h"
#include "real_talk/semantic/simple_semantic_analyzer.h"
#include "real_talk/code/code.h"
#include "real_talk/code/simple_code_generator.h"
#include "real_talk/code/simple_cast_cmd_generator.h"
#include "real_talk/code/simple_code_container_writer.h"
#include "real_talk/compiler/simple_msg_printer.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/simple_compiler_config_parser.h"
#include "real_talk/compiler/compiler_app.h"

using std::cout;
using real_talk::compiler::CompilerApp;
using real_talk::compiler::CompilerConfig;
using real_talk::compiler::SimpleCompilerConfigParser;
using real_talk::compiler::SimpleMsgPrinter;
using real_talk::lexer::SimpleLexerFactory;
using real_talk::parser::SimpleParser;
using real_talk::semantic::SimpleLitParser;
using real_talk::semantic::SimpleSemanticAnalyzer;
using real_talk::code::Code;
using real_talk::code::SimpleCodeGenerator;
using real_talk::code::SimpleCastCmdGenerator;
using real_talk::code::SimpleCodeContainerWriter;
using real_talk::util::SimpleDirCreator;
using real_talk::util::SimpleFile;
using real_talk::util::SimpleFileSearcher;

int main(int argc, const char *argv[]) {
  SimpleParser src_parser;
  const SimpleLitParser lit_parser;
  SimpleSemanticAnalyzer semantic_analyzer(lit_parser);
  const SimpleCastCmdGenerator cast_cmd_generator;
  SimpleCodeGenerator code_generator(cast_cmd_generator);
  CompilerConfig compiler_config;
  Code code;
  CompilerApp compiler(SimpleFileSearcher(),
                       SimpleLexerFactory(),
                       &src_parser,
                       lit_parser,
                       SimpleCompilerConfigParser(),
                       &semantic_analyzer,
                       &code_generator,
                       SimpleMsgPrinter(&cout),
                       SimpleDirCreator(),
                       SimpleCodeContainerWriter(),
                       SimpleFile(),
                       &compiler_config,
                       &code);
  compiler.Run(argc, argv);
  return EXIT_SUCCESS;
}
