#include "argument_parser.h"

// ./program --encode --from_bin

void set_output_format(Program_args &args, const std::string &output) {
    if (args.oformat_set || args.iformat_set) {
        throw Bech32mException("Format for the program has been already specified.");
    }

    if (output == BIN_STRING) {
        args.output_format = DataFormat::Bin;
    } else if (output == HEX_STRING) {
        args.output_format = DataFormat::Hex;
    } else if (output == BASE64_STRING) {
        args.output_format = DataFormat::Base64;
    } else {
        throw Bech32mException("Invalid parameter " + output + " passed to argument --output-format.");
    }
    args.input_format = DataFormat::Bech32m;
    args.oformat_set = true;
    args.mode = Mode::Decode;
}

void set_input_format(Program_args &args, const std::string &input) {
    if (args.oformat_set || args.iformat_set) {
        throw Bech32mException("Format for the program has been already specified.");
    }

    if (input == BIN_STRING) {
        args.input_format = DataFormat::Bin;
    } else if (input == HEX_STRING) {
        args.input_format = DataFormat::Hex;
    } else if (input == BASE64_STRING) {
        args.input_format = DataFormat::Base64;
    } else {
        throw Bech32mException("Invalid parameter " + input + " passed to argument --input-format.");
    }

    args.iformat_set = true;
}

void set_output_file(Program_args &args, std::string file) {
    if (!args.outpu_file.empty()) {
        throw Bech32mException("Output file already selected.");
    }
    if (file.empty()) {
        throw Bech32mException("Invalid argument parameter passed to program.");
    }
    // validation of the path if wanted
    args.outpu_file = std::move(file);
    args.output_type = Output::File;
}

void set_input_file(Program_args &args, std::string file) {
    if (!args.input_file.empty()) {
        throw Bech32mException("Input file already selected.");
    }
    if (!args.input_text.empty()) {
        throw Bech32mException("Different input already selected.");
    }
    if (file.empty()) {
        throw Bech32mException("Invalid parameter " + file + " passed to argument --input-file.");
    }
    // validation of the path if wanted
    args.input_file = std::move(file);
    args.input_type = Input::File;
}

void set_input_text(Program_args &args, std::string text) {
    if (!args.input_file.empty()) {
        throw Bech32mException("Different input already selected.");
    }
    if (!args.input_text.empty()) {
        throw Bech32mException("Input text already selected.");
    }
    if (text.empty()) {
        throw Bech32mException("Invalid parameter " + text + " passed to argument --input-text.");
    }
    // validation of the path if wanted
    args.input_text = std::move(text);
    args.input_type = Input::Argument;
}

Program_args Parser::parse(const int &argc, const char **argv) {
    Program_args result;
    // skipping the first argument containing the name of the program
    int counter = 1;
    // iterating over all arguments
    while (counter < argc) {
        std::cout << counter << std::endl;
        // loading the argument
        std::string arg = argv[counter];

        std::cout << arg << std::endl;
        // verifying that actual argument was specified as one of the valid ones
        if (arguments.find(arg) == arguments.end()) {
            throw Bech32mException("Program does not support the argument " + arg + ".");
        }
        // increasing counter to the parameter/next argument
        counter++;

        // loading argument ass object
        Argument current_arg = arguments[arg];
        std::string param;
        std::cout << current_arg.has_param() << std::endl;

        // checking if current argument should have parameters and if there is enough of the arguments provided
        if (current_arg.has_param() && counter >= argc) {
            throw Bech32mException("Argument requiring parameter passed to program without parameter.");
        }
        if (current_arg.has_param()) {
            // loading the parameter as string
            param = argv[counter];
            std::cout << param << std::endl;
            // verifying that parameter is valid for current argument
            if (!current_arg.is_valid_param(param)) {
                throw Bech32mException("Argument " + arg + " was given invalid parameter " + param + ".");
            }
            // continuing to next argument
            counter++;
        }
        current_arg.invoke_handler(result, param);
    }
    return result;
}

Parser get_default_parser() {
    // clang-format off
    return Parser()
            .add_argument(Argument()
                                  .set_name("--input-format")
                                  .add_param_value(BIN_STRING)
                                  .add_param_value(HEX_STRING)
                                  .add_param_value(BASE64_STRING)
                                  .add_handler(set_input_format))
            .add_argument(Argument()
                                  .set_name("--output-format")
                                  .add_param_value(BIN_STRING)
                                  .add_param_value(HEX_STRING)
                                  .add_param_value(BASE64_STRING)
                                  .add_handler(set_output_format))
            .add_argument( Argument()
                                   .set_name("--input-file")
                                   .set_variable_param()
                                   .add_handler(set_input_file))
            .add_argument(Argument()
                                  .set_name("--input-text")
                                  .set_variable_param()
                                  .add_handler(set_input_text))
            .add_argument(Argument()
                                  .set_name("--output-file")
                                  .set_variable_param()
                                  .add_handler(set_output_file));
    // clang-format on
}