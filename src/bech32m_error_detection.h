#ifndef BECH32M_BECH32M_ERROR_DETECTION_H
#define BECH32M_BECH32M_ERROR_DETECTION_H
#include <string>
#include <utility>

enum class check_result {
    // The input string was valid, no correction took place
    VALID,
    // The input string contained one character substitution, corrected string is in the second pair element
    ONE_CHAR_SUBST,
    // The input string was too long for efficient processing
    TOO_LONG,
    // The input string was too short and one char addition did not yield a valid encoded value
    TOO_SHORT,
    // The input string contains two or more error and could not be repaired
    INVALID
};

/**
 * Checks for errors in the encoded Bech32m string and attempts validation
 * @param bech32m encoded Bech32m string
 * @return a pair, boolean indicating an error in the string, string containing the corrected input,
 * if the boolean is `false` the string contains the input
 */
std::pair<bool, std::string> check(const std::string &bech32m_enc);
#endif // BECH32M_BECH32M_ERROR_DETECTION_H
