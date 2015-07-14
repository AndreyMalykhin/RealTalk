
#ifndef _REAL_TALK_VM_FUNC_FRAME_H_
#define _REAL_TALK_VM_FUNC_FRAME_H_

#include <cstdint>
#include <iostream>

namespace real_talk {
namespace vm {

class FuncFrame {
 public:
  FuncFrame(size_t local_vars_start_index, uint32_t return_address) noexcept
      : local_vars_start_index_(local_vars_start_index),
        return_address_(return_address) {}

  inline size_t GetLocalVarsStartIndex() const noexcept {
    return local_vars_start_index_;
  }

  inline uint32_t GetReturnAddress() const noexcept {
    return return_address_;
  }

  friend bool operator==(const FuncFrame &lhs, const FuncFrame &rhs) {
    return lhs.local_vars_start_index_ == rhs.local_vars_start_index_;
  }

  friend std::ostream &operator<<(
      std::ostream &stream, const FuncFrame &frame) {
    return stream << "local_vars_start_index=" << frame.local_vars_start_index_
                  << "; return_address=" << frame.return_address_;
  }

 private:
  size_t local_vars_start_index_;
  uint32_t return_address_;
};
}
}
#endif
