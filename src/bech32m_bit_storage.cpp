#include "bech32m_bit_storage.h"
#include "bech32m.h"
#include "bech32m_exception.h"

Bech32mBitStorage::Bech32mBitStorage(const std::string &bech32m) {
    for (const char bech_ch : bech32m) {
        size_t ind = BECH_SYMBOLS.find(bech_ch);
        if (ind == std::string::npos) {
            throw Bech32mException("Invalid Bech32m character");
        }

        uint16_t BECH32M_CHAR_MOST_SIGNIFICANT_BIT_MASK = 16;
        for (int i = 0; i < BECH32M_CHAR_BIT_COUNT; ++i) {
            value.set(length, ind & (BECH32M_CHAR_MOST_SIGNIFICANT_BIT_MASK >> i));
            ++length;
        }
    }
}