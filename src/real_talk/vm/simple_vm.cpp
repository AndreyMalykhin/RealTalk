
#include <cassert>
#include <vector>
#include "real_talk/code/exe.h"
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd_reader.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/simple_vm.h"

using std::vector;
using real_talk::code::Exe;
using real_talk::code::Code;
using real_talk::code::CmdReader;
using real_talk::code::CmdVisitor;
using real_talk::code::AndCmd;

namespace real_talk {
namespace vm {

class SimpleVM::Impl: private CmdVisitor {
 public:
  Impl(Exe *exe, const vector<NativeFunc> &native_funcs);
  void Execute();
  const DataStorage &GetGlobalVars() const;
  const DataStorage &GetLocalVars() const;
  const DataStorage &GetOperands() const;
  const FuncFrames &GetFuncFrames() const;

 private:
  virtual void VisitAnd(const AndCmd &cmd) override;

  Exe *exe_;
  const vector<NativeFunc> &native_funcs_;
  Code &cmds_code_;
  CmdReader cmd_reader_;
  DataStorage global_vars_;
  DataStorage local_vars_;
  DataStorage operands_;
  FuncFrames func_frames_;
};

SimpleVM::Impl::Impl(Exe *exe, const vector<NativeFunc> &native_funcs)
    : exe_(exe),
      native_funcs_(native_funcs),
      cmds_code_(exe->GetCmdsCode()),
      global_vars_(exe->GetGlobalVarsSize()) {
  assert(exe_);
  assert(cmds_code_.GetPosition() <= exe_->GetMainCmdsCodeSize());
  cmd_reader_.SetCode(&cmds_code_);
}

void SimpleVM::Impl::Execute() {
  assert(func_frames_.empty());
  const uint32_t main_cmds_end_position = exe_->GetMainCmdsCodeSize();
  const size_t local_vars_start_index = 0;
  const uint32_t return_address = UINT32_C(0);
  const FuncFrame main_func_frame(local_vars_start_index, return_address);
  func_frames_.push_back(main_func_frame);

  while (true) {
    const bool is_in_main_func = func_frames_.size() == 1;

    if (cmds_code_.GetPosition() == main_cmds_end_position && is_in_main_func) {
      break;
    }

    cmd_reader_.GetNextCmd().Accept(this);
  }
}

const DataStorage &SimpleVM::Impl::GetGlobalVars() const {
  return global_vars_;
}

const DataStorage &SimpleVM::Impl::GetLocalVars() const {
  return local_vars_;
}

const DataStorage &SimpleVM::Impl::GetOperands() const {
  return operands_;
}

const SimpleVM::FuncFrames &SimpleVM::Impl::GetFuncFrames() const {
  return func_frames_;
}

SimpleVM::SimpleVM(Exe *exe, const vector<NativeFunc> &native_funcs)
    : impl_(new Impl(exe, native_funcs)) {}

SimpleVM::~SimpleVM() {}

void SimpleVM::Execute() {
  impl_->Execute();
}

const DataStorage &SimpleVM::GetGlobalVars() const {
  return impl_->GetGlobalVars();
}

const DataStorage &SimpleVM::GetLocalVars() const {
  return impl_->GetLocalVars();
}

const DataStorage &SimpleVM::GetOperands() const {
  return impl_->GetOperands();
}

const SimpleVM::FuncFrames &SimpleVM::GetFuncFrames() const {
  return impl_->GetFuncFrames();
}
}
}
