#ifndef BECH32M_BIT_STORAGE_H
#define BECH32M_BIT_STORAGE_H

#include <bitset>
#include <iostream>

static uint16_t const BECH32M_SEPARATOR_LENGTH = 1;
static uint16_t const BECH32M_CHECKSUM_LENGTH = 6;
static uint16_t const BECH32M_MAXIMUM_CODE_LENGTH = 90;
static uint16_t const BECH32M_MAX_DATA_CHAR_COUNT =
    BECH32M_MAXIMUM_CODE_LENGTH - BECH32M_SEPARATOR_LENGTH - BECH32M_CHECKSUM_LENGTH;
static uint16_t const BECH32M_CHAR_BIT_COUNT = 5;
static uint16_t const BECH32M_MAX_BITSET_LENGTH = BECH32M_MAX_DATA_CHAR_COUNT * BECH32M_CHAR_BIT_COUNT; // 415 bits
using Bech32mBitset = std::bitset<BECH32M_MAX_BITSET_LENGTH>;
using Bech32mChar = std::bitset<BECH32M_CHAR_BIT_COUNT>;

/**
 * Represents a base class for all type of inputs.
 * Its main function is to provide a unified access to input data using an iterator.
 */
class BitStorage {
  protected:
    Bech32mBitset value{};
    uint16_t length = 0;

  public:
    virtual ~BitStorage() = default;

    /**
     * Provides iterating over the bitset.
     */
    struct Iterator {
      private:
        const Bech32mBitset *ptr;
        uint16_t index = 0;

      public:
        explicit Iterator(const Bech32mBitset *_ptr, uint8_t _index) : ptr(_ptr), index(_index) {}

        Bech32mChar operator*() const;

        inline Iterator &operator++() {
            index += BECH32M_CHAR_BIT_COUNT;
            return *this;
        }
        inline Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        inline friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.ptr == b.ptr && a.index == b.index;
        };
        inline friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.ptr != b.ptr || a.index != b.index;
        };

        /**
         * Looks stupid to implement operator<< for iterator. It's for tests.
         */
        friend std::ostream &operator<<(std::ostream &stream, const BitStorage::Iterator &it) {
            return stream << it.ptr->to_string() << ", i=" << it.index;
        }
    };

    BitStorage::Iterator begin() const { return Iterator(&value, 0); }

    BitStorage::Iterator end() const { return Iterator(&value, length); }

    /**
     * @return the bitlength of the internal value (not the size of the static bitset)
     */
    uint16_t size() const { return length; }

    /**
     * Pads the decoded value to a multiple of BECH32M_CHAR_BIT_COUNT
     */
    void pad();
};

#endif // BECH32M_BIT_STORAGE_H
