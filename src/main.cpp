#include "base64_bit_storage.h"
#include "bech32m.h"
#include "hex_bit_storage.h"
#include "bech32m_bit_storage.h"

#include <iostream>
#include <fstream>
#include <sstream>
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
    BitStorage storage;
    switch (arguments.input_format) {
    case data_form::Bech32m:
        storage = Bech32mBitStorage(input);
        break;
    case data_form::base64:
        storage = Base64BitStorage(input);
        break;
    case data_form::hex:
        storage = HexBitStorage(input);
        break;
    case data_form::bin:
        storage = HexBitStorage(input);
        break;
    default:
        break;
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
            output_file.open(arguments.outpu_file, std::ios::out | std::ios::app);
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
            input_file.open(arguments.input_file, std::ios::in);
        } catch (const std::ios_base::failure &e) {
            input_file.close();
            std::cout << "Something went wrong when handling the output file." << std::endl << e.what() << std::endl;
            return 1;
        }
    }

    std::istream& source = arguments.input_type == input::file ? input_file : 
                           arguments.input_type == input::argument ? input_string 
                                                                : std::cin;

    std::string line;
    while (std::getline(source, line) && !std::all_of(line.begin(), line.end(), isspace)) {
        result = presentation_layer(arguments, line);
        target << result << std::endl;
    }

    // comented out, based on cppreference the ofstream and ifstream .close() methods are called in destructor
    /*if (arguments.output_type == output::file) {
        output_file.close();
    }
    if (arguments.input_type == input::file) {
        output_file.close();
    }*/
}

