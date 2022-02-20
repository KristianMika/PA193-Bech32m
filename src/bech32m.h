#ifndef BECH32M_BECH32M_H
#define BECH32M_BECH32M_H

#include <string>
/**
 * @brief Encodes the input string to bach32m
 *
 * @param plain as a hex string
 * @return std::string bech32m code of the input string
 */
std::string encode(const std::string &plain);

/**
 * @brief Decodes the input bech32m string
 *
 * @param code in bech32m
 * @return std::string decoded string
 */
std::string decode(const std::string &code);
#endif // BECH32M_BECH32M_H
