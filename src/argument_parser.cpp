#include "argument_parser.h"

// ./program --encode --from_bin

void set_output_format(Program_args &args, std::string output) {
    if (args.oformat_set || args.iformat_set) {
        throw Bech32mException("Format for the program has been already specified.\n");
    }

    if (output == "bin") {
        args.output_format = data_form::bin;
    } else if (output == "hex") {
        args.output_format = data_form::hex;
    } else if (output == "base64") {
        args.output_format = data_form::base64;
    } else {
        throw Bech32mException("Invalid parameter " + output + " passed to argumen --output-format.\n");
    }
    args.input_format = data_form::Bech32m;
    args.oformat_set = true;
    args.mode = program_mode::decode;
}

void set_input_format(Program_args &args, std::string input) {
    if (args.oformat_set || args.iformat_set) {
        throw Bech32mException("Format for the program has been already specified.\n");
    }

    if (input == "bin") {
        args.input_format = data_form::bin;
    } else if (input == "hex") {
        args.input_format = data_form::hex;
    } else if (input == "base64") {
        args.input_format = data_form::base64;
    } else {
        throw Bech32mException("Invalid parameter " + input + " passed to argumen --input-format.\n");
    }

    args.iformat_set = true;
}

void set_output_file(Program_args &args, std::string file) {
    if (!args.outpu_file.empty()) {
        throw Bech32mException("Output file already selected.\n");
    }
    if (file.empty()) {
        throw Bech32mException("Invalid argument parameter passed to program.\n");
    }
    // validation of the path if wanted
    args.outpu_file = std::move(file);
    args.output_type = output::file;
}

void set_input_file(Program_args &args, std::string file) {
    if (!args.input_file.empty()) {
        throw Bech32mException("Input file already selected.\n");
    }
    if (!args.input_text.empty()) {
        throw Bech32mException("Different input already selected.\n");
    }
    if (file.empty()) {
        throw Bech32mException("Invalid parameter " + file + " passed to argument --input-file.\n");
    }
    // validation of the path if wanted
    args.input_file = std::move(file);
    args.input_type = input::file;
}

void set_input_text(Program_args &args, std::string text) {
    if (!args.input_file.empty()) {
        throw Bech32mException("Different input already selected.\n");
    }
    if (!args.input_text.empty()) {
        throw Bech32mException("Input text already selected.\n");
    }
    if (text.empty()) {
        throw Bech32mException("Invalid parameter " + text + " passed to argument --input-text.\n");
    }
    // validation of the path if wanted
    args.input_text = std::move(text);
    args.input_type = input::argument;
}

Program_args Parser::parse(const int &argc, char **argv) {
    Program_args result;
    // skipping the first argument containing the name of the program
    int counter = 1;
    // iterating over all arguments
    while (counter < argc) {
        std::cout << counter << std::endl;
        // loading the argument
        std::string arg = argv[counter];

        std::cout << arg << std::endl;
        // veryfying that actual argument was specified as one of the valid ones
        if (arguments.find(arg) == arguments.end()) {
            throw Bech32mException("Program does not support the argument " + arg + ".\n");
        }
        // increasing counter to the parameter/next argument
        counter++;

        // loading argument ass object
        Argument current_arg = arguments[arg];
        std::string param;
        std::cout << current_arg.has_param() << std::endl;

        // checking if current argument should have parameters and if there is enaught of the arguments provided
        if (current_arg.has_param() && counter >= argc) {
            throw Bech32mException("Argument requiring parameter passed to program without parameter.\n");
        } else if (current_arg.has_param()) {
            // loading the parameter as string
            param = argv[counter];
            std::cout << param << std::endl;
            // veryfing that parameter is valid for current argument
            if (!current_arg.is_valid_param(param)) {
                throw Bech32mException("Argument " + arg + " was given invalid parameter " + param + " .\n");
            }
            // continuing to next argument
            counter++;
        }
        current_arg.invoke_handler(result, param);
    }
    return result;
}