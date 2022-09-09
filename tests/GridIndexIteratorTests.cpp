#include <gtest/gtest.h>

#include "Grid.h"
#include "GridFixture.h"

#include <algorithm>

TYPED_TEST(GridFixture, Iterator)
{
    using Iterator = decltype(this->m_Grid)::iterator;
    using ConstIterator = decltype(this->m_Grid)::const_iterator;
    using Size = decltype(this->m_Grid)::Size;
    using Index = decltype(this->m_Grid)::Index;
    using Indices = std::set<Index>;
    using VectorOfIndices = std::vector<std::set<Index>>;

    auto indices = VectorOfIndices(5);

    const auto& grid = this->m_Grid;

    for(const auto& i : grid)
        indices[0].insert(i);

    std::for_each(std::begin(grid), std::end(grid), [&](const auto& index) { indices[1].insert(index); });
    std::for_each(std::cbegin(grid), std::cend(grid), [&](const auto& index) { indices[2].insert(index); });

    for(Iterator it = grid.begin(); it != grid.end(); ++it)
        indices[3].insert(*it);

    for(ConstIterator it = grid.cbegin(); it != grid.cend(); ++it)
        indices[4].insert(*it);

    std::for_each(std::cbegin(indices), std::cend(indices), [&](const auto& i) {  //
        EXPECT_EQ(i.size(), grid.numCoordinates());
        EXPECT_EQ(*i.begin(), 0);
        EXPECT_EQ(*i.rbegin(), grid.maxIndex());
    });
}