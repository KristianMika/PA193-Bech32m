#include "argument_parser.h"

// ./program --encode --from_bin

void set_output_format(ProgramArgs &args, const std::string &output) {
    if (args.oformat_set || args.iformat_set) {
        throw Bech32mException("Format for the input or output data has been already specified.");
    }
    if (!args.hrp.empty()) {
        throw Bech32mException("Default hrp can not be set for decoding.");
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
    args.oformat_set = true;
}

void set_input_format(ProgramArgs &args, const std::string &input) {
    if (args.oformat_set || args.iformat_set) {
        throw Bech32mException("Format for the input or output data has been already specified.");
    }
    if (args.mode == Mode::Decode) {
        throw Bech32mException("Input format can be set only for encoding.");
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

void set_output_file(ProgramArgs &args, std::string file) {
    if (!args.output_file.empty()) {
        throw Bech32mException("Output file already selected.");
    }
    if (file.empty()) {
        throw Bech32mException("Invalid argument parameter passed to program.");
    }
    // validation of the path if wanted
    args.output_file = std::move(file);
    args.output_type = Output::File;
}

void set_input_file(ProgramArgs &args, std::string file) {
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

void set_input_text(ProgramArgs &args, std::string text) {
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

void set_help(ProgramArgs &args, std::string) {
    args.print_help = true;
}

void set_defualt_hrp(ProgramArgs& args, std::string hrp) { 
    // validate hrp

    if (hrp.empty()) {
        throw Bech32mException("Invalid parameter " + hrp + " passed to argument --hrp.");
    }
    if (!args.hrp.empty()) {
        throw Bech32mException("Default hrp was already set.");
    }
    if (args.allow_empty_hrp) {
        throw Bech32mException("Can not use default hrp if empty hrp is allowed.");
    }
    if (args.output_format != DataFormat::Bech32m) {
        throw Bech32mException("Default hrp can not be set for decoding.");
    }
    if (args.mode == Mode::Decode) {
        throw Bech32mException("Default hrp can not be set for decoding.");
    }
    args.hrp = std::move(hrp);
}

void allow_empty_hrp(ProgramArgs &args, std::string) {

    if (!args.hrp.empty()) {
        throw Bech32mException("Default hrp was already set.");
    }
    if (args.allow_empty_hrp) {
        throw Bech32mException("Empty hrp was already allowed.");
    }

    args.allow_empty_hrp = true;
}

void set_mode_decode(ProgramArgs& args, std::string) { 
    if (args.mode == Mode::Decode) {
        throw Bech32mException("Decoding was already set.");
    }
    if (args.iformat_set) {
        throw Bech32mException("Input format was already specified, which is invalid for decoding.");
    }
    if (!args.hrp.empty()) {
        throw Bech32mException("Default hrp was set. Invalid for decoding.");
    }

    args.input_format = DataFormat::Bech32m;
    args.mode = Mode::Decode;

}

void set_trimming(ProgramArgs &args, std::string) { 
    args.trim_trailing_zeros = true;
}

void Parser::postprocess(const ProgramArgs& args) { 
    if (args.mode == Mode::Encode) {
        if (args.output_format != DataFormat::Bech32m) {
            throw Bech32mException("Output format changed for encoding.");
        }
        if (args.input_format == DataFormat::Bech32m) {
            throw Bech32mException("Invalid input format set for encoding.");
        }
    }

    if (args.mode == Mode::Decode) {
        if (args.output_format == DataFormat::Bech32m) {
            throw Bech32mException("No output format set for decoding.");
        }
        if (args.input_format != DataFormat::Bech32m) {
            throw Bech32mException("Input format changed for decoding.");
        }
        if (!args.hrp.empty()) {
            throw Bech32mException("Default hrp set for decoding.");
        }
    }

    if (!args.hrp.empty() && args.allow_empty_hrp) {
        throw Bech32mException("Default hrp set and empty hrp allowed at the same time.");
    }

    if (args.iformat_set && args.oformat_set) {
        throw Bech32mException("Both input and output formats has been changed.");
    }

    if (!args.input_file.empty() && !args.input_text.empty()) {
        throw Bech32mException("Both input text and input files has been selected.");
    }

    if (args.input_type == Input::File && args.input_file.empty()) {
        throw Bech32mException("Empty name for input file.");
    }

    if (args.output_type == Output::File && args.output_file.empty()) {
        throw Bech32mException("Empty name for output file.");
    }

    if (args.input_type == Input::Argument && args.input_text.empty()) {
        throw Bech32mException("Empty input text given as argument.");
    }
}

ProgramArgs Parser::parse(const int &argc, const char **argv) {
    ProgramArgs result;
    // skipping the first argument containing the name of the program
    int counter = 1;
    // iterating over all arguments
    while (counter < argc) {
        // loading the argument
        std::string arg = argv[counter];
        
        // verifying that actual argument was specified as one of the valid ones
        if (arguments.find(arg) == arguments.end()) {
            throw Bech32mException("Program does not support the argument " + arg + ".");
        }
        // increasing counter to the parameter/next argument
        counter++;

        // loading argument ass object
        Argument current_arg = arguments[arg];
        std::string param;

        // checking if current argument should have parameters and if there is enough of the arguments provided
        if (current_arg.has_param() && counter >= argc) {
            throw Bech32mException("Argument requiring parameter passed to program without parameter.");
        }
        if (current_arg.has_param()) {
            // loading the parameter as string
            param = argv[counter];
            // verifying that parameter is valid for current argument
            if (!current_arg.is_valid_param(param)) {
                throw Bech32mException("Argument " + arg + " was given invalid parameter " + param + ".");
            }
            if (!param.empty() && param[0] == '-') {
                throw Bech32mException("Argument " + arg + " expected argument and was given None or one starting with '-' symbol.");
            }
            // continuing to next argument
            counter++;
        }
        current_arg.invoke_handler(result, param);
    }
    postprocess(result);
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
                                  .add_handler(set_input_format)
                                  .set_description("The format of input data. Encoding selected-format -> Bech32m."
                                                   "Mutually exclusive with --decode and --output-format."))
            .add_argument(Argument()
                                  .set_name("--output-format")
                                  .add_param_value(BIN_STRING)
                                  .add_param_value(HEX_STRING)
                                  .add_param_value(BASE64_STRING)
                                  .add_handler(set_output_format)
                                  .set_description("The format of output data. Decoding Bech32m -> selected-format"
                                                   "Mutually exclusive with --input-format and --hrp."))
            .add_argument( Argument()
                                   .set_name("--input-file")
                                   .set_variable_param()
                                   .add_handler(set_input_file)
                                   .set_description("File with the inputs. Mutually exclusive with --input-text."))
            .add_argument(Argument()
                                  .set_name("--input-text")
                                  .set_variable_param()
                                  .add_handler(set_input_text)
                                  .set_description("Next CLI argument will be interpreted as the program input."
                                                   "Mutually exclusive with --input-file."))
            .add_argument(Argument()
                                  .set_name("--output-file")
                                  .set_variable_param()
                                  .add_handler(set_output_file)
                                  .set_description("The file into which the program outputs will be written."))
            .add_argument(Argument()
                                   .set_name("--help")
                                   .set_description("Prints this help.")
                                   .add_handler(set_help))
            .add_argument(Argument()
                                   .set_name("--hrp")
                                   .set_variable_param()
                                   .set_description("Default hrp. Mutually exclusive with --allow-empty-hrp, --decode and --output-format.")
                                   .add_handler(set_defualt_hrp))
            .add_argument(Argument()
                                   .set_name("--allow-empty-hrp")
                                   .set_description("Allows usage of empty hrp. Mutually exclusive with --hrp.")
                                   .add_handler(allow_empty_hrp))
            .add_argument(Argument()
                                   .set_name("--decode")
                                   .set_description("Changes program mode to decode. Output format has to be specified."
                                                    "Mutually exclusive with --input-format and --hrp.")
                                   .add_handler(set_mode_decode))
            .add_argument(Argument()
                                   .set_name("--trim")
                                   .set_description("Sets the trimming of the trailing zero in decode mode. Trimming additional bits"
                                                    "instead of padding the input in encode mode.")
                                   .add_handler(set_trimming));
    // clang-format on
}