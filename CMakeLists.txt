cmake_minimum_required (VERSION 3.0.2)
project(REAL_TALK)
include_directories(include)
include_directories(SYSTEM thirdparty/gmock-1.7.0/include thirdparty/gmock-1.7.0/gtest/include)
find_library(GMOCK_LIBRARY NAMES gmock PATHS thirdparty/gmock-1.7.0/build)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Weverything -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-weak-vtables -Wno-padded -Wno-missing-noreturn -Wno-switch-enum -Wno-used-but-marked-unused")
# add_executable(real_talk
#   src/main.cpp
# )
add_library(real_talk
  src/real_talk/parser/simple_parser.cpp
  src/real_talk/lexer/token_info.cpp
  src/real_talk/lexer/simple_lexer.cpp
)
add_executable(real_talk_tests
  test/main.cpp
  test/real_talk/parser/simple_parser_test.cpp
  test/real_talk/lexer/simple_lexer_test.cpp
)
# target_link_libraries(real_talk ${BOOST_LIBRARIES})
target_link_libraries(real_talk_tests ${GMOCK_LIBRARY} real_talk)
