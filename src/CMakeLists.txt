add_library(Bech32m bech32m.h bech32m.cpp)
add_executable(app main.cpp)
target_link_libraries(app Bech32m)