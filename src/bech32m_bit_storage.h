
#ifndef BECH32M_BECH32MBITSTORAGE_H
#define BECH32M_BECH32MBITSTORAGE_H
#include "bit_storage.h"

/**
 * Serves for Bech32m decoding
 */
class Bech32mBitStorage : public BitStorage {
  public:
    explicit Bech32mBitStorage(const std::string &bech32m);
    ~Bech32mBitStorage() override = default;
};

#endif // BECH32M_BECH32MBITSTORAGE_H
