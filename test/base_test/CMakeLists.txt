add_executable(errc_test errc_test.cc)
target_link_libraries(errc_test GTest::GTest GTest::Main)
add_test(NAME ERRC_TEST COMMAND errc_test)

