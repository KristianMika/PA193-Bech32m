#include "../src/hex_bit_storage.h"
#include "test_macros.h"

// counter for failed errors
extern long errors_count;

/**
 * TODO: more tests
 */
void test_hex_bit_storage() {
    // f    a    f    a    f    a
    // 1111 1010 1111 1010 1111 1010
    // 11111 01011 11101 01111 10100
    const std::string to_encode = "fafafa";
    HexBitStorage storage = HexBitStorage(to_encode);
    int valid_length = to_encode.size() * HEX_CHAR_BIT_COUNT; // 24
    valid_length += 1;                                        // padding
    ASSERT_EQUALS(storage.size(), valid_length);
    auto it = storage.begin();
    ASSERT_EQUALS(*(it++), Bech32mChar(0b11111));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b01011));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b11101));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b01111));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b10100));
    ASSERT_EQUALS(it, storage.end());
}