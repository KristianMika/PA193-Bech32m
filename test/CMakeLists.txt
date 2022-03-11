include_directories(.)
include_directories(${CTEST_SOURCE_DIRECTORY}/src)
add_executable(Test test_bech32m.cpp test_macros.h test_bit_storage.cpp)
target_link_libraries(Test Bech32m)
