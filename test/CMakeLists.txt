include_directories(${CTEST_SOURCE_DIRECTORY}/src)
add_executable(Test test_bech32m.cpp)
target_link_libraries(Test Bech32m)
