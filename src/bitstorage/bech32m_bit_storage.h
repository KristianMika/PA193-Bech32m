
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
    explicit Bech32mBitStorage(const Bech32mVector &bech32m, bool _trim = false);

    Bech32mBitStorage(const BitStorage &storage) : BitStorage(storage) {}

    ~Bech32mBitStorage() override = default;

    std::string to_string() const;
    BitStorage::Iterator<BECH32M_CHAR_BIT_COUNT> begin() const { return Iterator<BECH32M_CHAR_BIT_COUNT>(value, 0); }
    BitStorage::Iterator<BECH32M_CHAR_BIT_COUNT> end() const {
        if (trim) {
            return Iterator<BECH32M_CHAR_BIT_COUNT>(value, length - (length % BECH32M_CHAR_BIT_COUNT));
        } 
        return Iterator<BECH32M_CHAR_BIT_COUNT>(value, compute_padding(BECH32M_CHAR_BIT_COUNT) + length);
    }
};

#endif // BECH32M_BECH32MBITSTORAGE_H
