
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "real_talk/util/errors.h"
#include "real_talk/util/dir_creator.h"
#include "real_talk/util/file.h"
#include "real_talk/util/file_searcher.h"
#include "real_talk/code/code.h"
#include "real_talk/code/module.h"
#include "real_talk/code/module_reader.h"
#include "real_talk/code/code_container.h"
#include "real_talk/code/code_container_writer.h"
#include "real_talk/linker/linker_cmd.h"
#include "real_talk/linker/linker_config.h"
#include "real_talk/linker/linker_config_parser.h"
#include "real_talk/linker/linker_factory.h"
#include "real_talk/linker/linker.h"
#include "real_talk/linker/msg_printer.h"
#include "real_talk/linker/linker_app.h"

using std::string;
using std::unique_ptr;
using std::vector;
using std::istream;
using std::stringstream;
using testing::Test;
using testing::Ref;
using testing::InSequence;
using testing::NotNull;
using testing::Return;
using testing::Throw;
using testing::_;
using testing::SetArgPointee;
using boost::filesystem::path;
using boost::format;
using real_talk::util::IOError;
using real_talk::util::DirCreator;
using real_talk::util::File;
using real_talk::util::FileSearcher;
using real_talk::code::Code;
using real_talk::code::CodeContainer;
using real_talk::code::CodeContainerWriter;
using real_talk::code::ModuleReader;
using real_talk::code::Module;
using real_talk::code::Exe;

namespace real_talk {
namespace linker {
namespace {

class LinkerConfigParserMock: public LinkerConfigParser {
 public:
  MOCK_CONST_METHOD4(
      Parse, void(int, const char*[], LinkerConfig*, LinkerCmd*));
  MOCK_CONST_METHOD0(GetHelp, string());
};

class MsgPrinterMock: public MsgPrinter {
 public:
  MOCK_CONST_METHOD1(PrintError, void(const string&));
  MOCK_CONST_METHOD1(PrintHelp, void(const string&));
};

class DirCreatorMock: public DirCreator {
 public:
  virtual void Create(const path &dir_path) const override {
    Create_(dir_path.string());
  }

  MOCK_CONST_METHOD1(Create_, void(const string&));
};

class FileMock: public File {
 public:
  virtual unique_ptr<istream> Read(const path &file_path) const override {
    return unique_ptr<istream>(Read_(file_path.string()));
  }

  virtual void Write(const path &file_path, const Code &code) const override {
    Write_(file_path.string(), code);
  }

  MOCK_CONST_METHOD1(Read_, istream*(const string&));
  MOCK_CONST_METHOD2(Write_, void(const string&, const Code&));
};

class ModuleReaderMock: public ModuleReader {
 public:
  virtual unique_ptr<Module> ReadFromCode(Code*) const override {
    assert(false);
  }

  virtual unique_ptr<Module> ReadFromStream(
      std::istream *code_stream) const override {
    return unique_ptr<Module>(ReadFromStream_(code_stream));
  }

  MOCK_CONST_METHOD1(ReadFromStream_, Module*(istream*));
};

class LinkerFactoryMock: public LinkerFactory {
 public:
  virtual unique_ptr<Linker> Create(OutputFileType output_file_type) const override {
    return unique_ptr<Linker>(Create_(output_file_type));
  }

  MOCK_CONST_METHOD1(Create_, Linker*(OutputFileType));
};

class LinkerMock: public Linker {
 public:
  virtual unique_ptr<CodeContainer> Link(
      const Linker::Modules &modules,
      uint32_t output_code_version) const override {
    vector<Module*> transformed_modules;

    for (const unique_ptr<Module> &module: modules) {
      transformed_modules.push_back(module.get());
    }

    return unique_ptr<CodeContainer>(
        Link_(transformed_modules, output_code_version));
  }

  MOCK_CONST_METHOD2(Link_, CodeContainer*(vector<Module*>, uint32_t));
};

class CodeContainerWriterMock: public CodeContainerWriter {
 public:
  virtual void Write(const Module&, Code*) const override {
    assert(false);
  }

  virtual void Write(const Exe&, Code*) const override {
    assert(false);
  }

  MOCK_CONST_METHOD2(Write, void(const CodeContainer&, Code*));
};

class FileSearcherMock: public FileSearcher {
 public:
  virtual path Search(const path &file_path,
                      const path &src_dir_path,
                      const path &vendor_dir_path,
                      const vector<path> &import_dir_paths) const override {
    vector<string> transformed_import_dir_paths;

    for (const path &import_dir_path: import_dir_paths) {
      transformed_import_dir_paths.push_back(import_dir_path.string());
    }

    return path(Search_(file_path.string(),
                        src_dir_path.string(),
                        vendor_dir_path.string(),
                        transformed_import_dir_paths));
  }

