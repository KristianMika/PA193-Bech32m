#include "../src/bech32m.h"
#include "../src/bech32m_bit_storage.h"
#include "../src/bech32m_error_detection.h"
#include "../src/bech32m_exception.h"
#include "test_macros.h"
#include "test_bit_storage.h"
#include <algorithm>
#include <iostream>
#include <string>

// counter for failed errors
long errors_count = 0;
long success_count = 0;

/**
 * Some basic inputs, test they are identified as valid Bech32m codes
 */
void test_basic() {
    ASSERT_DOES_NOT_THROW(decode("A1LQFN3A"));
    ASSERT_DOES_NOT_THROW(decode("a1lqfn3a"));
    ASSERT_DOES_NOT_THROW(
        decode("an83characterlonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber11sg7hg6"));
    ASSERT_DOES_NOT_THROW(decode("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx"));
    ASSERT_DOES_NOT_THROW(
        decode("11llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllludsr8"));
    ASSERT_DOES_NOT_THROW(decode("split1checkupstagehandshakeupstreamerranterredcaperredlc445v"));
    ASSERT_DOES_NOT_THROW(decode("?1v759aa"));
}

/**
 * Tests the proper Bech32m encoding
 */
void test_encode() {
    // v0-v16 native segregated witness addresses
    ASSERT_EQUALS(encode("BC", "0014751e76e8199196d454941c45d1b3a323f1433bd6"),
                  "BC1qq2828nkaqver9k52j2pc3w3kw3j8u2r80tqa77jaq");
    ASSERT_EQUALS(encode("tb", "00201863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262"),
                  "tb1qqspscc58s2v29ngqj73jgpn2mdpxmyc2euv6nf85xuvvv5kqjgrycswfkuav");
    ASSERT_EQUALS(encode("bc", "5128751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6"),
                  "bc12y5828nkaqver9k52j2pc3w3kw3j8u2r80t828nkaqver9k52j2pc3w3kw3j8u2r80tqspla6g");
    ASSERT_EQUALS(encode("BC", "6002751e"), "BC1vqp828sqtpk4a");
    ASSERT_EQUALS(encode("bc", "5210751e76e8199196d454941c45d1b3a323"), "bc12gg828nkaqver9k52j2pc3w3kw3jxf5uccw");
    ASSERT_EQUALS(encode("tb", "0020000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"),
                  "tb1qqsqqqqqcjju44rzyxe2rpustefxvd3tn827j8rvufx3vhdtj05xgvc9qertc");
    ASSERT_EQUALS(encode("tb", "5120000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"),
                  "tb12ysqqqqqcjju44rzyxe2rpustefxvd3tn827j8rvufx3vhdtj05xgvctfnkcd");
    ASSERT_EQUALS(encode("bc", "512079be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
                  "bc12ys8n0nx0muaewav2ksx99wwsu9swq5mlndjmn3gm9vl9q2mzmup0xqpy4uzg");
}

/**
 * Tests if invalid Bech32m codes can be identified
 */
void test_invalid_bech32m() {
    ASSERT_THROWS(decode(std::to_string(0x20) + "1xj0phk"), Bech32mException);
    ASSERT_THROWS(decode(std::to_string(0x7f) + "1g6xzxy"), Bech32mException);
    ASSERT_THROWS(decode(std::to_string(0x80) + "1vctc34"), Bech32mException);
    ASSERT_THROWS(decode("an84characterslonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber11d6pts4"),
                  Bech32mException);
    ASSERT_THROWS(decode("qyrz8wqd2c9m"), Bech32mException);
    ASSERT_THROWS(decode("1qyrz8wqd2c9m"), Bech32mException);
    ASSERT_THROWS(decode("y1b0jsk6g"), Bech32mException);
    ASSERT_THROWS(decode("lt1igcx5c0"), Bech32mException);
    ASSERT_THROWS(decode("in1muywd"), Bech32mException);
    ASSERT_THROWS(decode("mm1crxm3i"), Bech32mException);
    ASSERT_THROWS(decode("au1s5cgom"), Bech32mException);
    ASSERT_THROWS(decode("M1VUXWEZ"), Bech32mException);
    ASSERT_THROWS(decode("16plkw9"), Bech32mException);
    ASSERT_THROWS(decode("1p2gdwpf"), Bech32mException);
}

// std::string detected_str(const std::string &in) {
//     size_t idx_separator = in.rfind('1');
//     return to_hex(Bech32mBitStorage(detect_error(in, idx_separator).data));
// }
/**
 * Some basic inputs without any substitutions
 */
void test_without_errors() {
    // std::string str = to_hex(Bech32mBitStorage(detect_error("aaaaaaaaaaa14242424259sqyag70hggh0", 11).data));

    const auto storage = HexBitStorage(Bech32mBitStorage(detect_error("aaaaaaaaaaa14242424259sqyag70hggh0", 11).data));
    std::string str_pls = std::string(90, '_');
    size_t idx = 0;
    for(const auto& bs : storage) {
        str_pls[idx++] = static_cast<char>(bs.to_ulong());
    }
    std::stringstream stream{};
    stream << std::hex << storage.begin() << std::endl;
    std::string plsss = stream.str();
//    std::transform(storage.begin(), storage.end(), str_pls,
//                   [](std::bitset<4> it) -> char { return static_cast<char>(it.to_ulong()); });
    std::cout << plsss << std::endl;
    // ASSERT_EQUALS(detected_str("aaaaaaaaaaa14242424259sqyag70hggh0"), "aaaaaaaaaaa14242424259sqyag70hggh0")
}

/**
 * Prints the results of the tests
 * @return 0 if all tests have passed successfully, 1 otherwise
 */
int interpret_test_results() {
    std::cout << std::endl << "***** Test results *****" << std::endl;
    // Just to make sure we are running some tests
    std::cout << " ✓ " << success_count << " check(s) passed successfully." << std::endl;
    if (errors_count == 0) {
        std::cout << " ✓ "
                  << "All tests have passed successfully." << std::endl;
        return 0;
    } else {
        std ::cout << " ✘ " << errors_count << " check(s) failed." << std::endl;
        return 1;
    }
}
/**
 * Runs all tests.
 * @return Either 0 or 1, based on the number of failed tests.
 */
int main() {
    RUN_TEST(test_without_errors);

    RUN_TEST(test_basic);
    RUN_TEST(test_encode);
    RUN_TEST(test_without_errors);
    RUN_TEST(test_invalid_bech32m);
    RUN_TEST(test_bit_storage)

    return interpret_test_results();
}
