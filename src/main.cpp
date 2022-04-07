#include "base64_bit_storage.h"
#include "bech32m.h"
#include "hex_bit_storage.h"
#include "bech32m_bit_storage.h"
#include "bin_bit_storage.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "argument_parser.h"


int read_write(const Program_args &arguments);

int main(int argc, char** argv) {

    Parser parser = Parser();

    parser
        .add_argument(Argument()
            .add_name("--input-format")
            .add_param_value("bin")
            .add_param_value("hex")
            .add_param_value("base64")
            .add_handler(set_input_format))
        .add_argument(Argument()
            .add_name("--output-format")
            .add_param_value("bin")
            .add_param_value("hex")
            .add_param_value("base64")
            .add_handler(set_output_format))
        .add_argument(Argument()
            .add_name("--input-file")
            .set_variable_param()
            .add_handler(set_input_file))
        .add_argument(Argument()
            .add_name("--input-text")
            .add_name("-t")
            .set_variable_param()
            .add_handler(set_input_text))
        .add_argument(Argument()
            .add_name("--output-file")
            .set_variable_param()
            .add_handler(set_output_file));
    
    Program_args arguments;
    try {
        arguments = parser.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Parsed" << std::endl;

    read_write(arguments);
    return 0;
}

std::string presentation_layer(const Program_args &arguments, const std::string& input) {
    std::string result;
    std::string hrp;
    std::string data;
    BitStorage storage;

    size_t separator = input.find(':');
    if (separator == std::string::npos) {
        data = input;
    } else {
        hrp = input.substr(0, separator);
        data = input.substr(separator + 1, input.size() - separator);
    }


    switch (arguments.input_format) {
    case data_form::base64:
        storage = Base64BitStorage(data);
        break;
    case data_form::hex:
        storage = HexBitStorage(data);
        break;
    case data_form::bin:
        storage = BinBitStorage(data);
        break;
    default:
        break;
    }

    if (arguments.mode == program_mode::encode) {
        return encode(hrp, storage);
    } else {
        Bech32mVector result = decode(input);
    }
    return input;
}


int read_write(const Program_args& arguments) {
    std::string input = "";
    std::string result;
    std::ofstream output_file;
    std::ifstream input_file;
    std::istringstream input_string(arguments.input_text);

    // try to open output file if needed
    if (arguments.output_type == output::file) {
        try {
            if (arguments.output_format == data_form::bin) {
                output_file.open(arguments.outpu_file, std::ios::out | std::ios::app | std::ios::binary);
            } else {
                output_file.open(arguments.outpu_file, std::ios::out | std::ios::app);
            }
        } catch (const std::ios_base::failure &e) {
            output_file.close();
            std::cout << "Something went wrong when handling the output file." << std::endl << e.what() << std::endl;
            return 1;
        }
    }

    // set target as file or std::cout
    std::ostream& target = arguments.output_type == output::file ? output_file : std::cout;

    // try to open input file if needed
    if (arguments.input_type == input::file) {
        try {
            if (arguments.input_format == data_form::bin) {
                input_file.open(arguments.input_file, std::ios::in | std::ios::binary);
            } else {
                input_file.open(arguments.input_file, std::ios::in);
            }           
        } catch (const std::ios_base::failure &e) {
            input_file.close();
            std::cout << "Something went wrong when handling the output file." << std::endl << e.what() << std::endl;
            return 1;
        }
    }

    std::istream& source = arguments.input_type == input::file ? input_file : 
                           arguments.input_type == input::argument ? input_string 
                                                                : std::cin;

    if (arguments.input_format == data_form::bin) {
        std::string in;
        double max_read = std::ceil(BECH32M_MAX_BITSET_LENGTH / 8);
        in.resize(max_read);
        source.read(&in[0], max_read);
        if (source) {
            std::cout << "More than max amount of bytes in the file." << std::endl;
            return 1;
        }

        result = presentation_layer(arguments, in);
        if (arguments.output_format == data_form::bin) {
            target.write(&result[0], result.size());
        } else {
            target << result << std::endl;
        }
    } else {
        std::string line;
        while (std::getline(source, line) && !std::all_of(line.begin(), line.end(), isspace)) {
            result = presentation_layer(arguments, line);
            if (arguments.output_format == data_form::bin) {
                target.write(&result[0], result.size());
            } else {
                target << result << std::endl;
            }
        }
    }
    // comented out, based on cppreference the ofstream and ifstream .close() methods are called in destructor
    /*if (arguments.output_type == output::file) {
        output_file.close();
    }
    if (arguments.input_type == input::file) {
        output_file.close();
    }*/
}

