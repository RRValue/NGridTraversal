#include <gtest/gtest.h>

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