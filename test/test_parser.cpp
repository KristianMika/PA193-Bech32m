#include "test_parser.h"
#include "../src/argument_parser.h"
#include "test_macros.h"

// counter for failed errors
extern long errors_count;
extern long success_count;

/**
 * operator<<, required only for tests
 */
inline std::ostream &operator<<(std::ostream &os, const DataFormat &data) {
    switch (data) {
    case DataFormat::Base64:
        return os << "Base64";

    case DataFormat::Bech32m:
        return os << "Bech32m";

    case DataFormat::Bin:
        return os << "bin";

    case DataFormat::Hex:
        return os << "hex";
    }
    throw std::runtime_error("Unreachable code");
}

inline std::ostream &operator<<(std::ostream &os, const Mode &mode) {
    switch (mode) {
    case Mode::Decode:
        return os << "Decode";

    case Mode::Encode:
        return os << "Encode";
    }
    throw std::runtime_error("Unreachable code");
}

inline std::ostream &operator<<(std::ostream &os, const Output &out) {
    switch (out) {
    case Output::File:
        return os << "File";

    case Output::Stdout:
        return os << "Stdout";
    }
    throw std::runtime_error("Unreachable code");
}

inline std::ostream &operator<<(std::ostream &os, const Input &in) {
    switch (in) {
    case Input::File:
        return os << "File";

    case Input::Stdin:
        return os << "Stdout";

    case Input::Argument:
        return os << "Argument";
    }
    throw std::runtime_error("Unreachable code");
}

const std::string argv0 = "bech32m";

const std::string input_format("--input-format");
const std::string output_format("--output-format");
const std::string help("--help");
const std::string decode("--decode");
const std::string input_file("--input-file");
const std::string input_text("--input-text");
const std::string output_file("--output-file");
const std::string hrp("--hrp");
const std::string empty_hrp("--allow-empty-hrp");
const std::string file_in("filenamein");
const std::string text_input("asddfasdfasdf");
const std::string default_hrp("testhrp");
const std::string file_out("filenameout");


int to_argv(std::string args, char** argv) { 
    int word_counter = 0;
    int letter_counter = 0;
    for (const char &c : args) {
        if (c == ' ') {
            argv[word_counter][letter_counter] = '\0';
            word_counter++;
            letter_counter = 0;
        } else {
            argv[word_counter][letter_counter] = 'c';
        }     
    }
    if (args[args.size() - 1] != ' ') {
        argv[word_counter][letter_counter] = '\0';
    }
    return word_counter;

 }

void test_simple_parse() {
    Parser parser = get_default_parser();
    ProgramArgs arguments;
    std::string argv1("--input-format");
    std::string argv2("base64");
    const char *argv[] = {argv0.c_str(), argv1.c_str(), argv2.c_str()};
    int argc = 3;
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(argc, argv));

    ASSERT_EQUALS(arguments.input_format, DataFormat::Base64);
}

void test_parse_invalid() {
    Parser parser = get_default_parser();
    ProgramArgs arguments;

    const char *argv[] = {argv0.c_str(), input_format.c_str(), "bin", decode.c_str()};
    ASSERT_THROWS(arguments = parser.parse(4, argv), Bech32mException);

    const char *argv1[] = {argv0.c_str(), "hex", decode.c_str()};
    ASSERT_THROWS(arguments = parser.parse(3, argv1), Bech32mException);

    const char *argv2[] = {argv0.c_str(), decode.c_str(), output_format.c_str(), "hex", hrp.c_str(), default_hrp.c_str()};
    ASSERT_THROWS(arguments = parser.parse(6, argv2), Bech32mException);

    const char *argv3[] = {argv0.c_str(), empty_hrp.c_str(), hrp.c_str()};
    ASSERT_THROWS(arguments = parser.parse(3, argv3), Bech32mException);

    const char *argv4[] = {argv0.c_str(), input_file.c_str(), file_in.c_str(), input_text.c_str(), text_input.c_str()};
    ASSERT_THROWS(arguments = parser.parse(5, argv4), Bech32mException);

    const char *argv5[] = {argv0.c_str(), input_file.c_str(), file_in.c_str(), input_file.c_str(), file_in.c_str()};
    ASSERT_THROWS(arguments = parser.parse(5, argv5), Bech32mException);

    const char *argv6[] = {argv0.c_str(), input_file.c_str(), file_in.c_str(), input_file.c_str(), file_in.c_str()};
    ASSERT_THROWS(arguments = parser.parse(5, argv6), Bech32mException);

    const char *argv7[] = {argv0.c_str(), "asdfghhjkl"};
    ASSERT_THROWS(arguments = parser.parse(2, argv7), Bech32mException);

    const char *argv8[] = {argv0.c_str(), output_format.c_str(), "asdfghhjkl"};
    ASSERT_THROWS(arguments = parser.parse(3, argv8), Bech32mException);

    const char *argv9[] = {argv0.c_str(), input_format.c_str(), "asdfghhjkl"};
    ASSERT_THROWS(arguments = parser.parse(3, argv9), Bech32mException);

    const char *argv10[] = {argv0.c_str(), decode.c_str()};
    ASSERT_THROWS(arguments = parser.parse(2, argv10), Bech32mException);

    const char *argv11[] = {argv0.c_str(), decode.c_str(), hrp.c_str(), default_hrp.c_str()};
    ASSERT_THROWS(arguments = parser.parse(4, argv11), Bech32mException);

}


