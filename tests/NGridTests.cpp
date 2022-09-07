#include <gtest/gtest.h>

#include "Grid.h"

#include <algorithm>
#include <iostream>

template<typename T>
class GridFixture : public ::testing::Test
{
public:
    using GridType = typename T;
    using GridSize = typename GridType::Size;

protected:
    void SetUp() final override
    {
        if constexpr(GridType::Dimensions == 1)
            m_Size = {3};
        else if constexpr(GridType::Dimensions == 2)
            m_Size = {3, 5};
        else if constexpr(GridType::Dimensions == 3)
            m_Size = {3, 5, 7};
        else if constexpr(GridType::Dimensions == 4)
            m_Size = {3, 5, 7, 11};
        else
            static_assert(GridType::Dimensions >= 5, "dimension not supported");

        m_Grid = GridType(m_Size);
    }

    void TearDown() final override
    {
    }

protected:
    GridType m_Grid;
    GridSize m_Size;
};

using GridTypes = ::testing::Types<Grid<unsigned long long, 1>,  //
                                   Grid<unsigned long long, 2>,  //
                                   Grid<unsigned long long, 3>,  //
                                   Grid<unsigned long long, 4>,  //
                                   Grid<unsigned long, 1>,       //
                                   Grid<unsigned long, 2>,       //
                                   Grid<unsigned long, 3>,       //
                                   Grid<unsigned long, 4>,       //
                                   Grid<unsigned int, 1>,        //
                                   Grid<unsigned int, 2>,        //
                                   Grid<unsigned int, 3>,        //
                                   Grid<unsigned int, 4>,        //
                                   Grid<unsigned short, 1>,      //
                                   Grid<unsigned short, 2>,      //
                                   Grid<unsigned short, 3>,      //
                                   Grid<unsigned short, 4>,      //
                                   Grid<unsigned char, 1>,       //
                                   Grid<unsigned char, 2>,       //
                                   Grid<unsigned char, 3>,       //
                                   Grid<unsigned char, 4>>;

TYPED_TEST_CASE(GridFixture, GridTypes);

TYPED_TEST(GridFixture, Size)
{
    EXPECT_EQ(this->m_Size, this->m_Grid.size());
}

TYPED_TEST(GridFixture, NumCoordinates)
{
    const auto& dim = this->m_Grid.Dimensions;

    if constexpr(dim == 1)
        EXPECT_EQ(this->m_Grid.numCoordinates(), 3);
    else if constexpr(dim == 2)
        EXPECT_EQ(this->m_Grid.numCoordinates(), 3 * 5);
    else if constexpr(dim == 3)
        EXPECT_EQ(this->m_Grid.numCoordinates(), 3 * 5 * 7);
    else if constexpr(dim == 4)
        EXPECT_EQ(this->m_Grid.numCoordinates(), 3 * 5 * 7 * 11);
}

TYPED_TEST(GridFixture, LargestIndex)
{
    const auto& dim = this->m_Grid.Dimensions;

    if constexpr(dim == 1)
        EXPECT_EQ(this->m_Grid.largestIndex(), (3) - 1);
    else if constexpr(dim == 2)
        EXPECT_EQ(this->m_Grid.largestIndex(), (3 * 5) - 1);
    else if constexpr(dim == 3)
        EXPECT_EQ(this->m_Grid.largestIndex(), (3 * 5 * 7) - 1);
    else if constexpr(dim == 4)
        EXPECT_EQ(this->m_Grid.largestIndex(), (3 * 5 * 7 * 11) - 1);
}

TYPED_TEST(GridFixture, Positions)
{
    using PositionSet = std::set<decltype(this->m_Grid)::Position>;

    const auto positions_0 = this->m_Grid.positions();
    auto positions_1 = PositionSet();

    std::copy(std::cbegin(positions_0), std::cend(positions_0), std::inserter(positions_1, std::cend(positions_1)));

    EXPECT_EQ(positions_0.size(), positions_1.size());
}

TYPED_TEST(GridFixture, IndicesFromPosition)
{
    using IndexSet = std::set<decltype(this->m_Grid)::Index>;

    const auto positions = this->m_Grid.positions();
    auto indices = IndexSet();

    std::transform(std::cbegin(positions), std::cend(positions),  //
                   std::inserter(indices, std::cend(indices)),    //
                   [&](const auto& p) { return this->m_Grid.index(p); });

    EXPECT_EQ(positions.size(), indices.size());
    EXPECT_EQ(*std::cbegin(indices), 0);
    EXPECT_EQ(*std::crbegin(indices), this->m_Grid.largestIndex());
}

