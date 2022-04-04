#ifndef BECH32M_BECH32M_ERROR_DETECTION_H
#define BECH32M_BECH32M_ERROR_DETECTION_H
#include <string>
/**
 * Checks for errors in the encoded Bech32m string and attempts validation
 * @param bech32m encoded Bech32m string
 * @return a pair, boolean indicating an error in the string, string containing the corrected input,
 * if the boolean in `false` the string contains the input
 */
std::pair<bool, std::string> check(const std::string &bech32m_enc);
#endif // BECH32M_BECH32M_ERROR_DETECTION_H
