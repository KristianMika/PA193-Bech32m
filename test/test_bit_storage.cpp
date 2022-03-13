#include "../src/base64_bit_storage.h"
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
    HexBitStorage storage(to_encode);
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
/**
 * TODO: more tests
 */
void test_base64_bit_storage() {
    // Y      T      h      2      C      g==
    // 24     19     33     54     2      32
    // 011000 010011 100001 110110 000010 100000
    // 01100 00100 11100 00111 01100 00010 10000 00000
    const std::string to_encode = "YTh2Cg==";
    Base64BitStorage storage(to_encode);
    int valid_length = (to_encode.size() - 2) * BASE64_CHAR_BIT_LENGTH; // 36, -2 for padding
    valid_length += 4;                                                  // padding
    ASSERT_EQUALS(storage.size(), valid_length);
    auto it = storage.begin();
    ASSERT_EQUALS(*(it++), Bech32mChar(0b01100));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b00100));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b11100));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b00111));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b01100));

    ASSERT_EQUALS(*(it++), Bech32mChar(0b00010));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b10000));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b00000));
    ASSERT_EQUALS(it, storage.end());
}

void test_bit_storage() {
    test_hex_bit_storage();
    test_base64_bit_storage();
}