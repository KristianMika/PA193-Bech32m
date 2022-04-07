#include "bin_bit_storage.h"
#include <algorithm>

BinBitStorage::BinBitStorage(const std::string &in) {
    unsigned char mask = 0b10000000;
    for (const char ch : in) {
        for (int i = 0; i < 8; ++i) {
            value.set(length++, ch & (mask >> i));
        }
    }
}

std::string BinBitStorage::to_string() const {
    std::string out;
    std::transform(begin(), end(), std::back_inserter(out),
                   [](const std::bitset<8> &val) -> char { return val.to_ulong(); });
    return out;
}
