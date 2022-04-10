#include "bech32m_bit_storage.h"
#include "../bech32m.h"
#include "../bech32m_exception.h"
#include "bit_storage.h"

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

Bech32mBitStorage::Bech32mBitStorage(const Bech32mVector &bech32m, bool _trim) {
    for (const Bech32mChar bech_ch : bech32m) {
        for (int i = BECH32M_CHAR_BIT_COUNT - 1; i >= 0; --i) {
            value.set(length, bech_ch[i]);
            ++length;
        }
    }
    trim = _trim;
}

// TODO: make some functions inline
char to_char(const Bech32mChar &val) { return BECH_SYMBOLS[val.to_ulong()]; }

std::string Bech32mBitStorage::to_string() const {
    std::string out;
    for (const auto &val : *this) {
        out.push_back(to_char(val));
    }
    return out;
}
