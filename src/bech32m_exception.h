#ifndef BECH32M_BECH32M_EXCEPTION_H
#define BECH32M_BECH32M_EXCEPTION_H
#include <exception>
#include <string>
struct Bech32mException : public std::exception {
    std::string cause = "Undefined error";

    Bech32mException(std::string &_cause) : cause(_cause) {}

    virtual const char *what() const throw() { return cause.c_str(); }
};

#endif // BECH32M_BECH32M_EXCEPTION_H
