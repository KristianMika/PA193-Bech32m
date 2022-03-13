#ifndef BECH32M_TEST_MACROS_H
#define BECH32M_TEST_MACROS_H

/**
 * @brief A simple assert macro for testing.
 * In case of an error, the @param errors_count is incremented.
 */
#define ASSERT_EQUALS(EXPRESSION, TARGET)                                                                              \
    {                                                                                                                  \
        try {                                                                                                          \
            if ((EXPRESSION) == (TARGET)) {                                                                            \
                ++success_count;                                                                                       \
            } else {                                                                                                   \
                std::cout << __TIME__ << ": ERROR on line " << __LINE__ << ": " << #EXPRESSION << " != \"" << (TARGET) \
                          << "\"." << std::endl;                                                                       \
                ++errors_count;                                                                                        \
            }                                                                                                          \
        } catch (std::exception & e) {                                                                                 \
            std::cout << __TIME__ << ": ERROR on line " << __LINE__ << ": " << #EXPRESSION                             \
                      << " threw an exception: " << e.what() << std::endl;                                             \
            ++errors_count;                                                                                            \
        }                                                                                                              \
    }

/**
 * Tests if the @param EXPRESSION throws @param EXCEPTION_CLASS
 */
#define ASSERT_THROWS(EXPRESSION, EXCEPTION_CLASS)                                                                     \
    {                                                                                                                  \
        try {                                                                                                          \
            (EXPRESSION);                                                                                              \
            std::cout << __TIME__ << ": ERROR on line " << __LINE__ << ": " << #EXPRESSION                             \
                      << " did not throw, an instance of " << #EXCEPTION_CLASS << " expected." << std::endl;           \
            ++errors_count;                                                                                            \
        } catch (EXCEPTION_CLASS & _) {                                                                                \
            ++success_count;                                                                                           \
        } catch (std::exception & e) {                                                                                 \
            std::cout << __TIME__ << ": ERROR on line " << __LINE__ << ": " << #EXPRESSION                             \
                      << " threw an unexpected exception: " << typeid(e).name() << ":" << e.what() << ", "             \
                      << #EXCEPTION_CLASS << " expected." << std::endl;                                                \
            ++errors_count;                                                                                            \
        }                                                                                                              \
    }

/**
 * A macro that tests the provided expression does not throw any exceptions
 */
#define ASSERT_DOES_NOT_THROW(EXPRESSION)                                                                              \
    {                                                                                                                  \
        try {                                                                                                          \
            (EXPRESSION);                                                                                              \
            ++success_count;                                                                                           \
        } catch (std::exception & e) {                                                                                 \
            std::cout << __TIME__ << ": ERROR on line " << __LINE__ << ": " << #EXPRESSION                             \
                      << " threw an unexpected exception: " << typeid(e).name() << ":" << e.what() << std::endl;       \
            ++errors_count;                                                                                            \
        }                                                                                                              \
    }

#endif // BECH32M_TEST_MACROS_H
