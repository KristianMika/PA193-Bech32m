#include "bech32m_error_detection.h"
#include "bech32m.h"
#include <utility>

ErrorDetectionResult detect_error(const std::string &bech32m_enc_hex, size_t idx_separator) {
    std::string cpy = bech32m_enc_hex;

    if (idx_separator == std::string::npos || idx_separator == 0) {
        // the separator can occur only at positions [1 - bech32m_enc_hex.length()-6)
        // but for safety we'll run to the end of the string
        for (size_t i = 1; i < bech32m_enc_hex.length(); ++i) {
            char original = cpy[i];
            cpy[i] = '1';

            std::string hrp = cpy.substr(0, i);

            Bech32mVector data = reverse_code(hrp.length() + 1, cpy.length(), cpy);

            if (bech32_verify_checksum(hrp, data)) {
                // the substitution was in the separator character
                return ErrorDetectionResult(DetectionResult::OneCharSubs, data);
            }
            cpy[i] = original;
        }
        return ErrorDetectionResult(DetectionResult::Invalid);
    }

    std::string hrp = bech32m_enc_hex.substr(0, idx_separator);

    Bech32mVector init_attempt = reverse_code(hrp.length() + 1, cpy.length(), cpy);

    if (bech32_verify_checksum(hrp, init_attempt)) {
        // no substitution necessary
        return ErrorDetectionResult(DetectionResult::Valid, init_attempt);
    }

    // check hrp
    for (size_t i = 0; i < idx_separator; ++i) {
        char c = cpy[i];
        for (int j = 33; j < 127; ++j) {
            char valid_char = static_cast<char>(j);
            if (c == valid_char) {
                continue;
            }
            if (valid_char >= 'A' && valid_char <= 'Z') {
                continue;
            }
            cpy[i] = valid_char;
            hrp[i] = valid_char;

            try {
                verify_bech32m(cpy);

                Bech32mVector data = reverse_code(hrp.length() + 1, cpy.length(), cpy);

                if (bech32_verify_checksum(hrp, data)) {
                    // the substitution was in the hrp
                    return ErrorDetectionResult(DetectionResult::OneCharSubs, data);
                }
            } catch (Bech32mException _) {
                // continue with the next character
            }
        }
        hrp[i] = c;
    }
    // check checksum
    for (size_t i = idx_separator + 1; i < bech32m_enc_hex.length(); ++i) {
        char c = cpy[i];
        for (char valid_char : BECH_SYMBOLS) {
            if (c == valid_char) {
                continue;
            }
            cpy[i] = valid_char;
            Bech32mVector data = reverse_code(hrp.length() + 1, cpy.length(), cpy);

            if (bech32_verify_checksum(hrp, data)) {
                // the substitution was in the checksum
                return ErrorDetectionResult(DetectionResult::OneCharSubs, data);
            }
        }
        cpy[i] = c;
    }
    return ErrorDetectionResult(DetectionResult::Invalid);
}
