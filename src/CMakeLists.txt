add_library(Bech32m_lib bech32m.cpp bit_storage.cpp hex_bit_storage.cpp base64_bit_storage.cpp bech32m_bit_storage.cpp argument_parser.cpp bech32m_error_detection.cpp bin_bit_storage.cpp)
add_executable(bech32m main.cpp)
target_link_libraries(bech32m Bech32m_lib)