cmake_minimum_required (VERSION 3.0.2)
project(REAL_TALK)
include_directories(include)
include_directories(SYSTEM thirdparty/gmock-1.7.0/include thirdparty/gmock-1.7.0/gtest/include)
find_library(GMOCK_LIBRARY NAMES gmock PATHS thirdparty/gmock-1.7.0/build)
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost 1.56.0 REQUIRED COMPONENTS system filesystem)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Weverything -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-weak-vtables -Wno-padded -Wno-missing-noreturn -Wno-switch-enum -Wno-used-but-marked-unused")
add_library(real_talk
  src/real_talk/lexer/token_info.cpp
  src/real_talk/lexer/simple_lexer.cpp
  src/real_talk/lexer/lexer.cpp
  src/real_talk/lexer/simple_lexer_factory.cpp
  src/real_talk/parser/simple_parser.cpp
  src/real_talk/parser/parser.cpp
  src/real_talk/parser/simple_parser_factory.cpp
  src/real_talk/semantic/data_type.cpp
  src/real_talk/semantic/array_data_type.cpp
  src/real_talk/semantic/func_data_type.cpp
  src/real_talk/semantic/int_data_type.cpp
  src/real_talk/semantic/long_data_type.cpp
  src/real_talk/semantic/double_data_type.cpp
  src/real_talk/semantic/string_data_type.cpp
  src/real_talk/semantic/char_data_type.cpp
  src/real_talk/semantic/bool_data_type.cpp
  src/real_talk/semantic/void_data_type.cpp
  src/real_talk/semantic/lit.cpp
  src/real_talk/semantic/int_lit.cpp
  src/real_talk/semantic/long_lit.cpp
  src/real_talk/semantic/double_lit.cpp
  src/real_talk/semantic/string_lit.cpp
  src/real_talk/semantic/char_lit.cpp
  src/real_talk/semantic/bool_lit.cpp
  src/real_talk/semantic/semantic_analysis.cpp
  src/real_talk/semantic/node_semantic_analysis.cpp
  src/real_talk/semantic/semantic_problem.cpp
  src/real_talk/semantic/semantic_problems.cpp
  src/real_talk/semantic/local_var_def_analysis.cpp
  src/real_talk/semantic/global_var_def_analysis.cpp
  src/real_talk/semantic/func_def_analysis.cpp
  src/real_talk/semantic/base_expr_analysis.cpp
  src/real_talk/semantic/common_expr_analysis.cpp
  src/real_talk/semantic/lit_analysis.cpp
  src/real_talk/semantic/id_analysis.cpp
  src/real_talk/semantic/subscript_analysis.cpp
  src/real_talk/semantic/scope_analysis.cpp
  src/real_talk/semantic/control_flow_transfer_analysis.cpp
  src/real_talk/semantic/return_analysis.cpp
  src/real_talk/semantic/simple_semantic_analyzer.cpp
  src/real_talk/semantic/simple_import_file_searcher.cpp
  src/real_talk/semantic/simple_lit_parser.cpp
  src/real_talk/semantic/lit_parser.cpp
  src/real_talk/semantic/cast_resolver.cpp
  src/real_talk/semantic/cast_handler.cpp
  src/real_talk/code/simple_code_generator.cpp
  src/real_talk/code/cmd_reader.cpp
  src/real_talk/code/module_reader.cpp
  src/real_talk/code/module.cpp
  src/real_talk/code/id_addresses.cpp
  src/real_talk/code/id_address.cpp
  src/real_talk/code/code.cpp
  src/real_talk/code/simple_cast_cmd_generator.cpp
  src/real_talk/compiler/file_parser.cpp
  src/real_talk/compiler/compiler_config.cpp
  src/real_talk/compiler/compiler.cpp
  src/real_talk/util/errors.cpp
)
add_executable(real_talk_tests
  test/main.cpp
  test/real_talk/parser/simple_parser_test.cpp
  test/real_talk/lexer/simple_lexer_test.cpp
  test/real_talk/semantic/simple_semantic_analyzer_test.cpp
  test/real_talk/semantic/simple_import_file_searcher_test.cpp
  test/real_talk/semantic/simple_lit_parser_test.cpp
  test/real_talk/semantic/cast_resolver_test.cpp
  test/real_talk/code/simple_code_generator_test.cpp
  test/real_talk/code/module_reader_test.cpp
  test/real_talk/code/cmd_reader_test.cpp
  test/real_talk/code/test_utils.cpp
  test/real_talk/code/simple_cast_cmd_generator_test.cpp
  test/real_talk/compiler/compiler_test.cpp
  test/real_talk/util/endianness_test.cpp
  )
target_include_directories(real_talk_tests PRIVATE test)
target_link_libraries(real_talk_tests ${GMOCK_LIBRARY} ${Boost_LIBRARIES} real_talk)
