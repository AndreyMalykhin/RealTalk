
#ifndef _REAL_TALK_CODE_LOAD_GLOBAL_VAR_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_GLOBAL_VAR_VALUE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadGlobalVarValueCmd: public Cmd {
 public:
  inline explicit LoadGlobalVarValueCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadGlobalVarValueCmd &rhs =
        static_cast<const LoadGlobalVarValueCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};

class LoadGlobalIntVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalIntVarValue(*this);
  }
};

class LoadGlobalLongVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalLongVarValue(*this);
  }
};

class LoadGlobalDoubleVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalDoubleVarValue(*this);
  }
};

class LoadGlobalCharVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalCharVarValue(*this);
  }
};

class LoadGlobalStringVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalStringVarValue(*this);
  }
};

class LoadGlobalBoolVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalBoolVarValue(*this);
  }
};

class LoadGlobalIntArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalIntArrayVarValue(*this);
  }
};

class LoadGlobalLongArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalLongArrayVarValue(*this);
  }
};

class LoadGlobalDoubleArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalDoubleArrayVarValue(*this);
  }
};

class LoadGlobalCharArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalCharArrayVarValue(*this);
  }
};

class LoadGlobalBoolArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalBoolArrayVarValue(*this);
  }
};

class LoadGlobalStringArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  using LoadGlobalVarValueCmd::LoadGlobalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadGlobalStringArrayVarValue(*this);
  }
};
}
}
#endif
