#include "bit_storage.h"
#include "bech32m_exception.h"

Bech32mChar BitStorage::Iterator::operator*() const {
    Bech32mChar num = 0;
    for (int i = 0; i < BECH32M_CHAR_BIT_COUNT; ++i) {
        num = num << 1;
        num |= (*ptr)[index + i];
    }
    return num;
}

void BitStorage::pad() {
    int padding_length = BECH32M_CHAR_BIT_COUNT - (length % BECH32M_CHAR_BIT_COUNT);
    padding_length %= BECH32M_CHAR_BIT_COUNT;

    if (length + padding_length > BECH32M_MAX_BITSET_LENGTH) {
        throw Bech32mException("Invalid Bech32m internal length");
    }

    for (int i = 0; i < padding_length; ++i) {
        value[length + i] = false;
    }

    length += padding_length;
}