
#include <cassert>
#include <string>
#include <vector>
#include "real_talk/code/cmd.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/simple_exe_reader.h"
#include "real_talk/code/code.h"

using std::string;
using std::vector;
using std::ios;
using std::istream;
using std::unique_ptr;

namespace real_talk {
namespace code {

unique_ptr<Exe> SimpleExeReader::ReadFromCode(Code *code) const {
  assert(code);
  const uint32_t version = code->ReadUint32();
  const uint32_t global_vars_size = code->ReadUint32();
  const uint32_t cmds_address = code->ReadUint32();
  const uint32_t main_cmds_size = code->ReadUint32();
  const uint32_t func_cmds_size = code->ReadUint32();
  const uint32_t native_func_defs_metadata_address = code->ReadUint32();
  const uint32_t native_func_defs_metadata_size = code->ReadUint32();
  const uint32_t native_func_refs_metadata_address = code->ReadUint32();
  const uint32_t native_func_refs_metadata_size = code->ReadUint32();

  code->SetPosition(cmds_address);
  unique_ptr<Code> cmds_code(new Code(
      code->GetDataAtPosition(), main_cmds_size + func_cmds_size));

  code->SetPosition(native_func_defs_metadata_address);
  vector<string> native_func_defs;
  const unsigned char * const native_func_defs_metadata_end =
      code->GetDataAtPosition() + native_func_defs_metadata_size;

  while (code->GetDataAtPosition() != native_func_defs_metadata_end) {
    native_func_defs.push_back(code->ReadString());
  }

  code->SetPosition(native_func_refs_metadata_address);
  vector<IdAddresses> native_func_refs;
  const unsigned char * const native_func_refs_metadata_end =
      code->GetDataAtPosition() + native_func_refs_metadata_size;

  while (code->GetDataAtPosition() != native_func_refs_metadata_end) {
    native_func_refs.push_back(code->ReadIdAddresses());
  }

  return unique_ptr<Exe>(new Exe(version,
                                 move(cmds_code),
                                 main_cmds_size,
                                 global_vars_size,
                                 native_func_defs,
                                 native_func_refs));
}

unique_ptr<Exe> SimpleExeReader::ReadFromStream(istream *stream) const {
  assert(stream);
  Code code(*stream);
  return ReadFromCode(&code);
}
}
}
