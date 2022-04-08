#include "argument_parser.h"
#include "bitstorage/base64_bit_storage.h"
#include "bech32m.h"
#include "bitstorage/bin_bit_storage.h"
#include "bitstorage/hex_bit_storage.h"

#include <cmath>
#include <fstream>
#include <iostream>

void read_write(const Program_args &arguments);

int main(int argc, const char **argv) {
    Parser parser = get_default_parser();
    Program_args arguments;
    try {
        arguments = parser.parse(argc, argv);
    } catch (const Bech32mException &e) {
        std::cerr << e.what() << std::endl << std::endl;
        std::cout << parser.usage();
        return EXIT_FAILURE;
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    read_write(arguments);
    return EXIT_SUCCESS;
}

std::string presentation_layer(const Program_args &arguments, const std::string &input) {
    std::string result;
    std::string hrp;
    std::string data;
    BitStorage storage;
    const char INPUT_HRP_SEPARATOR = ':';

    size_t separator = input.find(INPUT_HRP_SEPARATOR);
    if (separator == std::string::npos) {
        data = input;
    } else {
        hrp = input.substr(0, separator);
        data = input.substr(separator + 1, input.size() - separator);
    }

    switch (arguments.input_format) {
    case DataFormat::Base64:
        storage = Base64BitStorage(data);
        break;
    case DataFormat::Hex:
        storage = HexBitStorage(data);
        break;
    case DataFormat::Bin:
        storage = BinBitStorage(data);
        break;
    default:
        break;
    }

    if (arguments.mode == Mode::Encode) {
        return encode(hrp, storage);
    }
    try {
        return decode(input, arguments.output_format);
    } catch (const Bech32mException &e) {
        // std::cerr << "Decode error." << std::endl << e.what() << std::endl;
        // TODO: where do we want to log error messages?
        // rethrow for now
        throw;
    }
}

void read_write(const Program_args &arguments) {
    std::string result;
    std::ofstream output_file;
    std::ifstream input_file;
    std::istringstream input_string(arguments.input_text);

    // try to open output file if needed
    if (arguments.output_type == Output::File) {
        try {
            if (arguments.output_format == DataFormat::Bin) {
                output_file.open(arguments.outpu_file, std::ios::out | std::ios::app | std::ios::binary);
            } else {
                output_file.open(arguments.outpu_file, std::ios::out | std::ios::app);
            }
        } catch (const std::ios_base::failure &e) {
            throw Bech32mException("Something went wrong when handling the output file");
        }
    }

    // set target as file or std::cout
    std::ostream &target = arguments.output_type == Output::File ? output_file : std::cout;

    // try to open input file if needed
    if (arguments.input_type == Input::File) {
        try {
            if (arguments.input_format == DataFormat::Bin) {
                input_file.open(arguments.input_file, std::ios::in | std::ios::binary);
            } else {
                input_file.open(arguments.input_file, std::ios::in);
            }
        } catch (const std::ios_base::failure &e) {
            throw Bech32mException("Something went wrong when handling the output file");
        }
    }

    std::istream &source = arguments.input_type == Input::File
                               ? input_file
                               : arguments.input_type == Input::Argument ? input_string : std::cin;

    if (arguments.input_format == DataFormat::Bin) {
        std::string in;
        int64_t max_byte_count = std::floor(BECH32M_MAX_BITSET_LENGTH / BYTE_BIT_COUNT);
        in.resize(max_byte_count);
        source.read(&in[0], max_byte_count);
        if (source) {
            throw Bech32mException("More than max amount of bytes in the file.");
        }

        result = presentation_layer(arguments, in);
        if (arguments.output_format == DataFormat::Bin) {
            target.write(&result[0], static_cast<int64_t>(result.size()));
        } else {
            target << result << std::endl;
        }
    } else {
        std::string line;
        while (std::getline(source, line) && !std::all_of(line.begin(), line.end(), isspace)) {
            result = presentation_layer(arguments, line);
            if (arguments.output_format == DataFormat::Bin) {
                target.write(&result[0], static_cast<int64_t>(result.size()));
            } else {
                target << result << std::endl;
            }
        }
    }
}
