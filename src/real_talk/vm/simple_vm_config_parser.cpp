
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cassert>
#include <vector>
#include <string>
#include "real_talk/vm/vm_cmd.h"
#include "real_talk/vm/vm_config.h"
#include "real_talk/vm/simple_vm_config_parser.h"

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
namespace vm {
namespace {

options_description GetOptions() {
  options_description options("Options");
  options.add_options()
      ("help,h", "")
      ("input", value<path>(), "Input file")
      ("debug,d", "Display debug info");
  return options;
}
}

void SimpleVMConfigParser::Parse(int argc,
                                 const char *argv[],
                                 VMConfig *config,
                                 VMCmd *cmd) const {
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
    bool has_args = false;

    for (const auto &var_pair: vars) {
      const variable_value &var = var_pair.second;

      if (!var.defaulted()) {
        has_args = true;
        break;
      }
    }

    *cmd = (!has_args || vars.count("help"))
           ? VMCmd::kHelp : VMCmd::kExecute;

    if (vars.count("input")) {
      config->SetInputFilePath(vars["input"].as<path>());
    } else if (*cmd == VMCmd::kExecute) {
      throw BadArgsError("Input file is required");
    }

    if (vars.count("debug")) {
      config->SetDebug(true);
    }
  } catch (const error &e) {
    throw BadArgsError(e.what());
  }
}

string SimpleVMConfigParser::GetHelp() const {
  ostringstream stream;
  stream << GetOptions();
  return stream.str();
}
}
}
