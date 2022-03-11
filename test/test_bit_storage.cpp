#include "../src/hex_bit_storage.h"
#include "test_macros.h"

// counter for failed errors
extern long errors_count;

/**
 * TODO: more tests
 */
void test_hex_bit_storage() {
    const std::string to_encode = "fafafa";
    HexBitStorage storage = HexBitStorage(to_encode);
    int valid_length = to_encode.size() * 4 + 5 - (to_encode.size() * 4 % 5);
    ASSERT_EQUALS(storage.size(), valid_length);
    auto it = storage.begin();
    ASSERT_EQUALS(*(it++), BechCharType(0b11110));
    ASSERT_EQUALS(*(it++), BechCharType(0b10111));
    ASSERT_EQUALS(*(it++), BechCharType(0b11010));
    ASSERT_EQUALS(*(it++), BechCharType(0b11111));
    ASSERT_EQUALS(*(it++), BechCharType(0b01010));
    ASSERT_EQUALS(it, storage.end());

}