#include "../src/base64_bit_storage.h"
#include "../src/bech32m_bit_storage.h"
#include "../src/bech32m_exception.h"
#include "../src/hex_bit_storage.h"
#include "test_macros.h"

// counter for failed errors
extern long errors_count;
extern long success_count;

/**
 * TODO: more tests
 */
void test_hex_bit_storage() {
    // f    a    f    a    f    a
    // 1111 1010 1111 1010 1111 1010
    // 11111 01011 11101 01111 10100
    const std::string to_encode = "fafafa";
    Bech32mBitStorage storage = HexBitStorage(to_encode);
    int valid_length = static_cast<int>(to_encode.size()) * HEX_CHAR_BIT_COUNT; // 24
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
    Bech32mBitStorage storage = Base64BitStorage(to_encode);
    int valid_length = (static_cast<int>(to_encode.size()) - 2) * BASE64_CHAR_BIT_LENGTH; // 36, -2 for padding
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

    ASSERT_THROWS(Base64BitStorage("YTh2Cg."), Bech32mException);
}

// TODO: add more tests
void test_bech32m_bit_storage() {
    // l      t     a     0     5
    // 31     11    29    15    20
    // 11111  01011 11101 01111 10100
    const std::string to_encode = "lta05";
    Bech32mBitStorage storage(to_encode);
    int valid_length = (static_cast<int>(to_encode.size())) * BECH32M_CHAR_BIT_COUNT; // 25

    ASSERT_EQUALS(storage.size(), valid_length);
    auto it = storage.begin();
    ASSERT_EQUALS(*(it++), Bech32mChar(0b11111));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b01011));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b11101));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b01111));
    ASSERT_EQUALS(*(it++), Bech32mChar(0b10100));
    ASSERT_EQUALS(it, storage.end());
}

// TODO: more tests
// TODO: make a hex bitset type alias
void test_hex_iterator() {
    // l      t     a     0     5
    // 31     11    29    15    20
    // 11111  01011 11101 01111 10100
    // 1111 1010 1111 1010 1111 1010 0
    const std::string to_encode = "lta05";
    HexBitStorage storage = Bech32mBitStorage(to_encode);
    int storage_length = static_cast<int>(to_encode.size()) * BECH32M_CHAR_BIT_COUNT;
    ASSERT_EQUALS(storage.size(), storage_length);
    ASSERT_EQUALS(storage.size(), storage_length);

    auto it = storage.begin();
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b1111));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b1010));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b1111));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b1010));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b1111));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b1010));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0b0000));
    ASSERT_EQUALS(it, storage.end());
}

void test_insert() {
    HexBitStorage storage = HexBitStorage("fa");
    storage.insert(4, std::bitset<4>(0xd));

    auto it = storage.begin();
    ASSERT_EQUALS(*(it++), std::bitset<4>(0xf));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0xd));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0xa));
    // ASSERT_EQUALS(it, storage.end<4>());
}

void test_insert() {
    BitStorage storage = HexBitStorage("fa");
    storage.insert(4, std::bitset<4>(0xd));

    auto it = storage.begin<4>();
    ASSERT_EQUALS(*(it++), std::bitset<4>(0xf));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0xd));
    ASSERT_EQUALS(*(it++), std::bitset<4>(0xa));
    // ASSERT_EQUALS(it, storage.end<4>());
}
void test_bit_storage() {
    test_hex_bit_storage();
    test_base64_bit_storage();
    test_bech32m_bit_storage();
    test_hex_iterator();
    test_insert();
}