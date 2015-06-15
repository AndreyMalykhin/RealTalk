
#ifndef _REAL_TALK_CODE_CODE_CONTAINER_H_
#define _REAL_TALK_CODE_CODE_CONTAINER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "real_talk/code/code.h"
#include "real_talk/code/id_addresses.h"

namespace real_talk {
namespace code {

class CodeContainerVisitor;

class CodeContainer {
 public:
  CodeContainer(CodeContainer&&) = default;
  virtual ~CodeContainer() {}
  virtual void Accept(const CodeContainerVisitor &visitor) const = 0;
  Code &GetCmdsCode();
  const Code &GetCmdsCode() const;
  uint32_t GetMainCmdsCodeSize() const;
  uint32_t GetFuncCmdsCodeSize() const;
  uint32_t GetVersion() const;
  const std::vector<IdAddresses> &GetIdAddressesOfNativeFuncRefs() const;
  const std::vector<std::string> &GetIdsOfNativeFuncDefs() const;
  friend bool operator==(const CodeContainer &lhs, const CodeContainer &rhs);
  friend std::ostream &operator<<(
      std::ostream &stream, CodeContainer &container);

 protected:
    CodeContainer(
        uint32_t version,
        std::unique_ptr<Code> cmds_code,
        uint32_t main_cmds_code_size,
        const std::vector<std::string> &ids_of_native_func_defs,
        const std::vector<IdAddresses> &id_addresses_of_native_func_refs);

 private:
  virtual bool IsEqual(const CodeContainer &container) const = 0;
  virtual void Print(std::ostream &stream) = 0;

  uint32_t version_;
  std::unique_ptr<Code> cmds_code_;
  uint32_t main_cmds_code_size_;
  std::vector<std::string> ids_of_native_func_defs_;
  std::vector<IdAddresses> id_addresses_of_native_func_refs_;
};
}
}
#endif
