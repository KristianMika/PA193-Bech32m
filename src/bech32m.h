#ifndef BECH32M_BECH32M_H
#define BECH32M_BECH32M_H

#include "argument_parser.h"
#include "bit_storage.h"
#include <algorithm>
#include <string>
#include <vector>

static const std::string BECH_SYMBOLS = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

/**
 * Encodes the input string with hrp to bach32m
 * @param hrp plaintext human readable part
 * @param input text as a hex string
 * @return std::string bech32m code of the input string
 **/
std::string encode(const std::string &hrp, const BitStorage &input);

std::string encode(const std::string &hrp, const std::string &input);

/**
 * Decodes the input bech32m string
 * @param code in bech32m
 * @return std::string decoded string
 */
std::string decode(const std::string &code, data_form output_format = data_form::hex);

/**
 * Encodes the data part without the checksum
 * @param storage
 * @return Bech32m value
 */
std::string encodeDataPart(const BitStorage &storage);

/**
 * TODO decodes data part
 * @param bech TODO input
 * @return TODO return
 */
std::string decode_data_part(const std::string &bech);

/**
 * Convert internal bit representation of Bech32m to hexadecimal string
 * @param bech internal Bech32m bit representation
 * @return hexadecimal string representation of the internal state
 */
std::string to_hex(const BitStorage &bech);

/**
 * Remaps the input checksum string to the bech32m alphabet
 * @param begin start of the checksum string
 * @param end total length of the code
 * @param code the encoded string
 * @return intermediate representation of the checksum
 */
Bech32mVector reverse_code(size_t begin, size_t end, const std::string &code);

/**
 * Verify the validity of the checksum against the human readable part
 * @param hrp human readable part
 * @param data intermediate representation of the checksum
 * @return true if the checksum is valid, false otherwise
 */
bool bech32_verify_checksum(std::string const &hrp, const Bech32mVector &data);

/**
 * Validates the input string to conform to the bech32m specification
 * @param code the bech32m string
 * @return true if length and character set are valid, false otherwise
 */
bool verify_bech32m(const std::string &code);

#endif // BECH32M_BECH32M_H
