
#ifndef _REAL_TALK_CODE_CMD_H_
#define _REAL_TALK_CODE_CMD_H_

#include <cstdint>
#include <typeinfo>
#include <iostream>

namespace real_talk {
namespace code {

enum class CmdId: uint8_t {
  kCreateGlobalIntVar = UINT8_C(1),
  kCreateGlobalArrayVar,
  kCreateGlobalLongVar,
  kCreateGlobalDoubleVar,
  kCreateGlobalCharVar,
  kCreateGlobalStringVar,
  kCreateGlobalBoolVar,
  kCreateAndInitGlobalIntVar,
  kCreateAndInitGlobalLongVar,
  kCreateAndInitGlobalDoubleVar,
  kCreateAndInitGlobalCharVar,
  kCreateAndInitGlobalStringVar,
  kCreateAndInitGlobalBoolVar,
  kCreateAndInitGlobalIntArrayVar,
  kCreateAndInitGlobalLongArrayVar,
  kCreateAndInitGlobalDoubleArrayVar,
  kCreateAndInitGlobalCharArrayVar,
  kCreateAndInitGlobalStringArrayVar,
  kCreateAndInitGlobalBoolArrayVar,
  kCreateLocalIntVar,
  kCreateLocalArrayVar,
  kCreateLocalLongVar,
  kCreateLocalDoubleVar,
  kCreateLocalCharVar,
  kCreateLocalStringVar,
  kCreateLocalBoolVar,
  kCreateAndInitLocalIntVar,
  kCreateAndInitLocalLongVar,
  kCreateAndInitLocalDoubleVar,
  kCreateAndInitLocalCharVar,
  kCreateAndInitLocalStringVar,
  kCreateAndInitLocalBoolVar,
  kCreateAndInitLocalIntArrayVar,
  kCreateAndInitLocalLongArrayVar,
  kCreateAndInitLocalDoubleArrayVar,
  kCreateAndInitLocalCharArrayVar,
  kCreateAndInitLocalStringArrayVar,
  kCreateAndInitLocalBoolArrayVar,
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
  kDestroyLocalIntVar,
  kDestroyLocalLongVar,
  kDestroyLocalDoubleVar,
  kDestroyLocalCharVar,
  kDestroyLocalStringVar,
  kDestroyLocalBoolVar,
  kDestroyLocalIntArrayVar,
  kDestroyLocalLongArrayVar,
  kDestroyLocalDoubleArrayVar,
  kDestroyLocalCharArrayVar,
  kDestroyLocalStringArrayVar,
  kDestroyLocalBoolArrayVar,
  kUnloadInt,
  kUnloadLong,
  kUnloadDouble,
  kUnloadBool,
  kUnloadChar,
  kUnloadString,
  kUnloadIntArray,
  kUnloadLongArray,
  kUnloadDoubleArray,
  kUnloadBoolArray,
  kUnloadCharArray,
  kUnloadStringArray,
  kLoadIntValue,
  kLoadLongValue,
  kLoadBoolValue,
  kLoadCharValue,
  kLoadStringValue,
  kLoadDoubleValue,
  kLoadFuncValue,
  kLoadNativeFuncValue,
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
  kLoadLocalIntArrayVarValue,
  kLoadLocalLongArrayVarValue,
  kLoadLocalDoubleArrayVarValue,
  kLoadLocalBoolArrayVarValue,
  kLoadLocalCharArrayVarValue,
  kLoadLocalStringArrayVarValue,
  kLoadGlobalVarAddress,
  kLoadLocalVarAddress,
  kLoadIntArrayElementValue,
  kLoadLongArrayElementValue,
  kLoadDoubleArrayElementValue,
  kLoadCharArrayElementValue,
  kLoadBoolArrayElementValue,
  kLoadStringArrayElementValue,
  kLoadIntArrayElementAddress,
  kLoadLongArrayElementAddress,
  kLoadDoubleArrayElementAddress,
  kLoadCharArrayElementAddress,
  kLoadBoolArrayElementAddress,
  kLoadStringArrayElementAddress,
  kStoreInt,
  kStoreLong,
  kStoreDouble,
  kStoreBool,
  kStoreChar,
  kStoreString,
  kStoreIntArray,
  kStoreLongArray,
  kStoreDoubleArray,
  kStoreBoolArray,
  kStoreCharArray,
  kStoreStringArray,
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
  kReturn,
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
  kDivDouble,
  kSumChar,
  kSumInt,
  kSumLong,
  kSumDouble,
  kSumString,
  kSubChar,
  kSubInt,
  kSubLong,
  kSubDouble,
  kEqualInt,
  kEqualLong,
  kEqualDouble,
  kEqualBool,
  kEqualChar,
  kEqualString,
  kEqualIntArray,
  kEqualLongArray,
  kEqualDoubleArray,
  kEqualBoolArray,
  kEqualCharArray,
  kEqualStringArray,
  kNotEqualInt,
  kNotEqualLong,
  kNotEqualDouble,
  kNotEqualBool,
  kNotEqualChar,
  kNotEqualString,
  kNotEqualIntArray,
  kNotEqualLongArray,
  kNotEqualDoubleArray,
  kNotEqualBoolArray,
  kNotEqualCharArray,
  kNotEqualStringArray,
  kGreaterChar,
  kGreaterInt,
  kGreaterLong,
  kGreaterDouble,
  kGreaterOrEqualChar,
  kGreaterOrEqualInt,
  kGreaterOrEqualLong,
  kGreaterOrEqualDouble,
  kLessChar,
  kLessInt,
  kLessLong,
  kLessDouble,
  kLessOrEqualChar,
  kLessOrEqualInt,
  kLessOrEqualLong,
  kLessOrEqualDouble,
  kPreDecChar,
  kPreDecInt,
  kPreDecLong,
  kPreDecDouble,
  kPreIncChar,
  kPreIncInt,
  kPreIncLong,
  kPreIncDouble,
  kLogicalNegateBool,
  kArithmeticNegateInt,
  kArithmeticNegateLong,
  kArithmeticNegateDouble
};

class CmdVisitor;

class Cmd {
 public:
  virtual ~Cmd() {}
  virtual void Accept(CmdVisitor *visitor) const = 0;

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
