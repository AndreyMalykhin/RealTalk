
#ifndef _REAL_TALK_CODE_CMD_VISITOR_H_
#define _REAL_TALK_CODE_CMD_VISITOR_H_

namespace real_talk {
namespace code {

class AndCmd;
class CreateGlobalIntVarCmd;
class CreateGlobalArrayVarCmd;
class CreateGlobalLongVarCmd;
class CreateGlobalDoubleVarCmd;
class CreateGlobalCharVarCmd;
class CreateGlobalStringVarCmd;
class CreateGlobalBoolVarCmd;
class CreateLocalIntVarCmd;
class CreateLocalArrayVarCmd;
class CreateLocalLongVarCmd;
class CreateLocalDoubleVarCmd;
class CreateLocalCharVarCmd;
class CreateLocalStringVarCmd;
class CreateLocalBoolVarCmd;
class DestroyLocalIntVarCmd;
class DestroyLocalArrayVarCmd;
class DestroyLocalLongVarCmd;
class DestroyLocalDoubleVarCmd;
class DestroyLocalCharVarCmd;
class DestroyLocalStringVarCmd;
class UnloadArrayCmd;
class UnloadLongCmd;
class UnloadDoubleCmd;
class UnloadCharCmd;
class UnloadStringCmd;
class UnloadBoolCmd;
class LoadIntValueCmd;
class LoadLongValueCmd;
class LoadBoolValueCmd;
class LoadCharValueCmd;
class LoadStringValueCmd;
class LoadDoubleValueCmd;
class LoadFuncValueCmd;
class LoadNativeFuncValueCmd;

class CmdVisitor {
 public:
  virtual ~CmdVisitor() {}
  virtual void VisitCreateGlobalIntVar(
      const CreateGlobalIntVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalArrayVar(
      const CreateGlobalArrayVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalLongVar(
      const CreateGlobalLongVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalDoubleVar(
      const CreateGlobalDoubleVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalCharVar(
      const CreateGlobalCharVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalStringVar(
      const CreateGlobalStringVarCmd &cmd) = 0;
  virtual void VisitCreateGlobalBoolVar(
      const CreateGlobalBoolVarCmd &cmd) = 0;
  virtual void VisitCreateLocalIntVar(
      const CreateLocalIntVarCmd &cmd) = 0;
  virtual void VisitCreateLocalArrayVar(
      const CreateLocalArrayVarCmd &cmd) = 0;
  virtual void VisitCreateLocalLongVar(
      const CreateLocalLongVarCmd &cmd) = 0;
  virtual void VisitCreateLocalDoubleVar(
      const CreateLocalDoubleVarCmd &cmd) = 0;
  virtual void VisitCreateLocalCharVar(
      const CreateLocalCharVarCmd &cmd) = 0;
  virtual void VisitCreateLocalStringVar(
      const CreateLocalStringVarCmd &cmd) = 0;
  virtual void VisitCreateLocalBoolVar(
      const CreateLocalBoolVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalIntVar(
      const DestroyLocalIntVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalArrayVar(
      const DestroyLocalArrayVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalLongVar(
      const DestroyLocalLongVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalDoubleVar(
      const DestroyLocalDoubleVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalCharVar(
      const DestroyLocalCharVarCmd &cmd) = 0;
  virtual void VisitDestroyLocalStringVar(
      const DestroyLocalStringVarCmd &cmd) = 0;
  virtual void VisitUnloadArray(
      const UnloadArrayCmd &cmd) = 0;
  virtual void VisitUnloadLong(
      const UnloadLongCmd &cmd) = 0;
  virtual void VisitUnloadDouble(
      const UnloadDoubleCmd &cmd) = 0;
  virtual void VisitUnloadChar(
      const UnloadCharCmd &cmd) = 0;
  virtual void VisitUnloadString(
      const UnloadStringCmd &cmd) = 0;
  virtual void VisitUnloadBool(
      const UnloadBoolCmd &cmd) = 0;
  virtual void VisitLoadIntValue(
      const LoadIntValueCmd &cmd) = 0;
  virtual void VisitLoadLongValue(
      const LoadLongValueCmd &cmd) = 0;
  virtual void VisitLoadBoolValue(
      const LoadBoolValueCmd &cmd) = 0;
  virtual void VisitLoadCharValue(
      const LoadCharValueCmd &cmd) = 0;
  virtual void VisitLoadStringValue(
      const LoadStringValueCmd &cmd) = 0;
  virtual void VisitLoadDoubleValue(
      const LoadDoubleValueCmd &cmd) = 0;
  virtual void VisitLoadFuncValue(
      const LoadFuncValueCmd &cmd) = 0;
  virtual void VisitLoadNativeFuncValue(
      const LoadNativeFuncValueCmd &cmd) = 0;
  virtual void VisitAnd(
      const AndCmd &cmd) = 0;
};
}
}
#endif
