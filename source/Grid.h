#pragma once

#include <array>
#include <limits>
#include <vector>

template<typename CoordinateType, CoordinateType Dim>
class Grid
{
public:
    using Coordinate = typename CoordinateType;
    using Index = unsigned long long;
    using Position = std::array<Coordinate, Dim>;
    using Positions = std::vector<Position>;
    using Size = Position;

public:
    static const Coordinate Dimensions = Dim;

private:
    static_assert(Dim > 0, "Dimension must be greater zero");
    static_assert(std::is_integral_v<Coordinate>, "Coordinate must be integral");
    static_assert(std::is_unsigned_v<Coordinate>, "Coordinate must be unsigned");
    static_assert(!std::is_same_v<Coordinate, bool>, "Coordinate must not be bool");

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
        return numCoordinatesImpl<0>();
    }

    constexpr Positions positions() const noexcept
    {
        Positions result;
        result.reserve(numCoordinates());

        positionsImpl<0>(result);

        return result;
    }

    constexpr Index index(const Position& position) const noexcept
    {
        return indexImpl<0>(position);
    }

    constexpr Position position(const Index& index) const noexcept
    {
        Position result;

        positionImpl<0>(index, result);

        return result;
    }

    template<class Calleable, class... Parameter>
    constexpr void run(const Calleable& callable, const Parameter&... parameter) const noexcept
    {
        runImpl<0>(parameter..., callable);
    }

private:
    template<Coordinate _D>
    constexpr Index indexImpl(const Position& params) const noexcept
    {
        if constexpr(_D == Dim - 1)
            return params[Dim - _D - 1];
        else
            return params[Dim - _D - 1] + m_Size[Dim - _D - 1] * indexImpl<_D + 1>(params);
    }

    template<Coordinate _D>
    constexpr void positionImpl(const Index& index, Position& position) const noexcept
    {
        if constexpr(_D == Dim - 1)
        {
            position[Dim - _D - 1] = index;
        }
        else
        {
            position[Dim - _D - 1] = index % m_Size[Dim - _D - 1];

            positionImpl<_D + 1>((index - position[Dim - _D - 1]) / m_Size[Dim - _D - 1], position);
        }
    }

    template<Coordinate _D>
    constexpr Index numCoordinatesImpl() const noexcept
    {
        if constexpr(_D == Dim - 1)
            return m_Size[_D];
        else
            return numCoordinatesImpl<_D + 1>() * m_Size[_D];
    }

    template<Coordinate _D, class... _Coords>
    constexpr void positionsImpl(Positions& result, const _Coords&... coords) const noexcept
    {
        const auto& s = m_Size[_D];

        for(Coordinate c = 0; c < s; c++)
            if constexpr(_D == Dim - 1)
                result.push_back({coords..., c});
            else
                positionsImpl<_D + 1>(result, coords..., c);
    }

    template<Coordinate _D, class... Parameter, class _Calleable, class... _Coords>
    constexpr void runImpl(const Parameter&... parameter, const _Calleable& callable, const _Coords&... coords) const noexcept
    {
        if constexpr(_D == Dim)
            callable(coords..., parameter...);
        else
        {
            const auto& s = m_Size[_D];

            for(Coordinate c = 0; c < s; c++)
                runImpl<_D + 1>(parameter..., callable, coords..., c);
        }
    }

private:
    Size m_Size;
};