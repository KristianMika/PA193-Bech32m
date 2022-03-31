#ifndef BECH32M_HEXBITSTORAGE_H
#define BECH32M_HEXBITSTORAGE_H

#include "bit_storage.h"
#include <sstream>

static const int HEX_CHAR_BIT_COUNT = 4;
using HexChar = std::bitset<HEX_CHAR_BIT_COUNT>;
class HexBitStorage : public BitStorage {
  public:
    explicit HexBitStorage(const std::string &hex_value);
    ~HexBitStorage() override = default;
};

#endif // BECH32M_HEXBITSTORAGE_H
