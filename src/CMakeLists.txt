add_library(Bech32m bech32m.h bech32m.cpp bech32m_exception.h bit_storage.h bit_storage.cpp hex_bit_storage.h hex_bit_storage.cpp base64_bit_storage.h base64_bit_storage.cpp)
add_executable(app main.cpp)
target_link_libraries(app Bech32m)