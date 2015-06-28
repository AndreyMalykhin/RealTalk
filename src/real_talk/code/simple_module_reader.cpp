
#include <cassert>
#include <string>
#include <vector>
#include "real_talk/code/cmd.h"
#include "real_talk/code/module.h"
#include "real_talk/code/simple_module_reader.h"
#include "real_talk/code/code.h"

using std::string;
using std::vector;
using std::ios;
using std::istream;
using std::unique_ptr;

namespace real_talk {
namespace code {

unique_ptr<Module> SimpleModuleReader::ReadFromCode(Code *code) const {
  assert(code);
  const uint32_t version = code->ReadUint32();
  const uint32_t cmds_address = code->ReadUint32();
  const uint32_t main_cmds_size = code->ReadUint32();
  const uint32_t func_cmds_size = code->ReadUint32();
  const uint32_t global_var_defs_metadata_address = code->ReadUint32();
  const uint32_t global_var_defs_metadata_size = code->ReadUint32();
  const uint32_t func_defs_metadata_address = code->ReadUint32();
  const uint32_t func_defs_metadata_size = code->ReadUint32();
  const uint32_t native_func_defs_metadata_address = code->ReadUint32();
  const uint32_t native_func_defs_metadata_size = code->ReadUint32();
  const uint32_t global_var_refs_metadata_address = code->ReadUint32();
  const uint32_t global_var_refs_metadata_size = code->ReadUint32();
  const uint32_t func_refs_metadata_address = code->ReadUint32();
  const uint32_t func_refs_metadata_size = code->ReadUint32();
  const uint32_t native_func_refs_metadata_address = code->ReadUint32();
  const uint32_t native_func_refs_metadata_size = code->ReadUint32();

  code->SetPosition(cmds_address);
  unique_ptr<Code> cmds_code(new Code(
      code->GetDataAtPosition(), main_cmds_size + func_cmds_size));

  code->SetPosition(global_var_defs_metadata_address);
  vector<string> global_var_defs;
  const unsigned char * const global_var_defs_metadata_end =
      code->GetDataAtPosition() + global_var_defs_metadata_size;

  while (code->GetDataAtPosition() != global_var_defs_metadata_end) {
    global_var_defs.push_back(code->ReadString());
  }

  code->SetPosition(func_defs_metadata_address);
  vector<IdAddress> func_defs;
  const unsigned char * const func_defs_metadata_end =
      code->GetDataAtPosition() + func_defs_metadata_size;

  while (code->GetDataAtPosition() != func_defs_metadata_end) {
    func_defs.push_back(code->ReadIdAddress());
  }

  code->SetPosition(native_func_defs_metadata_address);
  vector<string> native_func_defs;
  const unsigned char * const native_func_defs_metadata_end =
      code->GetDataAtPosition() + native_func_defs_metadata_size;

  while (code->GetDataAtPosition() != native_func_defs_metadata_end) {
    native_func_defs.push_back(code->ReadString());
  }

  code->SetPosition(global_var_refs_metadata_address);
  vector<IdAddresses> global_var_refs;
  const unsigned char * const global_var_refs_metadata_end =
      code->GetDataAtPosition() + global_var_refs_metadata_size;

  while (code->GetDataAtPosition() != global_var_refs_metadata_end) {
    global_var_refs.push_back(code->ReadIdAddresses());
  }

  code->SetPosition(func_refs_metadata_address);
  vector<IdAddresses> func_refs;
  const unsigned char * const func_refs_metadata_end =
      code->GetDataAtPosition() + func_refs_metadata_size;

  while (code->GetDataAtPosition() != func_refs_metadata_end) {
    func_refs.push_back(code->ReadIdAddresses());
  }

  code->SetPosition(native_func_refs_metadata_address);
  vector<IdAddresses> native_func_refs;
  const unsigned char * const native_func_refs_metadata_end =
      code->GetDataAtPosition() + native_func_refs_metadata_size;

  while (code->GetDataAtPosition() != native_func_refs_metadata_end) {
    native_func_refs.push_back(code->ReadIdAddresses());
  }

  return unique_ptr<Module>(new Module(version,
                                       move(cmds_code),
                                       main_cmds_size,
                                       func_defs,
                                       global_var_defs,
                                       native_func_defs,
                                       func_refs,
                                       native_func_refs,
                                       global_var_refs));
}

unique_ptr<Module> SimpleModuleReader::ReadFromStream(istream *stream) const {
  assert(stream);
  Code code(*stream);
  return ReadFromCode(&code);
}
}
}
