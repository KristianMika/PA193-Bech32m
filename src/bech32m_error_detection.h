#ifndef BECH32M_BECH32M_ERROR_DETECTION_H
#define BECH32M_BECH32M_ERROR_DETECTION_H
#include "bit_storage.h"
#include <string>
#include <utility>

enum class DetectionResult {
    // The input string was valid, no correction took place
    Valid,
    // The input string contained one character substitution, corrected string is in the second pair element
    OneCharSubs,
    // The input string contains two or more error and could not be repaired
    Invalid
};

class ErrorDetectionResult {
  public:
    DetectionResult result;
    Bech32mVector data;

    ErrorDetectionResult(DetectionResult _result, Bech32mVector _data) : result(_result), data(std::move(_data)) {}
    explicit ErrorDetectionResult(DetectionResult _result) : result(_result), data() {}
};

/**
 * Checks for errors in the encoded Bech32m string and attempts validation
 * @param bech32m encoded Bech32m string
 * @return a pair, boolean indicating an error in the string, string containing the corrected input,
 * if the boolean is `false` the string contains the input
 */
ErrorDetectionResult detect_error(const std::string &bech32m_enc_hex, size_t idx_separator);
#endif // BECH32M_BECH32M_ERROR_DETECTION_H
