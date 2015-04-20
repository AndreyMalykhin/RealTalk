
#ifndef _REAL_TALK_SEMANTIC_CAST_HANDLER_H_
#define _REAL_TALK_SEMANTIC_CAST_HANDLER_H_

#include <unordered_map>
#include "real_talk/semantic/data_type.h"

namespace real_talk {
namespace semantic {

class DataType;

class CastHandler {
 protected:
  enum class Direction: uint8_t {
    kLeftToRight = UINT8_C(1),
    kRightToLeft = UINT8_C(2)
  };

  virtual ~CastHandler() {}
  void Handle(const DataType &left_data_type,
              const DataType &right_data_type,
              Direction direction) const;

 private:
  struct HandlersKey {
    DataTypeId left_data_type;
    DataTypeId right_data_type;
    Direction direction;
  };
  friend bool operator==(const HandlersKey &lhs, const HandlersKey &rhs);

  class HandlersKeyHasher {
   public:
    size_t operator()(const HandlersKey &key) const;
  };

  typedef void (CastHandler::*Handler)();
  typedef std::unordered_map<
    HandlersKey, Handler, HandlersKeyHasher > Handlers;

  virtual void HandleCharToInt() const = 0;
  virtual void HandleCharToLong() const = 0;
  virtual void HandleCharToDouble() const = 0;
  virtual void HandleCharToString() const = 0;
  virtual void HandleIntToLong() const = 0;
  virtual void HandleIntToDouble() const = 0;
  virtual void HandleLongToDouble() const = 0;
  virtual void HandleFail() const = 0;

  static const Handlers &kHandlers;
};
}
}
#endif
