#pragma once

#include <vector>

template<int Size>
class GridIterater : public GridIterater<Size - 1>
{
private:
    using IndexVector = std::vector<int>;
    using IndexVectorIter = std::vector<int>::iterator;
    using ParentType = GridIterater<Size - 1>;

public:
    template<class Dim, class... Dims>
    GridIterater(const Dim& size, const Dims&... sizes) noexcept : ParentType(sizes...), m_Size{size}
    {
    }

    template<class Dim, class... Dims>
    int idx(const Dim& idx, const Dims&... idxs) const noexcept
    {
        return idx + m_Size * ParentType::idx(idxs...);
    }

    template<class Calleable, class... Params>
    void doWorkPack(const Calleable& callable, const Params&... params) const noexcept
    {
        for(int i = 0; i < m_Size; i++)
            ParentType::doWorkPack(callable, params..., i);
    }

    template<class Calleable>
    void doWorkArray(const Calleable& callable) const noexcept
    {
        std::array<int, Size> indices;

        doWorkArrayImpl(0, indices, callable);
    }

    IndexVector indices(const int& idx) const noexcept
    {
        IndexVector result(Size);

        index_impl(idx, result.begin());

        return result;
    }

protected:
    void index_impl(const int& resIdx, IndexVectorIter idxIter) const noexcept
    {
        auto res = resIdx % m_Size;
        *idxIter = res;

        ParentType::index_impl((resIdx - res) / m_Size, ++idxIter);
    }

    template<class Calleable, typename _Indices>
    void doWorkArrayImpl(const int& dim, _Indices& indices, const Calleable& callable) const noexcept
    {
        for(int i = 0; i < m_Size; i++)
        {
            indices[dim] = i;

            ParentType::doWorkArrayImpl(dim + 1, indices, callable);
        }
    }

private:
    int m_Size;
};

template<>
class GridIterater<1>
{
private:
    using IndexVector = std::vector<int>;
    using IndexVectorIter = std::vector<int>::iterator;

public:
    template<class Dim>
    GridIterater(const Dim& size) noexcept : m_Size{size}
    {
    }

    template<class Dim, class... Dims>
    int idx(const Dim& idx, const Dims&... idxs) const noexcept
    {
        return idx;
    }

    template<class Calleable, class... Params>
    void doWorkPack(const Calleable& callable, const Params&... params) const noexcept
    {
        for(int i = 0; i < m_Size; i++)
            callable(params..., i);
    }

protected:
    void index_impl(const int& resIdx, IndexVectorIter idxIter) const noexcept
    {
        *idxIter = resIdx;
    }

    template<class Calleable, typename _Indices>
    void doWorkArrayImpl(const int& dim, _Indices& indices, const Calleable& callable) const noexcept
    {
        for(int i = 0; i < m_Size; i++)
        {
            indices[dim] = i;

            callable(indices);
        }
    }

private:
    int m_Size;
};