cmake_minimum_required(VERSION 3.11.0)
project(cqss VERSION 0.1.0 LANGUAGES C CXX)
enable_testing()

################################################################################
# install
################################################################################
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}"
        CACHE PATH "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}" FORCE)
endif()
message(STATUS "install: ${CMAKE_INSTALL_PREFIX}")

################################################################################
# 开启编译器最高特性
################################################################################
option(COMPILER_FEATURE "Enable compiler features." ON)
include(cmake/std.cmake)

################################################################################
# 拉取子模块
################################################################################
execute_process(COMMAND git submodule init)
execute_process(COMMAND git submodule update)

################################################################################
# 生成compile_commands.json
################################################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# compile_options
################################################################################
add_compile_options( -Wall)
add_compile_options( -g)
add_compile_options( -Wextra)
add_compile_options( -std=c++${CMAKE_CXX_STANDARD})

################################################################################
# find lib
################################################################################
# # boost
# find_package(Boost 1.80.0 REQUIRED)
# if(Boost_FOUND)
#     message(STATUS "Boost version: ${Boost_VERSION}")
# else()
#     message(FATAL_ERROR "Boost not found. Please install Boost.")
# endif()

# gtest
find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS "GTest version: ${GTest_VERSION}")
else()
    message(FATAL_ERROR "GTest not found. Please install GTest.")
endif()

################################################################################
# add internal lib
################################################################################
# set(INTERNAL_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
# add_library(testlib INTERFACE IMPORTED GLOBAL)
# target_include_directories(testlib INTERFACE {INTERNAL_LIB_DIR}/test/include)
# target_link_libraries(test INTERFACE ${INTERNAL_LIB_DIR}/test/lib/test.a)

################################################################################
# build dir
################################################################################
include_directories(src)
include_directories(lib)
include_directories(tools)
include_directories(submodule)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(test)

