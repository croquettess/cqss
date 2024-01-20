add_executable(log_test log_test.cc)
target_link_libraries(log_test GTest::GTest GTest::Main)
add_test(NAME LOG_TEST COMMAND log_test)
