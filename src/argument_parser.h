#include "bech32m_exception.h"
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>

#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER
enum class Mode { Encode, Decode };
enum class DataFormat { Bin, Hex, Base64, Bech32m };
enum class Output {
    Stdout,
    File,
};
enum class Input { Stdin, File, Argument };

const char *const BIN_STRING = "bin";
const char *const HEX_STRING = "hex";
const char *const BASE64_STRING = "base64";
const char *const BECH32M_STRING = "bech32m";

struct ProgramArgs {
    Mode mode = Mode::Encode;

    Input input_type = Input::Stdin;
    std::string input_file;
    std::string input_text;

    // used in Encode mode
    DataFormat input_format = DataFormat::Hex;

    Output output_type = Output::Stdout;
    std::string outpu_file;
    // used in Decode mode
    DataFormat output_format = DataFormat::Bech32m;

    bool print_help = false;

    // flags to prevent setting both formats or one format repeatably
    bool oformat_set = false;
    bool iformat_set = false;

    // hrp handling
    bool allow_empty_hrp = false;
    std::string hrp;

    bool trim_trailing_zeros = false;
};

class Argument {
  public:
    using HandlerType = std::function<void(ProgramArgs &, std::string)>;

  private:
    // set of valid parameters for the argument
    std::set<std::string> valid_params;
    // set of valid names for the argument
    std::string name;
    // description of the argument
    std::string description = "No description provided.";
    // function pointer
    HandlerType handler;


    bool param = false;
    bool variable_parameter = false;

  public:
    bool has_param() const { return param; }
    bool has_variable_param() const { return variable_parameter; }

    Argument set_variable_param() {
        param = true;
        variable_parameter = true;
        return *this;
    }

    Argument add_param_value(std::string _param) {
        valid_params.insert(std::move(_param));
        this->param = true;
        return *this;
    }

    bool is_valid_param(const std::string &_param) {
        return variable_parameter || valid_params.find(_param) != valid_params.end();
    }

    Argument set_name(std::string _name) {
        name = std::move(_name);
        return *this;
    }

    std::string get_name() const { return name; }

    Argument add_handler(HandlerType handle) {
        handler = std::move(handle);
        return *this;
    }

    Argument set_description(std::string _description) { 
        description = std::move(_description);
        return *this;
    }

    std::set<std::string> get_valid_params() const { 
        return valid_params;
    }

    std::string get_description() const { return description; }

    void invoke_handler(ProgramArgs &args, std::string _param) { handler(args, std::move(_param)); }
};

void set_output_format(ProgramArgs &args, const std::string &output);
void set_input_format(ProgramArgs &args, const std::string &input);
void set_output_file(ProgramArgs &args, std::string file);
void set_input_file(ProgramArgs &args, std::string file);
void set_input_text(ProgramArgs &args, std::string text);
void set_help(ProgramArgs &args, std::string);
void set_defualt_hrp(ProgramArgs &args, std::string);
void allow_empty_hrp(ProgramArgs &args, std::string);
void set_mode_decode(ProgramArgs &args, std::string);
void set_trimming(ProgramArgs &args, std::string);

class Parser {
  private:
    std::map<std::string, Argument> arguments;
    void postprocess(const ProgramArgs &args);

  public:
    // adding argument with all its names
    Parser add_argument(Argument argument) {
        arguments.emplace(argument.get_name(), std::move(argument));
        return *this;
    }
    ProgramArgs parse(const int &argc, const char **argv);

    std::string usage() {
        const std::string alignment_space = "    ";
        std::stringstream result;
        result << std::string("Usage:") << std::endl;
        for (auto const &[key, argument] : arguments) {
            result << key << std::endl << alignment_space << argument.get_description() << std::endl;
            result << alignment_space << "Possible parameters: ";
            if (argument.has_variable_param()) {
                result << "Arbitrary value.";
            } else if (argument.has_param()) {
                for (const std::string &parameter : argument.get_valid_params()) {
                    result << parameter << ", ";
                }
            } else {
                result << "No parameters.";
            }
            result << std::endl << std::endl;
        }
        return result.str();
    }
};

/**
 * Creates a default parser for the encoder
 * @return a parser object
 */
Parser get_default_parser();

#endif // ARGUMENT_PARSER