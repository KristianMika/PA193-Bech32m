#ifndef BECH32M_TEST_MACROS_H
#define BECH32M_TEST_MACROS_H

#define ERROR_HEADER std::endl << "Err: Line:" << __LINE__ << ":\n - "
/**
 * @brief A simple assert macro for testing.
 * In case of an error, the @param errors_count is incremented.
 */
#define ASSERT_EQUALS(EXPRESSION, TARGET)                                                                              \
    {                                                                                                                  \
        try {                                                                                                          \
            auto expr = (EXPRESSION);                                                                                  \
            if (expr == (TARGET)) {                                                                                    \
                ++success_count;                                                                                       \
            } else {                                                                                                   \
                std::cout << ERROR_HEADER << #EXPRESSION << " != \"" << (TARGET) << "\"." << std::endl;                \
                std::cout << "   Expression was evaluated to: " << expr << std::endl;                                  \
                ++errors_count;                                                                                        \
            }                                                                                                          \
        } catch (std::exception & e) {                                                                                 \
            std::cout << ERROR_HEADER << #EXPRESSION << " threw an exception: " << e.what() << std::endl;              \
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
            std::cout << ERROR_HEADER << #EXPRESSION << " did not throw, an instance of " << #EXCEPTION_CLASS          \
                      << " expected." << std::endl;                                                                    \
            ++errors_count;                                                                                            \
        } catch (EXCEPTION_CLASS & _) {                                                                                \
            ++success_count;                                                                                           \
        } catch (std::exception & e) {                                                                                 \
            std::cout << ERROR_HEADER << #EXPRESSION << " threw an unexpected exception: " << typeid(e).name() << ":"  \
                      << e.what() << ", " << #EXCEPTION_CLASS << " expected." << std::endl;                            \
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
            std::cout << ERROR_HEADER << #EXPRESSION << " threw an unexpected exception: " << typeid(e).name() << ":"  \
                      << e.what() << std::endl;                                                                        \
            ++errors_count;                                                                                            \
        }                                                                                                              \
    }

#define RUN_TEST(TEST_NAME)                                                                                            \
    {                                                                                                                  \
        std::cout << std::endl << " ** Test " << (#TEST_NAME) << " **" << std::endl;                                   \
        long err_prev = errors_count;                                                                                  \
        (TEST_NAME());                                                                                                 \
        if (err_prev == errors_count) {                                                                                \
            std::cout << "  [OK]" << std::endl;                                                                        \
        }                                                                                                              \
    }

#endif // BECH32M_TEST_MACROS_H
