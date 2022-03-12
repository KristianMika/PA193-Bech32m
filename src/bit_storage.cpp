#include "bit_storage.h"

std::bitset<5> BitStorage::Iterator::operator*() const {
    std::bitset<5> num = 0;
    for (int i = 0; i < BECH32M_CHAR_BIT_COUNT; ++i) {
        num = num << 1;
        num |= (*ptr)[index + i];
    }
    return num;
}

void BitStorage::pad() {
    // TODO: check bounds
    int padding_length = 5 - (length % 5);
    for (int i = 0; i < padding_length; ++i) {
        value[length + i] = false;
    }
    length += padding_length;
}