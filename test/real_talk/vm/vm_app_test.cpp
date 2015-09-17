
#include <boost/format.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "real_talk/code/code.h"
#include "real_talk/code/exe_reader.h"
#include "real_talk/code/exe.h"
#include "real_talk/util/file.h"
#include "real_talk/util/errors.h"
#include "real_talk/vm/native_func_storage.h"
#include "real_talk/vm/native_func_linker.h"
#include "real_talk/vm/vm.h"
#include "real_talk/vm/vm_cmd.h"
#include "real_talk/vm/vm_factory.h"
#include "real_talk/vm/vm_config_parser.h"
#include "real_talk/vm/vm_config.h"
#include "real_talk/vm/msg_printer.h"
#include "real_talk/vm/data_storage.h"
#include "real_talk/vm/vm_app.h"

using std::istream;
using std::stringstream;
using std::vector;
using std::string;
using std::move;
using std::unique_ptr;
using std::unordered_map;
using boost::filesystem::path;
using boost::format;
using testing::NiceMock;
using testing::Test;
using testing::Throw;
using testing::Eq;
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
using real_talk::util::IOError;

namespace real_talk {
namespace vm {
namespace {

class MsgPrinterMock: public MsgPrinter {
 public:
  MOCK_CONST_METHOD1(PrintHelp, void(const string&));
  MOCK_CONST_METHOD1(PrintError, void(const string&));
};

class VMConfigParserMock: public VMConfigParser {
 public:
  MOCK_CONST_METHOD0(GetHelp, string());
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
  MOCK_CONST_METHOD0(GetAll, const NativeFuncsMap&());
};

class NativeFuncLinkerMock: public NativeFuncLinker {
 public:
  MOCK_CONST_METHOD3(Link, void(const NativeFuncStorage::NativeFuncsMap&,
                                vector<NativeFuncValue>*, Exe*));
};

class VMMock: public VM {
 public:
  virtual const DataStorage &GetGlobalVars() const noexcept override {
    assert(false);
  }

  virtual const DataStorage &GetLocalVars() const noexcept override {
    assert(false);
  }

  virtual const DataStorage &GetOperands() const noexcept override {
    assert(false);
  }

  virtual DataStorage &GetOperands() noexcept override {
    assert(false);
  }

  virtual const FuncFrames &GetFuncFrames() const noexcept override {
    assert(false);
  }

  MOCK_METHOD0(Execute, void());
};

class VMFactoryMock: public VMFactory {
 public:
  virtual unique_ptr<VM> Create(
      Exe *exe,
      const vector<NativeFuncValue> &used_native_funcs) const override {
    return unique_ptr<VM>(Create_(exe, used_native_funcs));
  }

