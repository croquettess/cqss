# cmake/std.cmake

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using ${CMAKE_CXX_COMPILER_ID} compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using ${CMAKE_CXX_COMPILER_ID} compiler")
else()
    message(FATAL_ERROR "Unsupported compiler. Only GCC and Clang.")
endif()

if(COMPILER_FEATURE)
    check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)
    check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX20)

    if(COMPILER_SUPPORTS_CXX20)
        set(CMAKE_CXX_STANDARD 20)
    elseif(COMPILER_SUPPORTS_CXX17)
        set(CMAKE_CXX_STANDARD 17)
    else()
        message(FATAL_ERROR "Cxx standard < c++17")
    endif()

    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    message(STATUS "Using -std=c++${CMAKE_CXX_STANDARD}")
endif()

