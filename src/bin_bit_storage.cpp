#include "bin_bit_storage.h"


BinBitStorage::BinBitStorage(const std::string & in) {
    unsigned char mask = 0b10000000;
    for (const char ch:in) {
        for (int i = 0; i < 8; ++i) {
            value.set(length++, ch & (mask >> i));
        }
    }
}
