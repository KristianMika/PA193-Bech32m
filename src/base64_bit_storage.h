#ifndef BECH32M_BASE64_BIT_STORAGE_H
#define BECH32M_BASE64_BIT_STORAGE_H

#include "bit_storage.h"
#include "hex_bit_storage.h"

static const uint16_t BASE64_CHAR_BIT_LENGTH = 6;

/**
 * Represents a Base64 encoded input
 */
class Base64BitStorage : public BitStorage {
  public:
    explicit Base64BitStorage(const std::string &base64);
    Base64BitStorage(const BitStorage &storage) : BitStorage(storage) {}
    ~Base64BitStorage() override = default;

    BitStorage::Iterator<BASE64_CHAR_BIT_LENGTH> begin() const { return Iterator<BASE64_CHAR_BIT_LENGTH>(value, 0); }
    BitStorage::Iterator<BASE64_CHAR_BIT_LENGTH> end() const {
        return Iterator<BASE64_CHAR_BIT_LENGTH>(value, compute_padding(BASE64_CHAR_BIT_LENGTH) + length);
    }
};
#endif // BECH32M_BASE64_BIT_STORAGE_H
