
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "real_talk/code/cmd.h"
#include "real_talk/code/module.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/code.h"

using std::string;
using std::vector;
using std::ios;
using std::istream;
using std::unique_ptr;
using std::stringstream;
using boost::filesystem::path;

namespace real_talk {
namespace code {

unique_ptr<Module> ModuleReader::Read(istream &code_stream) {
  Code module_code(code_stream);
  const uint32_t version = module_code.ReadUint32();
  const uint32_t cmds_address = module_code.ReadUint32();
  const uint32_t cmds_size = module_code.ReadUint32();
  const uint32_t imports_metadata_address = module_code.ReadUint32();
  const uint32_t imports_metadata_size = module_code.ReadUint32();
  const uint32_t global_var_defs_metadata_address = module_code.ReadUint32();
  const uint32_t global_var_defs_metadata_size = module_code.ReadUint32();
  const uint32_t func_defs_metadata_address = module_code.ReadUint32();
  const uint32_t func_defs_metadata_size = module_code.ReadUint32();
  const uint32_t native_func_defs_metadata_address = module_code.ReadUint32();
  const uint32_t native_func_defs_metadata_size = module_code.ReadUint32();
  const uint32_t global_var_refs_metadata_address = module_code.ReadUint32();
  const uint32_t global_var_refs_metadata_size = module_code.ReadUint32();
  const uint32_t func_refs_metadata_address = module_code.ReadUint32();
  const uint32_t func_refs_metadata_size = module_code.ReadUint32();

  module_code.SetPosition(cmds_address);
  unique_ptr<Code> cmds_code(
      new Code(module_code.GetDataAtPosition(), cmds_size));

  module_code.SetPosition(imports_metadata_address);
  vector<path> import_file_paths;
  const unsigned char * const imports_metadata_end =
      module_code.GetDataAtPosition() + imports_metadata_size;

  while (module_code.GetDataAtPosition() != imports_metadata_end) {
    import_file_paths.push_back(module_code.ReadFilePath());
  }

  module_code.SetPosition(global_var_defs_metadata_address);
  vector<string> ids_of_global_var_defs;
  const unsigned char * const global_var_defs_metadata_end =
      module_code.GetDataAtPosition() + global_var_defs_metadata_size;

  while (module_code.GetDataAtPosition() != global_var_defs_metadata_end) {
    ids_of_global_var_defs.push_back(module_code.ReadString());
  }

  module_code.SetPosition(func_defs_metadata_address);
  vector<IdAddress> id_addresses_of_func_defs;
  const unsigned char * const func_defs_metadata_end =
      module_code.GetDataAtPosition() + func_defs_metadata_size;

  while (module_code.GetDataAtPosition() != func_defs_metadata_end) {
    id_addresses_of_func_defs.push_back(module_code.ReadIdAddress());
  }

  module_code.SetPosition(native_func_defs_metadata_address);
  vector<string> ids_of_native_func_defs;
  const unsigned char * const native_func_defs_metadata_end =
      module_code.GetDataAtPosition() + native_func_defs_metadata_size;

  while (module_code.GetDataAtPosition() != native_func_defs_metadata_end) {
    ids_of_native_func_defs.push_back(module_code.ReadString());
  }

  module_code.SetPosition(global_var_refs_metadata_address);
  vector<IdAddress> id_addresses_of_global_var_refs;
  const unsigned char * const global_var_refs_metadata_end =
      module_code.GetDataAtPosition() + global_var_refs_metadata_size;

  while (module_code.GetDataAtPosition() != global_var_refs_metadata_end) {
    id_addresses_of_global_var_refs.push_back(module_code.ReadIdAddress());
  }

  module_code.SetPosition(func_refs_metadata_address);
  vector<IdAddress> id_addresses_of_func_refs;
  const unsigned char * const func_refs_metadata_end =
      module_code.GetDataAtPosition() + func_refs_metadata_size;

  while (module_code.GetDataAtPosition() != func_refs_metadata_end) {
    id_addresses_of_func_refs.push_back(module_code.ReadIdAddress());
  }

  return unique_ptr<Module>(new Module(
      version,
      move(cmds_code),
      id_addresses_of_func_defs,
      ids_of_global_var_defs,
      ids_of_native_func_defs,
      id_addresses_of_func_refs,
      id_addresses_of_global_var_refs,
      import_file_paths));
}
}
}