void test_parse() { 
    //argc = to_argv(arguments, argv);
    Parser parser = get_default_parser();
    ProgramArgs arguments;

    const char *argv1[] = {argv0.c_str(), input_format.c_str(), "hex", input_file.c_str(), file_in.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(5, argv1));
    
    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::File);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, file_in);
    ASSERT_EQUALS(arguments.input_format, DataFormat::Hex);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);


    const char *argv2[] = {argv0.c_str(), input_format.c_str(), "hex", input_text.c_str(), text_input.c_str(),
                           output_file.c_str(), file_out.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(7, argv2));
    
    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::File);
    ASSERT_EQUALS(arguments.input_type, Input::Argument);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, text_input);
    ASSERT_EQUALS(arguments.outpu_file, file_out);
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Hex);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);
    
    
    const char *argv3[] = {argv0.c_str(), input_format.c_str(), "bin", input_file.c_str(), file_in.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(5, argv3));

    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::File);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, file_in);
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bin);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);


    const char *argv4[] = {argv0.c_str(), output_format.c_str(), "hex", decode.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(4, argv4));

    ASSERT_EQUALS(arguments.mode, Mode::Decode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bech32m);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Hex);

    const char *argv5[] = {argv0.c_str(), decode.c_str(), output_format.c_str(), "bin", empty_hrp.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(5, argv5));

    ASSERT_EQUALS(arguments.mode, Mode::Decode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, true);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bech32m);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bin);


    const char *argv6[] = {argv0.c_str(), input_format.c_str(), "base64", hrp.c_str(), default_hrp.c_str(), output_file.c_str(), file_out.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(7, argv6));

    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, default_hrp);
    ASSERT_EQUALS(arguments.output_type, Output::File);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, file_out.c_str());
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Base64);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);


    const char *argv7[] = {argv0.c_str(), help.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(2, argv7));
    ASSERT_EQUALS(arguments.print_help, true);


    const char *argv8[] = {argv0.c_str(), input_format.c_str(), "base64", output_file.c_str(), file_out.c_str(), hrp.c_str(), default_hrp.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(7, argv8));

    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, default_hrp);
    ASSERT_EQUALS(arguments.output_type, Output::File);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, file_out.c_str());
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Base64);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);


    const char *argv9[] = {argv0.c_str(), output_format.c_str(), "base64", decode.c_str(), input_file.c_str(), file_in.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(6, argv9));

    ASSERT_EQUALS(arguments.mode, Mode::Decode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::File);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, file_in.c_str());
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bech32m);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Base64);


    const char *argv10[] = {argv0.c_str(), empty_hrp.c_str(), input_format.c_str(), "bin", input_text.c_str(), text_input.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(6, argv10));

    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, true);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::Argument);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, text_input);
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bin);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);


    const char *argv11[] = {argv0.c_str(),    decode.c_str(),        output_file.c_str(),
                            file_out.c_str(), output_format.c_str(), "hex"};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(6, argv11));

    ASSERT_EQUALS(arguments.mode, Mode::Decode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::File);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, file_out.c_str());
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bech32m);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Hex);

    const char *argv12[] = {argv0.c_str(), input_format.c_str(), "bin", hrp.c_str(), default_hrp.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(5, argv12));

    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, default_hrp);
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bin);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);


    const char *argv13[] = {argv0.c_str(), input_format.c_str(), "base64"};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(3, argv13));

    ASSERT_EQUALS(arguments.mode, Mode::Encode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, false);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::Stdin);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, "");
    ASSERT_EQUALS(arguments.input_format, DataFormat::Base64);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Bech32m);

    const char *argv14[] = {argv0.c_str(), input_file.c_str(), file_in.c_str(), output_format.c_str(), "hex", decode.c_str(), empty_hrp.c_str()};
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(7, argv14));

    ASSERT_EQUALS(arguments.mode, Mode::Decode);
    ASSERT_EQUALS(arguments.allow_empty_hrp, true);
    ASSERT_EQUALS(arguments.hrp, "");
    ASSERT_EQUALS(arguments.output_type, Output::Stdout);
    ASSERT_EQUALS(arguments.input_type, Input::File);
    ASSERT_EQUALS(arguments.print_help, false);
    ASSERT_EQUALS(arguments.input_text, "");
    ASSERT_EQUALS(arguments.outpu_file, "");
    ASSERT_EQUALS(arguments.input_file, file_in.c_str());
    ASSERT_EQUALS(arguments.input_format, DataFormat::Bech32m);
    ASSERT_EQUALS(arguments.output_format, DataFormat::Hex);

}

void test_parser() {
    test_simple_parse();
    test_parse_invalid();
    test_parse();
}
