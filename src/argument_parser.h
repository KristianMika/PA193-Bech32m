#include "bech32m_exception.h"
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER
enum class program_mode { encode, decode };
enum class data_form { bin, hex, base64, Bech32m };
enum class output {
    stdoutput,
    file,
};
enum class input { stdinput, file, argument };

struct Program_args {
    program_mode mode = program_mode::encode;

    input input_type = input::stdinput;
    std::string input_file;
    std::string input_text;

    // used in encode mode
    data_form input_format = data_form::hex;

    output output_type = output::stdoutput;
    std::string outpu_file;
    // used in decode mode
    data_form output_format = data_form::Bech32m;

    // flags to prevent setting both formats or one format repeatably
    bool oformat_set = false;
    bool iformat_set = false;
};

class Argument {
  public:
    using HandlerType = std::function<void(Program_args &, std::string)>;

  private:
    // set of valid parameters for the argument
    std::set<std::string> valid_params;
    // set of valid names for the argument
    std::string name;
    // function pointer
    HandlerType handler;

    bool param = false;
    bool variable_parameter = false;

  public:
    bool has_param() const { return param; }

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

    void invoke_handler(Program_args &args, std::string _param) { handler(args, std::move(_param)); }
};

void set_output_format(Program_args &args, const std::string &output);
void set_input_format(Program_args &args, const std::string &input);
void set_output_file(Program_args &args, std::string file);
void set_input_file(Program_args &args, std::string file);
void set_input_text(Program_args &args, std::string text);

class Parser {
  private:
    std::map<std::string, Argument> arguments;

  public:
    // adding argument with all its names
    Parser add_argument(Argument argument) {
        arguments.emplace(argument.get_name(), std::move(argument));
        return *this;
    }
    Program_args parse(const int &argc, const char **argv);
};


/**
 * Creates a default parser for the encoder
 * @return a parser object
 */
Parser get_default_parser();
#endif // ARGUMENT_PARSER