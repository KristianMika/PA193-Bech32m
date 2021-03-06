#ifndef BECH32M_BIT_STORAGE_H
#define BECH32M_BIT_STORAGE_H

#include <bitset>
#include <iostream>
#include <vector>

static uint16_t const BECH32M_SEPARATOR_LENGTH = 1;
static uint16_t const BECH32M_CHECKSUM_LENGTH = 6;
static uint16_t const BECH32M_MAXIMUM_CODE_LENGTH = 90;
static uint16_t const BECH32M_MAX_DATA_CHAR_COUNT =
    BECH32M_MAXIMUM_CODE_LENGTH - BECH32M_SEPARATOR_LENGTH - BECH32M_CHECKSUM_LENGTH;
static uint16_t const BECH32M_CHAR_BIT_COUNT = 5;
static uint16_t const BECH32M_MAX_BITSET_LENGTH = BECH32M_MAX_DATA_CHAR_COUNT * BECH32M_CHAR_BIT_COUNT; // 415 bits
using Bech32mBitset = std::bitset<BECH32M_MAX_BITSET_LENGTH>;
using Bech32mChar = std::bitset<BECH32M_CHAR_BIT_COUNT>;
using Bech32mVector = std::vector<Bech32mChar>;

/**
 * Represents a base class for all types of inputs.
 * Its main function is to provide a unified access to input data using an iterator.
 */
class BitStorage {
  protected:
    Bech32mBitset value{};
    uint16_t length = 0;
    bool trim = false;

  public:
    virtual ~BitStorage() = default;

    /**
     * Provides iterating over the bitset.
     */
    template <uint16_t L = 5> struct Iterator {
      private:
        const Bech32mBitset &value;
        uint16_t index;

      public:
        explicit Iterator(const Bech32mBitset &_value, uint16_t _index) : value(_value), index(_index) {}

        std::bitset<L> operator*() const {
            std::bitset<L> num = 0;
            for (int i = 0; i < L; ++i) {
                num = num << 1;
                num |= value[index + i];
            }
            return num;
        }

        inline Iterator &operator++() {
            index += L;
            return *this;
        }
        inline Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        inline friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.value == b.value && a.index == b.index;
        };
        inline friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.value != b.value || a.index != b.index;
        };
    };

    int compute_padding(const int char_length) const { return (char_length - (length % char_length)) % char_length; }

    /**
     * @return the bitlength of the internal value (not the size of the static bitset)
     */
    uint16_t size() const { return length; }

    BitStorage &operator=(const BitStorage &other) = default;
};

/**
 * Looks stupid to implement operator<< for iterator. It's for tests.
 */
template <uint16_t U = 5> std::ostream &operator<<(std::ostream &stream, const BitStorage::Iterator<U> &it) {
    return stream << it.operator*();
}

#endif // BECH32M_BIT_STORAGE_H
