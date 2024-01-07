cmake_minimum_required(VERSION 3.21)

set(NaiveChainCpp_VERSION 0.1.0)

project(NaiveChainCpp
        VERSION ${NaiveChainCpp_VERSION}
        DESCRIPTION "Naive blockchain C++"
        HOMEPAGE_URL "https://github.com/wilricknl/NaiveChainCpp"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

## Interpret source as utf-8 on Windows to ensure
## non-ascii characters are read properly.
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

## Static builds on Windows
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -----------------------------------------------------------------------------
# DEPENDENCIES
# -----------------------------------------------------------------------------

find_package(Catch2 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_path(PICOSHA2_INCLUDE_DIRS "picosha2.h")

include_directories(include)

# -----------------------------------------------------------------------------
# NaiveChain library
# -----------------------------------------------------------------------------

add_library(naivechain
    src/block.cpp
    src/blockchain.cpp)
target_include_directories(naivechain
    PRIVATE
    ${PICOSHA2_INCLUDE_DIRS})

# -----------------------------------------------------------------------------
# NaiveChain node
# -----------------------------------------------------------------------------

add_executable(
    node
    tools/node/blockchain_window.cpp
    tools/node/blockchain_window.hpp
    tools/node/main.cpp
    tools/node/node.cpp
    tools/node/node.hpp
    tools/node/window.cpp
    tools/node/window.hpp)
target_link_libraries(
    node
    PRIVATE
    naivechain
	glad::glad
	glfw
    imgui::imgui
    OpenGL::GL)

# -----------------------------------------------------------------------------
# TEST
# -----------------------------------------------------------------------------

add_executable(
    test_naivechain
    test/test_block.cpp
    test/test_blockchain.cpp)
target_link_libraries(
    test_naivechain
    PRIVATE
    naivechain
    Catch2::Catch2
    Catch2::Catch2WithMain)
