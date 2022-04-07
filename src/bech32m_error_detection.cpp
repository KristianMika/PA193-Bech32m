#include "bech32m_error_detection.h"
#include "bech32m.h"
#include <utility>

error_detection_result detect_error(const std::string &bech32m_enc_hex, size_t idx_separator) {
    std::string cpy = bech32m_enc_hex;
    std::string hrp = bech32m_enc_hex.substr(0, idx_separator);

    Bech32mVector init_attempt = reverse_code(hrp.length() + 1, cpy.length(), cpy);

    if (bech32_verify_checksum(hrp, init_attempt)) {
        // no substitution necessary
        return error_detection_result(detection_result::VALID, init_attempt);
    }

    if (idx_separator == std::string::npos) {
        // the separator can occur only at positions [1 - bech32m_enc_hex.length()-6)
        // but for safety we'll run through the whole string
        for (int i = 0; i < bech32m_enc_hex.length(); ++i) {
            cpy[i] = '1';
            Bech32mVector data = reverse_code(hrp.length() + 1, cpy.length(), cpy);

            if (bech32_verify_checksum(hrp, data)) {
                // the substitution was in the separator character
                return error_detection_result(detection_result::ONE_CHAR_SUBST, data);
            }
        }
        return error_detection_result(detection_result::INVALID);
    }

    // check hrp
    for (size_t i = 0; i < idx_separator; ++i) {
        char c = cpy[i];
        for (int j = 33; j < 127; ++j) {
            char valid_char = static_cast<char>(j);
            if (c == valid_char) {
                continue;
            }
            cpy[i] = valid_char;
            hrp[i] = valid_char;
            Bech32mVector data = reverse_code(hrp.length() + 1, cpy.length(), cpy);

            if (bech32_verify_checksum(hrp, data)) {
                // the substitution was in the hrp
                return error_detection_result(detection_result::ONE_CHAR_SUBST, data);
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
                return error_detection_result(detection_result::ONE_CHAR_SUBST, data);
            }
        }
        cpy[i] = c;
    }
    return error_detection_result(detection_result::INVALID);
}
