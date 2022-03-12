

#ifndef BECH32M_BIT_STORAGE_H
#define BECH32M_BIT_STORAGE_H
#include <bitset>
#include <iostream>
static uint16_t const BECH32M_CHAR_BIT_COUNT = 5;
static uint16_t const MAX_BECH32M_HUMAN_READABLE_CHARS = 83;
static uint16_t const MAX_BITSET_LENGTH = MAX_BECH32M_HUMAN_READABLE_CHARS * BECH32M_CHAR_BIT_COUNT; // 415 bits
using BechCharType = std::bitset<BECH32M_CHAR_BIT_COUNT>;
/**
 * Represents a base class for all type of inputs.
 * Its main function is to provide a unified access to input data using an iterator.
 */
class BitStorage {
  protected:
    std::bitset<MAX_BITSET_LENGTH> value{};
    uint16_t length = 0;

  public:
    virtual ~BitStorage() = default;

    /**
     * Provides iterating over over the bitset.
     */
    struct Iterator {
      private:
        const std::bitset<MAX_BITSET_LENGTH> *ptr;
        uint16_t index = 0;

      public:
        explicit Iterator(const std::bitset<MAX_BITSET_LENGTH> *_ptr, uint8_t _index) : ptr(_ptr), index(_index) {}
        std::bitset<5> operator*() const;
        inline Iterator &operator++() {
            index += 5;
            return *this;
        }
        inline Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        inline friend bool operator==(const Iterator &a, const Iterator &b) { return a.ptr == b.ptr && a.index == b.index; };
        inline friend bool operator!=(const Iterator &a, const Iterator &b) { return a.ptr != b.ptr || a.index != b.index; };

        /**
         * Looks stupid to implement this for iterator. It's for tests.
         */
        friend std::ostream &operator<<(std::ostream &stream, const BitStorage::Iterator &it) {
            return stream << it.ptr->to_string() << ", i=" << it.index;
        }
    };
    BitStorage::Iterator begin() const { return Iterator(&value, 0); }

    BitStorage::Iterator end() const { return Iterator(&value, length); }

    uint16_t size() const { return length; }

    void pad();
};

#endif // BECH32M_BIT_STORAGE_H
