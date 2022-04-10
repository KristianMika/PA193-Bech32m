#include "../src/bech32m.h"
#include "test_bit_storage.h"
#include "test_macros.h"
#include "test_parser.h"
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

/**
 * Basic decode without any substitutions
 */
void test_without_substitutions() {
    const auto DATA1 = "00000000000000000000";
    const auto CODE1 = "0123456789abcdef1qqqqqqqqqqqqqqqqlaltj2";

	ASSERT_EQUALS(decode(CODE1, DataFormat::Hex), DATA1)

	const auto DATA2 = "11111111111111111111";
	const auto CODE2 = "helloworld1zyg3zyg3zyg3zyg33k9kks";

	ASSERT_EQUALS(decode(CODE2, DataFormat::Hex), DATA2)

	const auto DATA3 = "222222222222";
	const auto CODE3 = "haskellisthebest1yg3zyg3zyg0yf0ga";

	ASSERT_EQUALS(decode(CODE3, DataFormat::Hex, true), DATA3)

	const auto DATA4 = "42";
	const auto CODE4 = "whatisthemeaninigoflife1gg3769yz";

	ASSERT_EQUALS(decode(CODE4, DataFormat::Hex, true), DATA4)

	const auto DATA5 = "00";
	const auto CODE5 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1qq9cyytg";

	ASSERT_EQUALS(decode(CODE5, DataFormat::Hex, true), DATA5)

	const auto DATA6 = "00";
	const auto CODE6 = "11qq56x0sh";

	ASSERT_EQUALS(decode(CODE6, DataFormat::Hex, true), DATA6)

	const auto DATA7 = "1234567890";
	const auto CODE7 = "qwerty1zg69v7ysaucy29";

	ASSERT_EQUALS(decode(CODE7, DataFormat::Hex), DATA7)

	const auto DATA8 = "1234567890";
	const auto CODE8 = "?werty1zg69v7ys9qza3r";

	ASSERT_EQUALS(decode(CODE8, DataFormat::Hex), DATA8)
}


/**
 * Decode with 1 char substitution
 */
void test_with_substitutions() {
    const auto DATA1 = "00000000000000000000";
    const auto CODE1 =      "0123456789abcdef1qqqqqqqqqqqqqqqqlaltj2";
    const auto CODE1SUBST = "a123456789abcdef1qqqqqqqqqqqqqqqqlaltj2";

	ASSERT_EQUALS(decode(CODE1SUBST, DataFormat::Hex), DATA1)

	const auto DATA2 = "11111111111111111111";
	const auto CODE2 =      "helloworld1zyg3zyg3zyg3zyg33k9kks";
	const auto CODE2SUBST = "helloworld1zyg3zyg3zyg3zyg33k9kkw";

	ASSERT_EQUALS(decode(CODE2SUBST, DataFormat::Hex), DATA2)

	const auto DATA3 = "222222222222";
	const auto CODE3 =      "haskellisthebest1yg3zyg3zyg0yf0ga";
	const auto CODE3SUBST = "haskellisthebest1zg3zyg3zyg0yf0ga";

	ASSERT_EQUALS(decode(CODE3SUBST, DataFormat::Hex, true), DATA3)

	const auto DATA4 = "42";
	const auto CODE4 =      "whatisthemeaninigoflife1gg3769yz";
	const auto CODE4SUBST = "whatisthemeaninigofliff1gg3769yz";

	ASSERT_EQUALS(decode(CODE4SUBST, DataFormat::Hex, true), DATA4)

	const auto DATA5 = "00";
	const auto CODE5 =      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1qq9cyytg";
	const auto CODE5SUBST = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1qq9cyytg";

	ASSERT_EQUALS(decode(CODE5SUBST, DataFormat::Hex, true), DATA5)

	const auto DATA6 = "00";
	const auto CODE6 =      "11qq56x0sh";
	const auto CODE6SUBST = "10qq56x0sh";

	ASSERT_EQUALS(decode(CODE6SUBST, DataFormat::Hex, true), DATA6)

	const auto DATA7 = "00";
	const auto CODE7 =      "11qq56x0sh";
	const auto CODE7SUBST = "01qq56x0sh";

	ASSERT_EQUALS(decode(CODE7SUBST, DataFormat::Hex, true), DATA7)

	const auto DATA8 = "1234567890";
	const auto CODE8 =      "qwerty1zg69v7ysaucy29";
	const auto CODE8SUBST = "qwerty0zg69v7ysaucy29";

	ASSERT_EQUALS(decode(CODE8SUBST, DataFormat::Hex), DATA8)

    const auto DATA9 = "1234567890";
    const auto CODE9 =      "?werty1zg69v7ys9qza3r";
    const auto CODE9SUBST = "0werty1zg69v7ys9qza3r";

    ASSERT_EQUALS(decode(CODE9SUBST, DataFormat::Hex), DATA9)
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
        return EXIT_SUCCESS;
    }
    std ::cout << " ✘ " << errors_count << " check(s) failed." << std::endl;
    return EXIT_FAILURE;
}
/**
 * Runs all tests.
 * @return Either 0 or 1, based on the number of failed tests.
 */
int main() {
    RUN_TEST(test_basic);
    RUN_TEST(test_encode);
    RUN_TEST(test_without_substitutions);
    RUN_TEST(test_with_substitutions);
    RUN_TEST(test_invalid_bech32m);
    RUN_TEST(test_bit_storage)
    RUN_TEST(test_parser)

    return interpret_test_results();
}
