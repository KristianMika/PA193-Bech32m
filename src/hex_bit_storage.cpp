#include "hex_bit_storage.h"
#include "bech32m_exception.h"

BechCharType hex_2_int(char ch) {
    if (ch >= '0' && ch <= '9') {
        return BechCharType (ch - '0');
    }
    if (ch >= 'a' && ch <= 'f') {
        return BechCharType (10 + ch - 'a');
    }
    throw Bech32mException("Invalid hex symbol: " + std::string(1, ch));
}

bool check_hex_string(const std::string &str) { return str.length() % 2 == 0 && str.find_first_not_of("0123456789ABCDEFabcdef"); }

HexBitStorage::HexBitStorage(const std::string &hex_value) {
    if (!check_hex_string(hex_value) || hex_value.length() * HEX_CHAR_BIT_COUNT > MAX_BITSET_LENGTH) {
        throw(Bech32mException("Invalid hex string."));
    }

    for (int i = 0; i < hex_value.size(); ++i) {
        BechCharType  curr = hex_2_int(hex_value[i]);

        for (int j = 0; j < 4; ++j) {

            value.set(i * 4 + j, (curr & BechCharType (1 << j)).any());
        }
    }
    length = hex_value.size() * 4;

    pad();
}
