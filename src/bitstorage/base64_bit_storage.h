#ifndef BECH32M_BASE64_BIT_STORAGE_H
#define BECH32M_BASE64_BIT_STORAGE_H

#include "bit_storage.h"
#include "hex_bit_storage.h"

static const uint16_t BASE64_CHAR_BIT_LENGTH = 6;
using Base64Char = std::bitset<BASE64_CHAR_BIT_LENGTH>;

/**
 * Represents a Base64 encoded input
 */
class Base64BitStorage : public BitStorage {
  public:
    /**
     * Construct a Base64 bit store from a valid Base64 string
     * @param base64 Base64 string input
     */
    explicit Base64BitStorage(const std::string &base64, bool _trim = false);
    Base64BitStorage(const BitStorage &storage) : BitStorage(storage) {}
    ~Base64BitStorage() override = default;
    std::string to_string() const;
    BitStorage::Iterator<BASE64_CHAR_BIT_LENGTH> begin() const { return Iterator<BASE64_CHAR_BIT_LENGTH>(value, 0); }
    BitStorage::Iterator<BASE64_CHAR_BIT_LENGTH> end() const {
        if (trim) {
            return Iterator<BASE64_CHAR_BIT_LENGTH>(value, length - (length % BASE64_CHAR_BIT_LENGTH));
        } else {
            return Iterator<BASE64_CHAR_BIT_LENGTH>(value, compute_padding(BASE64_CHAR_BIT_LENGTH) + length);
        }
        
    }
};
#endif // BECH32M_BASE64_BIT_STORAGE_H
