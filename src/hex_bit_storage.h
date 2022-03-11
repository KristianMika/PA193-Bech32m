//
// Created by kiko on 11.03.22.
//

#ifndef BECH32M_HEXBITSTORAGE_H
#define BECH32M_HEXBITSTORAGE_H
#include "bit_storage.h"
#include <sstream>
static const int HEX_CHAR_BIT_COUNT = 4;
class HexBitStorage : public BitStorage {
  public:
    explicit HexBitStorage(const std::string &hex_value);
    ~HexBitStorage() override = default;
};
#endif // BECH32M_HEXBITSTORAGE_H
