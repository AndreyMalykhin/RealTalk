
#include <cassert>
#include "real_talk/linker/output_file_type.h"
#include "real_talk/linker/lib_linker.h"
#include "real_talk/linker/exe_linker.h"
#include "real_talk/linker/simple_linker_factory.h"

using std::unique_ptr;

namespace real_talk {
namespace linker {

unique_ptr<Linker> SimpleLinkerFactory::Create(
    OutputFileType output_file_type) const {
  unique_ptr<Linker> linker;

  switch (output_file_type) {
    case OutputFileType::kLib:
      linker.reset(new LibLinker());
      break;
    case OutputFileType::kExe:
      linker.reset(new ExeLinker());
      break;
  }

  assert(linker);
  return linker;
}
}
}
