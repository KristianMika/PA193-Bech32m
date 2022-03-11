#include "bech32m.h"
#include "hex_bit_storage.h"
#include <iostream>
int main() {
    auto to_encode = "fafafa";
    HexBitStorage storage = HexBitStorage(to_encode);

    for (const BechCharType val : storage) {
        std::cout << val << ", "; // val.to_ulong() to get the numeric value
    }
    std::cout << std::endl;

    // not finished!
    std::cout << "bech32m(" << to_encode << ") = " << encodeHumanReadablePart(storage) << std::endl;
    return 0;
}
