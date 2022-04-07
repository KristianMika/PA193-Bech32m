#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include "bech32m_exception.h"
#include <iostream>

#ifndef ARGUMENT_PARSER
#define ARGUMENT_PARSER
enum class program_mode { 
    encode, 
    decode };
enum class data_form { 
    bin, 
    hex, 
    base64, 
    Bech32m };
enum class output {
    stdoutput,
    file,
};
enum class input { 
    stdinput, 
    file, 
    argument };

struct Program_args {
    program_mode mode = program_mode::encode;

    input input_type = input::stdinput;
    std::string input_file = "";
    std::string input_text = "";

    // used in encode mode
    data_form input_format = data_form::hex;

    output output_type = output::stdoutput;
    std::string outpu_file = "";
    // used in decode mode
    data_form output_format = data_form::Bech32m;

    // flags to prevent setting both formats or one format repeatably
    bool oformat_set = false;
    bool iformat_set = false;
};

class Argument {
  private:
    // set of valid parameters for the argument
    std::set<std::string> valid_params;
    // set of valid names for the argument
    std::set<std::string> names;
    // function pointer
    std::function<void(Program_args &, const std::string &)> handler;
    bool param = false;
    bool variable_parameter = false;

  public:
    std::string key;
    bool has_param() { return param; }

    Argument &set_variable_param() {
        param = true;
        variable_parameter = true;
        return *this;
    }

    Argument &add_param_value(const std::string &param) {
        valid_params.insert(param);
        this->param = true;
        return *this;
    }

    bool is_valid_param(const std::string &param) {
        return variable_parameter || valid_params.find(param) != valid_params.end();
    }

    Argument &add_name(const std::string &name) {
        names.insert(name);
        return *this;
    }

    const std::set<std::string> all_names() { return names; }

    Argument &add_handler(std::function<void(Program_args &, const std::string &)> handle) {
        handler = handle;
        return *this;
    }

    void invoke_handler(Program_args &args, const std::string &param) { handler(args, param); }
};


void set_output_format(Program_args &args, const std::string &output);
void set_input_format(Program_args &args, const std::string &input);
void set_output_file(Program_args &args, const std::string &file);
void set_input_file(Program_args &args, const std::string &file);
void set_input_text(Program_args &args, const std::string &text);


class Parser {
  private:
    std::map<std::string, Argument> arguments;

  public:
    // adding argument with all its names
    Parser &add_argument(Argument argument) {
        arguments.emplace(argument.key, argument);
        for (const std::string &name : argument.all_names()) {
            arguments.emplace(name, argument);
        }
        return *this;
    }
    Program_args parse(const int &argc, char **argv);
};

#endif // ARGUMENT_PARSER