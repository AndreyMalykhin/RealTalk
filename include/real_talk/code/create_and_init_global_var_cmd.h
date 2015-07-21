
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_

#include <cassert>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitGlobalVarCmd: public Cmd {
 public:
  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 protected:
  inline explicit CreateAndInitGlobalVarCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateAndInitGlobalVarCmd &rhs =
        static_cast<const CreateAndInitGlobalVarCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

 private:
  uint32_t var_index_;
};

class CreateAndInitGlobalIntVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalIntVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalIntVar(*this);
  }
};

class CreateAndInitGlobalLongVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalLongVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalLongVar(*this);
  }
};

class CreateAndInitGlobalDoubleVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalDoubleVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalDoubleVar(*this);
  }
};

class CreateAndInitGlobalCharVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalCharVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalCharVar(*this);
  }
};

class CreateAndInitGlobalStringVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalStringVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalStringVar(*this);
  }
};

class CreateAndInitGlobalBoolVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalBoolVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalBoolVar(*this);
  }
};

class CreateAndInitGlobalArrayVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline uint8_t GetDimensionsCount() const noexcept {return dimensions_count_;}

  inline void SetDimensionsCount(uint8_t count) noexcept {
    dimensions_count_ = count;
  }

 protected:
  inline CreateAndInitGlobalArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalVarCmd(var_index),
        dimensions_count_(dimensions_count) {
        assert(dimensions_count != UINT8_C(0));
      }

 private:
  virtual void Print(std::ostream &stream) const override {
    CreateAndInitGlobalVarCmd::Print(stream);
    stream << "; dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const CreateAndInitGlobalArrayVarCmd&>(cmd);
    return CreateAndInitGlobalVarCmd::IsEqual(cmd)
        && dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class CreateAndInitGlobalIntArrayVarCmd: public CreateAndInitGlobalArrayVarCmd {
 public:
  inline CreateAndInitGlobalIntArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalArrayVarCmd(var_index, dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalIntArrayVar(*this);
  }
};

class CreateAndInitGlobalLongArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  inline CreateAndInitGlobalLongArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalArrayVarCmd(var_index, dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalLongArrayVar(*this);
  }
};

class CreateAndInitGlobalDoubleArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  inline CreateAndInitGlobalDoubleArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalArrayVarCmd(var_index, dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalDoubleArrayVar(*this);
  }
};

class CreateAndInitGlobalCharArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  inline CreateAndInitGlobalCharArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalArrayVarCmd(var_index, dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalCharArrayVar(*this);
  }
};

class CreateAndInitGlobalStringArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  inline CreateAndInitGlobalStringArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalArrayVarCmd(var_index, dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalStringArrayVar(*this);
  }
};

class CreateAndInitGlobalBoolArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  inline CreateAndInitGlobalBoolArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalArrayVarCmd(var_index, dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalBoolArrayVar(*this);
  }
};
}
}
#endif
