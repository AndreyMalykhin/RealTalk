
#ifndef _REAL_TALK_CODE_CMD_H_
#define _REAL_TALK_CODE_CMD_H_

#include <cstdint>
#include <typeinfo>
#include <iostream>

namespace real_talk {
namespace code {

enum class CmdId: uint8_t {
  kEndMain = UINT8_C(1),
  kEndFuncs,
  kCreateGlobalIntVar,
  kCreateGlobalArrayVar,
  kCreateGlobalLongVar,
  kCreateGlobalDoubleVar,
  kCreateGlobalCharVar,
  kCreateGlobalStringVar,
  kCreateGlobalBoolVar,
  kCreateLocalIntVar,
  kCreateLocalArrayVar,
  kCreateLocalLongVar,
  kCreateLocalDoubleVar,
  kCreateLocalCharVar,
  kCreateLocalStringVar,
  kCreateLocalBoolVar,
  kCreateAndInitGlobalIntVar,
  kCreateAndInitGlobalArrayVar,
  kCreateAndInitGlobalLongVar,
  kCreateAndInitGlobalDoubleVar,
  kCreateAndInitGlobalCharVar,
  kCreateAndInitGlobalStringVar,
  kCreateAndInitGlobalBoolVar,
  kCreateAndInitLocalIntVar,
  kCreateAndInitLocalArrayVar,
  kCreateAndInitLocalLongVar,
  kCreateAndInitLocalDoubleVar,
  kCreateAndInitLocalCharVar,
  kCreateAndInitLocalStringVar,
  kCreateAndInitLocalBoolVar,
  kCreateIntArray,
  kCreateLongArray,
  kCreateDoubleArray,
  kCreateBoolArray,
  kCreateCharArray,
  kCreateStringArray,
  kCreateAndInitIntArray,
  kCreateAndInitLongArray,
  kCreateAndInitDoubleArray,
  kCreateAndInitBoolArray,
  kCreateAndInitCharArray,
  kCreateAndInitStringArray,
  kUnload,
  kLoadIntValue,
  kLoadLongValue,
  kLoadBoolValue,
  kLoadCharValue,
  kLoadStringValue,
  kLoadDoubleValue,
  kLoadGlobalIntVarValue,
  kLoadGlobalLongVarValue,
  kLoadGlobalDoubleVarValue,
  kLoadGlobalBoolVarValue,
  kLoadGlobalCharVarValue,
  kLoadGlobalStringVarValue,
  kLoadGlobalArrayVarValue,
  kLoadLocalIntVarValue,
  kLoadLocalLongVarValue,
  kLoadLocalDoubleVarValue,
  kLoadLocalBoolVarValue,
  kLoadLocalCharVarValue,
  kLoadLocalStringVarValue,
  kLoadLocalArrayVarValue,
  kLoadGlobalVarAddress,
  kLoadLocalVarAddress,
  kLoadFuncAddress,
  kLoadNativeFuncAddress,
  kLoadArrayOfIntsElementValue,
  kLoadArrayOfLongsElementValue,
  kLoadArrayOfDoublesElementValue,
  kLoadArrayOfCharsElementValue,
  kLoadArrayOfBoolsElementValue,
  kLoadArrayOfStringsElementValue,
  kLoadArrayOfArraysElementValue,
  kLoadArrayOfIntsElementAddress,
  kLoadArrayOfLongsElementAddress,
  kLoadArrayOfDoublesElementAddress,
  kLoadArrayOfCharsElementAddress,
  kLoadArrayOfBoolsElementAddress,
  kLoadArrayOfStringsElementAddress,
  kLoadArrayOfArraysElementAddress,
  kStoreInt,
  kStoreLong,
  kStoreDouble,
  kStoreBool,
  kStoreChar,
  kStoreString,
  kStoreArray,
  kCastCharToInt,
  kCastCharToLong,
  kCastCharToDouble,
  kCastCharToString,
  kCastIntToLong,
  kCastIntToDouble,
  kCastLongToDouble,
  kJumpIfNot,
  kImplicitJumpIfNot,
  kImplicitJumpIf,
  kDirectJump,
  kDestroyLocalVarsAndJump,
  kDestroyLocalVars,
  kReturn,
  kReturnIntValue,
  kReturnLongValue,
  kReturnDoubleValue,
  kReturnBoolValue,
  kReturnCharValue,
  kReturnStringValue,
  kReturnArrayValue,
  kCall,
  kCallNative,
  kAnd,
  kOr,
  kMulChar,
  kMulInt,
  kMulLong,
  kMulDouble,
  kDivChar,
  kDivInt,
  kDivLong,
  kDivDouble
};

class Cmd {
 public:
  virtual ~Cmd() {}

  friend std::ostream &operator<<(std::ostream &stream, const Cmd &cmd) {
    stream << typeid(cmd).name() << "; ";
    cmd.Print(stream);
    return stream;
  }

  friend bool operator==(const Cmd &lhs, const Cmd &rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.IsEqual(rhs);
  }

 private:
  virtual void Print(std::ostream &stream) const = 0;
  virtual bool IsEqual(const Cmd &cmd) const = 0;
};
}
}
#endif
