#include "bech32m.h"
#include "bech32m_bit_storage.h"
#include "bech32m_exception.h"
#include "hex_bit_storage.h"
#include <sstream>
#include <vector>

static const uint32_t GEN[5] = {0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3};
static const uint32_t BECH32M_CONSTANT = 0x2bc830a3;

uint32_t polymod(const std::vector<std::bitset<5>> &input) {
    uint32_t checksum = 1;
    uint8_t val;
    for (const auto &value : input) {
        val = value.to_ulong();
        auto derived = (checksum >> 25);
        checksum = ((checksum & 0x1ffffff) << 5) ^ val;
        for (int i = 0; i < 5; ++i) {
            if ((derived >> i) & 1) {
                checksum = checksum ^ GEN[i];
            }
        }
    }
    return checksum;
}

std::vector<std::bitset<5>> expand_hrp(const std::string &hrp) {
    std::vector<std::bitset<5>> expanded;
    expanded.resize(hrp.length() * 2 + 1);
    for (int i = 0; i < hrp.length(); ++i) {
        expanded[i] = hrp[i] >> 5;
        expanded[hrp.length() + 1 + i] = hrp[i] & 0b11111;
    }
    expanded[hrp.length()] = 0;
    return expanded;
}

std::vector<std::bitset<5>> calculate_checksum(std::vector<std::bitset<5>> combined) {
    for (uint8_t i = 0; i < 6; ++i) {
        combined.push_back(0);
    }
    uint32_t poly = polymod(combined) ^ BECH32M_CONSTANT;
    std::vector<uint8_t> checksum;
    std::vector<std::bitset<5>> checksum2;
    for (int i = 0; i < 6; ++i) {
        uint32_t value = (poly >> (5 * (5 - i))) & 31;
        checksum.push_back(value);
        checksum2.push_back(value);
    }
    return checksum2;
}

std::string encode(const std::string &hrp, const std::string &input) {
    // creating storage and converting to bitset vector
    BitStorage storage = HexBitStorage(input);
    std::vector<std::bitset<5>> processed;
    for (const auto &symbol : storage) {
        processed.push_back(symbol);
    }
    // expanding the header
    std::vector<std::bitset<5>> expanded_hrp = expand_hrp(hrp);
    std::vector<std::bitset<5>> combined = std::vector<std::bitset<5>>(expanded_hrp);
    // combination of expanded header and processed data
    combined.insert(combined.end(), processed.begin(), processed.end());
    // calculating checksum
    auto checksum = calculate_checksum(combined);
    std::string result = hrp;
    result += "1";
    for (const auto &b_set : processed) {
        result += BECH_SYMBOLS[b_set.to_ulong()];
    }
    for (const auto &b_set : checksum) {
        result += BECH_SYMBOLS[b_set.to_ulong()];
    }
    return result;
}

std::string encode(const std::string &input) { return input; }

bool bech32_verify_checksum(std::string const &hrp, std::vector<std::bitset<5>> data) {
    std::vector<std::bitset<5>> combined = expand_hrp(hrp);
    combined.insert(combined.end(), data.begin(), data.end());
    return polymod(combined) == BECH32M_CONSTANT;
}

std::vector<std::bitset<5>> reverse_code(int begin, int end, const std::string &code) {
    std::vector<std::bitset<5>> result;
    int index;
    char c;
    for (int i = begin; i < end; i++) {
        c = std::tolower(code[i]);
        index = BECH_SYMBOLS.find(c);
        if (index == std::string::npos) {
            throw Bech32mException("Invalid character in the data part of the string to decode.");
        } else {
            result.push_back(index);
        }
    }
    return result;
}

std::string bytes_to_hex(std::vector<uint8_t> &in) {
    char hex_chars[] = "0123456789abcdef";

    std::string ret(in.size() * 2, '_');

    char *buffer = const_cast<char *>(ret.data());

    for (const auto &byte : in) {
        *buffer++ = hex_chars[byte >> 4];
        *buffer++ = hex_chars[byte & 0x0F];
    }
    return ret;
}

