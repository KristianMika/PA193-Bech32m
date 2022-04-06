
#include "bech32m.h"

int main() {
    std::string input = "aaaaaaaaaaa16002751e";
    std::string result = encode("aaaaaaaaaaa", input);
    std::cout << result << std::endl;
    return 0;
}
