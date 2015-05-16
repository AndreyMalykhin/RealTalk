
#include <cassert>
#include "real_talk/parser/program_node.h"
#include "real_talk/compiler/file_parser.h"

using std::unique_ptr;
using real_talk::parser::ProgramNode;

namespace real_talk {
namespace compiler {

FileParser::Programs::Programs(
    unique_ptr<ProgramNode> main, ImportPrograms import)
    : main_(move(main)), import_(move(import)) {
  assert(main_);
}

const ProgramNode &FileParser::Programs::GetMain() const {
  return *main_;
}

const FileParser::ImportPrograms &FileParser::Programs::GetImport() const {
  return import_;
}
}
}
