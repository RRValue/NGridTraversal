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

    template<class Calleable>
    void doWorkPack(const Calleable& callable) const noexcept
    {
        //doWorkPackImpl(0, callable);
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
    void doWorkArrayImpl(const int& dim, const Calleable& callable, Ints& indices) const noexcept
    {
        const auto final = dim == Size - 1;
        const auto& s = m_Sizes[dim];

        for(int i = 0; i < s; i++)
        {
            indices[dim] = i;

            if(final)
                callable(indices);
            else
                doWorkArrayImpl(dim + 1, callable, indices);
        }
    }

    /*template<class... Dims>
    struct ParamPack
    {
        Dims... m_Pack;
    };*/

    //template<class Calleable>
    //void doWorkPackImpl(const int& dim, const Calleable& callable, const ParamPack<>& paramPack = ParamPack<>()) const noexcept
    //{
    //    const auto final = dim == Size - 1;
    //    const auto& s = m_Sizes[dim];

    //    for(int i = 0; i < s; i++)
    //    {
    //        if(final)
    //            return; //callable(indices);
    //        else
    //            doWorkPackImpl(dim + 1, callable, ParamPack(paramPack, i));
    //    }
    //}

private:
    Ints m_Sizes;
};