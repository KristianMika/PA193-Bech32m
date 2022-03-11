#include "bit_storage.h"

std::bitset<5> BitStorage::Iterator::operator*() const {
    std::bitset<5> num = 0;
    for (int i = 0; i < BECH32M_CHAR_BIT_COUNT; ++i) {
        num = num << 1;
        num |= (*ptr)[index + i];
    }
    return num;
}