std::vector<uint8_t> _5to8(std::vector<Bech32mChar> in) {
    int acc = 0;
    int bits = 0;
    std::vector<uint8_t> ret = {};
    int maxv = (1 << 8) - 1;
    int max_acc = (1 << 12) - 1;
    for (const auto &val : in) {
        acc = ((acc << 5) | static_cast<uint8_t>(val.to_ulong())) & max_acc;
        bits += 5;
        while (bits >= 8) {
            bits -= 8;
            ret.push_back((acc >> bits) & maxv);
        }
    }
    return ret;
}

std::vector<Bech32mChar> decode(const std::string &code) {
    bool has_upper = false;
    bool has_lower = false;
    for (char const &c : code) {
        if (c < 33 || c > 126) {
            throw Bech32mException("Invalid character in the string to decode.");
        }
        if (c >= 'a' && c <= 'z') {
            has_lower = true;
        } else if (c >= 'A' && c <= 'Z') {
            has_upper = true;
        }
    }

    if (has_upper && has_lower) {
        throw Bech32mException("Both lower and upper case letters in the string to decode.");
    }

    std::string lowered(code.length(), '_');
    std::transform(code.begin(), code.end(), lowered.begin(), tolower);

    int separator = lowered.rfind('1');
    if (separator == std::string::npos) {
        throw Bech32mException("No separator of human readable part in the string to decode.");
    }
    if (separator > lowered.length() - 6) {
        throw Bech32mException("Data part is shorter than 6 symbols");
    }
    std::string hrp = "";
    if (separator > 0) {
        hrp = lowered.substr(0, separator);
    } else {
        throw Bech32mException("Empty human readable part");
    }
    std::vector<Bech32mChar> data = reverse_code(hrp.length() + 1, lowered.length(), lowered);

    if (!bech32_verify_checksum(hrp, data)) {
        throw Bech32mException("Sent data do not match the received data.");
    }
    return data;
}


std::vector<Bech32mChar> decode_segwit(const std::string &hrp, const std::string &code) {
    std::vector<Bech32mChar> data = decode(code);

    // TODO remove this code duplicate vvv
    std::string lowered(code.length(), '_');
    std::transform(code.begin(), code.end(), lowered.begin(), tolower);

    int separator = lowered.rfind('1');
    if (separator == std::string::npos) {
        throw Bech32mException("No separator of human readable part in the string to decode.");
    }
    if (separator > lowered.length() - 6) {
        throw Bech32mException("Data part is shorter than 6 symbols");
    }
    std::string decoded_hrp = "";
    if (separator > 0) {
        decoded_hrp = lowered.substr(0, separator);
    }
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    if (decoded_hrp != hrp) {
        throw Bech32mException("User data not matching!");
    }
    return data;
}

std::string get_pub_key(const std::vector<Bech32mChar>& in) {
    std::vector<Bech32mChar> data_filtered = {};
    data_filtered.insert(data_filtered.end(), in.begin() + 1, in.end() - 6);

    std::vector<uint8_t> pub_key = {};
    pub_key.emplace_back(static_cast<uint8_t>(in[0].to_ulong()) + 0x50);

    std::vector<uint8_t> bytes = _5to8(data_filtered);
    pub_key.emplace_back(static_cast<uint8_t>(bytes.size()));
    pub_key.insert(pub_key.end(), bytes.begin(), bytes.end());

    std::string str = bytes_to_hex(pub_key);
    return str;
}


inline char encodeBechChar(const Bech32mChar chr) { return BECH_SYMBOLS[chr.to_ulong()]; }

std::string encodeDataPart(const BitStorage &storage) {
    std::vector<char> out;

    for (const auto &b_set : storage) {
        out.push_back(encodeBechChar(b_set));
    }
    return {out.begin(), out.end()};
}

// TODO: support multiple output formats
std::string decode_data_part(const std::string &bech) {
    Bech32mBitStorage storage(bech);
    std::stringstream out;
    storage.pad(4);
    auto it = storage.begin<4>();
    std::cout << std::endl;
    while (it != storage.end<4>()) {
        out << std::hex << (*it).to_ulong();
        ++it;
    }
    return out.str();
}