
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
    code->Write<string>(id);
  }
}

void WriteIdAddresses(const vector<IdAddresses> &id_addresses, Code *code) {
  for (const IdAddresses &id_addresses_item: id_addresses) {
    code->Write<IdAddresses>(id_addresses_item);
  }
}
}

void SimpleCodeContainerWriter::Write(
    const CodeContainer &code_container, Code *output_code) const {
  assert(output_code);
  output_code_ = output_code;
  code_container.Accept(*this);
}

void SimpleCodeContainerWriter::Write(const Exe &exe, Code *output_code) const {
  assert(output_code);
  output_code_ = output_code;
  output_code->Write<uint32_t>(exe.GetVersion());
  output_code->Write<uint32_t>(exe.GetGlobalVarsSize());
  const uint32_t segments_metadata_address = output_code->GetPosition();
  output_code->Skip(7 * sizeof(uint32_t));
  const uint32_t cmds_address = output_code->GetPosition();
  output_code->Write(
      exe.GetCmdsCode().GetData(), exe.GetCmdsCode().GetSize());
  const uint32_t native_func_defs_metadata_address = output_code->GetPosition();
  WriteIds(exe.GetNativeFuncDefs(), output_code);
  const uint32_t native_func_defs_metadata_size =
      output_code->GetPosition() - native_func_defs_metadata_address;
  const uint32_t native_func_refs_metadata_address = output_code->GetPosition();
  WriteIdAddresses(exe.GetNativeFuncRefs(), output_code);
  const uint32_t native_func_refs_metadata_size =
      output_code->GetPosition() - native_func_refs_metadata_address;
  output_code->SetPosition(segments_metadata_address);
  output_code->Write<uint32_t>(cmds_address);
  output_code->Write<uint32_t>(exe.GetMainCmdsCodeSize());
  output_code->Write<uint32_t>(exe.GetFuncCmdsCodeSize());
  output_code->Write<uint32_t>(native_func_defs_metadata_address);
  output_code->Write<uint32_t>(native_func_defs_metadata_size);
  output_code->Write<uint32_t>(native_func_refs_metadata_address);
  output_code->Write<uint32_t>(native_func_refs_metadata_size);
}

void SimpleCodeContainerWriter::Write(
    const Module &module, Code *output_code) const {
  assert(output_code);
  output_code_ = output_code;
  output_code->Write<uint32_t>(module.GetVersion());
  const uint32_t segments_metadata_address = output_code->GetPosition();
  output_code->Skip(15 * sizeof(uint32_t));
  const uint32_t cmds_address = output_code->GetPosition();
  output_code->Write(
      module.GetCmdsCode().GetData(), module.GetCmdsCode().GetSize());
  const uint32_t global_var_defs_metadata_address = output_code->GetPosition();

  for (const IdSize &global_var_def: module.GetGlobalVarDefs()) {
    output_code->Write<IdSize>(global_var_def);
  }

  const uint32_t global_var_defs_metadata_size =
      output_code->GetPosition() - global_var_defs_metadata_address;
  const uint32_t func_defs_metadata_address = output_code->GetPosition();

  for (const IdAddress &id_address: module.GetFuncDefs()) {
    output_code->Write<IdAddress>(id_address);
  }

  const uint32_t func_defs_metadata_size =
      output_code->GetPosition() - func_defs_metadata_address;
  const uint32_t native_func_defs_metadata_address = output_code->GetPosition();
  WriteIds(module.GetNativeFuncDefs(), output_code);
  const uint32_t native_func_defs_metadata_size =
      output_code->GetPosition() - native_func_defs_metadata_address;
  const uint32_t global_var_refs_metadata_address = output_code->GetPosition();
  WriteIdAddresses(module.GetGlobalVarRefs(), output_code);
  const uint32_t global_var_refs_metadata_size =
      output_code->GetPosition() - global_var_refs_metadata_address;
  const uint32_t func_refs_metadata_address = output_code->GetPosition();
  WriteIdAddresses(module.GetFuncRefs(), output_code);
  const uint32_t func_refs_metadata_size =
      output_code->GetPosition() - func_refs_metadata_address;
  const uint32_t native_func_refs_metadata_address = output_code->GetPosition();
  WriteIdAddresses(module.GetNativeFuncRefs(), output_code);
  const uint32_t native_func_refs_metadata_size =
      output_code->GetPosition() - native_func_refs_metadata_address;
  output_code->SetPosition(segments_metadata_address);
  output_code->Write<uint32_t>(cmds_address);
  output_code->Write<uint32_t>(module.GetMainCmdsCodeSize());
  output_code->Write<uint32_t>(module.GetFuncCmdsCodeSize());
  output_code->Write<uint32_t>(global_var_defs_metadata_address);
  output_code->Write<uint32_t>(global_var_defs_metadata_size);
  output_code->Write<uint32_t>(func_defs_metadata_address);
  output_code->Write<uint32_t>(func_defs_metadata_size);
  output_code->Write<uint32_t>(native_func_defs_metadata_address);
  output_code->Write<uint32_t>(native_func_defs_metadata_size);
  output_code->Write<uint32_t>(global_var_refs_metadata_address);
  output_code->Write<uint32_t>(global_var_refs_metadata_size);
  output_code->Write<uint32_t>(func_refs_metadata_address);
  output_code->Write<uint32_t>(func_refs_metadata_size);
  output_code->Write<uint32_t>(native_func_refs_metadata_address);
  output_code->Write<uint32_t>(native_func_refs_metadata_size);
}

void SimpleCodeContainerWriter::VisitModule(const Module &module) const {
  Write(module, output_code_);
}

void SimpleCodeContainerWriter::VisitExe(const Exe &exe) const {
  Write(exe, output_code_);
}
}
}
