#include "../src/bech32m.h"
#include <iostream>

const bool EXIT_ON_FAILURE = false;
static long errors_count = 0;

/**
 * @brief A simple assert macro for testing.
 * In case of an error, the @param errors_count increments.
 * TODO: Replace this macro by the proper testing framework (IF ALLOWED)
 */
#define ASSERT_EQUALS(EXPRESSION, TARGET)                                      \
  {                                                                            \
    if ((EXPRESSION) != (TARGET)) {                                            \
      std::cout << __TIME__ << ": ERROR on line " << __LINE__ << ": "          \
                << #EXPRESSION << " != \"" << TARGET << "\"." << std::endl;    \
      ++errors_count;                                                          \
      if (EXIT_ON_FAILURE)                                                     \
        exit(1);                                                               \
    }                                                                          \
  }

void test_encode() { ASSERT_EQUALS(encode("something"), "something else"); }

void test_decode() {}

int main() {
  test_encode();
  test_decode();

  return errors_count == 0 ? 0 : 1;
}