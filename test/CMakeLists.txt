include_directories(${CTEST_SOURCE_DIRECTORY}/src)
add_executable(Test test_bech32m.cpp test_macros.h)
target_link_libraries(Test Bech32m)
