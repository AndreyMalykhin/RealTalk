
#include <boost/format.hpp>
#include <cassert>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include "real_talk/code/exe.h"
#include "real_talk/vm/simple_native_func_linker.h"

using std::string;
using std::find;
using std::numeric_limits;
using std::vector;
using boost::format;
using real_talk::code::Exe;
using real_talk::code::Code;
using real_talk::code::IdAddresses;

namespace real_talk {
namespace vm {

void SimpleNativeFuncLinker::Link(
    const NativeFuncStorage::NativeFuncsMap &available_funcs,
    vector<NativeFuncValue> *output_used_funcs,
    Exe *exe) const {
  assert(output_used_funcs);
  assert(exe);
  Code &cmds_code = exe->GetCmdsCode();

  for (const IdAddresses &func_refs: exe->GetNativeFuncRefs()) {
    const string &func_id = func_refs.GetId();
    const auto available_funcs_it = available_funcs.find(func_id);

    if (available_funcs_it == available_funcs.cend()) {
      throw MissingFuncError(
          func_id, (format("Missing native function \"%1%\"") % func_id).str());
    }

    const size_t func_index = output_used_funcs->size();
    const NativeFuncValue func_value = available_funcs_it->second;
    assert(func_index <= numeric_limits<uint32_t>::max());
    assert(find(output_used_funcs->cbegin(),
                output_used_funcs->cend(),
                func_value) == output_used_funcs->cend());
    output_used_funcs->push_back(func_value);

    for (const uint32_t func_index_placeholder: func_refs.GetAddresses()) {
      cmds_code.SetPosition(func_index_placeholder);
      cmds_code.Write(static_cast<uint32_t>(func_index));
    }
  }

  cmds_code.SetPosition(UINT32_C(0));
}
}
}
