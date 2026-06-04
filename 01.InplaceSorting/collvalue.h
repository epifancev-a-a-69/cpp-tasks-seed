#ifndef COLLVALUE_H
#define COLLVALUE_H

#include <cstddef>
#include <utility>

template <typename T> class CollectingValue
{
public:
    static inline size_t comps = 0;
    static inline size_t swaps = 0;
    static inline size_t moves = 0;

    T value;

    explicit CollectingValue(T val = T()) : value(val) {}

    CollectingValue(const CollectingValue &other) : value(other.value)
    {
        moves++;
    }

    CollectingValue(CollectingValue &&other) noexcept
        : value(std::move(other.value))
    {
        moves++;
    }

    CollectingValue &operator=(const CollectingValue &other)
    {
        if (this != &other)
        {
            value = other.value;
            moves++;
        }
        return *this;
    }

    CollectingValue &operator=(CollectingValue &&other) noexcept
    {
        if (this != &other)
        {
            value = std::move(other.value);
            moves++;
        }
        return *this;
    }

    bool operator<(const CollectingValue &other) const
    {
        comps++;
        return value < other.value;
    }

    bool operator<=(const CollectingValue &other) const
    {
        return value <= other.value;
    }

    CollectingValue &operator++()
    {
        ++value;
        return *this;
    }

    static void reset_stats()
    {
        comps = 0;
        swaps = 0;
        moves = 0;
    }
};

template <typename T> void swap(CollectingValue<T> &a, CollectingValue<T> &b)
{
    CollectingValue<T>::swaps++;
    std::swap(a.value, b.value);
}

#endif // COLLVALUE_H