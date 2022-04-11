import os
import sys

ENC_CODE = '''
#include "libbech32.h"
#include <iostream>

int main(int argc, char **argv) {
    std::string hrp = argv[1];
    std::vector<unsigned char> data = {};
    for (int i = 2; i < argc; i++) {
        std::string integer = argv[i];
        long l = std::stol(integer);
        data.push_back(static_cast<unsigned char>(l));
    }
    std::string bstr = bech32::encode(hrp, data);
    std::cout << bstr << std::endl;
}

'''

DEC_CODE = '''
#include "libbech32.h"
#undef NDEBUG
#include <cassert>
#include <iostream>

int main(int argc, char** argv) {
    static const std::string BECH_SYMBOLS = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
    bech32::DecodedResult decodedResult = bech32::decode(argv[1]);
    std::string s = std::string(decodedResult.dp.size(), '0');
    size_t i = 0;
    for(unsigned char c : decodedResult.dp) {
        s[i++] = BECH_SYMBOLS[static_cast<int>(c)];
    }
    std::cout << s << std::endl;
    return 0;
}

'''

if __name__ == '__main__':
    ENC_CPP_FILE = sys.argv[1]
    DEC_CPP_FILE = sys.argv[2]

    os.remove(ENC_CPP_FILE)
    with open(ENC_CPP_FILE, 'w') as f:
        f.write(ENC_CODE)

    os.remove(DEC_CPP_FILE)
    with open(DEC_CPP_FILE, 'w') as f:
        f.write(DEC_CODE)