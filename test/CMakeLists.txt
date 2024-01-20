include_directories(${CMAKE_SOURCE_DIR}/src/cmn)
message(STATUS ${CMAKE_SOURCE_DIR}/src/cmn)
add_subdirectory(cmn_test)

include_directories(${CMAKE_SOURCE_DIR}/src/log)
message(STATUS ${CMAKE_SOURCE_DIR}/src/log)
add_subdirectory(log_test)
