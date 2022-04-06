add_library(Bech32m bech32m.cpp bit_storage.cpp hex_bit_storage.cpp base64_bit_storage.cpp bech32m_bit_storage.cpp argument_parser.cpp)
add_executable(app main.cpp)
target_link_libraries(app Bech32m)