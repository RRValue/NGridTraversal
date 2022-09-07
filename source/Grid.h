#pragma once

#include <array>
#include <numeric>
#include <vector>

template<typename T>
concept NonBoolUnsignedIntegral = std::unsigned_integral<T> && !std::same_as<T, bool>;

template<NonBoolUnsignedIntegral Coordinate, Coordinate Dim>
class Grid
{
public:
    using Index = unsigned long long;
    using Position = std::array<Coordinate, Dim>;
    using Positions = std::vector<Position>;
    using Size = Position;

public:
    static const Coordinate Dimensions = Dim;

private:
    static_assert(Dim > 0, "Dimension must be greater zero");

public:
    constexpr Grid() noexcept : m_Size{}
    {
    }

    constexpr Grid(const Size& size) noexcept : m_Size{size}
    {
    }

    constexpr const Size& size() const noexcept
    {
        return m_Size;
    }

    constexpr Index largestIndex() const noexcept
    {
        return numCoordinates() - 1;
    }

    constexpr Index numCoordinates() const noexcept
    {
        return std::accumulate(std::cbegin(m_Size), std::cend(m_Size), Index(1), std::multiplies<Index>());
    }

    constexpr Positions positions() const noexcept
    {
        Positions positions;

        run([&positions](const auto&... params) { positions.push_back({params...}); });

        return positions;
    }

    constexpr Index index(const Position& position) const noexcept
    {
        auto index = Index(0);

        for(Coordinate c = 0; c < Dim; c++)
            index = index * m_Size[c] + position[c];

        return index;
    }

    constexpr Position position(const Index& index) const noexcept
    {
        auto position = Position();
        auto index_residual = index;

        for(Coordinate c = 0; c < Dim; c++)
            if(c == Dim - 1)
                position[Dim - c - 1] = index_residual;
            else
            {
                const auto& size = m_Size[Dim - c - 1];
                const auto coord = index_residual % size;

                position[Dim - c - 1] = coord;
                index_residual = (index_residual - coord) / size;
            }

        return position;
    }

    template<class Calleable, class... Parameter>
    constexpr void run(const Calleable& callable, const Parameter&... parameter) const noexcept
    {
        runImpl<0>(callable, parameter...);
    }

private:
    template<Coordinate _D, class _Calleable, class... Parameter>
    constexpr void runImpl(const _Calleable& callable, const Parameter&... parameter) const noexcept
    {
        for(Coordinate c = 0; c < m_Size[Dim - _D - 1]; c++)
            if constexpr(_D == Dim - 1)
                callable(c, parameter...);
            else
                runImpl<_D + 1>(callable, c, parameter...);
    }

private:
    Size m_Size;
};