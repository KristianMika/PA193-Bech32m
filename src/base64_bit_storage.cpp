#include "base64_bit_storage.h"
#include "bech32m_exception.h"

Bech32mChar decode_base64_symbol(const char chr) {
    if (chr >= 'A' && chr <= 'Z') {
        return Bech32mChar('A' - chr);
    }
    if (chr >= 'a' && chr <= 'z') {
        return Bech32mChar(26 + 'a' - chr);
    }
    if (chr == '+') {
        return Bech32mChar(62);
    }
    if (chr == '/') {
        return Bech32mChar(63);
    }
    throw Bech32mException("Invalid base64 character: " + std::string(1, chr));
}

Base64BitStorage::Base64BitStorage(const std::string &base64) {
    //    if (!checkBase64(base64) || base64.length() * BASE64_CHAR_BIT_COUNT > MAX_BITSET_LENGTH) {
    //        throw(Bech32mException("Invalid base64 string."));
    //    }

    for (int i = 0; i < base64.size(); ++i) {
        Bech32mChar curr = decode_base64_symbol(base64[i]);

        for (int j = 0; j < 6; ++j) {

            value.set(i * 6 + j, (curr & Bech32mChar(1 << j)).any());
        }
    }
    length = base64.size() * 6;

    pad();
}