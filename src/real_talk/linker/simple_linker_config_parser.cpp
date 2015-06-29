
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cassert>
#include <vector>
#include <string>
#include "real_talk/linker/linker_cmd.h"
#include "real_talk/linker/linker_config.h"
#include "real_talk/linker/simple_linker_config_parser.h"

using std::vector;
using std::ostringstream;
using std::string;
using boost::program_options::value;
using boost::program_options::bool_switch;
using boost::program_options::options_description;
using boost::program_options::positional_options_description;
using boost::program_options::variables_map;
using boost::program_options::variable_value;
using boost::program_options::command_line_parser;
using boost::program_options::error;
using boost::program_options::validation_error;
using boost::program_options::validators::check_first_occurrence;
using boost::program_options::validators::get_single_string;
using boost::filesystem::path;
using boost::any;

namespace real_talk {
namespace linker {
namespace {

options_description GetOptions() {
  options_description options("Options");
  options.add_options()
      ("help,h", "")
      ("input", value< vector<path> >(), "Input file")
      ("output", value<path>(), "Output file")
      ("output-type",
       value<OutputFileType>()->default_value(OutputFileType::kExe),
       "Output file type")
      ("import,i",
       value< vector<path> >(),
       "Path to project, where input files will be searched")
      ("debug,d", "Display debug info");
  return options;
}
}

void SimpleLinkerConfigParser::Parse(int argc,
                                     const char *argv[],
                                     LinkerConfig *config,
                                     LinkerCmd *cmd) const {
  assert(config);
  assert(cmd);

  try {
    options_description options = GetOptions();
    positional_options_description positional_options;
    positional_options.add("output", 1).add("input", -1);
    variables_map vars;
    command_line_parser cmd_line_parser(argc, argv);
    cmd_line_parser.options(options).positional(positional_options);
    store(cmd_line_parser.run(), vars);
    notify(vars);
    bool has_args = false;

    for (const auto &var_pair: vars) {
      const variable_value &var = var_pair.second;

      if (!var.defaulted()) {
        has_args = true;
        break;
      }
    }

    *cmd = (!has_args || vars.count("help"))
           ? LinkerCmd::kHelp : LinkerCmd::kLink;

    if (vars.count("output")) {
      config->SetOutputFilePath(vars["output"].as<path>());
    } else if (*cmd == LinkerCmd::kLink) {
      throw BadArgsError("Output file is required");
    }

    if (vars.count("input")) {
      config->SetInputFilePaths(vars["input"].as< vector<path> >());
    } else if (*cmd == LinkerCmd::kLink) {
      throw BadArgsError("Input files are required");
    }

    assert(vars.count("output-type"));
    config->SetOutputFileType(vars["output-type"].as<OutputFileType>());

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

string SimpleLinkerConfigParser::GetHelp() const {
  ostringstream stream;
  stream << GetOptions();
  return stream.str();
}

static void validate(any &output_value,
                     const vector<string> &input_values,
                     OutputFileType*,
                     int) {
  check_first_occurrence(output_value);
  const string &input_value = get_single_string(input_values);

  if (input_value == "exe") {
    output_value = OutputFileType::kExe;
  } else if (input_value == "lib") {
    output_value = OutputFileType::kLib;
  } else {
    throw validation_error(validation_error::invalid_option_value);
  }
}
}
}
