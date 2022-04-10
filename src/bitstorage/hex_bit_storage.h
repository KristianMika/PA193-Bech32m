#ifndef BECH32M_HEXBITSTORAGE_H
#define BECH32M_HEXBITSTORAGE_H

#include "bit_storage.h"
#include <sstream>

static const int HEX_CHAR_BIT_COUNT = 4;
using HexChar = std::bitset<HEX_CHAR_BIT_COUNT>;
class HexBitStorage : public BitStorage {
  public:
    explicit HexBitStorage(const std::string &hex_value, bool _trim = false);
    ~HexBitStorage() override = default;
    HexBitStorage(const BitStorage &storage) : BitStorage(storage) {}

    std::string to_string() const;
    BitStorage::Iterator<HEX_CHAR_BIT_COUNT> begin() const { return Iterator<HEX_CHAR_BIT_COUNT>(value, 0); }
    BitStorage::Iterator<HEX_CHAR_BIT_COUNT> end() const {
        if (trim) {
            //bitset_length - (bitset_length % char_length)
            //bitset_length + padding
            return Iterator<HEX_CHAR_BIT_COUNT>(value, length - (length % HEX_CHAR_BIT_COUNT));
        } else {
            return Iterator<HEX_CHAR_BIT_COUNT>(value, compute_padding(HEX_CHAR_BIT_COUNT) + length);
        }  
    }
};

#endif // BECH32M_HEXBITSTORAGE_H
