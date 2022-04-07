#include "hex_bit_storage.h"
#include "bech32m_exception.h"

/**
 * Converts a hex character into its numeric form
 * @param ch a hex char
 * @return the numeric representation in a bitset of length 5. Even though the value will always fit into 4 bits, we are
 * returning it in a 5 bit bitset for compatibility reasons.
 */
Bech32mChar hex_2_int(char ch) {
    static const int HEX_A_VALUE = 10;
    if (ch >= '0' && ch <= '9') {
        return Bech32mChar(ch - '0');
    }
    if (ch >= 'a' && ch <= 'f') {
        return Bech32mChar(HEX_A_VALUE + ch - 'a');
    }
    throw Bech32mException("Invalid hex symbol: " + std::string(1, ch));
}
/**
 * Checks, if the input string is a valid hex string, i.e. the length is even and the string does not contain invalid
 * characters
 * @param str a hex string to be verified
 * @return true if the string is valid, false otherwise
 */
bool check_hex_string(const std::string &str) {
    static const std::string VALID_HEX_SYMBOLS = "0123456789ABCDEFabcdef";
    return str.length() % 2 == 0 && str.find_first_not_of(VALID_HEX_SYMBOLS);
}

/**
 * Checks the length of the input hex string
 * @param hex_string to be checked
 * @return true if the string can be encoded into Bech32m, false otherwise
 */
inline bool check_length(const std::string &hex_string) {
    return hex_string.length() * HEX_CHAR_BIT_COUNT <= BECH32M_MAX_BITSET_LENGTH;
}

HexBitStorage::HexBitStorage(const std::string &hex_value) {
    if (!check_hex_string(hex_value) || !check_length(hex_value)) {
        throw(Bech32mException("Invalid hex string."));
    }
    static const int MOST_SIGNIFICANT_HEX_BIT_MASK = 0b1000;

    for (int i = 0; i < hex_value.size(); ++i) {
        Bech32mChar curr = hex_2_int(hex_value[i]);

        for (int j = 0; j < HEX_CHAR_BIT_COUNT; ++j) {
            bool bit_val = (curr & Bech32mChar(MOST_SIGNIFICANT_HEX_BIT_MASK >> j)).any();
            value.set(i * HEX_CHAR_BIT_COUNT + j, bit_val);
        }
    }
    length = hex_value.size() * HEX_CHAR_BIT_COUNT;
}

static const std::string HEX_CHARS = "0123456789abcdef";

char to_char(const HexChar val) { return HEX_CHARS[val.to_ulong()]; }

std::string HexBitStorage::to_string() const {
    std::string out;
    for (const auto &val : *this) {
        out.push_back(to_char(val));
    }
    return out;
}
