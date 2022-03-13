#include "bit_storage.h"
#include "bech32m_exception.h"

void BitStorage::pad(int char_bit_length) {
    int padding_length = char_bit_length - (length % char_bit_length);
    padding_length %= char_bit_length;

    if (length + padding_length > BECH32M_MAX_BITSET_LENGTH) {
        throw Bech32mException("Invalid Bech32m internal length");
    }

    for (int i = 0; i < padding_length; ++i) {
        value[length + i] = false;
    }

    length += padding_length;
}