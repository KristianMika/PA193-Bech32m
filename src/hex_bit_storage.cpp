#include "hex_bit_storage.h"
#include "bech32m_exception.h"

uint8_t hex_2_int(char ch) {
    if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    return 10 + ch - 'a';
}

bool check_hex_string(const std::string &str) { return str.length() % 2 == 0 && str.find_first_not_of("0123456789ABCDEFabcdef"); }

HexBitStorage::HexBitStorage(const std::string &hex_value) {
    if (!check_hex_string(hex_value) || hex_value.length() * HEX_CHAR_BIT_COUNT > MAX_BITSET_LENGTH) {
        throw(Bech32mException("Invalid hex string."));
    }

    for (int i = 0; i < hex_value.size(); ++i) {
        uint8_t curr = hex_2_int(hex_value[i]);

        for (int j = 0; j < 4; ++j) {

            value.set(i * 4 + j, curr & (1 << j));
        }
    }
    length = hex_value.size() * 4;

    // add padding
    int padding_length = 5 - (length % 5);
    for (int i = 0; i < padding_length; ++i) {
        value[length + i] = 0;
    }
    length += padding_length;
}
