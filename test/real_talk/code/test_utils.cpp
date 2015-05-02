
#include <boost/filesystem.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include "real_talk/util/endianness.h"
#include "real_talk/code/id_address.h"
#include "real_talk/code/id_addresses.h"
#include "real_talk/code/module.h"
#include "real_talk/code/test_utils.h"
#include "real_talk/code/code.h"

namespace real_talk {
namespace code {
namespace {

void WriteSegmentMetadata(Code &module_code,
                          uint32_t segment_address,
                          uint32_t segment_size) {
  module_code.WriteUint32(segment_address);
  module_code.WriteUint32(segment_size);
}

void WriteIdsSegment(
    Code &module_code, const std::vector<std::string> &ids) {
  for (const std::string &id: ids) {
    module_code.WriteString(id);
  }
}

void WriteIdAddressesSegment(
    Code &module_code, const std::vector<IdAddresses> &id_addresses) {
  for (const IdAddresses &id_addresses_item: id_addresses) {
    module_code.WriteIdAddresses(id_addresses_item);
  }
}
}

void WriteModule(const Module &module, Code &module_code) {
  module_code.WriteUint32(module.GetVersion());
  uint32_t segments_metadata_address = module_code.GetPosition();
  module_code.Skip(17 * sizeof(uint32_t));

  uint32_t cmds_address = module_code.GetPosition();
  module_code.WriteBytes(
      module.GetCmdsCode().GetData(), module.GetCmdsCode().GetSize());

  uint32_t imports_metadata_address = module_code.GetPosition();

  for (const boost::filesystem::path &import_file_path
           : module.GetImportFilePaths()) {
    module_code.WriteFilePath(import_file_path);
  }

  uint32_t imports_metadata_size =
      module_code.GetPosition() - imports_metadata_address;

  uint32_t global_var_defs_metadata_address = module_code.GetPosition();
  WriteIdsSegment(module_code, module.GetIdsOfGlobalVarDefs());
  uint32_t global_var_defs_metadata_size =
      module_code.GetPosition() - global_var_defs_metadata_address;

  uint32_t func_defs_metadata_address = module_code.GetPosition();

  for (const IdAddress &id_address: module.GetIdAddressesOfFuncDefs()) {
    module_code.WriteIdAddress(id_address);
  }

  uint32_t func_defs_metadata_size =
      module_code.GetPosition() - func_defs_metadata_address;

  uint32_t native_func_defs_metadata_address = module_code.GetPosition();
  WriteIdsSegment(module_code, module.GetIdsOfNativeFuncDefs());
  uint32_t native_func_defs_metadata_size =
      module_code.GetPosition() - native_func_defs_metadata_address;

  uint32_t global_var_refs_metadata_address = module_code.GetPosition();
  WriteIdAddressesSegment(
      module_code, module.GetIdAddressesOfGlobalVarRefs());
  uint32_t global_var_refs_metadata_size =
      module_code.GetPosition() - global_var_refs_metadata_address;

  uint32_t func_refs_metadata_address = module_code.GetPosition();
  WriteIdAddressesSegment(module_code, module.GetIdAddressesOfFuncRefs());
  uint32_t func_refs_metadata_size =
      module_code.GetPosition() - func_refs_metadata_address;

  uint32_t native_func_refs_metadata_address = module_code.GetPosition();
  WriteIdAddressesSegment(module_code, module.GetIdAddressesOfNativeFuncRefs());
  uint32_t native_func_refs_metadata_size =
      module_code.GetPosition() - native_func_refs_metadata_address;

  module_code.SetPosition(segments_metadata_address);
  module_code.WriteUint32(cmds_address);
  module_code.WriteUint32(module.GetMainCmdsCodeSize());
  module_code.WriteUint32(module.GetFuncCmdsCodeSize());
  WriteSegmentMetadata(
      module_code, imports_metadata_address, imports_metadata_size);
  WriteSegmentMetadata(module_code,
                       global_var_defs_metadata_address,
                       global_var_defs_metadata_size);
  WriteSegmentMetadata(module_code,
                       func_defs_metadata_address,
                       func_defs_metadata_size);
  WriteSegmentMetadata(module_code,
                       native_func_defs_metadata_address,
                       native_func_defs_metadata_size);
  WriteSegmentMetadata(module_code,
                       global_var_refs_metadata_address,
                       global_var_refs_metadata_size);
  WriteSegmentMetadata(module_code,
                       func_refs_metadata_address,
                       func_refs_metadata_size);
  WriteSegmentMetadata(module_code,
                       native_func_refs_metadata_address,
                       native_func_refs_metadata_size);
}
}
}

