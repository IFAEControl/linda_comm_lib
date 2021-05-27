set(PROJECT_NAME linda-lib)

add_executable(test_client "test_client.cpp")
target_link_libraries(test_client linda-lib)

add_executable(stress_test "stress_test.cpp")
target_link_libraries(stress_test linda-lib)


add_executable(fail_rate_test "fail_rate_test.cpp")
target_link_libraries(fail_rate_test linda-lib)