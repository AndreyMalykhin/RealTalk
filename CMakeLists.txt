cmake_minimum_required (VERSION 3.3.0)
project(REAL_TALK)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Weverything -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-weak-vtables -Wno-padded -Wno-missing-noreturn -Wno-switch-enum -Wno-used-but-marked-unused -Wno-tautological-constant-out-of-range-compare -Wno-float-equal")

include_directories(include)
include_directories(SYSTEM
  thirdparty/boost
  thirdparty/googletest/googlemock/include
  thirdparty/googletest/googletest/include
)

find_library(gmock_lib NAMES gmock PATHS thirdparty/googletest/googlemock/build NO_DEFAULT_PATH)
find_package(Threads REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_NO_SYSTEM_PATHS ON)
set(BOOST_ROOT thirdparty/boost/build)
find_package(Boost 1.59.0 REQUIRED COMPONENTS system filesystem program_options)

add_library(real_talk_lib SHARED
  src/real_talk/lexer/token_info.cpp
  src/real_talk/lexer/simple_lexer.cpp
  src/real_talk/lexer/lexer.cpp
  src/real_talk/lexer/simple_lexer_factory.cpp
  src/real_talk/parser/simple_parser.cpp
  src/real_talk/parser/parser.cpp
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
  src/real_talk/semantic/simple_lit_parser.cpp
  src/real_talk/semantic/lit_parser.cpp
  src/real_talk/semantic/cast_resolver.cpp
  src/real_talk/semantic/cast_handler.cpp
  src/real_talk/code/simple_code_generator.cpp
  src/real_talk/code/cmd_reader.cpp
  src/real_talk/code/simple_module_reader.cpp
  src/real_talk/code/simple_exe_reader.cpp
  src/real_talk/code/module.cpp
  src/real_talk/code/exe.cpp
  src/real_talk/code/code_container.cpp
  src/real_talk/code/simple_code_container_writer.cpp
  src/real_talk/code/id_addresses.cpp
  src/real_talk/code/id_address.cpp
  src/real_talk/code/id_size.cpp
  src/real_talk/code/code.cpp
  src/real_talk/code/simple_cast_cmd_generator.cpp
  src/real_talk/compiler/simple_msg_printer.cpp
  src/real_talk/compiler/compiler_config.cpp
  src/real_talk/compiler/simple_compiler_config_parser.cpp
  src/real_talk/compiler/compiler_app.cpp
  src/real_talk/linker/linker_config.cpp
  src/real_talk/linker/linker_app.cpp
  src/real_talk/linker/base_linker.cpp
  src/real_talk/linker/lib_linker.cpp
  src/real_talk/linker/exe_linker.cpp
  src/real_talk/linker/simple_linker_factory.cpp
  src/real_talk/linker/simple_msg_printer.cpp
  src/real_talk/linker/simple_linker_config_parser.cpp
  src/real_talk/vm/data_storage.cpp
  src/real_talk/vm/vm_config.cpp
  src/real_talk/vm/vm_app.cpp
  src/real_talk/vm/array_value.cpp
  src/real_talk/vm/string_value.cpp
  src/real_talk/vm/simple_vm.cpp
  src/real_talk/vm/simple_vm_factory.cpp
  src/real_talk/vm/simple_native_func_storage.cpp
  src/real_talk/vm/simple_msg_printer.cpp
  src/real_talk/vm/simple_vm_config_parser.cpp
  src/real_talk/vm/simple_native_func_linker.cpp
  src/real_talk/stdlib/io.cpp
  src/real_talk/util/errors.cpp
  src/real_talk/util/logger.cpp
  src/real_talk/util/simple_dir_creator.cpp
  src/real_talk/util/simple_file.cpp
  src/real_talk/util/simple_file_searcher.cpp
)
target_link_libraries(real_talk_lib ${Boost_LIBRARIES})
set_target_properties(real_talk_lib PROPERTIES OUTPUT_NAME realtalk)

add_executable(real_talk_tests_exe
  test/main.cpp
  test/real_talk/parser/simple_parser_test.cpp
  test/real_talk/lexer/simple_lexer_test.cpp
  test/real_talk/semantic/simple_semantic_analyzer_test.cpp
  test/real_talk/semantic/simple_lit_parser_test.cpp
  test/real_talk/semantic/cast_resolver_test.cpp
  test/real_talk/code/simple_code_generator_test.cpp
  test/real_talk/code/simple_module_reader_test.cpp
  test/real_talk/code/simple_exe_reader_test.cpp
  test/real_talk/code/simple_code_container_writer_test.cpp
  test/real_talk/code/cmd_reader_test.cpp
  test/real_talk/code/simple_cast_cmd_generator_test.cpp
  test/real_talk/compiler/simple_msg_printer_test.cpp
  test/real_talk/compiler/simple_compiler_config_parser_test.cpp
  test/real_talk/compiler/compiler_app_test.cpp
  test/real_talk/linker/linker_app_test.cpp
  test/real_talk/linker/lib_linker_test.cpp
  test/real_talk/linker/exe_linker_test.cpp
  test/real_talk/linker/simple_linker_factory_test.cpp
  test/real_talk/linker/simple_msg_printer_test.cpp
  test/real_talk/linker/simple_linker_config_parser_test.cpp
  test/real_talk/vm/vm_app_test.cpp
  test/real_talk/vm/simple_vm_test.cpp
  test/real_talk/vm/simple_msg_printer_test.cpp
  test/real_talk/vm/simple_vm_config_parser_test.cpp
  test/real_talk/vm/simple_native_func_linker_test.cpp
  test/real_talk/util/simple_dir_creator_test.cpp
  test/real_talk/util/simple_file_test.cpp
  test/real_talk/util/simple_file_searcher_test.cpp
  test/real_talk/util/endianness_test.cpp
)
target_include_directories(real_talk_tests_exe PRIVATE test)
target_link_libraries(real_talk_tests_exe ${CMAKE_THREAD_LIBS_INIT} ${gmock_lib} real_talk_lib)
set_target_properties(real_talk_tests_exe PROPERTIES OUTPUT_NAME realtalktests)

add_executable(real_talk_compiler_exe src/real_talk/compiler/main.cpp)
target_link_libraries(real_talk_compiler_exe real_talk_lib)
set_target_properties(real_talk_compiler_exe PROPERTIES OUTPUT_NAME realtalkc)

add_executable(real_talk_linker_exe src/real_talk/linker/main.cpp)
target_link_libraries(real_talk_linker_exe real_talk_lib)
set_target_properties(real_talk_linker_exe PROPERTIES OUTPUT_NAME realtalkl)

add_executable(real_talk_vm_exe src/real_talk/vm/main.cpp)
target_link_libraries(real_talk_vm_exe real_talk_lib)
set_target_properties(real_talk_vm_exe PROPERTIES OUTPUT_NAME realtalkvm)

file(COPY test/fixtures DESTINATION .)

install(TARGETS real_talk_compiler_exe real_talk_linker_exe real_talk_vm_exe real_talk_lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(DIRECTORY stdlib DESTINATION share/realtalk)
add_custom_target(std_lib_dir COMMAND make WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/realtalk/stdlib)
add_custom_target(fixtures_dir COMMAND make WORKING_DIRECTORY fixtures/myapp)
add_dependencies(fixtures_dir std_lib_dir)
add_custom_target(post_install)
add_dependencies(post_install fixtures_dir std_lib_dir)
