#include "base64_bit_storage.h"
#include "../bech32m_exception.h"

static const std::string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";
static const uint16_t BASE64_CHAR_MOST_SIGNIFICANT_BIT_MASK = 0b100000;

/**
 * Decodes a single Base64 variable
 * @param chr a character to be decoded
 * @return the decoded variable as
 */
uint8_t decode_base64_symbol(const char chr) {
    size_t val = BASE64_CHARS.find(chr);
    if (val == std::string::npos) {
        throw Bech32mException("Invalid base64 symbol: " + std::string(1, chr));
    }
    return val;
}

/**
 * Validates a base64 string
 * @param base64_str a hex string to be validated
 * @return true if base64_str is a valid base64 string, else otherwise
 */
bool checkBase64(const std::string &base64_str) {
    return base64_str.find_first_not_of(BASE64_CHARS) == std::string::npos;
}

/**
 * Checks if the base64 input chan be encoded into Bech32m
 * @param base64 to be checked
 * @return true if the code can fit the limits, false otherwise
 */
bool checkBase64length(const std::string &base64) {
    return base64.length() * BASE64_CHAR_BIT_LENGTH <= BECH32M_MAX_BITSET_LENGTH;
}

/**
 * Returns the index of the first padding symbol (=)
 * @param str
 * @return the index of the first padding symbol
 */
int getPaddingIndex(const std::string &str) {
    for (int i = static_cast<int>(str.size()) - 1; i >= 0; --i) {
        if (str[i] != '=') {
            return i;
        }
    }
    throw Bech32mException("Invalid Base64 format: " + str);
}

Base64BitStorage::Base64BitStorage(const std::string &base64, bool _trim) {
    int padding_index = getPaddingIndex(base64);
    std::string trimmed_string(base64.begin(), base64.begin() + padding_index + 1);

    // TODO: check if the input is a valid base64, mostly padding length, length with padding, etc.
    if (!checkBase64(trimmed_string) || !checkBase64length(trimmed_string)) {
        throw(Bech32mException("Invalid base64 string: " + base64));
    }

    for (size_t i = 0; i < trimmed_string.size(); ++i) {
        uint8_t curr = decode_base64_symbol(trimmed_string[i]);
        for (int j = 0; j < BASE64_CHAR_BIT_LENGTH; ++j) {
            bool bit = (curr & BASE64_CHAR_MOST_SIGNIFICANT_BIT_MASK >> j);
            value.set(i * BASE64_CHAR_BIT_LENGTH + j, bit);
        }
    }
    length = trimmed_string.size() * BASE64_CHAR_BIT_LENGTH;
    trim = _trim;
}
char to_char(const Base64Char &val) { return BASE64_CHARS[val.to_ulong()]; }
std::string Base64BitStorage::to_string() const {
    std::string out;
    for (const auto &val : *this) {
        out.push_back(to_char(val));
    }

    if (trim && out[out.size() - 1] == 'A' && out.size() % BASE64_CHAR_BIT_LENGTH != 0) {
        out.pop_back();
    }

    return out;
}
