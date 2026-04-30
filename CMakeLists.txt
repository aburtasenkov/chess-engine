cmake_minimum_required(VERSION 3.13)
project(chess-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# separate engine logic from the main executable
# this allows the unit tests to link against the engine logic without causing errors
file(GLOB_RECURSE engine_logic "src/main/*.cpp")
list(FILTER engine_logic EXCLUDE REGEX "main\\.cpp$")
set(main_file "src/main/main.cpp")

# core engine library
add_library(engine_lib STATIC ${engine_logic})
target_include_directories(engine_lib PUBLIC src/main)

# main exec
add_executable(chess-engine ${main_file})
target_link_libraries(chess-engine PRIVATE engine_lib)

# cross platform options
# link time optimizations to allow the compiler to inline functions across different translation units
if (MSVC)
  target_compile_options(engine_lib PUBLIC /W4 /WX /O2 /Oi /Ot /GL)
  target_link_options(engine_lib INTERFACE /LTCG)
else ()
  target_compile_options(engine_lib PUBLIC -O3 -flto=auto -march=native -Wall -Wextra -Werror)
  target_link_options(engine_lib INTERFACE -flto)
endif()

# unit tests
find_package(GTest)
if (GTEST_FOUND)
  file(GLOB_RECURSE sources_test "src/test/*.cpp")
  add_executable(unit_tests ${sources_test})

  # GTest::gtest_main provides the test entry point automatically
  target_link_libraries(unit_tests PRIVATE GTest::gtest_main engine_lib)
  target_compile_definitions(unit_tests PRIVATE UNIT_TESTS)
endif()

install(TARGETS chess-engine DESTINATION bin)