#ifndef BECH32M_BECH32M_H
#define BECH32M_BECH32M_H

#include "bit_storage.h"
#include <algorithm>
#include <string>
#include <vector>

static const std::string BECH_SYMBOLS = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

/**
 * @brief Encodes the input string to bach32m
 *
 * TODO: will be removed
 * @param plain as a hex string
 * @return std::string bech32m code of the input string
 */
std::string encode(const std::string &plain);

/**
 * @brief Encodes the input string with hrp to bach32m
 * @param input text as a hex string
 * @param hrp plaintext humen readable part
 * @return std::string bech32m code of the input string
 **/
std::string encode(const std::string &hrp, const std::string &input);

/**
 * @brief Decodes the input bech32m string
 *
 * @param code in bech32m
 * @return std::string decoded string
 */
std::vector<Bech32mChar> decode(const std::string &code);

/**
 * @brief Decodes the input bech32m string
 *
 * @param hrp as plain text 'bc' or 'tb'
 * @param code in bech32m
 * @return std::string decoded string
 */
std::vector<Bech32mChar> decode_segwit(const std::string &hrp, const std::string &code);

/**
 * Encodes the data part without the checksum
 * @param storage
 * @return Bech32m value
 */
std::string encodeDataPart(const BitStorage &storage);

std::string decode_data_part(const std::string &bech);

std::string get_pub_key(const std::vector<Bech32mChar> &in);

std::vector<uint8_t> _5to8(std::vector<Bech32mChar> in);

#endif // BECH32M_BECH32M_H
