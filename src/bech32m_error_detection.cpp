#include "bech32m_error_detection.h"

std::pair<bool, std::string> check(const std::string &bech32m_enc) {
    // check length is greater than min checksum length + separator + hrp
    if (bech32m_enc.length() < 8) {
        return std::make_pair(false, "bech32m_enc too short");
    }
    if (bech32m_enc.length() > 90) {
        return std::make_pair(false, "bech32m_enc too long");
    }

    // check contains 1
    if (bech32m_enc.find('1') == std::string::npos) {
        // TODO find a position for '1' since that can be the only reason for failure (in single char errors)
        return std::make_pair(false, "bech32m_enc does not contain 1");
    }

    // iterate over bech32m_enc
    for (int i = 0; i < bech32m_enc.length(); ++i) {
        // check if char is in alphabet
        if (bech32m_enc[i] < 'a' || bech32m_enc[i] > 'z') {
            return std::make_pair(false, "bech32m_enc contains invalid char");
        }
    }

    // TODO
    return std::make_pair(true, bech32m_enc);
}
