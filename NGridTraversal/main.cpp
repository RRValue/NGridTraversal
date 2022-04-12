#include "InheritanceApproach.h"
//#include "FlatApproach.h"

#include <array>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    const int s_d0 = 2;
    const int s_d1 = 3;
    const int s_d2 = 4;
    const int s_d3 = 5;

    auto ic = GridIterater<4>(s_d0, s_d1, s_d2, s_d3);

    ic.doWorkArray([](const auto& indices) {
        const auto& [x, y, z, w] = indices;

        std::cout << std::to_string(x) << " " << std::to_string(y) << " " << std::to_string(z) << " " << std::to_string(w) << std::endl;
    });

    ic.doWorkPack([](const auto& x, const auto& y, const auto& z, const auto& w) {
        std::cout << std::to_string(x) << " " << std::to_string(y) << " " << std::to_string(z) << " " << std::to_string(w) << std::endl;
    });

    // int sum = 0;

    // ic.doWork([&sum](const auto &x, const auto &y, const auto &z, const auto &w) {
    //  sum += x + y + z + w;
    //});

    // int ii = 0;
    // ii++;

    for(int i0_in = 0; i0_in < s_d0; i0_in++)
        for(int i1_in = 0; i1_in < s_d1; i1_in++)
            for(int i2_in = 0; i2_in < s_d2; i2_in++)
                for(int i3_in = 0; i3_in < s_d3; i3_in++)
                {
                    int idx =                                      //
                        (i0_in + s_d0 *                            //
                                     (i1_in + s_d1 *               //
                                                  (i2_in + s_d2 *  //
                                                               (i3_in))));

                    int idx_1 = ic.idx(i0_in, i1_in, i2_in, i3_in);

                    if(idx != idx_1)
                    {
                        int ii = 0;
                        ii++;
                    }

                    int i0_out, i1_out, i2_out, i3_out;
                    int idx_out = idx;

                    i0_out = idx_out % s_d0;
                    idx_out = (idx_out - i0_out) / s_d0;

                    i1_out = idx_out % s_d1;
                    idx_out = (idx_out - i1_out) / s_d1;

                    i2_out = idx_out % s_d2;
                    idx_out = (idx_out - i2_out) / s_d2;

                    i3_out = idx_out;              // % nothing
                    idx_out = (idx_out - i3_out);  // / nothing

                    if(i0_out != i0_in || i1_out != i1_in || i2_out != i2_in || i3_out != i3_in)
                    {
                        int ii = 0;
                        ii++;
                    }

                    auto res = ic.indices(idx_1);

                    if(res[0] != i0_in || res[1] != i1_in || res[2] != i2_in || res[3] != i3_in)
                    {
                        int ii = 0;
                        ii++;
                    }
                }

    return 0;
}