  MOCK_CONST_METHOD2(Create_, VM*(Exe*, const vector<NativeFuncValue>&));
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
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  uint32_t exe_version = UINT32_C(1);
  uint32_t main_cmds_code_size = UINT32_C(0);
  unique_ptr<Code> cmds(new Code());
  vector<string> native_func_defs;
  vector<IdAddresses> native_func_refs;
  uint32_t global_vars_size = UINT32_C(0);
  auto *exe = new Exe(exe_version,
                      move(cmds),
                      main_cmds_code_size,
                      global_vars_size,
                      native_func_defs,
                      native_func_refs);
  auto *exe_stream = new stringstream();
  vector<NativeFuncValue> used_native_funcs;
  NativeFuncStorage::NativeFuncsMap available_native_funcs;
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
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, ExecutionError) {
  auto *vm = new VMMock();
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  uint32_t exe_version = UINT32_C(1);
  uint32_t main_cmds_code_size = UINT32_C(0);
  unique_ptr<Code> cmds(new Code());
  vector<string> native_func_defs;
  vector<IdAddresses> native_func_refs;
  uint32_t global_vars_size = UINT32_C(0);
  auto *exe = new Exe(exe_version,
                      move(cmds),
                      main_cmds_code_size,
                      global_vars_size,
                      native_func_defs,
                      native_func_refs);
  auto *exe_stream = new stringstream();
  vector<NativeFuncValue> used_native_funcs;
  NativeFuncStorage::NativeFuncsMap available_native_funcs;
  VM::ExecutionError error("swag");
  string msg = "swag";
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
      .Times(1)
      .WillOnce(Throw(error));
  EXPECT_CALL(msg_printer, PrintError(msg))
      .Times(1);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, Help) {
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  string help = "test";
  EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
      .Times(1)
      .WillOnce(SetArgPointee<3>(VMCmd::kHelp));
  EXPECT_CALL(config_parser, GetHelp())
      .Times(1)
      .WillOnce(Return(help));
  EXPECT_CALL(msg_printer, PrintHelp(help))
      .Times(1);
  EXPECT_CALL(file, Read_(_))
      .Times(0);
  EXPECT_CALL(exe_reader, ReadFromStream_(_))
      .Times(0);
  EXPECT_CALL(native_func_storage, GetAll())
      .Times(0);
  EXPECT_CALL(native_func_linker, Link(_, _, _))
      .Times(0);
  EXPECT_CALL(vm_factory, Create_(_, _))
      .Times(0);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, MissingFuncErrorWhileLinkingNativeFuncs) {
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  uint32_t exe_version = UINT32_C(1);
  uint32_t main_cmds_code_size = UINT32_C(0);
  unique_ptr<Code> cmds(new Code());
  vector<string> native_func_defs;
  vector<IdAddresses> native_func_refs;
  uint32_t global_vars_size = UINT32_C(0);
  auto *exe = new Exe(exe_version,
                      move(cmds),
                      main_cmds_code_size,
                      global_vars_size,
                      native_func_defs,
                      native_func_refs);
  auto *exe_stream = new stringstream();
  NativeFuncStorage::NativeFuncsMap available_native_funcs;
  NativeFuncLinker::MissingFuncError error("func", "msg");
  string msg = "Function \"func\" is not exists";
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
      .Times(1)
      .WillOnce(Throw(error));
  EXPECT_CALL(msg_printer, PrintError(msg))
      .Times(1);
  EXPECT_CALL(vm_factory, Create_(_, _))
      .Times(0);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, IOErrorWhileReadingExeStream) {
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  auto *exe_stream = new stringstream();
  IOError error("msg");
  string msg = (format("Failed to read file %1%")
                % config.GetInputFilePath().string()).str();
  EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
      .Times(1)
      .WillOnce(SetArgPointee<3>(VMCmd::kExecute));
  EXPECT_CALL(file, Read_(config.GetInputFilePath().string()))
      .Times(1)
      .WillOnce(Return(exe_stream));
  EXPECT_CALL(exe_reader, ReadFromStream_(exe_stream))
      .Times(1)
      .WillOnce(Throw(error));
  EXPECT_CALL(msg_printer, PrintError(msg))
      .Times(1);
  EXPECT_CALL(native_func_storage, GetAll())
      .Times(0);
  EXPECT_CALL(native_func_linker, Link(_, _, _))
      .Times(0);
  EXPECT_CALL(vm_factory, Create_(_, _))
      .Times(0);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, CodeSizeOverflowErrorWhileReadingExe) {
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  auto *exe_stream = new stringstream();
  Code::CodeSizeOverflowError error("msg");
  string msg = "Code size exceeds 32 bits";
  EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
      .Times(1)
      .WillOnce(SetArgPointee<3>(VMCmd::kExecute));
  EXPECT_CALL(file, Read_(config.GetInputFilePath().string()))
      .Times(1)
      .WillOnce(Return(exe_stream));
  EXPECT_CALL(exe_reader, ReadFromStream_(exe_stream))
      .Times(1)
      .WillOnce(Throw(error));
  EXPECT_CALL(msg_printer, PrintError(msg))
      .Times(1);
  EXPECT_CALL(native_func_storage, GetAll())
      .Times(0);
  EXPECT_CALL(native_func_linker, Link(_, _, _))
      .Times(0);
  EXPECT_CALL(vm_factory, Create_(_, _))
      .Times(0);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, IOErrorWhileReadingExeFile) {
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  config.SetInputFilePath("/myproject/myapp.rte");
  IOError error("msg");
  string msg = (format("Failed to read file %1%")
                % config.GetInputFilePath().string()).str();
  EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
      .Times(1)
      .WillOnce(SetArgPointee<3>(VMCmd::kExecute));
  EXPECT_CALL(file, Read_(config.GetInputFilePath().string()))
      .Times(1)
      .WillOnce(Throw(error));
  EXPECT_CALL(msg_printer, PrintError(msg))
      .Times(1);
  EXPECT_CALL(exe_reader, ReadFromStream_(_))
      .Times(0);
  EXPECT_CALL(native_func_storage, GetAll())
      .Times(0);
  EXPECT_CALL(native_func_linker, Link(_, _, _))
      .Times(0);
  EXPECT_CALL(vm_factory, Create_(_, _))
      .Times(0);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}

TEST_F(VMAppTest, BadArgsErrorWhileParsingConfig) {
  VMFactoryMock vm_factory;
  NativeFuncLinkerMock native_func_linker;
  NativeFuncStorageMock native_func_storage;
  ExeReaderMock exe_reader;
  FileMock file;
  VMConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  int argc = 1;
  const char *argv[] = {"realtalkvm"};
  VMConfig config;
  VMConfigParser::BadArgsError error("msg");
  string msg = "Invalid arguments";
  EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
      .Times(1)
      .WillOnce(Throw(error));
  EXPECT_CALL(msg_printer, PrintError(msg))
      .Times(1);
  EXPECT_CALL(file, Read_(_))
      .Times(0);
  EXPECT_CALL(exe_reader, ReadFromStream_(_))
      .Times(0);
  EXPECT_CALL(native_func_storage, GetAll())
      .Times(0);
  EXPECT_CALL(native_func_linker, Link(_, _, _))
      .Times(0);
  EXPECT_CALL(vm_factory, Create_(_, _))
      .Times(0);
  VMApp app(config_parser,
            file,
            exe_reader,
            native_func_storage,
            native_func_linker,
            vm_factory,
            msg_printer,
            &config);
  app.Run(argc, argv);
}
}
}
