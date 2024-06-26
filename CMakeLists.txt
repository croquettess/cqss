cmake_minimum_required(VERSION 3.11.0)
project(cqss VERSION 0.1.0 LANGUAGES C CXX)
enable_testing()

################################################################################
# install
################################################################################
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/app"
        CACHE PATH "${CMAKE_SOURCE_DIR}/app" FORCE)
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
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake")

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

# glog
find_package(glog REQUIRED)
if(glog_FOUND)
    message(STATUS "glog version: ${glog_VERSION}")
else()
    message(FATAL_ERROR "glog not found. Please install glog.")
endif()

# absl
find_package(absl REQUIRED)
if(absl_FOUND)
    message(STATUS "absl version: ${absl_VERSION}")
else()
    message(FATAL_ERROR "absl not found. Please install absl.")
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
include_directories(./)
include_directories(lib)
include_directories(tools)
include_directories(submodule)
add_subdirectory(cqss)
add_subdirectory(tools)
add_subdirectory(test)