  MOCK_CONST_METHOD4(Search_, string(const string&,
                                     const string&,
                                     const string&,
                                     const vector<string>&));
};
}

class LinkerAppTest: public Test {
 protected:
  virtual void SetUp() override {}
  virtual void TearDown() override {}
};

TEST_F(LinkerAppTest, Link) {
  struct TestModuleRead {
    path search_input_file_path;
    path found_input_file_path;
    istream *input_file_stream;
    Module *module;
  };
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  auto *linker = new LinkerMock();
  uint32_t output_code_version = UINT32_C(1);
  auto *output_code_container = new Module(
      output_code_version,
      unique_ptr<Code>(new Code()),
      0,
      {},
      {},
      {},
      {},
      {},
      {});
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  path output_dir_path("build2/bin2/app");
  path output_file_path("build2/bin2/app/lib.rtl2");
  LinkerConfig config;
  config.SetInputFilePaths(
      vector<path>({"app/module/component.rtm", "app/module/component2.rtm"}));
  config.SetOutputFilePath("app/lib.rtl2");
  config.SetOutputFileType(OutputFileType::kLib);
  config.SetBinDirPath("build2/bin2");
  config.SetVendorDirPath("vendor2");
  config.SetImportDirPaths(vector<path>({"/mylib", "/mylib2"}));
  vector<string> import_dir_paths;

  for (const path &path: config.GetImportDirPaths()) {
    import_dir_paths.push_back(path.string());
  }

  vector<TestModuleRead> test_module_reads;
  vector<Module*> modules;

  {
    path search_input_file_path("app/module/component.rtm");
    path found_input_file_path("build2/bin2/app/module/component.rtm");
    auto *input_file_stream = new stringstream();
    auto *module = new Module(
        2, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
    TestModuleRead test_module_read = {search_input_file_path,
                                       found_input_file_path,
                                       input_file_stream,
                                       module};
    test_module_reads.push_back(test_module_read);
    modules.push_back(module);
  }

  {
    path search_input_file_path("app/module/component2.rtm");
    path found_input_file_path("build2/bin2/app/module/component2.rtm");
    auto *input_file_stream = new stringstream();
    auto *module = new Module(
        3, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
    TestModuleRead test_module_read = {search_input_file_path,
                                       found_input_file_path,
                                       input_file_stream,
                                       module};
    test_module_reads.push_back(test_module_read);
    modules.push_back(module);
  }

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));

    for (const TestModuleRead &test_module_read: test_module_reads) {
      EXPECT_CALL(file_searcher, Search_(
          test_module_read.search_input_file_path.string(),
          config.GetBinDirPath().string(),
          config.GetVendorDirPath().string(),
          import_dir_paths))
          .Times(1)
          .WillOnce(Return(test_module_read.found_input_file_path.string()))
          .RetiresOnSaturation();
      EXPECT_CALL(file, Read_(test_module_read.found_input_file_path.string()))
          .Times(1)
          .WillOnce(Return(test_module_read.input_file_stream))
          .RetiresOnSaturation();
      EXPECT_CALL(module_reader,
                  ReadFromStream_(test_module_read.input_file_stream))
          .Times(1)
          .WillOnce(Return(test_module_read.module))
          .RetiresOnSaturation();
    }

    EXPECT_CALL(linker_factory, Create_(config.GetOutputFileType()))
        .Times(1)
        .WillOnce(Return(linker));
    EXPECT_CALL(*linker, Link_(modules, output_code_version))
        .Times(1)
        .WillOnce(Return(output_code_container));
    EXPECT_CALL(code_container_writer,
                Write(Ref(*output_code_container), &output_code))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
        .Times(1);
    EXPECT_CALL(file, Write_(output_file_path.string(), Ref(output_code)))
        .Times(1);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, Help) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  string help = "test";

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kHelp));
    EXPECT_CALL(config_parser, GetHelp())
        .Times(1)
        .WillOnce(Return(help));
    EXPECT_CALL(msg_printer, PrintHelp(help))
        .Times(1);
    EXPECT_CALL(file_searcher, Search_(_, _, _, _))
        .Times(0);
    EXPECT_CALL(file, Read_(_))
        .Times(0);
    EXPECT_CALL(module_reader, ReadFromStream_(_))
        .Times(0);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, IOErrorWhileWritingFile) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  auto *linker = new LinkerMock();
  uint32_t output_code_version = UINT32_C(1);
  auto *output_code_container = new Module(
      output_code_version,
      unique_ptr<Code>(new Code()),
      0,
      {},
      {},
      {},
      {},
      {},
      {});
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  path output_dir_path("build2/bin2/app");
  path output_file_path("build2/bin2/app/lib.rtl2");
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetOutputFilePath("app/lib.rtl2");
  config.SetOutputFileType(OutputFileType::kLib);
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();
  auto *module = new Module(
      2, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
  vector<Module*> modules;
  modules.push_back(module);
  string msg = (format("Failed to write output file \"%1%\"")
                % output_file_path.string()).str();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(linker_factory, Create_(config.GetOutputFileType()))
        .Times(1)
        .WillOnce(Return(linker));
    EXPECT_CALL(*linker, Link_(modules, output_code_version))
        .Times(1)
        .WillOnce(Return(output_code_container));
    EXPECT_CALL(code_container_writer,
                Write(Ref(*output_code_container), &output_code))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
        .Times(1);
    EXPECT_CALL(file, Write_(output_file_path.string(), Ref(output_code)))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, IOErrorWhileCreatingDir) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  auto *linker = new LinkerMock();
  uint32_t output_code_version = UINT32_C(1);
  auto *output_code_container = new Module(
      output_code_version,
      unique_ptr<Code>(new Code()),
      0,
      {},
      {},
      {},
      {},
      {},
      {});
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  path output_dir_path("build2/bin2/app");
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetOutputFilePath("app/lib.rtl2");
  config.SetOutputFileType(OutputFileType::kLib);
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();
  auto *module = new Module(
      2, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
  vector<Module*> modules;
  modules.push_back(module);
  string msg = (format("Failed to create output folder \"%1%\"")
                % output_dir_path.string()).str();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(linker_factory, Create_(config.GetOutputFileType()))
        .Times(1)
        .WillOnce(Return(linker));
    EXPECT_CALL(*linker, Link_(modules, output_code_version))
        .Times(1)
        .WillOnce(Return(output_code_container));
    EXPECT_CALL(code_container_writer,
                Write(Ref(*output_code_container), &output_code))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(output_dir_path.string()))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, CodeSizeOverflowErrorWhileWritingCode) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  auto *linker = new LinkerMock();
  uint32_t output_code_version = UINT32_C(1);
  auto *output_code_container = new Module(
      output_code_version,
      unique_ptr<Code>(new Code()),
      0,
      {},
      {},
      {},
      {},
      {},
      {});
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetOutputFileType(OutputFileType::kLib);
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();
  auto *module = new Module(
      2, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
  vector<Module*> modules;
  modules.push_back(module);

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(linker_factory, Create_(config.GetOutputFileType()))
        .Times(1)
        .WillOnce(Return(linker));
    EXPECT_CALL(*linker, Link_(modules, output_code_version))
        .Times(1)
        .WillOnce(Return(output_code_container));
    EXPECT_CALL(code_container_writer,
                Write(Ref(*output_code_container), &output_code))
        .Times(1)
        .WillOnce(Throw(Code::CodeSizeOverflowError("test")));
    EXPECT_CALL(msg_printer, PrintError("Code size exceeds 32 bits"))
        .Times(1);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, DuplicateDefErrorWhileLinkingModules) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  auto *linker = new LinkerMock();
  uint32_t output_code_version = UINT32_C(1);
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetOutputFileType(OutputFileType::kLib);
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();
  auto *module = new Module(
      2, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
  vector<Module*> modules;
  modules.push_back(module);

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(linker_factory, Create_(config.GetOutputFileType()))
        .Times(1)
        .WillOnce(Return(linker));
    EXPECT_CALL(*linker, Link_(modules, output_code_version))
        .Times(1)
        .WillOnce(Throw(Linker::DuplicateDefError("var", "test")));
    EXPECT_CALL(msg_printer, PrintError("Duplicate definition of id \"var\""))
        .Times(1);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, CodeSizeOverflowErrorWhileLinkingModules) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  auto *linker = new LinkerMock();
  uint32_t output_code_version = UINT32_C(1);
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetOutputFileType(OutputFileType::kLib);
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();
  auto *module = new Module(
      2, unique_ptr<Code>(new Code()), 0, {}, {}, {}, {}, {}, {});
  vector<Module*> modules;
  modules.push_back(module);

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Return(module));
    EXPECT_CALL(linker_factory, Create_(config.GetOutputFileType()))
        .Times(1)
        .WillOnce(Return(linker));
    EXPECT_CALL(*linker, Link_(modules, output_code_version))
        .Times(1)
        .WillOnce(Throw(Code::CodeSizeOverflowError("test")));
    EXPECT_CALL(msg_printer, PrintError("Code size exceeds 32 bits"))
        .Times(1);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, CodeSizeOverflowErrorWhileReadingModule) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Throw(Code::CodeSizeOverflowError("test")));
    EXPECT_CALL(msg_printer, PrintError("Code size exceeds 32 bits"))
        .Times(1);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, IOErrorWhileReadingFileStream) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  auto *input_file_stream = new stringstream();
  string msg = (format("Failed to read input file %1%")
                % found_input_file_path.string()).str();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Return(input_file_stream));
    EXPECT_CALL(module_reader, ReadFromStream_(input_file_stream))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, IOErrorWhileReadingFile) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  config.SetBinDirPath("build2/bin2");
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path("build2/bin2/app/module/component.rtm");
  string msg = (format("Failed to read input file %1%")
                % found_input_file_path.string()).str();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(file, Read_(found_input_file_path.string()))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(module_reader, ReadFromStream_(_))
        .Times(0);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, IOErrorWhileSearchingFile) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  string msg = (format("IO error while searching file %1%")
                % search_input_file_path.string()).str();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Throw(IOError("test")));
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(file, Read_(_))
        .Times(0);
    EXPECT_CALL(module_reader, ReadFromStream_(_))
        .Times(0);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, FileNotExists) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;
  config.SetInputFilePaths(vector<path>({"app/module/component.rtm"}));
  vector<string> import_dir_paths;
  path search_input_file_path("app/module/component.rtm");
  path found_input_file_path;
  string msg = (format("File not found %1%")
                % search_input_file_path.string()).str();

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(SetArgPointee<3>(LinkerCmd::kLink));
    EXPECT_CALL(file_searcher, Search_(
        search_input_file_path.string(),
        config.GetBinDirPath().string(),
        config.GetVendorDirPath().string(),
        import_dir_paths))
        .Times(1)
        .WillOnce(Return(found_input_file_path.string()));
    EXPECT_CALL(msg_printer, PrintError(msg))
        .Times(1);
    EXPECT_CALL(file, Read_(_))
        .Times(0);
    EXPECT_CALL(module_reader, ReadFromStream_(_))
        .Times(0);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}

