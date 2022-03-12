#include "bech32m.h"
#include "hex_bit_storage.h"
#include "base64_bit_storage.h"
#include <iostream>
int main() {
    // HEX DEMO
    auto to_encode = "fafafa";
    HexBitStorage storage = HexBitStorage(to_encode);

    for (const BechCharType val : storage) {
        std::cout << val << ", "; // val.to_ulong() to get the numeric value
    }
    std::cout << std::endl;

    std::cout << "bech32m(" << to_encode << ") = " << encodeHumanReadablePart(storage) << std::endl;

    // BASE64 demo
    auto some_base64 = "aGkK";
    Base64BitStorage base_storage = Base64BitStorage(some_base64);

    for (const BechCharType val : base_storage) {
        std::cout << val << ", "; // val.to_ulong() to get the numeric value
    }
    std::cout << std::endl;
    std::cout << "bech32m(" << some_base64 << ") = " << encodeHumanReadablePart(base_storage) << std::endl;

    // encoding
    std::cout << encode("abcd", "00201863143c14c5166804bd19203356da136c985678cd4d27a1b8c63296") << std::endl;
    return 0;
}
