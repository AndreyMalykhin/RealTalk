
#include <cassert>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/exe.h"
#include "real_talk/code/simple_code_container_writer.h"

using std::vector;
using std::string;

namespace real_talk {
namespace code {
namespace {

void WriteIds(const vector<string> &ids, Code *code) {
  for (const string &id: ids) {
    code->WriteString(id);
  }
}

void WriteIdAddresses(const vector<IdAddresses> &id_addresses, Code *code) {
  for (const IdAddresses &id_addresses_item: id_addresses) {
    code->WriteIdAddresses(id_addresses_item);
  }
}
}

void SimpleCodeContainerWriter::Write(
    const CodeContainer&, Code*) const {
  assert(false);
}

void SimpleCodeContainerWriter::Write(const Exe &exe, Code *code) const {
  assert(code);
  code->WriteUint32(exe.GetVersion());
  const uint32_t segments_metadata_address = code->GetPosition();
  code->Skip(7 * sizeof(uint32_t));
  const uint32_t cmds_address = code->GetPosition();
  code->WriteBytes(
      exe.GetCmdsCode().GetData(), exe.GetCmdsCode().GetSize());
  const uint32_t native_func_defs_metadata_address = code->GetPosition();
  WriteIds(exe.GetIdsOfNativeFuncDefs(), code);
  const uint32_t native_func_defs_metadata_size =
      code->GetPosition() - native_func_defs_metadata_address;
  const uint32_t native_func_refs_metadata_address = code->GetPosition();
  WriteIdAddresses(exe.GetIdAddressesOfNativeFuncRefs(), code);
  const uint32_t native_func_refs_metadata_size =
      code->GetPosition() - native_func_refs_metadata_address;
  code->SetPosition(segments_metadata_address);
  code->WriteUint32(cmds_address);
  code->WriteUint32(exe.GetMainCmdsCodeSize());
  code->WriteUint32(exe.GetFuncCmdsCodeSize());
  code->WriteUint32(native_func_defs_metadata_address);
  code->WriteUint32(native_func_defs_metadata_size);
  code->WriteUint32(native_func_refs_metadata_address);
  code->WriteUint32(native_func_refs_metadata_size);
}

void SimpleCodeContainerWriter::Write(const Module &module, Code *code) const {
  assert(code);
  code->WriteUint32(module.GetVersion());
  const uint32_t segments_metadata_address = code->GetPosition();
  code->Skip(15 * sizeof(uint32_t));
  const uint32_t cmds_address = code->GetPosition();
  code->WriteBytes(
      module.GetCmdsCode().GetData(), module.GetCmdsCode().GetSize());
  const uint32_t global_var_defs_metadata_address = code->GetPosition();
  WriteIds(module.GetIdsOfGlobalVarDefs(), code);
  const uint32_t global_var_defs_metadata_size =
      code->GetPosition() - global_var_defs_metadata_address;
  const uint32_t func_defs_metadata_address = code->GetPosition();

  for (const IdAddress &id_address: module.GetIdAddressesOfFuncDefs()) {
    code->WriteIdAddress(id_address);
  }

  const uint32_t func_defs_metadata_size =
      code->GetPosition() - func_defs_metadata_address;
  const uint32_t native_func_defs_metadata_address = code->GetPosition();
  WriteIds(module.GetIdsOfNativeFuncDefs(), code);
  const uint32_t native_func_defs_metadata_size =
      code->GetPosition() - native_func_defs_metadata_address;
  const uint32_t global_var_refs_metadata_address = code->GetPosition();
  WriteIdAddresses(module.GetIdAddressesOfGlobalVarRefs(), code);
  const uint32_t global_var_refs_metadata_size =
      code->GetPosition() - global_var_refs_metadata_address;
  const uint32_t func_refs_metadata_address = code->GetPosition();
  WriteIdAddresses(module.GetIdAddressesOfFuncRefs(), code);
  const uint32_t func_refs_metadata_size =
      code->GetPosition() - func_refs_metadata_address;
  const uint32_t native_func_refs_metadata_address = code->GetPosition();
  WriteIdAddresses(module.GetIdAddressesOfNativeFuncRefs(), code);
  const uint32_t native_func_refs_metadata_size =
      code->GetPosition() - native_func_refs_metadata_address;
  code->SetPosition(segments_metadata_address);
  code->WriteUint32(cmds_address);
  code->WriteUint32(module.GetMainCmdsCodeSize());
  code->WriteUint32(module.GetFuncCmdsCodeSize());
  code->WriteUint32(global_var_defs_metadata_address);
  code->WriteUint32(global_var_defs_metadata_size);
  code->WriteUint32(func_defs_metadata_address);
  code->WriteUint32(func_defs_metadata_size);
  code->WriteUint32(native_func_defs_metadata_address);
  code->WriteUint32(native_func_defs_metadata_size);
  code->WriteUint32(global_var_refs_metadata_address);
  code->WriteUint32(global_var_refs_metadata_size);
  code->WriteUint32(func_refs_metadata_address);
  code->WriteUint32(func_refs_metadata_size);
  code->WriteUint32(native_func_refs_metadata_address);
  code->WriteUint32(native_func_refs_metadata_size);
}
}
}