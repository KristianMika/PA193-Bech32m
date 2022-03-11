#include "bech32m.h"
#include "bech32m_exception.h"
#include <vector>

static const std::string BECH_SYMBOLS = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

std::string encode(const std::string &input) { return input; }

std::string decode(const std::string &code) { return code; }

inline char encodeBechChar(const BechCharType chr) { return BECH_SYMBOLS[chr.to_ulong()]; }

// only human-readable part!
std::string encodeHumanReadablePart(const BitStorage &storage) {
    std::vector<char> out;

    for (const BechCharType val : storage) {
        out.push_back(encodeBechChar(val));
    }
    return {out.begin(), out.end()};
}
