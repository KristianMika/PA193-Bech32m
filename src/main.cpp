#include "base64_bit_storage.h"
#include "bech32m.h"
#include "hex_bit_storage.h"

#include <iostream>
#include <fstream>
#include "argument_parser.h"


int encoding(const Program_args &arguments);
void decoding(const Program_args &arguments);


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

    switch (arguments.mode) {
    case program_mode::encode:
        encoding(arguments);
        break;
    case program_mode::decode:
        decoding(arguments);
        break;
    default:
        break;
    }
    return 0;
}

void get_input_storage(const Program_args& arguments, const std::string& value, BitStorage& storage) {
    switch (arguments.input_format) {
    case data_form::bin:
        std::cout << "Creating bin input sotrage" << std::endl;
        storage = HexBitStorage(value);
        break;
    case data_form::hex:
        std::cout << "Creating hex input sotrage" << std::endl;
        storage = HexBitStorage(value);
        break;
    case data_form::base64:
        std::cout << "Creating base64 input sotrage" << std::endl;
        storage = Base64BitStorage(value);
        break;
    default:
        break;
    }
}

void get_output_storage(const Program_args &arguments, const std::string &value, BitStorage &storage) {
    switch (arguments.output_format) {
    case data_form::bin:
        std::cout << "Creating bin output sotrage" << std::endl;
        storage = HexBitStorage(value);
        break;
    case data_form::hex:
        std::cout << "Creating hex output sotrage" << std::endl;
        storage = HexBitStorage(value);
        break;
    case data_form::base64:
        std::cout << "Creating base64 output sotrage" << std::endl;
        storage = Base64BitStorage(value);
        break;
    default:
        break;
    }
}

int encoding(const Program_args& arguments) { 
    std::cout << "Encoding" << std::endl;
    BitStorage storage;
    std::string input = "";
    std::string result;
    std::ofstream outpu_file;
    if (arguments.output == output::file) {
        try {
            outpu_file.open(arguments.outpu_file, std::ios::out | std::ios::app);
        } catch (const std::ios_base::failure &e) {
            outpu_file.close();
            std::cout << "Something went wrong when handling the output file." << std::endl << e.what() << std::endl;
            return 1;
        }
    }

    std::ifstream input_file;
    switch (arguments.input) {
    case input::stdinput: 

        std::cout << "Enter the value to be encoded:" << std::endl;
        std::cin >> input;
        get_input_storage(arguments, input, storage);
        result = encode("BC", input);
        std::cout << input + " : " << result << std::endl;
        if (arguments.output == output::file) {
            outpu_file << result << std::endl;
        }
        
        break;
    case input::argument:
        get_input_storage(arguments, arguments.input_text, storage);
        result = encode("BC", arguments.input_text);
        std::cout << arguments.input_text + " : " << result << std::endl;
        if (arguments.output == output::file) {
            outpu_file << result << std::endl;
        }
        break;
    case input::file:
        try {
            input_file.open("example.txt", std::ios::in);

            while (std::getline(input_file, input)) {
                result = encode("BC", input);
                get_input_storage(arguments, input, storage);
                std::cout << input + " : " << result << std::endl;
                if (arguments.output == output::file) {
                    outpu_file << result << std::endl;
                }
            }
            input_file.close();
        } catch (const std::ios_base::failure& e) {
            input_file.close();
            std::cout << "Something went wrong when handling the input file." << std::endl << e.what() << std::endl;
        } 
        break;
    default:
        break;
    }

    if (arguments.output == output::file) {
        outpu_file.close();
    }
    return 0;
}

void decoding(const Program_args& arguments) {
    std::cout << "Decoding" << std::endl;
    BitStorage storage;
    std::string input = "";
    std::string result;
    std::ofstream outpu_file;
    if (arguments.output == output::file) {
        try {
            outpu_file.open(arguments.outpu_file, std::ios::out | std::ios::app);
        } catch (const std::ios_base::failure &e) {
            outpu_file.close();
            std::cout << "Something went wrong when handling the output file." << std::endl << e.what() << std::endl;
        }
    }

    std::ifstream input_file;
    switch (arguments.input) {
    case input::stdinput:
        while (input != "end") {
            std::cout << "Enter the value to be encoded. For ending the program, enter 'end'" << std::endl;
            std::cin >> input;
            get_input_storage(arguments, input, storage);
            //result = decode(input);
            std::cout << input + " : " << result << std::endl;
            if (arguments.output == output::file) {
                outpu_file << result << std::endl;
            }
        }
        break;
    case input::argument:
        get_input_storage(arguments, arguments.input_text, storage);
        //result = decode(input);
        std::cout << arguments.input_text + " : " << result << std::endl;
        if (arguments.output == output::file) {
            outpu_file << result << std::endl;
        }
        break;
    case input::file:
        try {
            input_file.open("example.bin", std::ios::in);

            while (std::getline(input_file, input)) {
                //result = get_pub_key(decode(input));
                get_input_storage(arguments, input, storage);
                std::cout << input + " : " << result << std::endl;
                if (arguments.output == output::file) {
                    outpu_file << result << std::endl;
                }
            }
            input_file.close();
        } catch (const std::ios_base::failure &e) {
            input_file.close();
            std::cout << "Something went wrong when handling the input file." << std::endl << e.what() << std::endl;
        }
        break;
    default:
        break;
    }

    if (arguments.output == output::file) {
        outpu_file.close();
    }
}
