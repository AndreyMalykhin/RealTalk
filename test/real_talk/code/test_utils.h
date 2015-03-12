
#ifndef _REAL_TALK_CODE_TEST_UTILS_H_
#define _REAL_TALK_CODE_TEST_UTILS_H_

#include <iostream>

namespace real_talk {
namespace code {

class Code;
class Module;

void WriteModule(const Module &module, Code &module_code);
}
}
#endif

