#pragma once

#include <array>

template<int Size>
class GridIterater
{
private:
    using Ints = std::array<int, Size>;

public:
    template<class... Dims>
    GridIterater(const Dims&... sizes) noexcept : m_Sizes{sizes...}
    {
    }

    template<class... Dims>
    int idx(const Dims&... idxs) const noexcept
    {
        const auto param = Ints{idxs...};
        return idxImpl(param);
    }

    Ints indices(const int& idx) const noexcept
    {
        Ints result;

        indicesImpl(idx, result);

        return result;
    }

    template<class Calleable>
    void doWorkArray(const Calleable& callable) const noexcept
    {
        Ints indices;

        doWorkArrayImpl(0, callable, indices);
    }

protected:
    int idxImpl(const Ints& params, const int& dim = 0) const noexcept
    {
        if(dim == Size - 1)
            return params[dim];
        else
            return params[dim] + m_Sizes[dim] * idxImpl(params, dim + 1);
    }

    void indicesImpl(const int& resIdx, Ints& result, const int& dim = 0) const noexcept
    {
        if(dim == Size - 1)
        {
            result[dim] = resIdx;

            return;
        }

        result[dim] = resIdx % m_Sizes[dim];

        indicesImpl((resIdx - result[dim]) / m_Sizes[dim], result, dim + 1);
    }

    template<class Calleable>
    void doWorkImpl(const int& dim, const Calleable& callable, Ints& indices) const noexcept
    {
        if(dim == Size - 1)
            callable(indices);
        else
            doWorkArrayImpl(dim + 1, callable, indices);
    }

private:
    Ints m_Sizes;
};