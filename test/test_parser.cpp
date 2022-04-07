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

const std::string argv0 = "bech32m";

void test_simple_parse() {
    Parser parser = get_default_parser();
    Program_args arguments;
    std::string argv1("--input-format");
    std::string argv2("base64");
    const char *argv[] = {argv0.c_str(), argv1.c_str(), argv2.c_str()};
    int argc = 3;
    ASSERT_DOES_NOT_THROW(arguments = parser.parse(argc, argv));

    ASSERT_EQUALS(arguments.input_format, DataFormat::Base64);
}

void test_parser() { test_simple_parse(); }