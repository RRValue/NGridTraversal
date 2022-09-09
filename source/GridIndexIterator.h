#pragma once

#include <compare>

template<class Grid>
class GridIndexIterator
{
public:
    using Value = typename Grid::Index;

    using value_type = typename Value;
    using difference_type = typename value_type;
    using pointer = typename value_type*;
    using reference = const value_type&;

    constexpr GridIndexIterator() noexcept : m_Index{0}
    {
    }

    constexpr GridIndexIterator(value_type index) noexcept : m_Index(index)
    {
    }

    [[nodiscard]] constexpr reference operator*() const noexcept
    {
        return m_Index;
    }

    [[nodiscard]] constexpr pointer operator->() noexcept
    {
        return &m_Index;
    }

    constexpr GridIndexIterator& operator++() noexcept
    {
        ++m_Index;
        return *this;
    }

    constexpr GridIndexIterator operator++(int) noexcept
    {
        GridIndexIterator tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr GridIndexIterator& operator--() noexcept
    {
        --m_Index;
        return *this;
    }

    constexpr GridIndexIterator operator--(int) noexcept
    {
        GridIndexIterator tmp = *this;
        --*this;
        return tmp;
    }

    constexpr GridIndexIterator& operator+=(const difference_type offset) noexcept
    {
        m_Index += offset;

        return *this;
    }

    [[nodiscard]] constexpr GridIndexIterator operator+(const difference_type offset) const noexcept
    {
        GridIndexIterator tmp = *this;
        tmp += offset;
        return tmp;
    }

    constexpr GridIndexIterator& operator-=(const difference_type offset) noexcept
    {
        return *this += -offset;
    }

    [[nodiscard]] constexpr GridIndexIterator operator-(const difference_type offset) const noexcept
    {
        GridIndexIterator tmp = *this;
        tmp -= offset;
        return tmp;
    }

    [[nodiscard]] constexpr difference_type operator-(const GridIndexIterator& other) const noexcept
    {
        return m_Index - other.m_Index;
    }

    [[nodiscard]] constexpr reference operator[](const difference_type offset) const noexcept
    {
        return *(*this + offset);
    }

    [[nodiscard]] constexpr bool operator==(const GridIndexIterator& other) const noexcept
    {
        return m_Index == other.m_Index;
    }

    [[nodiscard]] constexpr std::strong_ordering operator<=>(const GridIndexIterator& other) const noexcept
    {
        return m_Index <=> other.m_Index;
    }

private:
    value_type m_Index;
};