set(PROJECT_NAME linda-lib)

add_executable(test_client "test_client.cpp")
target_link_libraries(test_client linda-lib)