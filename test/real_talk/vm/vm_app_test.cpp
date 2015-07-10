
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/exe_reader.h"
#include "real_talk/code/exe.h"
#include "real_talk/util/file.h"
#include "real_talk/vm/native_func_storage.h"
#include "real_talk/vm/native_func_linker.h"
#include "real_talk/vm/vm.h"
#include "real_talk/vm/vm_cmd.h"
#include "real_talk/vm/vm_factory.h"
#include "real_talk/vm/vm_config_parser.h"
#include "real_talk/vm/vm_config.h"
#include "real_talk/vm/vm_app.h"

using std::istream;
using std::stringstream;
using std::vector;
using std::string;
using std::move;
using std::unique_ptr;
using std::unordered_map;
using boost::filesystem::path;
using testing::Test;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::SetArgPointee;
using testing::NotNull;
using testing::_;
using real_talk::code::Code;
using real_talk::code::ExeReader;
using real_talk::code::Exe;
using real_talk::code::IdAddresses;
using real_talk::util::File;

namespace real_talk {
namespace vm {
namespace {

class VMConfigParserMock: public VMConfigParser {
 public:
  MOCK_CONST_METHOD4(Parse, void(int, const char*[], VMConfig*, VMCmd*));
};

class FileMock: public File {
 public:
  virtual unique_ptr<istream> Read(const path& file_path) const override {
    return unique_ptr<istream>(Read_(file_path.string()));
  }

  virtual void Write(const path&, const Code&) const override {
    assert(false);
  }

  MOCK_CONST_METHOD1(Read_, istream*(const string&));
};

class ExeReaderMock: public ExeReader {
 public:
  virtual unique_ptr<Exe> ReadFromStream(istream *code) const override {
    return unique_ptr<Exe>(ReadFromStream_(code));
  }

  virtual unique_ptr<Exe> ReadFromCode(Code*) const override {
    assert(false);
  }

  MOCK_CONST_METHOD1(ReadFromStream_, Exe*(istream*));
};

class NativeFuncStorageMock: public NativeFuncStorage {
 public:
  MOCK_CONST_METHOD0(GetAll, const unordered_map<string, NativeFunc>&());
};

class NativeFuncLinkerMock: public NativeFuncLinker {
 public:
  MOCK_CONST_METHOD3(Link, void(
      const unordered_map<string, NativeFunc>&, vector<NativeFunc>*, Exe*));
};

class VMMock: public VM {
 public:
  MOCK_CONST_METHOD0(Execute, void());
};

class VMFactoryMock: public VMFactory {
 public:
  virtual unique_ptr<VM> Create(
      Exe *exe, const vector<NativeFunc> &used_native_funcs) const override {
    return unique_ptr<VM>(Create_(exe, used_native_funcs));
  }

  MOCK_CONST_METHOD2(Create_, VM*(Exe*, const vector<NativeFunc>&));
};
}

class VMAppTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(VMAppTest, Execute) {
  auto *vm = new VMMock();
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  uint32_t exe_version = UINT32_C(1);
  uint32_t main_cmds_code_size = UINT32_C(0);
  unique_ptr<Code> cmds(new Code());
  vector<string> native_func_defs;
  vector<IdAddresses> native_func_refs;
  auto *exe = new Exe(exe_version,
                      move(cmds),
                      main_cmds_code_size,
                      native_func_defs,
                      native_func_refs);
  auto *exe_stream = new stringstream();
  vector<NativeFunc> used_native_funcs;
  unordered_map<string, NativeFunc> available_native_funcs;
  EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
      .Times(1)
      .WillOnce(SetArgPointee<3>(VMCmd::kExecute));
  EXPECT_CALL(file, Read_(config.GetInputFilePath().string()))
      .Times(1)
      .WillOnce(Return(exe_stream));
  EXPECT_CALL(exe_reader, ReadFromStream_(exe_stream))
      .Times(1)
      .WillOnce(Return(exe));
  EXPECT_CALL(native_func_storage, GetAll())
      .Times(1)
      .WillOnce(ReturnRef(available_native_funcs));
  EXPECT_CALL(native_func_linker,
              Link(Ref(available_native_funcs), NotNull(), exe))
      .Times(1);
  EXPECT_CALL(vm_factory, Create_(exe, used_native_funcs))
      .Times(1)
      .WillOnce(Return(vm));
  EXPECT_CALL(*vm, Execute())
      .Times(1);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            &config);
  app.Run(argc, argv);
}
}
}
