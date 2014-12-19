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
  src/real_talk/lexer/simple_lexer_factory.cpp
  src/real_talk/parser/simple_parser.cpp
  src/real_talk/parser/simple_parser_factory.cpp
  src/real_talk/parser/simple_file_parser.cpp
  src/real_talk/semantic/data_type.cpp
  src/real_talk/semantic/array_data_type.cpp
  src/real_talk/semantic/bounded_array_data_type.cpp
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
  src/real_talk/semantic/semantic_hint.cpp
  src/real_talk/semantic/semantic_error.cpp
  src/real_talk/semantic/def_analysis.cpp
  src/real_talk/semantic/var_def_analysis.cpp
  src/real_talk/semantic/arg_def_analysis.cpp
  src/real_talk/semantic/func_def_analysis.cpp
  src/real_talk/semantic/expr_analysis.cpp
  src/real_talk/semantic/lit_analysis.cpp
  src/real_talk/semantic/id_analysis.cpp
  src/real_talk/semantic/import_analysis.cpp
  src/real_talk/semantic/data_storage.cpp
  src/real_talk/semantic/local_data_storage.cpp
  src/real_talk/semantic/global_data_storage.cpp
  src/real_talk/semantic/simple_semantic_analyzer.cpp
  src/real_talk/semantic/simple_import_file_searcher.cpp
  src/real_talk/semantic/simple_lit_parser.cpp
)
add_executable(real_talk_tests
  test/main.cpp
  test/real_talk/parser/simple_parser_test.cpp
  test/real_talk/parser/simple_file_parser_test.cpp
  test/real_talk/lexer/simple_lexer_test.cpp
  test/real_talk/semantic/simple_semantic_analyzer_test.cpp
  test/real_talk/semantic/simple_import_file_searcher_test.cpp
)
target_link_libraries(real_talk_tests ${GMOCK_LIBRARY} ${Boost_LIBRARIES} real_talk)
