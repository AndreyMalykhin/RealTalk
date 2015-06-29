
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cassert>
#include <vector>
#include <string>
#include "real_talk/compiler/compiler_cmd.h"
#include "real_talk/compiler/compiler_config.h"
#include "real_talk/compiler/simple_compiler_config_parser.h"

using std::vector;
using std::ostringstream;
using std::string;
using boost::program_options::value;
using boost::program_options::bool_switch;
using boost::program_options::options_description;
using boost::program_options::positional_options_description;
using boost::program_options::variables_map;
using boost::program_options::command_line_parser;
using boost::program_options::error;
using boost::filesystem::path;

namespace real_talk {
namespace compiler {
namespace {

options_description GetOptions() {
  options_description options("Options");
  options.add_options()
      ("help,h", "")
      ("input", value<path>(), "Input file")
      ("import,i",
       value< vector<path> >(),
       "Folder, where import files will be searched")
      ("debug,d", "Display debug info");
  return options;
}
}

void SimpleCompilerConfigParser::Parse(int argc,
                                       const char *argv[],
                                       CompilerConfig *config,
                                       CompilerCmd *cmd) const {
  assert(config);
  assert(cmd);

  try {
    options_description options = GetOptions();
    positional_options_description positional_options;
    positional_options.add("input", -1);
    variables_map vars;
    command_line_parser cmd_line_parser(argc, argv);
    cmd_line_parser.options(options).positional(positional_options);
    store(cmd_line_parser.run(), vars);
    notify(vars);
    *cmd = (vars.empty() || vars.count("help"))
           ? CompilerCmd::kHelp : CompilerCmd::kCompile;

    if (vars.count("input")) {
      config->SetInputFilePath(vars["input"].as<path>());
    } else if (*cmd == CompilerCmd::kCompile) {
      throw BadArgsError("Input file is required");
    }

    if (vars.count("import")) {
      config->SetImportDirPaths(vars["import"].as< vector<path> >());
    }

    if (vars.count("debug")) {
      config->SetDebug(true);
    }
  } catch (const error &e) {
    throw BadArgsError(e.what());
  }
}

string SimpleCompilerConfigParser::GetHelp() const {
  ostringstream stream;
  stream << GetOptions();
  return stream.str();
}
}
}
