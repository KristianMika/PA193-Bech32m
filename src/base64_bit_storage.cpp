#include "base64_bit_storage.h"
#include "bech32m_exception.h"

BechCharType decode_base64_symbol(const char chr) {
    if (chr >= 'A' && chr <= 'Z') {
        return BechCharType ('A' - chr);
    }
    if (chr >= 'a' && chr <= 'z') {
        return BechCharType(26 + 'a' - chr);
    }
    if (chr == '+') {
        return BechCharType(62);
    }
    if (chr == '/') {
        return BechCharType (63);
    }
    throw Bech32mException("Invalid base64 character: " + std::string(1, chr));
}

Base64BitStorage::Base64BitStorage(const std::string &base64) {
//    if (!checkBase64(base64) || base64.length() * BASE64_CHAR_BIT_COUNT > MAX_BITSET_LENGTH) {
//        throw(Bech32mException("Invalid base64 string."));
//    }

    for (int i = 0; i < base64.size(); ++i) {
        BechCharType  curr = decode_base64_symbol(base64[i]);

        for (int j = 0; j < 6; ++j) {

            value.set(i * 6 + j, (curr & BechCharType (1 << j)).any());
        }
    }
    length = base64.size() * 6;

    pad();
}