TYPED_TEST(GridFixture, PositionFromIndices)
{
    using Index = decltype(this->m_Grid)::Index;
    using Position = decltype(this->m_Grid)::Position;
    using Positions = decltype(this->m_Grid)::Positions;
    using IndexVector = std::vector<Index>;
    using PositionSet = std::set<Position>;

    const auto count = this->m_Grid.numCoordinates();
    const auto& dim = this->m_Grid.Dimensions;

    auto indices = IndexVector();
    indices.reserve(count);

    auto i = Index(0);
    std::generate_n(std::back_inserter(indices), count, [&i]() mutable { return i++; });

    auto positions_vector = Positions();
    positions_vector.reserve(count);

    std::transform(std::cbegin(indices), std::cend(indices),  //
                   std::back_inserter(positions_vector),      //
                   [&](const auto& i) { return this->m_Grid.position(i); });

    auto positions = PositionSet();

    std::copy(std::cbegin(positions_vector), std::cend(positions_vector),  //
              std::inserter(positions, std::cend(positions)));

    EXPECT_EQ(indices.size(), positions.size());

    auto exp_begin_pos = Position();
    auto exp_end_pos = Position();

    if constexpr(dim == 1)
    {
        exp_begin_pos = {0};
        exp_end_pos = {2};
    }
    else if constexpr(dim == 2)
    {
        exp_begin_pos = {0, 0};
        exp_end_pos = {2, 4};
    }
    else if constexpr(dim == 3)
    {
        exp_begin_pos = {0, 0, 0};
        exp_end_pos = {2, 4, 6};
    }
    else if constexpr(dim == 4)
    {
        exp_begin_pos = {0, 0, 0, 0};
        exp_end_pos = {2, 4, 6, 10};
    }

    EXPECT_EQ(*std::cbegin(positions_vector), exp_begin_pos);
    EXPECT_EQ(*std::crbegin(positions_vector), exp_end_pos);
}

TYPED_TEST(GridFixture, PositionsRoundTrip)
{
    const auto positions_in = this->m_Grid.positions();
    auto positions_out = decltype(positions_in)();

    positions_out.reserve(positions_in.size());

    std::transform(std::cbegin(positions_in), std::cend(positions_in),  //
                   std::back_inserter(positions_out),                   //
                   [&](const auto& p) { return this->m_Grid.position(this->m_Grid.index(p)); });

    EXPECT_EQ(positions_in, positions_out);
}

TYPED_TEST(GridFixture, IndexRoundTrip)
{
    using Index = decltype(this->m_Grid)::Index;
    using IndexVector = std::vector<Index>;

    const auto count = this->m_Grid.numCoordinates();

    auto indices_in = IndexVector();
    auto indices_out = IndexVector();

    indices_in.reserve(count);
    indices_out.reserve(count);

    auto i = Index(0);
    std::generate_n(std::back_inserter(indices_in), count, [&i]() mutable { return i++; });

    std::transform(std::cbegin(indices_in), std::cend(indices_in),  //
                   std::back_inserter(indices_out),                 //
                   [&](const auto& i) { return this->m_Grid.index(this->m_Grid.position(i)); });

    EXPECT_EQ(indices_in, indices_out);
}

TYPED_TEST(GridFixture, DoWorkOnSamePositions)
{
    using Position = decltype(this->m_Grid)::Position;

    const auto& dim = this->m_Grid.Dimensions;

    const auto positions_in = this->m_Grid.positions();
    auto positions_out = decltype(positions_in)();
    positions_out.reserve(positions_in.size());

    this->m_Grid.run([&positions_out](const auto&... params) { positions_out.push_back({params...}); });

    EXPECT_EQ(positions_in, positions_out);
}

TYPED_TEST(GridFixture, DoWorkWithExtraParams)
{
    using Position = decltype(this->m_Grid)::Position;

    const auto& dim = this->m_Grid.Dimensions;

    const auto positions_in = this->m_Grid.positions();
    auto positions_out = decltype(positions_in)();
    positions_out.reserve(positions_in.size());

    struct SomeData
    {
        int m_Int = 23;
        float m_Float = 23.0f;
        double m_Double = 23.0;
    };

    const auto callable = [&dim, &positions_out]() {
        if constexpr(dim == 1)
            return [&positions_out](const auto& x, const auto& p1, const auto& p2) { positions_out.push_back({x}); };
        else if constexpr(dim == 2)
            return [&positions_out](const auto& x, const auto& y, const auto& p1, const auto& p2) { positions_out.push_back({x, y}); };
        else if constexpr(dim == 3)
            return [&positions_out](const auto& x, const auto& y, const auto& z, const auto& p1, const auto& p2) { positions_out.push_back({x, y, z}); };
        else if constexpr(dim == 4)
            return [&positions_out](const auto& x, const auto& y, const auto& z, const auto& w, const auto& p1, const auto& p2) {
                positions_out.push_back({x, y, z, w});
            };
    }();

    this->m_Grid.run(callable, "Text", SomeData{});

    EXPECT_EQ(positions_in, positions_out);
}
