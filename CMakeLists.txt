cmake_minimum_required(VERSION 3.22)

project(RecordType CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RT_CXX_FLAGS -Wall -Wextra -pedantic -Wconversion -Wshadow)
set(RT_LINK_FLAGS)

# - Check if `-march=native` is available ---------------------------------------------------------
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(RT_CXX_FLAGS ${RT_CXX_FLAGS} -march=native)
endif()

# - Use ccache to cache compiled code -------------------------------------------------------------
option(USE_CCACHE "Use ccache to speed up compilation process" ON)
find_program(CCACHE ccache)
if (CCACHE AND USE_CCACHE)
    message(STATUS "Use ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
    message(STATUS "Do not use ccache")
endif()

# - Examples; default is ON -----------------------------------------------------------------------
option(BUILD_EXAMPLE "Build examples" ON)
if(BUILD_EXAMPLE)
    message(STATUS "Build examples")
    add_subdirectory(${CMAKE_SOURCE_DIR}/examples/)
endif()

# - Tests; default is OFF -------------------------------------------------------------------------
option(BUILD_TEST "Build tests using the google test library" OFF)
if(BUILD_TEST)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        1f643f71d4151c3b364c0e9302042f7a6debd439
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    message(STATUS "Build tests")
    add_subdirectory(${CMAKE_SOURCE_DIR}/test/)
endif()
