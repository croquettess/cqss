add_executable(err_test err_test.cc)
target_link_libraries(err_test GTest::GTest GTest::Main)
add_test(NAME ERRC_TEST COMMAND err_test)

add_executable(design_test design_test.cc)
target_link_libraries(design_test GTest::GTest GTest::Main)
add_test(NAME DESIGN_TEST COMMAND design_test)
