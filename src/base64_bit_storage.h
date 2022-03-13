#ifndef BECH32M_BASE64_BIT_STORAGE_H
#define BECH32M_BASE64_BIT_STORAGE_H

#include "bit_storage.h"

/**
 * Represents a Base64 encoded input
 */
class Base64BitStorage : public BitStorage {
  public:
    explicit Base64BitStorage(const std::string &base64);
    ~Base64BitStorage() override = default;
};
#endif // BECH32M_BASE64_BIT_STORAGE_H
