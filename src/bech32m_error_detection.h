#ifndef BECH32M_BECH32M_ERROR_DETECTION_H
#define BECH32M_BECH32M_ERROR_DETECTION_H
#include <string>
#include <utility>
#include "bit_storage.h"

enum class detection_result {
    // The input string was valid, no correction took place
    VALID,
    // The input string contained one character substitution, corrected string is in the second pair element
    ONE_CHAR_SUBST,
    // The input string contains two or more error and could not be repaired
    INVALID
};

class error_detection_result {
  public:
    detection_result result;
    Bech32mVector data;

    error_detection_result(detection_result _result, Bech32mVector _data) : result(_result), data(std::move(_data)) {}
    explicit error_detection_result(detection_result _result) : result(_result), data() {}
};

/**
 * Checks for errors in the encoded Bech32m string and attempts validation
 * @param bech32m encoded Bech32m string
 * @return a pair, boolean indicating an error in the string, string containing the corrected input,
 * if the boolean is `false` the string contains the input
 */
error_detection_result detect_error(const std::string &bech32m_enc_hex, size_t idx_separator);
#endif // BECH32M_BECH32M_ERROR_DETECTION_H
