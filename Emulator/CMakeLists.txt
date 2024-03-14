cmake_minimum_required (VERSION 3.8)
project (Runner)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

option(SetDebugBuildType "Set on if want not to build as release and use O2" OFF)

if (NOT SetDebugBuildType)
    set(CMAKE_BUILD_TYPE Release)
    add_compile_options(-O2)
endif()

set(PATH_TO_PROGRAM "/Users/lehatrutenb/Desktop/hse/1_course/C++/Emulator/Emulator/Emulator/Runner/Fib")
configure_file(${PROJECT_SOURCE_DIR}/Runner/include/runner.hpp.in ${PROJECT_SOURCE_DIR}/Runner/include/runner.hpp)

add_executable(Runner Runner/src/main.cpp)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_subdirectory(Stack)
add_subdirectory(Emulator)
add_subdirectory(Parser)
add_subdirectory(Preprocessor)
