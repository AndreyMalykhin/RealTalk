
#ifndef _REAL_TALK_LINKER_SIMPLE_LINKER_FACTORY_H_
#define _REAL_TALK_LINKER_SIMPLE_LINKER_FACTORY_H_

#include <memory>
#include "real_talk/linker/linker_factory.h"

namespace real_talk {
namespace linker {

class SimpleLinkerFactory: public LinkerFactory {
 public:
  virtual std::unique_ptr<Linker> Create(
      OutputFileType output_file_type) const override;
};
}
}
#endif