TEST_F(LinkerAppTest, BadArgsErrorWhileParsingConfig) {
  LinkerConfigParserMock config_parser;
  MsgPrinterMock msg_printer;
  FileSearcherMock file_searcher;
  ModuleReaderMock module_reader;
  DirCreatorMock dir_creator;
  FileMock file;
  Code output_code;
  LinkerFactoryMock linker_factory;
  CodeContainerWriterMock code_container_writer;
  LinkerMock linker;
  int argc = 1;
  const char *argv[] = {"realtalkl"};
  LinkerConfig config;

  {
    InSequence sequence;
    EXPECT_CALL(config_parser, Parse(argc, argv, &config, NotNull()))
        .Times(1)
        .WillOnce(Throw(LinkerConfigParser::BadArgsError("test")));
    EXPECT_CALL(msg_printer, PrintError("Invalid arguments"))
        .Times(1);
    EXPECT_CALL(file_searcher, Search_(_, _, _, _))
        .Times(0);
    EXPECT_CALL(file, Read_(_))
        .Times(0);
    EXPECT_CALL(module_reader, ReadFromStream_(_))
        .Times(0);
    EXPECT_CALL(linker_factory, Create_(_))
        .Times(0);
    EXPECT_CALL(linker, Link_(_, _))
        .Times(0);
    EXPECT_CALL(code_container_writer, Write(_, _))
        .Times(0);
    EXPECT_CALL(dir_creator, Create_(_))
        .Times(0);
    EXPECT_CALL(file, Write_(_, _))
        .Times(0);
  }

  LinkerApp app(config_parser,
                msg_printer,
                file_searcher,
                module_reader,
                linker_factory,
                code_container_writer,
                dir_creator,
                file,
                &config,
                &output_code);
  app.Run(argc, argv);
}
}
}
