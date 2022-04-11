#ifndef BECH32M_BIN_BIT_STORAGE_H
#define BECH32M_BIN_BIT_STORAGE_H

#include "bit_storage.h"
static const uint16_t BYTE_BIT_COUNT = 8;
class BinBitStorage : public BitStorage {

  public:
    explicit BinBitStorage(const std::string &);
    explicit BinBitStorage(const BitStorage &storage) : BitStorage(storage) {}

    std::string to_string() const;
    BitStorage::Iterator<BYTE_BIT_COUNT> begin() const { return Iterator<BYTE_BIT_COUNT>(value, 0); }
    BitStorage::Iterator<BYTE_BIT_COUNT> end() const {
        return Iterator<BYTE_BIT_COUNT>(value, compute_padding(BYTE_BIT_COUNT) + length);
    }
};

#endif // BECH32M_BIN_BIT_STORAGE_H
