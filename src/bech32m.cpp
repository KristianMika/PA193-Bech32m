#include "bech32m.h"
#include "argument_parser.h"
#include "bech32m_error_detection.h"
#include "bech32m_exception.h"
#include "bitstorage/bech32m_bit_storage.h"
#include "bitstorage/bin_bit_storage.h"
#include "bitstorage/hex_bit_storage.h"
#include <vector>

static const uint32_t GEN[5] = {0x3b6a57b2, 0x26508e6d, 0x1ea119fa, 0x3d4233dd, 0x2a1462b3};
static const uint32_t BECH32M_CONSTANT = 0x2bc830a3;
static const char BECH_32M_SEPARATOR = '1';

uint32_t polymod(const Bech32mVector &input) {
    uint32_t checksum = 1;
    for (const auto &value : input) {
        uint8_t val = value.to_ulong();
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

Bech32mVector expand_hrp(const std::string &hrp) {
    Bech32mVector expanded;
    expanded.resize(hrp.length() * 2 + 1);
    for (size_t i = 0; i < hrp.length(); ++i) {
        expanded[i] = hrp[i] >> 5;
        expanded[hrp.length() + 1 + i] = hrp[i] & 0b11111;
    }
    expanded[hrp.length()] = 0;
    return expanded;
}

Bech32mVector calculate_checksum(const Bech32mVector &combined) {
    Bech32mVector combined_cpy = combined;
    const static Bech32mVector INNER_PADDING = Bech32mVector(6, 0);
    combined_cpy.insert(combined_cpy.end(), INNER_PADDING.begin(), INNER_PADDING.end());

    uint32_t poly = polymod(combined_cpy) ^ BECH32M_CONSTANT;
    Bech32mVector checksum;
    for (int i = 0; i < 6; ++i) {
        uint32_t value = (poly >> (5 * (5 - i))) & 31;
        checksum.push_back(value);
    }
    return checksum;
}

std::string encode(const std::string &hrp, const std::string &input) { return encode(hrp, HexBitStorage(input)); }

std::string encode(const std::string &hrp, const BitStorage &input) {
    // creating storage and converting to bitset vector
    Bech32mBitStorage storage = input;

    Bech32mVector processed;
    for (const auto &symbol : storage) {
        processed.push_back(symbol);
    }
    // expanding the header
    Bech32mVector expanded_hrp = expand_hrp(hrp);
    Bech32mVector combined(expanded_hrp);
    // combination of expanded header and processed data
    combined.insert(combined.end(), processed.begin(), processed.end());
    // calculating checksum
    Bech32mVector checksum = calculate_checksum(combined);
    std::string result = hrp;
    result.push_back(BECH_32M_SEPARATOR);
    for (const auto &b_set : processed) {
        result += BECH_SYMBOLS[b_set.to_ulong()];
    }
    for (const auto &b_set : checksum) {
        result += BECH_SYMBOLS[b_set.to_ulong()];
    }
    return result;
}

bool bech32_verify_checksum(std::string const &hrp, const Bech32mVector &data) {
    Bech32mVector combined = expand_hrp(hrp);
    combined.insert(combined.end(), data.begin(), data.end());
    return polymod(combined) == BECH32M_CONSTANT;
}

Bech32mVector reverse_code(int begin, int end, const std::string &code) {
    Bech32mVector result;
    for (int i = begin; i < end; i++) {
        char c = static_cast<char>(std::tolower(code[i]));
        size_t index = BECH_SYMBOLS.find(c);
        if (index == std::string::npos) {
            throw Bech32mException("Invalid character in the data part of the string to decode.");
        }
        result.push_back(index);
    }
    return result;
}

bool verify_bech32m(const std::string &code) {
    bool has_upper = false;
    bool has_lower = false;
    if (code.length() > 90) {
        throw Bech32mException("Invalid length of Bech32m code.");
    }
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
    return true;
}

std::string storage_to_output(const Bech32mVector &data, DataFormat output_format, bool trim) {
    Bech32mBitStorage converter = Bech32mBitStorage(data, trim);
    std::string result;

    switch (output_format) {
    case DataFormat::Hex:
        result = HexBitStorage(converter).to_string();
        break;

    case DataFormat::Bin:
        result = BinBitStorage(converter).to_string();
        break;

    case DataFormat::Base64:
        result = Base64BitStorage(converter).to_string();
        break;

    case DataFormat::Bech32m:
        result = converter.to_string();
        break;

    default:
        break;
    }
    return result;
}

std::string decode(const std::string &code, DataFormat output_format, bool trim) {
    verify_bech32m(code);

    std::string lowered(code.size(), 0x00);
    std::transform(code.begin(), code.end(), lowered.begin(), tolower);

    Bech32mVector data;

    size_t separator_i = lowered.rfind(BECH_32M_SEPARATOR);
    if (separator_i == std::string::npos) {
        ErrorDetectionResult detection = detect_error(lowered, separator_i);
        if (detection.result == DetectionResult::OneCharSubs) {
            data = detection.data;
            Bech32mVector without_checksum(data.begin(), data.end() - BECH32M_CHECKSUM_LENGTH);
            return storage_to_output(without_checksum, output_format, trim);
        }
        throw Bech32mException("No separator of human readable part in the string to decode "
                               "+ another substitution error.");
    }
    if (separator_i > lowered.length() - BECH32M_CHECKSUM_LENGTH) {
        throw Bech32mException("Data part is shorter than 6 symbols");
    }
    std::string hrp;
    if (separator_i > 0) {
        hrp = lowered.substr(0, separator_i);
    } else {
        ErrorDetectionResult detection = detect_error(lowered, separator_i);
        if (detection.result == DetectionResult::OneCharSubs) {
            data = detection.data;
            Bech32mVector without_checksum(data.begin(), data.end() - BECH32M_CHECKSUM_LENGTH);
            return storage_to_output(without_checksum, output_format, trim);
        }
    }

    data = reverse_code(static_cast<int>(hrp.length()) + 1, static_cast<int>(lowered.length()), lowered);

    if (!bech32_verify_checksum(hrp, data)) {
        ErrorDetectionResult detection = detect_error(lowered, separator_i);
        if (detection.result == DetectionResult::OneCharSubs) {
            data = detection.data;
        } else {
            throw Bech32mException("Sent data do not match the received data.");
        }
    }
    Bech32mVector wout_checksum(data.begin(), data.end() - BECH32M_CHECKSUM_LENGTH);
    return storage_to_output(wout_checksum, output_format, trim);
}
