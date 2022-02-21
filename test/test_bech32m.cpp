#include "../src/bech32m.h"
#include "../src/bech32m_exception.h"
#include "test_macros.h"
#include <iostream>

/**
 * Some basic inputs, test they are identified as valid Bech32m codes
 */
void test_basic() {
    ASSERT_DOES_NOT_THROW(decode("A1LQFN3A"));
    ASSERT_DOES_NOT_THROW(decode("a1lqfn3a"));
    ASSERT_DOES_NOT_THROW(decode("an83characterlonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber11sg7hg6"));
    ASSERT_DOES_NOT_THROW(decode("abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx"));
    ASSERT_DOES_NOT_THROW(decode("11llllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllludsr8"));
    ASSERT_DOES_NOT_THROW(decode("split1checkupstagehandshakeupstreamerranterredcaperredlc445v"));
    ASSERT_DOES_NOT_THROW(decode("?1v759aa"));
}

/**
 * Tests the proper Bech32m encoding
 */
void test_encode() {
    // v0-v16 native segregated witness addresses
    ASSERT_EQUALS(encode("0014751e76e8199196d454941c45d1b3a323f1433bd6"), "BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4");
    ASSERT_EQUALS(encode("00201863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262"),
                  "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7");
    ASSERT_EQUALS(encode("5128751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6"),
                  "bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y");
    ASSERT_EQUALS(encode("6002751e"), "BC1SW50QGDZ25J");
    ASSERT_EQUALS(encode("5210751e76e8199196d454941c45d1b3a323"), "bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs");
    ASSERT_EQUALS(encode("0020000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"),
                  "tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy");
    ASSERT_EQUALS(encode("5120000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"),
                  "tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c");
    ASSERT_EQUALS(encode("512079be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
                  "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0");
}

/**
 * Tests the proper Bech32m decoding
 */
void test_decode() {
    // v0-v16 native segregated witness addresses
    ASSERT_EQUALS(decode("BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4"), "0014751e76e8199196d454941c45d1b3a323f1433bd6");
    ASSERT_EQUALS(decode("tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7"),
                  "00201863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262");
    ASSERT_EQUALS(decode("bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y"),
                  "5128751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6");
    ASSERT_EQUALS(decode("BC1SW50QGDZ25J"), "6002751e");
    ASSERT_EQUALS(decode("bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs"), "5210751e76e8199196d454941c45d1b3a323");
    ASSERT_EQUALS(decode("tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy"),
                  "0020000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433");
    ASSERT_EQUALS(decode("tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c"),
                  "5120000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433");
    ASSERT_EQUALS(decode("bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0"),
                  "512079be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798");
}

/**
 * Tests if invalid Bech32m codes can be identified
 */
void test_invalid_bech32m() {
    ASSERT_THROWS(decode(std::to_string(0x20) + "1xj0phk"), Bech32mException);
    ASSERT_THROWS(decode(std::to_string(0x7f) + "1g6xzxy"), Bech32mException);
    ASSERT_THROWS(decode(std::to_string(0x80) + "1vctc34"), Bech32mException);
    ASSERT_THROWS(decode("an84characterslonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber11d6pts4"), Bech32mException);
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

void test_invalid_segwit_addresses() {
    ASSERT_THROWS(decode("tc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vq5zuyut"), Bech32mException);
    ASSERT_THROWS(decode("bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqh2y7hd"), Bech32mException);
    ASSERT_THROWS(decode("BC1S0XLXVLHEMJA6C4DQV22UAPCTQUPFHLXM9H8Z3K2E72Q4K9HCZ7VQ54WELL"), Bech32mException);
    ASSERT_THROWS(decode("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kemeawh"), Bech32mException);
    ASSERT_THROWS(decode("tb1q0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vq24jc47"), Bech32mException);
    ASSERT_THROWS(decode("bc1p38j9r5y49hruaue7wxjce0updqjuyyx0kh56v8s25huc6995vvpql3jow4"), Bech32mException);
    ASSERT_THROWS(decode("BC130XLXVLHEMJA6C4DQV22UAPCTQUPFHLXM9H8Z3K2E72Q4K9HCZ7VQ7ZWS8R"), Bech32mException);
    ASSERT_THROWS(decode("bc1pw5dgrnzv"), Bech32mException);
    ASSERT_THROWS(decode("bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7v8n0nx0muaewav253zgeav"), Bech32mException);
    ASSERT_THROWS(decode("BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P"), Bech32mException);
    ASSERT_THROWS(decode("tb1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vq47Zagq"), Bech32mException);
    ASSERT_THROWS(decode("bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7v07qwwzcrf"), Bech32mException);
    ASSERT_THROWS(decode("tb1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vpggkg4j"), Bech32mException);
    ASSERT_THROWS(decode("bc1gmk9yu"), Bech32mException);
}

/**
 * Runs all tests.
 * @return Either 0 or 1, based on the number of failed tests.
 */
int main() {
    test_basic();
    test_encode();
    test_decode();
    test_invalid_bech32m();
    test_invalid_segwit_addresses();

    if (errors_count == 0) {
        std::cout << "All tests have passed successfully." << std::endl;
        return 0;
    } else {
        std ::cout << errors_count << " test(s) failed." << std::endl;
        return 1;
    }
}