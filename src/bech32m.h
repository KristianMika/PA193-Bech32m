#ifndef BECH32M_BECH32M_H
#define BECH32M_BECH32M_H

#include "argument_parser.h"
#include "bitstorage/bit_storage.h"
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
std::string decode(const std::string &code, DataFormat output_format = DataFormat::Hex);

/**
 * Remaps the input checksum string to the bech32m alphabet
 * @param begin start of the checksum string
 * @param end total length of the code
 * @param code the encoded string
 * @return intermediate representation of the checksum
 */
Bech32mVector reverse_code(int begin, int end, const std::string &code);

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
