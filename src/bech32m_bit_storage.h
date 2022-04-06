
#ifndef BECH32M_BECH32MBITSTORAGE_H
#define BECH32M_BECH32MBITSTORAGE_H
#include "base64_bit_storage.h"
#include "bit_storage.h"
#include <vector>
/**
 * Serves for Bech32m decoding
 */
class Bech32mBitStorage : public BitStorage {
  public:
    explicit Bech32mBitStorage(const std::string &bech32m);
    explicit Bech32mBitStorage(const std::vector<Bech32mChar> &bech32m);

    Bech32mBitStorage(const BitStorage &storage) : BitStorage(storage) {}

    ~Bech32mBitStorage() override = default;

    // TODO: a possible duplicate
    BitStorage::Iterator<BECH32M_CHAR_BIT_COUNT> begin() const { return Iterator<BECH32M_CHAR_BIT_COUNT>(value, 0); }
    BitStorage::Iterator<BECH32M_CHAR_BIT_COUNT> end() const {
        return Iterator<BECH32M_CHAR_BIT_COUNT>(value, compute_padding(BECH32M_CHAR_BIT_COUNT) + length);
    }
};

#endif // BECH32M_BECH32MBITSTORAGE_H
