//
// Created by Jlisowskyy on 08/12/24.
//

#include "../include/Transformations/DitheringMaps.hpp"

/* external includes */
#include <cstdlib>

std::pair<size_t, size_t> FindNearestSize(const double size) {
    if (size <= 2.0) {
        return {2, 1};
    }

    for (size_t idx = 0; idx < DITHERING_SIZES_COUNT; ++idx) {
        if (static_cast<double>(DITHERING_SIZES[idx]) > size) {
            return {DITHERING_SIZES[idx], idx};
        }
    }

    exit(1);
}
